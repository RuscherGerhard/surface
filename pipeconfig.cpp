#include "pipeconfig.h"
#include "ui_pipeconfig.h"

//c++ includes
//#include "utils.h"

//qt includes
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>


#include "mainwindow.h"

#define FILTERS "Input"<< "Box Filter"<< "GaussFilter"<<"Output"

PipeConfig::PipeConfig(MainWindow* mainWin, QWidget *parent) :
    _StartPointSet(false),
    QWidget(parent),
    _StartFilterSet(false),
    ui(new Ui::PipeConfig)
{

    _MainWin = mainWin;

    ui->setupUi(this);

    //_ConfigScene.setSceneRect(ui->graphicsView->rect());



    ui->graphicsView->setScene(&_ConfigScene);

    ui->graphicsView->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));

    _MakeItemList();

    _FilterItemVector.resize(0);





    //Connects
    connect(ui->AddFilterBtn, SIGNAL(pressed()), this, SLOT(OnBtnAddFilter()));
    connect(ui->DeleteItemBtn, SIGNAL(pressed()), this, SLOT(OnBtnRemoveItem()));
    connect(ui->ApplyPipeBtn, SIGNAL(pressed()), this, SLOT(OnBtnApplyConfig()));


    this->show();
    this->hide();
}


PipeConfig::~PipeConfig()
{
    delete ui;
}


void PipeConfig::_MakeItemList()
{
    _FilterList << FILTERS ;

    ui->FilterList->addItems(_FilterList);

    ui->FilterList->update();
}


FilterItem* PipeConfig::_GenerateFilterItem(FilterId id, const int posX, const int posY)
{
    Qt::GlobalColor GlaettungsFilterFarbe = Qt::red;


    FilterItem* item = nullptr;
    switch (id) {
    case OpInput:{item = new FilterItem("Input", OpInput ,Qt::white,this);}break;
    case OpOutput:{item = new FilterItem("Output", OpOutput ,Qt::white,this);}break;
    case OpBoxFilter:{item = new FilterItem("BoxFilter", OpBoxFilter ,GlaettungsFilterFarbe,this);}break;
    case OpGaussFilter: {item = new FilterItem("GaussFilter", OpGaussFilter ,GlaettungsFilterFarbe,this);}break;
    default:break;

    }

    if(posX != 0 || posY != 0)
    {
        QPointF Pos(posX, posY);
        item->setPos(Pos);
    }

    //In die Config Scene inserten
    _ConfigScene.addItem(item);
    //In meinen Vector inserten!
    _FilterItemVector.push_back(item);

    return item;
}

/////////
//SLOTS
/////////
void PipeConfig::OnBtnAddFilter()
{
   QList<QListWidgetItem*> selectedItems = ui->FilterList->selectedItems();

   //FilterItem* item = nullptr;
   QString selectedName = selectedItems[0]->text();
   if(!QString::compare(selectedName, QString("Box Filter")))
   {
        //Methode zum erzeugen eines QGraphicsItems für den Filter!!!!
        //item = new FilterItem(selectedName, OpBoxFilter ,Qt::red,this);
       _GenerateFilterItem(OpBoxFilter);
   }
   else if(!QString::compare(selectedName, QString("Input")))
   {
       //Methode zum erzeugen eines QGraphicsItems für den Filter!!!!
        //item = new FilterItem(selectedName, OpInput ,Qt::white,this);
       _GenerateFilterItem(OpInput);
   }
   else if(!QString::compare(selectedName, QString("Output")))
   {
       //Methode zum erzeugen eines QGraphicsItems für den Filter!!!!      
       //item = new FilterItem(selectedName, OpOutput ,Qt::white,this);
       _GenerateFilterItem(OpOutput);
   }
   else if(!QString::compare(selectedName, QString("GaussFilter")))
   {
       _GenerateFilterItem(OpGaussFilter);
   }


   //In die Config Scene inserten
   //_ConfigScene.addItem(item);
   //In meinen Vector inserten!
   //_FilterItemVector.push_back(item);
}



