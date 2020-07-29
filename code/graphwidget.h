#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QList>
#include "edge.h"
#include "node.h"

struct Edges   //хорощо бы от неё избавиться!
{
    Edge *edge;
    int source;
    int dest;
    int length;
};

class GraphWidget : public QGraphicsView // Виджет для рисования вершин и рёбер
{
    Q_OBJECT

public:
    QGraphicsScene *scene; // Окно, на котором рисуются все элементы
    QList<Node *> listOfNode; // Список вершин, отображающихся на окне

    Node* findNode(int val/*,QList<Node *> listOfNode*/);//по номеру надо найти вершину

    explicit GraphWidget(QWidget *parent = 0);//Конструктор
    QPointF getPosOfNode(int index);// Возвращает местоположение вершины с номером index
    int getListOfNodeSize();//Возвращает количество вершин
    void changeIndecesOfAllVerteces();//Переименовывет вершины после удаления некоторых вершин
    QVector<Edges *> getEdges();

    void addNode();
    void deleteEdge(Edge *edge);
    Node* addNode1(QPointF position,int t);
    Edge *addEdge(Node *source, Node *dest, unsigned length);
    void addEdge(Node *source, Node *dest);
    void deleteAllItems();//Удаляет все вершины
    bool createGraphFromInc(QString temp);
    QString add_spaces_and_simplifie(QString str_for_work);

public slots:
    void deleteSelectedItems();//Удаляет выбранные вершины

protected:
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
    void scaleView(qreal scaleFactor);
    void drawBackground(QPainter *painter, const QRectF &rect);// Отрисовка фона
    void resizeEvent(QResizeEvent *event);//Вызывается при изменении размера окна
    void mouseDoubleClickEvent(QMouseEvent *event);// Отслеживание двойного клика мышкой
};

#endif // GRAPHWIDGET_H
