#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include "Helpbrowser.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , m_settings("VEB","MyProgram")  //Для сохранения настроек
{
    ui->setupUi(this);
//    setWindowTitle("Редактор графов");
    setWindowIcon(QIcon(":/help/format_examp.png"));
    readSettings();//Для восстановления настроек
    // Создаётся сигнал для управления удалением вершин
    ui->actionDeleteItem->setShortcut(tr("Delete"));
    ui->pushFind->setDisabled(true);
    ui->actionDeleteItem->setStatusTip(tr("Удалить выбранные вершины из графа"));
    connect(ui->actionDeleteItem, SIGNAL(triggered(bool)), this, SLOT(deleteNode()));

    connect(this,SIGNAL(changeOpenFile()),
            SLOT(on_OpenButton_clicked()));  //удалить


//    QAction* pactOpen = new QAction("Open File", 0);
    ui->action_Open_file->setText("&Open...");
    ui->action_Open_file->setShortcut(QKeySequence("CTRL+O"));
    ui->action_Open_file->setToolTip("Open Document");
    ui->action_Open_file->setStatusTip("Open an existing file");
    ui->action_Open_file->setWhatsThis("Open an existing file");
    ui->action_Open_file->setIcon(QPixmap(":/cars/fileopen.png"));
    //connect(pactOpen, SIGNAL(triggered()), SLOT(slotLoad()));

    //QAction* pactSave = new QAction("Save File", 0);
    ui->action_Save_File->setText("&Save");
    ui->action_Save_File->setShortcut(QKeySequence("CTRL+S"));
    ui->action_Save_File->setToolTip("Save Document");
    ui->action_Save_File->setStatusTip("Save the file to disk");
    ui->action_Save_File->setWhatsThis("Save the file to disk");
    ui->action_Save_File->setIcon(QPixmap(":/cars/filesave.png"));
    //connect(pactSave, SIGNAL(triggered()), SLOT(slotSave()));

    ui->action_clear_all->setText("Очистить всё");
    ui->action_clear_all->setToolTip("Очистить всё");
    ui->action_clear_all->setStatusTip("Очистить всё");

    ui->action->setText("Помощь");
    ui->action->setToolTip("Помощь");
    ui->action->setStatusTip("Помощь");
    ui->action->setShortcut(QKeySequence("F1"));


    QToolBar* ptbFile = new QToolBar("File Operations");
    ptbFile->addAction(ui->action_Open_file);
    ptbFile->addAction(ui->action_Save_File);
    ptbFile->addAction(ui->action_clear_all);
    ptbFile->addAction(ui->action);

    ui->actionMenuFileExit->setShortcuts(QKeySequence::Quit);
    ui->actionMenuFileExit->setStatusTip(tr("Выйти из программы"));
    connect(ui->actionMenuFileExit, SIGNAL(triggered()), this, SLOT(close()));

    graph = ui->graphicsView;
    graph->setDragMode(QGraphicsView::RubberBandDrag);
    graph->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    graph->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    graph->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graph->setInteractive(true);
    graph->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
    writeSettings();
}

void MainWindow::readSettings()
{
//    Проверить
    m_settings.beginGroup("/Settings");

    int     nWidth     = m_settings.value("/width", width()).toInt();
    int     nHeight    = m_settings.value("/height", height()).toInt();
    QString nInputPath = m_settings.value("/fileData", "").toString();
    InputPath = nInputPath;
    resize(nWidth, nHeight);

    m_settings.endGroup();
}

void MainWindow::writeSettings()
{
    m_settings.beginGroup("/Settings");
    m_settings.setValue("/width", width());
    m_settings.setValue("/height", height());
    m_settings.setValue("/fileData", InputPath);
    m_settings.endGroup();
}


void MainWindow::deleteNode()
// Удаляет выбранные вершины графа
{
    graph->deleteSelectedItems();
    graph->changeIndecesOfAllVerteces();
}

