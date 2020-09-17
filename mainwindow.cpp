#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFileDialog>
#include <QTextBrowser>
#include <utils.h>

//#include <QGraphicsSceneMouseEvent>
//#include <QGraphicsItem>

#include "mainifc.h"


MainWindow::MainWindow( QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    MyImage(nullptr),
    MyItem(nullptr),
    _HistoOpen(false)
{

    ui->setupUi(this);

    _MainIfc = nullptr;

    _ResultPixMaps.resize(0);

    _SetupGui();

    _CreateMenus();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete(_PipeConfig);
}

void MainWindow::_SetupGui()
{
    //Hauptanzeige

     MyView = ui->graphicsView; //ui->MainView;

     MyScene  = new QGraphicsScene();
     MyView->setScene(MyScene);



     //Resultatauswahlfeld
     MyResultsView = ui->ResultsView;

     MyResultsScene = new QGraphicsScene();
     MyResultsView->setScene(MyResultsScene);


   ui->ErrorLabel->setText(tr("No Image loaded."));
}


void MainWindow::_CreateMenus()
{
    //Die QActions basteln

    //File Menu erzeugen
    QMenu* File = new QMenu(tr("File"));

    //die zur FileMenu zugehörigen Actions erzeugen
    QAction* Load = new QAction(tr("Load"));
    QAction* Save = new QAction(tr("Save"));
    QAction* Exit = new QAction(tr("Exit"));


    File->addAction(Load);
    File->addAction(Save);
    File->addAction(Exit);
    File->setVisible(true);

    ui->menuBar->addMenu(File);

    QMenu* Project = new QMenu(tr("Project"));
    QAction* PipeConf = new QAction(tr("Pipe Config"));
    QAction * LoadImage = new QAction(tr("Load Image"));

    Project->addAction(LoadImage);
    Project->addAction(PipeConf);
    ui->menuBar->addMenu(Project);

    //Debug Menü aufsetzen
    QAction* Log = new QAction(tr("View Log File"));
    QAction* Histo = new QAction(tr("Histogram View"));
    QMenu* Debug = new QMenu("Debug");
    Debug->addAction(Log);
    Debug->addAction(Histo);
    ui->menuBar->addMenu(Debug);



    //PipeConfig aufsetzen

    _PipeConfig = new PipeConfig(this);
    _PipeConfig->hide();

    _LogViewer = new LogView();
    _LogViewer->hide();

    _Histo = new Histogram();



    //Connects
    connect(LoadImage, SIGNAL(triggered()), this, SLOT(_OnMenuBtnLoadImg()));
    connect(PipeConf, SIGNAL(triggered()), this, SLOT(_OnMenuBtnPipeConfig()));
    connect(Load, SIGNAL(triggered()), this, SLOT(_OnBtnLoad()));
    connect(Save, SIGNAL(triggered()), this, SLOT(_OnBtnSave()));
    connect(Log, SIGNAL(triggered()), this, SLOT(_OnMenuBtnLog()));
    connect(Histo, SIGNAL(triggered()), this, SLOT(_OnMenuBtnHistogram()));
    connect(_Histo, SIGNAL(HistoClosed()), this, SLOT(_OnHistoClosed()));

}




void MainWindow::_CleanupResultPixMaps()
{
    size_t len = MyResultsScene->items().size();
    for(size_t j=0; j < len; j++)
    {

        MyResultsScene->removeItem(MyResultsScene->items()[0]);
    }


    for(size_t i = 0; i <  _ResultPixMaps.size(); i++)
    {
        delete (_ResultPixMaps[i]);

        _ResultPixMaps[i] = nullptr;
    }

}

void MainWindow::_AddResultsToScene()
{
    //Wenn wir noch kein Bild zum bearbeiten geladen haben, dann hier Abbrechen!
    if(MyImage == nullptr)
        return;

    //Aufräumen
    _CleanupResultPixMaps();

    //Mögliche results in die Scene inserten!
    std::vector<std::vector<QImage*>*>* results = _MainIfc->ProcessImage(MyImage);


    //Falls wir einen Vector zurückbekommen haben, also falls tatsächlich eine Pipe aufgebaut wurde!!!!
    if(results != nullptr)
    {
        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(results->at(0)->size()+1);

        int id = 1;

        for(size_t j=0; j < results->at(0)->size(); j++)
        {
            //Das Item basteln
            QPixmap map = QPixmap::fromImage(*results->at(0)->at(j));
            MySpecialPixMapItem* item = new MySpecialPixMapItem(map, id +j,this);

            //In die ResultsMap adden nicht skalliert!!!!
            _ResultPixMaps[j] = item;

            //Scaled Items für die results view basteln (skallirte items)
            QPixmap scaledMap =map.scaled(MyResultsView->width(), MyResultsView->height()/5, Qt::KeepAspectRatio);
            MySpecialPixMapItem* scaledItem = new MySpecialPixMapItem(scaledMap, id +j, this);

            scaledItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

            scaledItem->setPos(0, scaledMap.height()*(j+1) +10);

            //In die Scene adden!
            MyResultsScene->addItem(scaledItem);
        }
    }
    else
    {
        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(1);
    }

    //Das Item basteln
    QPixmap map = QPixmap::fromImage(*MyImage);
    MySpecialPixMapItem* item = new MySpecialPixMapItem(map, ORIGINAL_IMG, this);
    _ResultPixMaps[_ResultPixMaps.size()-1] = item;


    QPixmap scaledMap = map.scaled(MyResultsView->width(), MyResultsView->height()/5, Qt::KeepAspectRatio);
    MySpecialPixMapItem* scaledItem = new MySpecialPixMapItem(scaledMap, ORIGINAL_IMG, this);
    scaledItem->setPos(0,0);
    scaledItem->setFlag(QGraphicsItem::ItemIsSelectable, true);

    //In die ResultsMap adden
    MyResultsScene->addItem(scaledItem);

}



//Das geladene Bild in eine Pixmap packen und anzeigen!
void MainWindow::DisplayImage(MySpecialPixMapItem* Img)
{
    //Aufräumen
    if(MyItem != nullptr)
    {
        //falls ein altes Bild noch in der Scene vorhanden ist, löschen!
        MyScene->removeItem(MyItem);
        //delete(MyItem);
        MyItem = nullptr;
    }



    //Show Image in Main View
    MyItem = Img;

    //Die Dimension der Scene resizen!(Notwendig, sonst wird ständig FitInView gemacht, falls das erste Bild größer war als die View, die restlichen geladenen Bilder aber kleiner sind!)
    QRectF rect = MyItem->boundingRect();
    MyScene->setSceneRect(rect);

    //Bild einfügen!
    MyScene->addItem(MyItem);

    //Wenn die Dimensionen der Scene jene der View übersteigen, Anzeige anpassen!
    MyView->fitInView(MyScene->sceneRect(), Qt::KeepAspectRatio);

    //Die ResultsView bearbeiten!
    //_AddResultsToScene();

    MyView->show();
    ui->ErrorLabel->setText("Image loaded!");

    if(_HistoOpen)
    {
        _OpenHistogram(MyItem->getPixmap());
    }
}


void MainWindow::ApplyPipe(std::vector<std::vector<FilterId> > *pipePlan)
{

    //Die Pipeblaupause an den PipeManager weiter leiten!!
    _MainIfc->ForwardPipePlanToPipeManager(pipePlan);

    _AddResultsToScene();
    MyView->show();

}


void MainWindow::_SwitchImgInMainView(const unsigned int Id)
{

    bool idFound = false;

    MySpecialPixMapItem* itemToShow;

    //Neues Item erzeugen!
    for(size_t i = 0; i < _ResultPixMaps.size(); i++)
    {
        unsigned int lid = _ResultPixMaps[i]->GetId();
        if(lid == Id)
        {
            idFound = true;
            itemToShow = _ResultPixMaps[i];

            ui->ErrorLabel->setText("Found corresponding Image");
        }
    }

    if(idFound)
    {
        //Das gefundene Bild anzeigen
        DisplayImage(itemToShow);


    }
}



void MainWindow::_PrepareForNewImage(QImage *newImage, const QString &fileName)
{
    //Aufräumen
    if(MyImage != nullptr)
    {
        delete(MyImage);
    }

    //Das aktuelle Bild merken
    MyImage = newImage;

    //Den Pfad zum aktuellen Bild merken!
    MyImageAddress.clear();
    MyImageAddress.append(fileName);

    QPixmap map = QPixmap::fromImage(*newImage);
    MySpecialPixMapItem* item = new MySpecialPixMapItem(map, ORIGINAL_IMG, this);

    DisplayImage(item);

    _AddResultsToScene();

}


void MainWindow::_OpenHistogram(QImage* img)
{
    ui->ErrorLabel->setText("Showing Histogram!");
    if(img == nullptr && MyItem != nullptr)
    {
        img = MyImage;
    }

    if(img != nullptr)
    {
        _Histo->hide();
        _Histo->ShowHisto(img);
        _Histo->show();
    }
    else {
        {
            ui->ErrorLabel->setText("No Image to show histogram for!");
        }
    }

}

void MainWindow::_OpenHistogram(QPixmap* img)
{
    QImage image = img->toImage();
    _OpenHistogram(&image);
}

///////////
/// Setter
///////
void MainWindow::SetMainIfc(MainIfc *mainIfc)

{
    _MainIfc = mainIfc;
}

/////////////
// Getter
////////////

QString* MainWindow::GetCurrentImagePath()
{
    if(MyImageAddress.length() > 0)
        return &MyImageAddress;
    else {
        return nullptr;
    }

}



///////////////////
//MainWindow Slots
///////////////////
void MainWindow::_OnMenuBtnLoadImg()
{
    //Den FileDialog rufen um ein Bild zu suchen!
    QString filenName = QFileDialog::getOpenFileName(this, tr("Image Selection"), "/home/gerdie/Developement/test/Surface/images/", "Any File (*.*);; Images (*.png *.jpg *.JPG);;");
    if(!filenName.compare(""))
    {
        ui->ErrorLabel->setText("ERROR: No image selected!");
        return;
    }

    QImage* image = _MainIfc->LoadImg(filenName);
    if(image->isNull())
    {
        ui->ErrorLabel->setText("ERROR: Could not load image!");
    }
    else
    {
        _PrepareForNewImage(image, filenName);

    }
}

void MainWindow::_OnExit()
{
}

void MainWindow::_OnImageScaleToFit()
{

}

void MainWindow::_OnMenuBtnPipeConfig()
{
    ui->ErrorLabel->setText(tr("PipeConfig"));

    _PipeConfig->show();
}

void MainWindow::_OnBtnLoad()
{
    QString filenName = QFileDialog::getOpenFileName(this, tr("Image Selection"), "/home/gerdie/Developement/test/Surface/images/", "Any File (*.*);; XML File (*.xml);;");
    QImage* img;

    ProjectInfo info;

    Error_Codes err = _MainIfc->Load(filenName, &img, info);

    switch(err)
    {
    case TOTAL_ERROR: {ui->ErrorLabel->setText("ERROR: could not load project data!"); }
        break;
    case NO_IMG_FOUND: {_PipeConfig->DisplayPipeConfig(info.PipePlan); ui->ErrorLabel->setText("No Image found!");}
        break;
    case NO_PIPES_FOUND: { _PrepareForNewImage(img, filenName); ui->ErrorLabel->setText("No Pipes Found");}
        break;
    case ERROR_SUCCESS:
    {
        _PrepareForNewImage(img, filenName);
        _PipeConfig->DisplayPipeConfig(info.PipePlan);

        ui->ErrorLabel->setText("Project data loaded!");
    }

    default:
        break;

    }

}

void MainWindow::_OnBtnSave()
{
    QString filenName = QFileDialog::getSaveFileName(this, tr("Image Selection"), "/home/gerdie/Developement/test/Surface/images/", "Any File (*.*);; XML File (*.xml);;");
    if(!_MainIfc->Save(filenName))
    {
        ui->ErrorLabel->setText("Save not possible at the moment! FUCK YOU ASSHOLE!");
    }

}


void MainWindow::_OnMenuBtnLog()
{
    ui->ErrorLabel->setText("Showing Log File");

    _LogViewer->SetTextToBrowser(_MainIfc->GetLogFileText());
    _LogViewer->show();
}

void MainWindow::_OnMenuBtnHistogram()
{
    _OpenHistogram();

    _HistoOpen = true;

/*    ui->ErrorLabel->setText("Showing Histogram!");

    if(MyItem != nullptr)
    {
        _Histo->ShowHisto(MyImage);
        _Histo->show();
    }
    else {
        {
            ui->ErrorLabel->setText("No Image to show histogram for!");
        }
    }
    */
}

void MainWindow::_OnHistoClosed()
{
    _HistoOpen = false;
}


////////////////////////////////////////////////////////////
//  Klasse MySpecialPixMapItem
///////////////////////////////////////////////////////////

//C-TOR
MySpecialPixMapItem::MySpecialPixMapItem(QPixmap &map, const unsigned int Id, MainWindow* winMain):
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
    if(isSelected())
    {
        painter->setPen(Qt::green);
    }
    else
    {
        painter->setPen(Qt::blue);
    }

    painter->drawRect(boundingRect());

    painter->setBrush(Qt::black);

    QGraphicsPixmapItem::paint(painter, option, widget);
}



//D-TOR
//MySpecialPixMapItem::~MySpecialPixMapItem(){}


//Überschriebene Methoden
QRectF MySpecialPixMapItem::boundingRect()
{
    return QGraphicsPixmapItem::boundingRect();
}

