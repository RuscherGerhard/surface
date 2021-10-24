#include "myspecialpixmapitem.h"
#include <mainwindow.h>

////////////////////////////////////////////////////////////
//  Klasse MySpecialPixMapItem
///////////////////////////////////////////////////////////

//C-TOR
MySpecialPixMapItem::MySpecialPixMapItem(QPixmap &map, const unsigned long Id, MainWindow* winMain):
    QGraphicsPixmapItem(map),
    _map(map),
    _MyId(Id),
    _MainWin(winMain)
{
}

//MousePressEvent
void MySpecialPixMapItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    //Hier die AnzeigenMethode für den HauptView anwerfen mit der entsprechenden Id!!!!
    _MainWin->_SwitchImgInMainView(_MyId);
}

void MySpecialPixMapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /*if(isSelected())
    {
        painter->setPen(Qt::green);
    }
    else
    {
        painter->setPen(Qt::blue);
    }

    painter->drawRect(boundingRect());

    painter->setBrush(Qt::black);*/

    QGraphicsPixmapItem::paint(painter, option, widget);
}



//D-TOR
//MySpecialPixMapItem::~MySpecialPixMapItem(){}


//Überschriebene Methoden
QRectF MySpecialPixMapItem::boundingRect()
{
    return QGraphicsPixmapItem::boundingRect();
}

