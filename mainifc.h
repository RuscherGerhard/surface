#ifndef MAINIFC_H
#define MAINIFC_H

#include <QApplication>

#include "mainwindow.h"
#include "pipemanager.h"

#include <tinyxml2.h>


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

    bool Save(const QString &path);

    QString Load(const QString &path);

private:
    QApplication* _QApp;
    MainWindow* _WinMain;


    PipeManager* _PipeMan;


    bool _WriteToHrdDrive(ProjectInfo* info, const QString &path);

    bool _ReadFromHardDrive(QString* path);

};






#endif // MAINIFC_H
