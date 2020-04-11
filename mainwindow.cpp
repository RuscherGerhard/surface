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
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    _MainIfc = nullptr;

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


//Das geladene Bild in eine Pixmap packen und anzeigen!
void MainWindow::DisplayImage(QImage *Img)
{
    MyView = new QGraphicsView();
    ui->scrollArea->setWidget(MyView);
    MyView->setParent(ui->scrollArea);

    MyScene  = new QGraphicsScene();
    MyView->setScene(MyScene);

    MyItem = new QGraphicsPixmapItem(QPixmap::fromImage(*Img));
    MyScene->addItem(MyItem);


    MyView->show();
    ui->ErrorLabel->setText("Image loaded!");
}


void MainWindow::ApplyPipe(std::vector<std::vector<FilterId> > *pipePlan)
{
    //Erzeugen der Anzeigen
    size_t amountOfViews = pipePlan->size();




    //Die Pipeblaupause an den PipeManager weiter leiten!!
    _MainIfc->ForwardPipePlanToPipeManager(pipePlan);

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
    QString filenName = QFileDialog::getOpenFileName(this, tr("Image Selection"), "/home/gerdie", "Any File (*.*);; Images (*.png *.jpg *.JPG);;");
    //Hier geht Image versuchen zu laden und wenn es klappt dann dann Anzeige aufrufen!
    //QImage image("/home/gerdie/Developement/test/Surface/images/pic.JPG");

    QImage image(filenName);
    if(image.isNull())
    {
        ui->ErrorLabel->setText("No image loaded!");
    }
    else
    {

        _MainIfc->ProcessImage(&image);

        DisplayImage(&image);
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