void PipeConfig::_RemoveFilter(QGraphicsItem* item)
{
   _ConfigScene.removeItem(item);
}


void PipeConfig::OnBtnRemoveItem()
{
    QList <QGraphicsItem*> items = _ConfigScene.items();



    //Wir suchen und bei Fund löschen wir das Markierte FilterItem!
    std::vector<FilterItem*>::iterator vIt;
    for(vIt = _FilterItemVector.begin(); vIt < _FilterItemVector.end(); vIt++)
    {
        QGraphicsItem* item = *vIt;
        if(item->isSelected())
        {
            _FilterItemVector.erase(vIt);
        }
    }

   //Wir Löschen das Item aus der GraphicsScene!
    QList<QGraphicsItem*>::iterator lIt;
    for(lIt = items.begin(); lIt < items.end(); lIt++)
    {
        QGraphicsItem* item = *lIt;

        if(item->isSelected())
        {
            _ConfigScene.removeItem(item);
            //Den Speicher für das zu löschende FilterItem frei geben!
            delete(item);
        }
    }
}


void PipeConfig::_Crawler(const int currentVectorIdx, FilterItem* currentItem, std::vector<std::vector<FilterId>>* userData)
{
    bool ifInput = false;
    int VectorIdx = currentVectorIdx;
    /*else*/if(currentItem->GetFilterId() == OpInput)
    {
        VectorIdx++;

        std::vector<FilterId> vec(0);

        userData->push_back(vec);
        ifInput = true;

    }


    FilterId currentId = currentItem->GetFilterId();

    userData->at(VectorIdx).push_back(currentId);

    if(currentId == OpOutput)
    {
        return;
    }
    else if(currentItem->getNachfolger() == nullptr)
    {
        userData->erase(userData->begin()+VectorIdx);
        return;
    }

    //Nicht löschen, wichtig für verzweigungen!!!!
    /*if(_NachFolger.size()>1)
    {
       //anfügen zusätzlicher Vektoren für die Verzweigungen
       size_t i = 0;
       if(ifIntput)
            i = 1;//falls wir gerade einen InputKnoten hatten, haben wir ja schon einen Zusätzliche Vektor....
                  //Brauchen also einen weniger!!!

       for(i < _NachFolger.size(); i++)
       {
           std::vector<FilterId> vector(0);
           userData.push_back(vector);
       }
    }*/

    else
    {
        //Do recursion
        _Crawler(VectorIdx, currentItem->getNachfolger(), userData);
    }



}

void PipeConfig::OnBtnApplyConfig()
{
    //QList<QGraphicsItem*>filterItems = _ConfigScene.items();

    std::vector<FilterItem*>::iterator fIIt;
    std::vector<FilterItem*> foo(0);

    //Die einzelnen Wurzeln finden
    for(fIIt = _FilterItemVector.begin(); fIIt < _FilterItemVector.end(); fIIt++)
    {
        FilterItem* item = reinterpret_cast<FilterItem*>(*fIIt);

        if(item->GetFilterId() == OpInput)
        {
            foo.push_back(reinterpret_cast<FilterItem*>(item));
        }
    }

    //über den Wald Crawlen
    assert(foo.size() > 0);//wenn die Länge des Vektors immernoch 0 ist, haben wir hier ein Problem!!!!

    std::vector<FilterItem*>::iterator it;
    std::vector<std::vector<FilterId>>* pipePlan = new std::vector<std::vector<FilterId>>(0);

    //int cnt = 0;
    for(it = foo.begin(); it < foo.end(); it++)
    {
        //Für jedes InputItem (Baumwurzel) den Crawler aufrufen./
        FilterId id = (*it)->GetFilterId();
        switch (id) {
        case OpInput:{
        _Crawler( static_cast<int>(pipePlan->size())-1, *it, pipePlan );
        } break;
        default: break;
        }


        //cnt++;
    }

    _MainWin->ApplyPipe(pipePlan);

    //Debug
    /*std::ofstream out;

    out.open("/home/gerdie/Desktop/SurfaceDebugOut.txt");
    if(out.is_open())
    {
        for(size_t i = 0; i < pipePlan->size(); i++)
        {
            size_t len = pipePlan->at(i).size();
            std::vector<FilterId> a_pipe = pipePlan->at(i);
            for(size_t j = 0; j < len; j++)
            {
                FilterId id = a_pipe[j];

                switch (id)
                {
                    case OpInput:{out << "Input | ";}
                    break;
                    case OpBoxFilter:{out << "BoxFilter | ";}
                    break;
                    case OpOutput:{out << "Output | \n";}
                    break;
                default:
                {out << "\n";}
                    break;
                }
            }
        }
        out.close();
    }*/


}



