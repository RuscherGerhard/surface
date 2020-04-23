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
    ProjectInfo info;

    info.PipePlan = _PipeMan->GetPipePlan();
    info.imagePath = _WinMain->GetCurrentImagePath();

    if(info.PipePlan == nullptr && info.imagePath == nullptr)
    {
        return false;
    }
    else
    {
        return _WriteToHrdDrive(&info, path);
    }
}

bool MainIfc::Load(const QString &path, QImage **img)
{
    ProjectInfo info;

    if(_ReadFromHardDrive(info, path))
    {
        ForwardPipePlanToPipeManager(info.PipePlan);


        *img = LoadImg(*info.imagePath);
        if(*img != nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    else
    {

        return false;
    }

}


bool MainIfc::_WriteToHrdDrive(ProjectInfo *info, const QString &path)
{
    tinyxml2::XMLDocument doc(true);

    tinyxml2::XMLNode* project = doc.NewElement("Surface_Project");
    doc.InsertFirstChild(project);

    //Bildpfad setzen
    tinyxml2::XMLNode* ImagePath = doc.NewElement("Image_Path");

    //Pfad nur adden, wenn auch wirklich einer da ist und imagePath kein NullPointer ist
    if(info->imagePath != nullptr)
    {
        tinyxml2::XMLText*  pwd = doc.NewText(info->imagePath->toStdString().c_str());
        ImagePath->InsertEndChild(pwd);
    }
    else
    {
        tinyxml2::XMLText* noImg = doc.NewText("No_Image");
        ImagePath->InsertEndChild(noImg);
    }

    //Den Pfad in das document setzen!
    project->InsertEndChild(ImagePath);



    //Pipes abfahren
    tinyxml2::XMLNode* Pipes = doc.NewElement("Pipe_Plan");

    if(info->PipePlan != nullptr)
    {

    for(size_t i = 0; i < info->PipePlan->size(); i++)
    {
        tinyxml2::XMLNode* A_Pipe = doc.NewElement("A_Pipe");
        for(size_t j = 0; j < info->PipePlan->at(i).size(); j++)
        {
            FilterId id = info->PipePlan->at(i).at(j);
            QString idText = QString::number(id);

            //Den XMLKnoten erzeugen
            tinyxml2::XMLNode* filterId = doc.NewElement("FilterId");
            tinyxml2::XMLText* text = doc.NewText(idText.toStdString().c_str());

            filterId->InsertEndChild(text);
            A_Pipe->InsertEndChild(filterId);
        }

        Pipes->InsertEndChild(A_Pipe);
    }

    }
    else
    {
        tinyxml2::XMLText* noPipePlan = doc.NewText("No Pipes");
        Pipes->InsertEndChild(noPipePlan);
    }

    project->InsertEndChild(Pipes);

    //Datei speichern
    QString where = path;
    //where.append("Surface.xml");
    if(!doc.SaveFile(where.toStdString().c_str()))
    {
        //Wenn die Rückgabe von SaveFile 0(ErrorSuccess) dann true;
        return true;
    }else
    {
        return false;
    }
}

bool MainIfc::_ReadFromHardDrive(ProjectInfo &info, const QString &path)
{
    tinyxml2::XMLDocument doc;
    std::string imgPath(path.toStdString());
    tinyxml2::XMLError error = doc.LoadFile(imgPath.c_str());

    if(!error)//Achtung 0 = ERROR_SUCCES ;)
    {


        tinyxml2::XMLNode* ImgPathNode = doc.FirstChild()->FirstChild();

        QString* pathString = new QString(ImgPathNode->FirstChild()->Value());

        info.imagePath = pathString;


        tinyxml2::XMLNode* PipePlanNode = ImgPathNode->NextSibling();


        //tinyxml2::XMLNode* currentPipe = FirstPipe;

        std::vector<std::vector<FilterId>>* pipePlan = new std::vector<std::vector<FilterId>>();

        for(tinyxml2::XMLNode* pipeNode = PipePlanNode->FirstChild(); pipeNode != nullptr; pipeNode = pipeNode->NextSibling())
        {
            std::vector<FilterId> APipe;

            bool corupt = false;
            for(tinyxml2::XMLNode* filterNode = pipeNode->FirstChild(); filterNode != nullptr; filterNode = filterNode->NextSibling())
            {
                bool ok = true;
                QString idString(filterNode->Value());
                int id = idString.toInt(&ok);

                //FAlls wir einen schlechten wert gelesen haben, verwerfen wir hier das weiterer einlesen
                if(id >= Undefined)
                {
                    corupt = true;
                    break;
                }

                APipe.push_back(static_cast<FilterId>(id));
            }
            if(!corupt)
                pipePlan->push_back(APipe);
        }
        info.PipePlan = pipePlan;
        return true;
    }
    else
    {
        return false;
    }
}


QImage* MainIfc::LoadImg(const QString &path)
{
    QImage* img = new QImage(path);
    if(img->isNull())
    {
        delete (img);
        img = nullptr;

    }

    return img;
}
