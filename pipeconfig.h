#ifndef PIPECONFIG_H
#define PIPECONFIG_H

#include <QWidget>
#include <qstringlist.h>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPointF>
#include <vector>


#include "utils.h"


class MainWindow;

namespace Ui {
class PipeConfig;
}

/**SceneItem**/
/*class SceneItem : public QGraphicsItem
{
public:
    SceneItem();
    //virtual ~SceneItem();

    virtual FilterId GetFilterId();


    //void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;

    virtual QRectF boundingRect();
    virtual QPainterPath shape();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)override;
protected:
    FilterId id;

};*/

class Kante;

/**FilterItem**/
class FilterItem: public QGraphicsTextItem
{
public:
    FilterItem(const QString text, const FilterId id, const Qt::GlobalColor color, QWidget *parent);
    ~FilterItem()override;

    QRectF boundingRect();
    QPainterPath shape();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event)override;



    void setVorgaenger(FilterItem* vorgaenger){_Vorgaenger = vorgaenger;}
    void setNachfolger(FilterItem* nachfolger){_Nachfolger = nachfolger;}
    void setVerbindung1(Kante* Verbindung){_Verbindung1 = Verbindung;}
    void setVerbindung2(Kante* Verbindung){_Verbindung2 = Verbindung;}


    void deleteVorgaenger(){if(_Vorgaenger != nullptr)_Vorgaenger = nullptr;}
    void deleteNachfolger(){if(_Nachfolger != nullptr)_Nachfolger = nullptr;}
    void deleteVerbindung1(){if(_Verbindung1 != nullptr)_Verbindung1 = nullptr;}
    void deleteVerbindung2(){if(_Verbindung2 != nullptr)_Verbindung2 = nullptr;}

    QPointF getInputCoord();
    QPointF getOutputCoord();

    FilterId GetFilterId(){return _id;}

    FilterItem* getVorgaenger(){return _Vorgaenger;}
    FilterItem* getNachfolger(){return _Nachfolger;}

    void SetBesucht(const bool besuch){_besucht = besuch;}
private:

    bool _besucht;

    FilterItem* _Nachfolger;
    FilterItem* _Vorgaenger;
    Kante* _Verbindung1;//Kannte zum Vorgänger
    Kante* _Verbindung2;

    QPointF _inputCoord;
    QPointF _outputCoord;

    QRectF _inputSource;
    QRectF _outputSource;
    FilterId _id;
    Qt::GlobalColor _color;

    qreal angle;
    bool selected;

    QPainter painter;
    QStyleOptionGraphicsItem option;
};


/**Klasse Kante*/
class Kante: public QGraphicsLineItem
{
public:
    Kante(FilterItem* pre, FilterItem* post, const QLineF &line);
    ~Kante();

    QRectF boundingRect();
    QPainterPath shape();
    QLineF line();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    void setPost(FilterItem* post){_post = post;}
    void setPre(FilterItem* pre){_pre = pre;}
    //FilterId getItemID(){return GetFilterId();}
    virtual FilterId GetFilterId(){return _id;}

    void disolveEdge();

    private:
    FilterId _id;
    FilterItem* _pre;
    FilterItem* _post;
    bool _selected;

};


/**PipeConfig**/
class PipeConfig : public QWidget
{
    Q_OBJECT

    friend class FilterItem;

public:
    explicit PipeConfig(MainWindow* mainWin, QWidget *parent = nullptr);
    ~PipeConfig();


protected:
        QGraphicsScene _ConfigScene;
        void setStartPoint(QPointF start){_startPoint = start; _StartPointSet = true;}
        void resetStartPoint(){_startPoint.rx()=0.0; _startPoint.ry() = 0.0; _StartPointSet = false;}
        QPointF getStartPoint(){return _startPoint;}
        bool getStartPointSet(){return _StartPointSet;}

        void setStartFilter(FilterItem* filterItem){_StartFilter=filterItem; _StartFilterSet = true;};
        void resetStartFilter(){_StartFilter = 0;  _StartFilterSet = false;}
        FilterItem* getStartFilter(){return _StartFilter;}
        bool getStartFilterSet(){return _StartPointSet;}

private:
    MainWindow* _MainWin;
    bool _StartPointSet;
    QPointF _startPoint;
    bool _StartFilterSet;
    FilterItem* _StartFilter;

    Ui::PipeConfig *ui;

    QStringList _FilterList;///< wird in der Oberfläche verwendet, in der Auswahlliste für



    std::vector<FilterItem*> _FilterItemVector;//Speichert Zeiger jedes FilterItem, das in die _ConfigScene eingefügt wurde!

    void _MakeItemList();

    //void _CreateFilterItem(const QString name, const FilterId id, const Qt::GlobalColor color);


    void _RemoveFilter(QGraphicsItem* item);

    void _Crawler(const int currentVectorIdx, FilterItem* currentItem, std::vector<std::vector<FilterId>>* userData);
private slots:
    void OnBtnAddFilter();
    void OnBtnRemoveItem();

    /**
     * @brief OnBtnApplyConfig: Die Methode liest alle dargestellten Pipes aus und erstellt eine Pipe Tabelle, die er dann an den
     *                          PipeManager weitergibt!
     *
     */
    void OnBtnApplyConfig();

};



#endif // PIPECONFIG_H
