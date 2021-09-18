#include "resultsview.h"
#include <myspecialpixmapitem.h>
#include <mainwindow.h>

resultsView::resultsView(QWidget* parent, MainWindow* MainWin): QGraphicsView (parent), _MainWin(MainWin)
{
    _MyResultsSecene = new QGraphicsScene();
    this->setScene(_MyResultsSecene);
}

resultsView::~resultsView(){};


void resultsView::CleanupResultPixMaps()
{
    size_t len = this->items().size();
    for(size_t j=0; j < len; j++)
    {

        _MyResultsSecene->removeItem(this->items()[0]);
    }


    for(size_t i = 0; i <  _ResultPixMaps.size(); i++)
    {
        delete (_ResultPixMaps[i]);

        _ResultPixMaps[i] = nullptr;
    }


}


void resultsView::AddResultsToScene(QImage* MyImage, std::vector<std::vector<QImage*>*>* results)
{
    //Falls wir einen Vector zurückbekommen haben, also falls tatsächlich eine Pipe aufgebaut wurde!!!!
    if(results != nullptr)//Wenn NULL dann keine Pipe, klar! //die Abfrage kann theoretisch Extern abgefrühstückt werden!!!!
    {
        unsigned int resSize = results->at(0)->size();

        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(resSize+1);

        unsigned int id = 1;

        int resVHeight = this->height()/5;
        int resVWidth = this->width();

        for(size_t j=0; j < resSize; j++)
        {
            //Das Item basteln
            QPixmap map = QPixmap::fromImage(*results->at(0)->at(j));
            MySpecialPixMapItem* item = new MySpecialPixMapItem(map, id +j,_MainWin);

            //In die ResultsMap adden, unskalliert!!!!
            _ResultPixMaps[j] = item;

            //Scaled Items für die results view basteln (skallierte items)            
            QPixmap scaledMap = QPixmap::fromImage(*results->at(0)->at(j)).scaled(resVWidth, resVHeight, Qt::IgnoreAspectRatio);
            //scaledMap = scaledMap.scaled(resVWidth, resVHeight, Qt::KeepAspectRatio);
            MySpecialPixMapItem* scaledItem = new MySpecialPixMapItem(scaledMap, id +j, _MainWin);

            scaledItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

            scaledItem->setPos(0, scaledMap.height()*(j+1) +10);

            //In die Scene adden!
            _MyResultsSecene->addItem(scaledItem);
        }
    }
    else
    {
        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(1);
    }


    //Das Item basteln
    QPixmap map = QPixmap::fromImage(*MyImage);
    MySpecialPixMapItem* item = new MySpecialPixMapItem(map, ORIGINAL_IMG, _MainWin);
    _ResultPixMaps[_ResultPixMaps.size()-1] = item;


    QPixmap scaledMap = map.scaled(this->width(), this->height()/5, Qt::KeepAspectRatio);
    MySpecialPixMapItem* scaledItem = new MySpecialPixMapItem(scaledMap, ORIGINAL_IMG, _MainWin);
    scaledItem->setPos(0,0);
    scaledItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

    //In die ResultsMap adden
    _MyResultsSecene->addItem(scaledItem);

}

MySpecialPixMapItem* resultsView::SearchInResultsPixmaps(const unsigned int &Id)
{
    MySpecialPixMapItem* returnItem = nullptr;

    //Neues Item erzeugen!
    for(size_t i = 0; i < _ResultPixMaps.size(); i++)
    {
        unsigned int lid = _ResultPixMaps[i]->GetId();
        if(lid == Id)
        {
            returnItem = _ResultPixMaps[i];
        }
    }

    return returnItem;
}
