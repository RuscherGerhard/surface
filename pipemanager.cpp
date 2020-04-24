#include "pipemanager.h"

//#include <fstream>

PipeManager::PipeManager()
{
    _Pipes.resize(0);
    _ResultVector.resize(0);
}


PipeManager::~PipeManager()
{
    //Da wir unseren Pipe Manager abbauen, wollen wir hier natürlich auch den Speicher für die Pipes frei geben!
    _DeletePipes();
    _DeleteResults();
}

/*Processing*/
std::vector<QImage*>* PipeManager::ProcessImage(QImage *imageToProcess)
{
    //für den Fall, daß noch keine Pipes aufgesetzt wurden!
    if(_Pipes.size() <= 0)
        return nullptr;


    for(size_t i = 0; i < _Pipes.size(); i++)
    {
        _ResultVector[i] = (_Pipes[i]->ProcessImage(imageToProcess));
    }

    return &_ResultVector;
}


/*Maintenance*/

void PipeManager::UpdatePipes(std::vector<std::vector<FilterId>>* Pipes)
{
    //Die Blaupause wegspeichern(Wenn wir das Projekt speichern wollen, ist das wichtig)
    _PipePlan = *Pipes;

    //Aufräumen
    //_DeleteResults();//Den alten Ergebnisvector abbauen!
    _ResultVector.resize(0);
    _DeletePipes();//Die alte Bildverarbeitung abbauen


    //den Resultvector initialisieren
    _ResultVector.resize(Pipes->size());

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

void PipeManager::_DeleteResults()
{
        for(size_t i = 0; i < _ResultVector.size(); i++)
        {
            if(_ResultVector.at(i) != nullptr)
            {

                delete(_ResultVector.at(i));
                _ResultVector.at(i)=nullptr;
            }
        }
        _ResultVector.resize(0);
}


//Getter...
std::vector<std::vector<FilterId>>* PipeManager::GetPipePlan()
{
    if(_PipePlan.size() > 0)
    {
        return &_PipePlan;
    }
    else
    {
        return nullptr;
    }
}
