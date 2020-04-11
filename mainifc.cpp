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


bool MainIfc::ProcessImage(QImage *image, std::vector<QImage*> *resultVector)
{
    _PipeMan->ProcessImage(image, resultVector);
    return true;
}
