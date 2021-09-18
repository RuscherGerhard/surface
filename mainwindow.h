#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pipeconfig.h>
#include <logview.h>
#include <fstream>
#include "histogram.h"

class QImage;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;
class MainIfc;
class QGridLayout;
class QTextBrowser;
class MySpecialPixMapItem;
class resultsView;


namespace Ui {
class MainWindow;
}






class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class MySpecialPixMapItem;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /**
     * @brief ApplyPipe: Die Methode erzeugt soviele Anzeigen wie es Pipes gibt!
     *                  Jede Pipe hat ihre eigene Anzeige! Zusätzlich wird der InputParameter,
     *                  nämlich die Blaupause für die Einzelnen Pipes an den MainIfc weitergegeben!
     * @param pipePlan : Vector von FilterId Vectoren. Jeder FilterId-Vector representiert dabei eine
     *                  Bildverarbeitungspipe!
     */
    void ApplyPipe(std::vector<std::vector<FilterId>>* pipePlan);


    void SetMainIfc(MainIfc* mainIfc);

    QString* GetCurrentImagePath();//{return &MyImageAddress;}

protected:
    /**
     * @brief _SwitchImgInMainView: Die Methode zeigt das Bild mit der angegebenen Id in dem Hauptview an!
     *
     * @param Id: Ist die Id des neu anzuzeigenden Bildes!
     */
    void _SwitchImgInMainView(const unsigned int Id);

private:
    Ui::MainWindow *ui;

    std::ofstream debugOut;

    std::vector<MySpecialPixMapItem*> _ResultPixMaps;

    //Pointer
    MainIfc* _MainIfc;



    //Variablen
    QString MyImageAddress;
    QGraphicsView* MyView;
    QGraphicsScene* MyScene;
    //QGraphicsView* MyResultsView;//Wird erstetzt durch die Externe Klasse MyResultsView
    //QGraphicsScene* MyResultsScene;//Gehört zur MyResultsView
    resultsView* MyResultsView;

    MySpecialPixMapItem* MyItem;///< Ist das Bild in der hauptanzeige

    QImage* MyImage;///< Ist mein Originalbild (als QImage)

    PipeConfig* _PipeConfig;
    LogView* _LogViewer;
    Histogram* _Histo;

    bool _HistoOpen;

    //Methoden

    /**
     * @brief DisplayImage: Bekommt das zu verarbeitende Bild und Zeigt es im view an.
     * @param Img: Das anzuzeigende Bild
     */
    //void DisplayImage(QImage* Img);
    void DisplayImage(MySpecialPixMapItem* Img);


    /**
     * @brief _CreateMenus: Setup Methode für die Menüs. Erzeugt die einzelnen Menupunkte und die einzelnen Menüs!
     */
    void _CreateMenus();

    /**
     * @brief _SetupGui: Setupmethode für die Gui. Arangiert Menüleiste, Graphicsview usw.....
     */
    void _SetupGui();

    /**
     * @brief _AddResultsToScene greift auf die ResultScene zu. gehört in die Klasse!!!!
     */
    void _AddResultsToScene();
    /**
     * @brief _CleanupResultPixMaps Greift auf die ResultScene zu, gehört in die Klasse!!
     */
    void _CleanupResultPixMaps();


    void _PrepareForNewImage(QImage* newImage, const QString &fileName);


    /**
     * @brief _OpenHistogram: Die Methode erstellt das Histogram des im MainView angezeigten Bildes!
     */
    void _OpenHistogram(QImage* img=nullptr);
    void _OpenHistogram(QPixmap* img);

private slots:
    void _OnMenuBtnLoadImg();
    void _OnExit();
    void _OnImageScaleToFit();
    void _OnMenuBtnPipeConfig();
    void _OnMenuBtnLog();
    void _OnMenuBtnHistogram();
    void _OnHistoClosed();

    /**
     * @brief _OnBtnLoad: Diese Methode läd ein Projekt
     */
    void _OnBtnLoad();//
    /**
     * @brief _OnBtnSave: Diese Methode speichern ein Projekt!
     */
    void _OnBtnSave();

};

#endif // MAINWINDOW_H
