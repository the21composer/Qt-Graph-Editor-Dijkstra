#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>
#include "iostream"
#include "graphwidget.h"
#include "QFile"
#include "QTextStream"
#include "QFileDialog"
#include "QMessageBox"
#include <QSettings>   //Для сохранения настроек
#include <limits>

namespace Ui {   //   ??
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

//    QAction *exitAction;
//    QAction *deleteAction;
//??    QAction *openFile;
//    QAction *saveFile;
//    QAction *aboutQtAct;

    GraphWidget *graph;
    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void writeSettings();//Для сохранения настроек
    void readSettings ();//Для восстановления настроек

    std::pair<bool,std::vector<int> > DFS(int from, int to, std::vector<int> marked, unsigned timer);

signals:
    void changeOpenFile();

private:
    bool stop;
    QSettings  m_settings;//Для сохранения настроек
    QString InputPath ="";
    std::vector<int> shortPath;

private slots:        
    void deleteNode();// Удаляет выбранные вершины графа
    void on_pb_CoordinatesOfAllNode_clicked();// Выписывает координаты всех вершин графа
    void on_pb_LengthOfEdges_clicked();// Выписывает длину всех рёбер графа
    void on_pb_ConnectGraph_clicked();// Сделать граф полным
    void on_pb_DisconnectGraph_clicked();// Удалить все рёбра в графе
    void on_OpenButton_clicked();  //Открыть форматированный файл с графом
    void on_saveButton_clicked();  // Сохранить форматированный граф в файл
    void on_pb_RemoveEdges_clicked();//Удалить рёбра delete
    void on_pb_DeleteSelectedNodes_clicked();//delete
    void on_pb_CreateNode_clicked();//создаем вершину
    void on_pb_CreateEdge_clicked();//Создаём дугу
    void on_pb_ClearAll_clicked();//Очистить всё
    void on_pb_Help_clicked();  //HelpBrowser
    void on_pb_IncidenceListFile_clicked();  // Создание графа из списка инцидентности

    void on_pushAlgorithm_clicked();
    void on_pushStop_clicked();
    void on_action_Open_file_triggered();
    void on_action_Save_File_triggered();
    void on_action_createNode_triggered();
    void on_action_createEdge_triggered();
    void on_action_Algoritm_triggered();
    void on_action_delEdge_triggered();
    void on_action_clear_all_triggered();
    void on_action_triggered();
    void on_action_About_triggered();
    void on_pushFind_clicked();
};

#endif // MAINWINDOW_H