void MainWindow::on_pb_CoordinatesOfAllNode_clicked()
// Выписывает координаты всех вершин графа
{
    QString s = "Координаты вершин: \n\n";
    for (int i = 0; i < graph->getListOfNodeSize(); i++)
    {
        s += QString("%1 - ( %2, %3 )\n").arg(i).arg(graph->getPosOfNode(i).x()).arg(graph->getPosOfNode(i).y());
    }
    s.chop(1);
    ui->textBrowser->setText(s);
}

void MainWindow::on_pb_LengthOfEdges_clicked()
// Выписывает длину всех рёбер графа
{
    QString s = "Длины рёбер:\n\n";
    int i = 0;
    foreach (Edges *item, graph->getEdges())
    {
        s += QString("%1. ( %2, %3 ) - %4\n").arg(++i).arg(item->source).arg(item->dest).arg(item->length);
    }
    s.chop(1);
    ui->textBrowser->setText(s);
}


void MainWindow::on_pb_ConnectGraph_clicked()
// Сделать граф полным
{
    on_pb_DisconnectGraph_clicked();
    int ic = 0, jc;
    foreach (Node* i, graph->listOfNode)
     // Добавляем новые рёбра на все пары рёбер, кроме одинаковых
    {
        jc = 0;
        foreach (Node* j, graph->listOfNode)
        {
            if(ic <= jc) break; // Если не будем останавливать цикл, то добавится 2 одинаковых ребра - (1,0) и (0,1)

            graph->scene->addItem(new Edge(j, i));
            jc++;
        }
        ic++;
    }
}

void MainWindow::on_pb_DisconnectGraph_clicked()
// Удалить все рёбра в графе
{
    foreach (Node* Node, graph->listOfNode)
        // Для каждой вершины -
    {
        foreach (Edge *edge, Node->edges())
            // - смотрим все, связанные с ней, рёбра
        {
            graph->scene->removeItem(edge);
            //Убрать ребро из окна

            edge->destNode()->removeEdge(edge);
            //Убрать ребро из списка рёбер первой вершины
            edge->sourceNode()->removeEdge(edge);
            //Убрать ребро из списка рёбер второй вершины

            delete edge;
        }
        //delete Node;
    }
}

void MainWindow::on_OpenButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this, "Открыть граф",
                                                    /*path*/InputPath, "Graph file (*.graph)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            on_pb_ClearAll_clicked();
            graph->getEdges().clear();
            graph->listOfNode.clear();

            QFileInfo fi(file);

            QString fn4 = fi.absolutePath();
            InputPath = fn4;

            int vertices_count, edges_count;
            in >> vertices_count;
            for (int i = 0; i < vertices_count; i++)
            {
                int id, x, y;
                in >> id >> x >> y;

                Node *Node1 = new Node(graph); // Создаём новую вершину
                Node1->setIndex(id);
                Node1->setPos(x,y);
                graph->scene->addItem(Node1); // Добавляем её на экран
                graph->listOfNode << Node1; // Добавить эту вершину в список вершин
            }
            graph->update();// Обновяем экран*/
            bool accept = true;
            in >> edges_count;
            for (int i = 0; i < edges_count; i++)
            {
                accept = true;
                int uid, vid;
                unsigned length;
                in >> uid >> vid >> length;
                Node *source, *dest;
                //по номеру надо найти вершину
                source = graph->findNode(uid);
                dest = graph->findNode(vid);
                foreach (Edges *edge, graph->getEdges())
                {
                    if (edge->source == uid && edge->dest == vid)
                        accept = false;
                }
                if (accept)
                    graph->scene->addItem(new Edge(source, dest, length));
            }
            graph->update();// Обновяем экран
            file.close();
            //пытаюсь открыть файл ещё раз с начала
            if(file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                ui->textBrowser->setText(in.readAll());
            }
            file.close();

        }
        else {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
        }
    }
    else return;
}

void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить граф", InputPath, "Graph file (*.graph)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            int n = graph->getListOfNodeSize();
            out <<  n << endl;
            for (int i = 0;i < n; i++)
              out << i << " " << graph->getPosOfNode(i).x() << " " << graph->getPosOfNode(i).y() << endl;
            int i = 0; foreach (Edges *item, graph->getEdges()) i=i+1;
            out << i << endl;
            foreach (Edges *item, graph->getEdges())
               out << item->source << " " << item->dest << " " << item->length << endl;
            file.close();
        }
        else {
            QMessageBox::information(this, "Невозможно открыть файл", file.errorString());
            return;
      }
    } else return;
}