void PipeConfig::DisplayPipeConfig(std::vector<std::vector<FilterId> > *PipePlan)
{
    _ClearAllItems();

    int amountOfPipe = static_cast<int>(PipePlan->size());

    int graphicsviewHeight = static_cast<int>(ui->graphicsView->height());

    int graphicsviewWidth = static_cast<int>(ui->graphicsView->width());

    size_t AmountofFilterItems = 0;
    for(int n = 0; n < amountOfPipe; n++)
    {
        size_t pipeSize = PipePlan->at(n).size();
        if(AmountofFilterItems < pipeSize)
        {
            AmountofFilterItems = pipeSize;
        }
    }
    int maxAmountOfFilterItems = static_cast<int>(AmountofFilterItems);

    //Die Abstände zwischen den FilterItems auf in der Graphcsview bestimmen!
    int aVertStep = static_cast<int>(graphicsviewHeight / amountOfPipe);//Der vertikale Schritt auf der y-Achse, in dem die Pipes positioniert werden!
    int aHorStep = graphicsviewWidth / maxAmountOfFilterItems;


    //Startposition ausrechnen
    int initPosY = (int)graphicsviewHeight / 2;
    int initPosX = -(int)graphicsviewWidth / 2;

    int posX = 0;
    int posY = 0;

    for(size_t i = 0; i < PipePlan->size(); i++)
    {
        FilterItem* pre = nullptr;
        FilterItem* post = nullptr;

        posX = initPosX;
        for(int j = 0; j < PipePlan->at(i).size(); j++)
        {
            //Die Filterknoten der Pipe erzeugen!
            FilterId id = PipePlan->at(i).at(j);

            post = _GenerateFilterItem(id, posX, posY);

            if(j > 0)
            {
                //Die Kantendarstellungen erzeugen

                QLineF line(pre->getOutputCoord(), post->getInputCoord());

                Kante* edge = new Kante(pre, post, line);

                _ConfigScene.addItem(edge);
            }

            pre = post;

            posX = posX + aHorStep;
        }
          posY = posY + aVertStep;

    }
}

void PipeConfig::_ClearAllItems()
{
    std::vector<FilterItem*>::iterator fit;
    for(fit = _FilterItemVector.begin(); fit < _FilterItemVector.end(); fit++)
    {
        _FilterItemVector.erase(fit);
    }
    _FilterItemVector.resize(0);
     //_ConfigScene.items();


    size_t listSize = _ConfigScene.items().size();
    for(size_t i = 0; i < listSize; i++)
    {
        QGraphicsItem* item = _ConfigScene.items()[0];
        _ConfigScene.removeItem(item);
        delete(item);
    }
}

/////////////////////
//Klasse FilterItem
////////////////////////
FilterItem::FilterItem(const QString text, const FilterId id, const Qt::GlobalColor color, QWidget *parent):
    _id(id),
    _Nachfolger(nullptr),
    _Vorgaenger(nullptr),
    _color(color),
    angle(0.0),
    selected(false),
    _Verbindung1(nullptr),
    _Verbindung2(nullptr)

