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


class MySpecialPixMapItem : public QGraphicsPixmapItem
{
public:
    MySpecialPixMapItem(QPixmap &map, const unsigned int Id,MainWindow* winMain);


    virtual QRectF boundingRect();

    void mousePressEvent(QGraphicsSceneMouseEvent* event)override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief GetId: returnt die Id.
     * @return unsigend int ID, der wert des Members _MyId.
     */
    unsigned int GetId(){return _MyId;}

private:
    MainWindow* _MainWin;
    unsigned int _MyId;

};




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
    QGraphicsView* MyResultsView;
    QGraphicsScene* MyResultsScene;

    MySpecialPixMapItem* MyItem;///< Ist mein Bild in der hauptanzeige

    QImage* MyImage;///< Ist mein Originalbild (als QImage)

    PipeConfig* _PipeConfig;

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


    void _AddResultsToScene();

    void _CleanupResultPixMaps();

    void _PrepareForNewImage(QImage* newImage, const QString &fileName);

private slots:
    void _OnMenuBtnLoadImg();
    void _OnExit();
    void _OnImageScaleToFit();
    void _OnMenuBtnPipeConfig();

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
