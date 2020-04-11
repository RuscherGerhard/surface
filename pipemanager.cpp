#include "pipemanager.h"

//#include <fstream>

PipeManager::PipeManager()
{
    _Pipes.resize(0);
}


PipeManager::~PipeManager()
{
    //Da wir unseren Pipe Manager abbauen, wollen wir hier natürlich auch den Speicher für die Pipes frei geben!
    _DeletePipes();
}

/*Processing*/
bool PipeManager::ProcessImage(QImage *imageToProcess, std::vector<QImage*> *resultVector)
{
    for(size_t i = 0; i < _Pipes.size(); i++)
    {
        resultVector->push_back(_Pipes[i]->ProcessImage(imageToProcess));
    }
}


/*Maintenance*/

void PipeManager::UpdatePipes(std::vector<std::vector<FilterId>>* Pipes)
{
    //Aufräumen
    if(_Pipes.size() == 0)
    {
        _DeletePipes();//Die alte Bildverarbeitung abbauen
    }

    std::ofstream stream;
    stream.open(DEBUG_PATH);
    stream<<"New Pipe Beginning Here!\n";
    stream.close();

    //Pipes Aufsetzen
     for(size_t i = 0; i < Pipes->size(); i++)
     {
         std::vector<FilterId>* pipe = &Pipes->at(i);
         Pipe* newPipe = new Pipe(pipe);//Im constructor wird bereits die FilterQueue der Pipe aufgesetzt!

         _Pipes.push_back(newPipe);
     }
}

void PipeManager::_DeletePipes()
{
    //Alles Pipes platt machen!
    for(size_t i = 0; i < _Pipes.size(); i++)
    {
        //Die Pipe-Instanzen Abbauen!
        delete(_Pipes[i]);
    }

    //Den Pipevektor auf die Länge 0 setzen, somit wissen wir, wenn der Vektor Länge größer 0 hat, gibt es ne Pipe!
    _Pipes.resize(0);
}
