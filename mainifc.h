#ifndef MAINIFC_H
#define MAINIFC_H

#include <QApplication>

#include "mainwindow.h"
#include "pipemanager.h"


class MainIfc
{
public:
    MainIfc(int argc, char *argv[]);





    /**
     * @brief ForwardPipePlanToPipeManager: Die Methode stößt im PipeManager das Nauaufbauen der
     *                                      kompletten Bildverarbeitung an.
     * @param pipePlan: Die Blaupause für die neue Bildverarbeitung!
     */
    void ForwardPipePlanToPipeManager(std::vector<std::vector<FilterId>>* pipePlan);

    QApplication* GetQApp();

    std::vector<QImage*>* ProcessImage(QImage* image);


private:
    QApplication* _QApp;
    MainWindow* _WinMain;


    PipeManager* _PipeMan;

};




#endif // MAINIFC_H
