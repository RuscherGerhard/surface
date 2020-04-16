#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pipeconfig.h>
#include <fstream>

class QImage;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;
class MainIfc;
class QGridLayout;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

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

private:
    Ui::MainWindow *ui;

    std::ofstream debugOut;

    std::vector<QGraphicsPixmapItem*> _ResultPixMaps;

    //Pointer
    MainIfc* _MainIfc;



    //Variablen
    QGraphicsView* MyView;
    QGraphicsScene* MyScene;
    QGraphicsView* MyResultsView;
    QGraphicsScene* MyResultsScene;


    QGraphicsPixmapItem* MyItem;///< Ist mein Originalbild als Picsmap
    QImage* MyImage;///< Ist mein Originalbild als QImage

    PipeConfig* _PipeConfig;

    //Methoden

    /**
     * @brief DisplayImage: Bekommt das zu verarbeitende Bild und Zeigt es im view an.
     * @param Img: Das anzuzeigende Bild
     */
    void DisplayImage(QImage* Img);


    /**
     * @brief _CreateMenus: Setup Methode für die Menüs. Erzeugt die einzelnen Menupunkte und die einzelnen Menüs!
     */
    void _CreateMenus();

    /**
     * @brief _SetupGui: Setupmethode für die Gui. Arangiert Menüleiste, Graphicsview usw.....
     */
    void _SetupGui();


    void _AddResultsToScene();

    void _CleanupResultPixMaps();

private slots:
    void _OnMenuBtnLoadImg();
    void _OnExit();
    void _OnImageScaleToFit();
    void _OnMenuBtnPipeConfig();

};

#endif // MAINWINDOW_H