{

    this->setParent(parent);
    this->setPlainText(text);
    this->setFlag(ItemIsMovable, true);
    this->setFlag(ItemIsSelectable, true);

    QRectF rect = boundingRect();

    rect.setWidth(rect.width()+40);


    int inOutWidth = 10;
    int inOutHeight = 8;

    QPointF inputStart(0.0,(boundingRect().height()/2)-(inOutHeight/2));
    QPointF outputStart(boundingRect().width()-inOutWidth,(boundingRect().height()/2)-(inOutHeight/2));
    _inputSource  = QRectF(inputStart,QSize(inOutWidth,inOutHeight));
    _outputSource = QRectF(outputStart, QSize(inOutWidth,inOutHeight));


    qreal inwidth = _inputSource.width();
    qreal inheight = _inputSource.height();
    qreal outwidth = _outputSource.width();
    qreal outheight = _outputSource.height();

    qreal inPosX = _inputSource.x();
    qreal inPosY = _inputSource.y();
    qreal outPosX = _outputSource.x();
    qreal outPosY = _outputSource.y();


    _inputCoord.setX(inPosX);
    _inputCoord.setY(inPosY+(inheight/2));
    _outputCoord.setX(_outputSource.x()+outwidth);
    _outputCoord.setY(_outputSource.y()+outheight/2);


}

FilterItem::~FilterItem()
{
    PipeConfig* conf = static_cast<PipeConfig*>(parent());
    if(_Verbindung1 != nullptr)
    {
    conf->_ConfigScene.removeItem(_Verbindung1);
    delete(_Verbindung1);
    _Verbindung1 = nullptr;
    }

    if(_Verbindung2 != nullptr)
    {
    conf->_ConfigScene.removeItem(_Verbindung2);
    delete(_Verbindung2);
    _Verbindung2 = nullptr;
    }
}


QRectF FilterItem::boundingRect()
{
    return QGraphicsTextItem::boundingRect();
}


QPainterPath FilterItem::shape()
{
    return QGraphicsTextItem::shape();
}


void FilterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(_color);

    if(selected)
    {
        painter->setPen(Qt::blue);
    }
    else
    {
        painter->setPen(Qt::green);
    }

    painter->drawRect(boundingRect());

    painter->setBrush(Qt::black);

    painter->drawRect(_inputSource);
    painter->drawRect(_outputSource);

    QGraphicsTextItem::paint(painter, option, widget);
}


void FilterItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    selected = true;

    QPointF clickPos = event->pos();

    qreal inwidth = _inputSource.width();
    qreal inheight = _inputSource.height();
    qreal outwidth = _outputSource.width();
    qreal outheight = _outputSource.height();

    qreal inPosX = _inputSource.x();
    qreal inPosY = _inputSource.y();
    qreal outPosX = _outputSource.x();
    qreal outPosY = _outputSource.y();

    PipeConfig* conf = reinterpret_cast<PipeConfig*>(parent());


    //hier wurde in den Input geclickt
    if(clickPos.rx() > inPosX && clickPos.rx() < (inwidth+inPosX))
    {
        if(clickPos.ry() > inPosY && clickPos.ry() < (inheight+inPosY))
        {
            //Falls ich einen Start(output) habe, dann zeichne ich meine Linie
            if(conf->getStartPointSet() && conf->getStartFilterSet())
            {
                //Falls wir schon einen vorgänger haben, dann setzen wir hier keine neue Kante zu einem Vorgänger ein!!!
                if(_Vorgaenger != nullptr)
                    return;

                //Linie Zeichnen
                _inputCoord.setX(inPosX);
                _inputCoord.setY(inPosY+(inheight/2));

                QPointF end = this->mapToScene(_inputCoord);
                QGraphicsScene* scene = &conf->_ConfigScene;
                QPointF start = conf->getStartPoint();
                QLineF line(start, end);

                Kante* _Verbindung1 = new Kante(conf->getStartFilter(), this, line);

                //Item zur Scene hinzufügen und Scene updaten
                scene->addItem(_Verbindung1);
                scene->update();
                conf->resetStartPoint();
            }
        }
    }

    //Hier wurde in den Output eines Items geclickt
