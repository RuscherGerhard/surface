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
#include <myspecialpixmapitem.h>
#include <btnresultswitch.h>
//#include <resultsview.h>

//#include <QGraphicsSceneMouseEvent>
//#include <QGraphicsItem>

#include "mainifc.h"


MainWindow::MainWindow( QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    MyItem(nullptr),
    MyImage(nullptr),
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

     MyScene = new QGraphicsScene();
     MyView->setScene(MyScene);



     //Resultatauswahlfeld
     //ui->tab->layout()->removeWidget()
     //MyResultsView = new resultsView(ui->tabWidget, this);//ui->ResultsView;
     //ui->tab->layout()->addWidget(MyResultsView);

     /*ÜMyResultsScene = new QGraphicsScene();
     MyResultsView->setScene(MyResultsScene);*/


   ui->ErrorLabel->setText(tr("No Image loaded."));
}


/********************************************
 * GUI Aufbauen
 *********************************************/

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


    //Den Tabbedpane basteln
    QVBoxLayout *layout = new QVBoxLayout( ui->tab);
    ui->tab->setLayout(layout);

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

void MainWindow::_MakeResultSwitchBtns(std::vector<std::vector<FilterId> > *pipePlan)
{

    _MyPipePlan = pipePlan;
    //Über den PipePlan iterieren
    for(int i = 0; i < pipePlan->size(); i++)
    {
        int size = pipePlan->at(i).size();
        int amountChildren = ui->tab->children().size();

        if (amountChildren < size)
        {
            int howMuch = size - amountChildren;

            for(int k = 0; k < howMuch; k++)
            {
                //Buttons erzeugen

                BtnResultSwitch* newBtn = new BtnResultSwitch(ui->tab, Undefined, "");
                ui->tab->layout()->addWidget(newBtn);

                connect(newBtn, SIGNAL(PushedId(int)), this, SLOT(_OnBtnSwitchImg(int)));
            }

        }
        else if (amountChildren > size){
            //amountChildren > size

            int howMuch =amountChildren - size;
            for(int k = 0; k < howMuch; k++)
            {
                if(ui->tab->children().size() == k)
                    break;

                QLayoutItem* item = ui->tab->layout()->itemAt(k);
                ui->tab->layout()->removeItem(item);
                item->widget()->deleteLater();

            }

        }


        for(int j = 0; j < size-1; j++)
        {

            BtnResultSwitch* newBtn = static_cast<BtnResultSwitch*>(ui->tab->layout()->itemAt(j)->widget());

            /*switch(pipePlan->at(i)[j])
            {
                case OpInput : newBtn->setNewBtnTitle(IN); newBtn->setNewBtnId(OpInput); break;
            case OpOutput : newBtn->setNewBtnTitle(OUT); newBtn->setNewBtnId(OpOutput); break;
            case OpBoxFilter : newBtn->setNewBtnTitle(BOX); newBtn->setNewBtnId(OpBoxFilter); break;
            case OpProbAddScramb : newBtn->setNewBtnTitle(ADDSCRMB); newBtn->setNewBtnId(OpProbAddScramb); break;
                default: newBtn->setNewBtnTitle("Unknown Filter");
            }*/

            switch(pipePlan->at(i)[j])
            {
                case OpInput : newBtn->setNewBtnTitle(IN);  break;
            case OpOutput : newBtn->setNewBtnTitle(OUT);  break;
            case OpBoxFilter : newBtn->setNewBtnTitle(BOX); break;
            case OpProbAddScramb : newBtn->setNewBtnTitle(ADDSCRMB); break;
                default: newBtn->setNewBtnTitle("Unknown Filter");
            }

            newBtn->setNewBtnId(j);

        }
    }

        ui->tab->show();
}



/**************************************************
 * Funktionalität
 * ************************************************/

//result widget
void MainWindow::_CleanupResultPixMaps()
{
    for(size_t i = 0; i <  _ResultPixMaps.size(); i++)
    {
        delete _ResultPixMaps[i];

        _ResultPixMaps[i] = nullptr;
    }
}