void MainWindow::on_pb_RemoveEdges_clicked() //Удалить рёбра
{
    on_pb_DisconnectGraph_clicked();
}

void MainWindow::on_pb_DeleteSelectedNodes_clicked()  //Удалить вершины
{
   deleteNode();
}

void MainWindow::on_pb_CreateNode_clicked()//создаем вершину
{
    graph->addNode();
}

void MainWindow::on_pb_CreateEdge_clicked()
{//Создаём ребро
    Node *source;// = graph->listOfNode.first();
    Node *dest;// = graph->listOfNode.last();
    int Source1 = (ui->lineEdit_Source->text()).toInt();//
    int Dest1 = (ui->lineEdit_Dest->text()).toInt();//
    //по номеру надо найти вершину  сделать функцию
    source = graph ->findNode(Source1);
    dest = graph->findNode(Dest1);
    graph->addEdge(source,dest);
}




void MainWindow::on_pb_ClearAll_clicked()
{
    graph->deleteAllItems();
    ui->textBrowser->setText("Двойной клик левой кнопкой мыши - добавить новую вершину. \n\n"
                             "Двойной клик правой кнопкой мыши и клавища Shift - "
                             "соединить выбранные вершины с той, на которую кликал.");
}

void MainWindow::on_pb_Help_clicked()
{
    HelpBrowser  helpBrowser(":/help/", "main.htm");
    helpBrowser.resize(1100, 500);
    helpBrowser.exec();
}

void MainWindow::on_pb_IncidenceListFile_clicked()
// Создание графа из списка инцидентности
{
    QString FileName = QFileDialog::getOpenFileName(this, "Открыть файл...", InputPath, "Text files (*.txt)");
    if (FileName.isEmpty())
        return;

    QFile File(FileName);

    if(File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        on_pb_ClearAll_clicked();
        if (!graph->createGraphFromInc(File.readAll())) {
            QMessageBox error;
            error.setText("Ошибка в файле! Вершины должны быть пронумерованы исключительно числами!");
            error.exec();
        }

        File.close();
        //пытаюсь открыть файл eщё раз с начала
        if(File.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&File);
            ui->textBrowser->setText(in.readAll());
        }
        File.close();
    }
}

void MainWindow::on_pushAlgorithm_clicked()
{
    ui->pushFind->setDisabled(false);
    unsigned timer = static_cast<unsigned>(ui->spinBox->value());
    std::vector<int> VParent(static_cast<unsigned>(graph->getListOfNodeSize()), 0);
    std::vector<int> SDist(static_cast<unsigned>(graph->getListOfNodeSize()), INT_MAX);
    std::vector<bool> marked(static_cast<unsigned>(graph->getListOfNodeSize()), false);
    unsigned source = static_cast<unsigned>(ui->lineEdit_AlgSource->text().toInt());
    SDist[source] = 0;
    int dMin;
    unsigned vMin = 0, u = 0;
    for (int p=0; p<graph->getListOfNodeSize(); p++)
    {
        if (stop)
        {
            stop = false;
            return;
        }
        dMin = INT_MAX;
        for (unsigned i=0; i<SDist.size(); i++)
        {
            if (SDist[i] < dMin && marked[i] == false)
            {
                dMin = SDist[i];
                vMin = i;
            }
        }
        if (dMin == INT_MAX)
            return;
        marked[vMin] = true;
        graph->findNode(static_cast<int>(vMin))->setSelected(true);
        for (auto i : graph->findNode(static_cast<int>(vMin))->edges())
        {
            u = static_cast<unsigned>(i->destNode()->getIndex());
            for (int j=10; j<255; j+=10)
            {
                i->setColor(QColor(j, 0, 0));
                i->update();
                Sleep(timer/40);
                QCoreApplication::processEvents();
            }
            if (SDist[vMin] + i->getLength() < SDist[u])
            {
                for (int j=10; j<255; j+=10)
                {
                    i->setColor(QColor(255-j, j, 0));
                    i->update();
                    Sleep(timer/40);
                    QCoreApplication::processEvents();
                }
                SDist[u] = SDist[vMin] + i->getLength();
                graph->findNode(static_cast<int>(u))->setDist(SDist[u]);
                VParent[u] = static_cast<int>(vMin);
                QCoreApplication::processEvents();
                Sleep(timer/2);
                for (int j=0; j<255; j+=10)
                {
                    i->setColor(QColor(0, 255-j, 0));
                    i->update();
                    Sleep(timer/40);
                    QCoreApplication::processEvents();
                }
            }
            else {
                Sleep(timer/2);
                for (int j=0; j<255; j+=10)
                {
                    i->setColor(QColor(255-j, 0, 0));
                    i->update();
                    Sleep(timer/40);
                    QCoreApplication::processEvents();
                }
            }
        }
        graph->findNode(static_cast<int>(vMin))->setSelected(false);
    }
    shortPath = VParent;
}

