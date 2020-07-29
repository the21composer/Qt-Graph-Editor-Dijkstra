#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include<QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

int Node::getDist() const
{
    return dist;
}

void Node::setDist(int value)
{
    dist = value;
    update();
}

Node::Node(GraphWidget *graphWidget): graph(graphWidget)
{
    setFlags(ItemIsSelectable | ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(+1);
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Node::removeEdge(Edge *deletedEdge)
{
    edgeList.removeAll(deletedEdge);
}

QList<Edge *> Node::edges()
{
    return edgeList;
}

int Node::getIndex()
{
    return index;
}

void Node::setIndex(int index)
{
    this->index = index;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 20;
    return QRectF( -15 - adjust, -15 - adjust, 33 + adjust, 33 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-15, -15, 30, 30);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    // Fill
    if (option->state & QStyle::State_Sunken)//затонувший
    {
        painter->setBrush(QColor(254, 254, 34, 255));
    }
    else if (option->state & QStyle::State_Selected)//выделение вершины
    {
        painter->setBrush(QColor(254, 254, 34, 255));
    }
    else
    {
        painter->setBrush(QColor(191, 255, 148, 255));
    }

    painter->setPen(QPen(QColor(130, 180, 130, 255), 3));
    painter->drawEllipse(-15, -15, 30, 30);
    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Roboto", 9));
    setZValue(1300);
    QString text = QString::number(index);
    if (index < 10)
    {
        painter->drawText(-4, 5, text);
    }
    else
    {
        painter->drawText(-8, 5, text);
    }
    text = QString::number(dist);
    painter->setFont(QFont("Roboto", 10, 100));
    painter->setPen(QPen(Qt::red));
    painter->drawText(-20, -20, text);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem::contextMenuEvent(event);
    if (!this->isSelected())
        return;
    QMenu menu;
    QAction *new_val = menu.addAction("Изменить пропускную способность");
    menu.addSeparator();
    QAction *selected_action = menu.exec(event->screenPos());
    if(selected_action == new_val)
    {
        QMessageBox msgBox;
        QSpinBox *dest = new QSpinBox();
        QSpinBox *length = new QSpinBox();
        length->setRange(0, 10000);
        dest->setRange(0, graph->getEdges().size());
        msgBox.layout()->addWidget(dest);
        msgBox.layout()->addWidget(length);
        msgBox.exec();
        int destNum = dest->value();
        int lengthNum = length->value();
        for (auto i : edges())
            if (i->destNode()->getIndex() == destNum)
            {
                i->setLength(static_cast<unsigned>(lengthNum));
            }
        update();
     }
     setSelected(false);
}

