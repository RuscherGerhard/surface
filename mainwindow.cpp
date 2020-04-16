#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFileDialog>


#include "mainifc.h"


MainWindow::MainWindow( QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    MyImage(nullptr),
    MyItem(nullptr)
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
    //Die QAction basteln
    QMenu* File = new QMenu(tr("File"));
    QAction * LoadImage = new QAction(tr("Load Image"));
    File->addAction(LoadImage);

    QAction* Exit = new QAction(tr("Exit"));
    File->addAction(Exit);
    File->setVisible(true);

    ui->menuBar->addMenu(File);

    QMenu* Project = new QMenu(tr("Project"));
    QAction* PipeConf = new QAction(tr("Pipe Config"));


    Project->addAction(PipeConf);
    ui->menuBar->addMenu(Project);


    //PipeConfig aufsetzen

    _PipeConfig = new PipeConfig(this);
    _PipeConfig->hide();


    //Connects
    connect(LoadImage, SIGNAL(triggered()), this, SLOT(_OnMenuBtnLoadImg()));
    connect(PipeConf, SIGNAL(triggered()), this, SLOT(_OnMenuBtnPipeConfig()));
}




void MainWindow::_CleanupResultPixMaps()
{
    for(size_t i=0; i < _ResultPixMaps.size(); i++)
    {
        MyScene->removeItem(_ResultPixMaps[i]);
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
    std::vector<QImage*>* results = _MainIfc->ProcessImage(MyImage);


    //Falls wir einen Vector zurückbekommen haben, also falls tatsächlich eine Pipe aufgebaut wurde!!!!
    if(results != nullptr)
    {
        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(results->size()+1);

        for(size_t j=0; j < results->size(); j++)
        {
            //Das Item basteln
            QPixmap map = QPixmap::fromImage(*results->at(j));
            QPixmap scaledMap =map.scaled(MyResultsView->width(), MyResultsView->height()/5.0, Qt::KeepAspectRatio);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(scaledMap);    

            //In die ResultsMap adden
            _ResultPixMaps[j] = item;

            QRectF rect = item->boundingRect();

            item->setPos(0,rect.y()*j +10);

            //In die Scene adden!
            MyResultsScene->addItem(item);



        }
    }
    else
    {
        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(1);
    }

    //Das Item basteln
    QPixmap map = QPixmap::fromImage(*MyImage);
    QPixmap scaledMap =map.scaled(MyResultsView->width(), MyResultsView->height()/5.0, Qt::KeepAspectRatio);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(scaledMap);

    //In die ResultsMap adden
    _ResultPixMaps[_ResultPixMaps.size()-1] = item;
    item->setPos(0,0);
    MyResultsScene->addItem(item);

}



//Das geladene Bild in eine Pixmap packen und anzeigen!
void MainWindow::DisplayImage(QImage *Img)
{
    //Aufräumen
    if(MyItem != nullptr)
    {
        //falls ein altes Bild noch in der Scene vorhanden ist, löschen!
        MyScene->removeItem(MyItem);
        delete(MyItem);
        MyItem = nullptr;
    }

    //Show Image in Main View
    QPixmap map = QPixmap::fromImage(*Img);
    MyItem = new QGraphicsPixmapItem(map);

    //Die Dimension der Scene resizen!(Notwendig, sonst wird ständig FitInView gemacht, falls das erste Bild größer war als die View, die restlichen geladenen Bilder aber kleiner sind!)
    QRectF rect = MyItem->boundingRect();
    MyScene->setSceneRect(rect);

    //Bild einfügen!
    MyScene->addItem(MyItem);

    //Wenn die Dimensionen der Scene jene der View übersteigen, Anzeige anpassen!
    MyView->fitInView(MyScene->sceneRect(), Qt::KeepAspectRatio);

    _AddResultsToScene();

    MyView->show();
    ui->ErrorLabel->setText("Image loaded!");
}


void MainWindow::ApplyPipe(std::vector<std::vector<FilterId> > *pipePlan)
{

    //Die Pipeblaupause an den PipeManager weiter leiten!!
    _MainIfc->ForwardPipePlanToPipeManager(pipePlan);

    _AddResultsToScene();
    MyView->show();

}


///////////
/// Setter
///////
void MainWindow::SetMainIfc(MainIfc *mainIfc)
{
    _MainIfc = mainIfc;
}



////////
//Slots
////////
void MainWindow::_OnMenuBtnLoadImg()
{
    //Den FileDialog rufen um ein Bild zu suchen!
    QString filenName = QFileDialog::getOpenFileName(this, tr("Image Selection"), "/home/gerdie/Developement/test/Surface/images/", "Any File (*.*);; Images (*.png *.jpg *.JPG);;");
    //Hier geht Image versuchen zu laden und wenn es klappt dann dann Anzeige aufrufen!
    //QImage image("/home/gerdie/Developement/test/Surface/images/pic.JPG");



    QImage* image = new QImage(filenName);
    if(image->isNull())
    {
        ui->ErrorLabel->setText("No image loaded!");
    }
    else
    {
        //std::vector<QImage*>* results =  _MainIfc->ProcessImage(&image);

        //Aufräumen
        if(MyImage != nullptr)
        {
            delete(MyImage);
        }

        MyImage = image;
        DisplayImage(image);
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