//result widget
void MainWindow::_AddResultsToScene()
{
    //Wenn wir noch kein Bild zum bearbeiten geladen haben, dann hier Abbrechen!
    if(MyImage == nullptr)
        return;

    //Aufräumen
    _CleanupResultPixMaps();

    //Results von der Pipe holen
    std::vector<std::vector<QImage*>*>* results = _MainIfc->ProcessImage(MyImage);

    //MyResultsView->AddResultsToScene(MyImage, results);
    //Falls wir einen Vector zurückbekommen haben, also falls tatsächlich eine Pipe aufgebaut wurde!!!!
    if(results != nullptr)//Wenn NULL dann keine Pipe, klar!
    {
        unsigned int resSize = results->at(0)->size();

        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(resSize+1);


        QPixmap item = QPixmap::fromImage(*MyImage);
        MyItem = new MySpecialPixMapItem(item, OpInput, this);
        _ResultPixMaps[0] = MyItem;

        int id = 1;

        for(size_t j=0; j < resSize; j++)
        {
            //Das Item basteln
            QPixmap map = QPixmap::fromImage(*results->at(0)->at(j));


            assert(_MyPipePlan != nullptr);
            MySpecialPixMapItem* item = new MySpecialPixMapItem(map, j+id,this);

            //In die ResultsMap adden, unskalliert!!!!
            _ResultPixMaps[j+id] = item;
        }


    }
    else
    {
        //Die Anzahl der Pipes definiert auch die Anzahl der Views bei der Anzeige.
        _ResultPixMaps.resize(1);
    }

}

void MainWindow::ApplyPipe(std::vector<std::vector<FilterId> > *pipePlan)
{

    //Die Pipeblaupause an den PipeManager weiter leiten!!
    _MainIfc->ForwardPipePlanToPipeManager(pipePlan);

    //Die Resultswitches zum Umschalten der Bilder in der Main-Anzeige aufbauen
    _MakeResultSwitchBtns(pipePlan);


    //Aufruf der Bildverarbeitung
    _AddResultsToScene();

    //Refresh der View
    MyView->show();

}


/*
 * Bilder Anzeige
 *
 */

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

    MyView->show();
    ui->ErrorLabel->setText("Image loaded!");

    if(_HistoOpen)
    {
        _OpenHistogram(MyItem->getPixmap());
    }
}




void MainWindow::_SwitchImgInMainView(const unsigned int Idx)
{

    bool idFound = false;

    MySpecialPixMapItem* itemToShow =nullptr;

    //Neues Item erzeugen!
   /*for(size_t i = 0; i < _ResultPixMaps.size(); i++)
    {
       assert(_ResultPixMaps[i] != nullptr);
        unsigned int lid = _ResultPixMaps[i]->GetId();
        if(lid == Id)
        {
            idFound = true;
            itemToShow = _ResultPixMaps[i];

            ui->ErrorLabel->setText("Found corresponding Image");
        }
    }*/

    itemToShow = _ResultPixMaps[Idx];

    ui->ErrorLabel->setText("Found corresponding Image");
    if(itemToShow != nullptr)
    {
        //Das gefundene Bild anzeigen
        DisplayImage(itemToShow);
        ui->ErrorLabel->setText("Found corresponding Image.");

    }
    else {
        ui->ErrorLabel->setText("No corresponding Image found!!!");
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


/*
 *
 * Histogramm
 *
 *
 */

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
    if(image == nullptr)
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
    case NO_IMG_FOUND: {_PipeConfig->DisplayPipeConfig(info.PipePlan); _MakeResultSwitchBtns(info.PipePlan); _MyPipePlan = info.PipePlan; ui->ErrorLabel->setText("No Image found!");}
        break;
    case NO_PIPES_FOUND: { _PrepareForNewImage(img, filenName); ui->ErrorLabel->setText("No Pipes Found");}
        break;
    case ERROR_SUCCESS:
    {
        //Neue image aufsetzen
        _PrepareForNewImage(img, filenName);

        //Den Pipe-Editor entsprechen einstellen, um die Aktuelle Pipe anzuzeigen.
        _PipeConfig->DisplayPipeConfig(info.PipePlan);

        //Die Image-Result-Switches entsprechend der eingestellten Pipe basteln.
        _MakeResultSwitchBtns(info.PipePlan);

        _MyPipePlan = info.PipePlan;

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


void MainWindow::_OnBtnSwitchImg(int Id)
{
    _SwitchImgInMainView(Id);
}


