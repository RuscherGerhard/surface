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
    ~MainIfc();




    /**
     * @brief ForwardPipePlanToPipeManager: Die Methode stößt im PipeManager das Nauaufbauen der
     *                                      kompletten Bildverarbeitung an.
     * @param pipePlan: Die Blaupause für die neue Bildverarbeitung!
     */
    void ForwardPipePlanToPipeManager(std::vector<std::vector<FilterId>>* pipePlan);

    QApplication* GetQApp();


    /**
     * @brief ProcessImage: Die Methode forwarded das Übergebene Bild an die Bildverarbeitung und gibt das Resultat der
     *                      Bildverarbeitung zurück an den Aufrufer!
     * @param image:        Das zu verarbeitende Bild!
     * @return std::vector<QImage*>*: Ein Zeiger auf den Vector mit den Ergebnisbildern der Bildverarbeitung!
     */
    std::vector< std::vector<QImage*>*>* ProcessImage(QImage* image);

    /**
     * @brief Save: Schnittstelle um Progarmmconfigurationen zu speichern
     * @param path: Der Pfad, unter dem die Configuration auf der Platte gespeichert werden soll!
     * @return :    Ergebhnis des speicherns. False heißt Speichern ist fehlgelschlagen!
     */
    bool Save(const QString &path);

    /**
     * @brief Load: Schnittstelle zum Laden von Programmconfigurationen
     * @param path: Der Pfad, von wo geladen werden soll!
     * @param img: QImage Zeiger in dem das Geladene Bild gespeichert werden soll!
     * @return :    Feedback über den Erfolg des Speicherns. True bedeutet SPeichern war erfolgreich!
     */
    Error_Codes Load(const QString &path, QImage** img, ProjectInfo &info);


    QImage* LoadImg(const QString &path);

    QString GetLogFileText();


private:
    QApplication* _QApp;
    MainWindow* _WinMain;


    PipeManager* _PipeMan;


    bool _WriteToHrdDrive(ProjectInfo* info, const QString &path);

    bool _ReadFromHardDrive(ProjectInfo &info, const QString &path);

};






#endif // MAINIFC_H
