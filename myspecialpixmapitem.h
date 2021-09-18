#ifndef MYSPECIALPIXMAPITEM_H
#define MYSPECIALPIXMAPITEM_H
#include <QGraphicsPixmapItem>

class MainWindow;

class MySpecialPixMapItem : public QGraphicsPixmapItem
{
public:
    MySpecialPixMapItem(QPixmap &map, const unsigned long Id,MainWindow* winMain);


    virtual QRectF boundingRect();

    void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief GetId: returnt die Id.
     * @return unsigend int ID, der wert des Members _MyId.
     */
    unsigned int GetId(){return _MyId;}

    QPixmap* getPixmap(){return &_map;}

private:
    MainWindow* _MainWin;
    unsigned int _MyId;

    QPixmap _map;

};


#endif // MYSPECIALPIXMAPITEM_H
