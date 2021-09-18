#ifndef RESULTSVIEW_H
#define RESULTSVIEW_H
#include <QGraphicsView>

class MySpecialPixMapItem;
class QGraphicsScene;
class MainWindow;

class resultsView :public QGraphicsView
{
    Q_OBJECT
public:
    resultsView(QWidget *parent, MainWindow* mainWin);
    ~resultsView();

    /**
     * @brief AddResultsToScene : Fügt die Resultatbilder der Pipe in die Szene ein. orher werden die einzufügenden Bildkopien noch passend skalliert!
     * @param MyImage : Ist das bild, das an erster stelle in die Szene eingefügt wird, und deshalb auch oben ist!
     * @param results : Ist ein Zeiger auf den Vektor mit den Ergebnisbildern der Pipe!
     */
    void AddResultsToScene(QImage* MyImage, std::vector<std::vector<QImage*>*>* results);

    /**
     * @brief CleanupResultPixMaps : Entfernt alle Bilder aus dem Ergebnisvector!
     */
    void CleanupResultPixMaps();

    /**
     * @brief SearchInResultsPixmaps : Schaut bei Aufruf, ob das Bild mit der Entsprechenden
     * @param Id : Die Id des Bildes, wonach gesucht werden soll
     * @return : Ein Zeiger auf das gefundene Bild mit der VorgabeId
     */
    MySpecialPixMapItem* SearchInResultsPixmaps(const unsigned int &Id);

private:
    //Das muss normalerweise in der Mainwindow und nicht hier sein.....
    std::vector<MySpecialPixMapItem*> _ResultPixMaps; ///< Der Vector mit den Ergebnissbildern der Pipe

    QGraphicsScene* _MyResultsSecene; ///< Die Szene der view, in der die Bilder eingefügt werden müssen
    MainWindow* _MainWin; ///< zeiger auf das MainWindow-Objekt

};

#endif // RESULTSVIEW_H