void MainWindow::on_pushFind_clicked()
{
    for (auto i : graph->getEdges())
    {
        i->edge->setColor(QColor(0,0,0));
        i->edge->update();
    }
    unsigned timer = static_cast<unsigned>(ui->spinBox->value());
    int source = ui->lineEdit_AlgSource->text().toInt();
    for (int i=0; i<graph->getListOfNodeSize(); i++)
    {
        if (graph->findNode(i)->isSelected())
        {
            int p = i;
            //creating path
            while(p != source)
            {
                for (auto i : graph->getEdges())
                {
                    if (i->dest == p && i->source == shortPath[p])
                    {
                        for (int j=0; j<255; j+=10)
                        {
                            i->edge->setColor(QColor(j, 0, 0));
                            i->edge->update();
                            Sleep(timer/40);
                            QCoreApplication::processEvents();
                        }
                    }
                }
                Sleep(timer/2);
                p = shortPath[p];
            }
        }
    }
}


void MainWindow::on_pushStop_clicked()
{
    stop = true;
    foreach (Edges* curr, graph->getEdges()) // Окрашиваем все рёбра в серый цвет
    {
        curr->edge->setFlow(0);
        curr->edge->setColor(QColor(0, 0, 0, 255));
        curr->edge->setZValue(0);
        curr->edge->update();
    }
    for (int i=0; i<graph->getListOfNodeSize(); i++)
    {
        graph->findNode(i)->setDist(0);
    }
    ui->textBrowser->setText("Двойной клик левой кнопкой мыши - добавить новую вершину. \n\n"
                             "Двойной клик правой кнопкой мыши и клавища Shift - "
                             "соединить выбранные вершины с той, на которую кликал.\nЧтобы изменить размер ребра, повторить процесс как при добавлении ребра");
}

void MainWindow::on_action_Open_file_triggered()
{
    on_OpenButton_clicked();
}

void MainWindow::on_action_Save_File_triggered()
{
    on_saveButton_clicked();
}

void MainWindow::on_action_createNode_triggered()
{
    on_pb_CreateNode_clicked();
}

void MainWindow::on_action_createEdge_triggered()
{
    on_pb_CreateEdge_clicked();
}

void MainWindow::on_action_Algoritm_triggered()
{
    on_pushAlgorithm_clicked();
}

void MainWindow::on_action_delEdge_triggered()
{
    on_pb_DisconnectGraph_clicked();
}

void MainWindow::on_action_clear_all_triggered()
{
    on_pb_ClearAll_clicked();
}

void MainWindow::on_action_triggered()
{
    on_pb_Help_clicked();
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox about;
    QLabel *photo = new QLabel;
    QLabel *text = new QLabel;
    text->setText("Графический редактор графов | LR3: Алгоритм Дейкстры \nАвтор программы: Облизанов Александр, группа 8381 \n2020");
    photo->setPixmap(QPixmap(":/help/me.jpg"));
    about.layout()->addWidget(photo);
    about.layout()->addWidget(text);
    about.exec();
}