else if(clickPos.rx()> outPosX && clickPos.rx() < (outwidth+outPosX))
    {
        if(clickPos.ry() > outPosY && clickPos.ry() < (outheight+outPosY))
        {
            //Wir erlauben zunächst auch hier nicht mehrfache nachfolger!
            if(_Nachfolger != nullptr)
                return;

            _outputCoord.setX(_outputSource.x()+outwidth);
            _outputCoord.setY(_outputSource.y()+outheight/2);
            QPointF start = this->mapToScene(_outputCoord);
            conf->setStartPoint(start);
            conf->setStartFilter(this);
        }
    }

    QGraphicsTextItem::mousePressEvent(event);
}

void FilterItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsTextItem::mouseMoveEvent(event);
}


void FilterItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    selected = false;
    QGraphicsTextItem::mouseReleaseEvent(event);
}

QPointF FilterItem::getInputCoord()
{
    qreal inheight = _inputSource.height();

    qreal inPosX = _inputSource.x();
    qreal inPosY = _inputSource.y();

    _inputCoord.setX(inPosX);
    _inputCoord.setY(inPosY+(inheight/2.0));

    QPointF end = this->mapToScene(_inputCoord);


    return end;
}

QPointF FilterItem::getOutputCoord()
{
    qreal outwidth = _outputSource.width();
    qreal outheight = _outputSource.height();

    _outputCoord.setX(_outputSource.x()+outwidth);
    _outputCoord.setY(_outputSource.y()+outheight/2.0);
    QPointF start = this->mapToScene(_outputCoord);

    return start;
}

///////////////////////////////////////////////
//  Klasse Kante
///////////////////////////////////////////////

Kante::Kante(FilterItem* pre, FilterItem* post, const QLineF &line):
_id(Edge),
_pre(pre),
_post(post),
  _selected(false)

{
    _id = Edge;

    this->setFlag(ItemIsSelectable, true);

    this->setLine(line);

    this->setPen(QPen(Qt::blue));


    _pre->setNachfolger(post);
    _pre->setVerbindung2(this);
    _post->setVorgaenger(pre);
    _post->setVerbindung1(this);
}

Kante::~Kante()
{
    _pre->deleteVerbindung2();
    _pre->deleteNachfolger();
    _post->deleteVerbindung1();
    _post->deleteVorgaenger();

}

QRectF Kante::boundingRect()
{
    return QGraphicsLineItem::boundingRect();
}

QLineF Kante::line()
{
    return QGraphicsLineItem::line();
}

QPainterPath Kante::shape()
{
    return QGraphicsLineItem::shape();
}

void Kante::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(_selected)
    {
        this->setPen(QPen(Qt::red));
    }
    else
    {
        this->setPen(QPen(Qt::blue));
    }

    QPointF postPos = _post->getInputCoord();
    QPointF prePos = _pre->getOutputCoord();


    QLineF line;
    line.setP1(prePos);
    line.setP2(postPos);
    this->setLine(line);


    QGraphicsLineItem::paint(painter, option, widget);
}

void Kante::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _selected = true;
    QGraphicsLineItem::mousePressEvent(event);
}

void Kante::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsLineItem::mouseMoveEvent(event);
}

void Kante::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsLineItem::mouseReleaseEvent(event);
}

void Kante::disolveEdge()
{
    _pre->deleteNachfolger();
    _post->deleteVorgaenger();
}

/*SceneItem::SceneItem(const FilterId id):
    _filterId(id)
{
}

SceneItem::~SceneItem(){}
*/
/////////////////////////////////
//  Klasse SceneItem
/////////////////////////////////

/*SceneItem::SceneItem()
{
    id = Undefined;
    this->setFlag(ItemIsSelectable, true);
}

QRectF SceneItem::boundingRect()
{
    return QGraphicsItem::boundingRect();
}

QPainterPath SceneItem::shape()
{
    return QGraphicsItem::shape();
}

FilterId SceneItem::GetFilterId()
{
    return id;
}

void SceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsItem::paint(painter, option, widget);
}*/
