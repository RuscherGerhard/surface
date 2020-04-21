#include "mainifc.h"

MainIfc::MainIfc(int argc, char *argv[])
{

    _QApp = new QApplication(argc, argv);

    _WinMain = new MainWindow();
    _WinMain->SetMainIfc(this);

    _WinMain->show();

    //Pipe Manager aufsetzen
    _PipeMan = new PipeManager();

}


QApplication* MainIfc::GetQApp()
{
    return _QApp;
}


void MainIfc::ForwardPipePlanToPipeManager(std::vector<std::vector<FilterId>>* pipePlan)
{
    _PipeMan->UpdatePipes(pipePlan);
}


std::vector<QImage*>* MainIfc::ProcessImage(QImage *image)
{
    return _PipeMan->ProcessImage(image);

}

bool MainIfc::Save(const QString &path)
{
    using namespace tinyxml2;

    ProjectInfo info;

    info.PipePlan = _PipeMan->GetPipePlan();
    info.imagePath = _WinMain->GetCurrentImagePath();

    tinyxml2::XMLDocument doc(true);

    tinyxml2::XMLNode* project = doc.NewElement("Surface_Project");

    doc.InsertFirstChild(project);


    //Datei speichern
    QString where = path;
    where.append("Surface.xml");
    doc.SaveFile(path.toStdString().c_str());

    return false;
}

QString MainIfc::Load(const QString &path)
{
    return QString("Cannot load anything at the moment! Don't get butt plugged!!!!");
}
