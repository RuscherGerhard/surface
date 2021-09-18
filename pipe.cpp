#include "pipe.h"

#include "filters/boxfilter.h"
#include "filters/input.h"
#include "filters/output.h"
#include "filters/gaussfilter.h"
#include "filters/gaussnl.h"
#include "filters/probaddscrambler.h"
#include "filters/segmentator.h"
#include "filters/linefindertransversal.h"
#include "filters/shapefollower.h"
#include "filters/turtleshapefollower.h"
#include "filters/skeletonizer.h"
#include <thread>

Pipe::Pipe(std::vector<FilterId>* ids)
{
   _FilterQueue.resize(0);
   _ResultImages.resize(0);

   for(size_t i = 0; i< ids->size(); i++)
   {
       _AddFilterToPipe(ids->at(i));
   }

   _HowManyTasks = 4;
}

Pipe::~Pipe()
{
    _DeleteFilters();
}

/**
 * @brief Pipe::_AddFilterToPipe in Abhängigkeit der Eingabe erzeugt die Methode einen neuen Filter und fügt ihn der Pipe hinzu!
 * @param id: die ID des gewünschten Filters
 */
void Pipe::_AddFilterToPipe(const FilterId id)
{

    Filter* toAd = nullptr;

    std::ofstream stream;
    stream.open(DEBUG_PATH, std::ofstream::app);


    switch (id)
    {
    case OpInput:
    {
        toAd = reinterpret_cast<Filter*>(new Input());

        if(toAd != nullptr)
            stream << "Input |";
        else {
            stream << " - |";
        }
    }
    break;

    case OpProbAddScramb :
    {
        toAd = reinterpret_cast<Filter*>(new probAddScrambler());

        if(toAd != nullptr)
            stream << "| Prob. Add. Scrambler \n";
        else {
            stream << "| - \n";
        }

    }
    break;

    case OpBoxFilter:
    {
        toAd = reinterpret_cast<Filter*>(new BoxFilter());
        if(toAd != nullptr)
            stream << " BoxFilter |";
        else {
            stream << " - |";
        }

    }    
    break;

    /*case OpGaussFilter:
    {
        toAd = reinterpret_cast<Filter*>(new GaussFilter());
        if(toAd != nullptr)
            stream << " GaussFilter |";
        else {
            stream << " - |";
        }

    }
    break;

    case OpGaussFilterNL:
    {
        toAd = reinterpret_cast<Filter*>( new GaussNL() );
        if(toAd != nullptr)
            stream << " GaussFilterNL |";
        else {
            stream << " - |";
        }

    }

    break;

    case OpSegmentator:
    {
        toAd = reinterpret_cast<Filter*>(new segmentator());
        toAd->setFilterId(OpSegmentator);

        if(toAd != nullptr)
            stream << "| Segmentator \n";
        else {
            stream << "| - \n";
        }

    }
    break;

    case OpLineFindTransVers:
    {
        toAd = reinterpret_cast<Filter*>(new LineFinderTransversal());
        toAd->setFilterId(OpLineFindTransVers);

        if(toAd != nullptr)
            stream << "| LineFinderTransversal \n";
        else {
            stream << "| - \n";
        }

    }
    break;
    case OpShapeFollower:
    {
        toAd = reinterpret_cast<Filter*>(new ShapeFollower());
        toAd->setFilterId(OpShapeFollower);

        if(toAd != nullptr)
            stream << "| ShapeFollower \n";
        else {
            stream << "| - \n";
        }

    }
    break;

    case OpSkeletonizer:
    {
        toAd = reinterpret_cast<Filter*>(new Skeletonizer());
        toAd->setFilterId(OpSkeletonizer);

        if(toAd != nullptr)
            stream << "| Skeletonizer \n";
        else {
            stream << "| - \n";
        }

    }
    break;*/

    /*case OpTurtleShapeFollower:
    {
        toAd = reinterpret_cast<Filter*>(new LineFinderTransversal());
        toAd->setFilterId(OpTurtleShapeFollower);

        if(toAd != nullptr)
            stream << "| TurtleShapeFollower \n";
        else {
            stream << "| - \n";
        }

    }
    break;*/


    case OpOutput:
    {
        toAd = reinterpret_cast<Filter*>(new output());

        if(toAd != nullptr)
            stream << "| Output \n";
        else {
            stream << "| - \n";
        }

    }
    break;


    default: break;


    }

    stream.close();

    _FilterQueue.push_back(toAd);
}

void Pipe::_DeleteFilters()
{
    if(_FilterQueue.size() == 0)
        return;

    for(size_t i = 0; i < _FilterQueue.size(); i++)
    {
        if(_FilterQueue[i] != nullptr)
            delete(_FilterQueue[i]);
    }

    _FilterQueue.resize(0);
}


//Imageprocessing
std::vector<QImage*>* Pipe::ProcessImage(QImage *imageToProcess)
{


    _ResultImages.resize(_FilterQueue.size()-2);//der Input und der Output wird nicht gebraucht. Nur die Ergebnisse der Filter in der Mitte, also zwei Filter weniger.

    _WorkingCopy = *imageToProcess;
    QImage* image = &_WorkingCopy;
    cv::Mat* imageMat = nullptr;

    Filter* filter = _FilterQueue[0];
    filter->ProcessImage(image);
    imageMat = filter->getImageMat(); //[filter, image]()->QImage*{  filter->ProcessImage(image); return filter->getImage();}
    for(unsigned int i = 1; i < _FilterQueue.size()-1; i++)
    {
        filter = _FilterQueue[i];
        if(filter->getParallel())
        {
            if(_HowManyTasks == 2)
            {
                filter->MakeResultImg(imageMat);
                //hier wird die ganze geschichte parallel ausgeführt

                int xs1 = 0;
                int ys1 = 0;
                int xe1 = imageMat->cols/2;
                int ye1 = imageMat->rows;

                int xs2 = imageMat->cols/2 +1;
                int ys2 = 0;
                int xe2 = imageMat->rows;
                int ye2 = imageMat->cols;

                std::thread t1(&Filter::ProcessImagePar, filter, imageMat, xs1, ys1, xe1, ye1);
                std::thread t2(&Filter::ProcessImagePar, filter,imageMat, xs2, ys2, xe2, ye2);

                t1.join();
                t2.join();

            }
            else if(_HowManyTasks == 4)
            {
                filter->MakeResultImg(imageMat);

                int xs1 = 0;
                int ys1 = 0;
                int xe1 = imageMat->cols/2;
                int ye1 = imageMat->rows/2;

                int xs2 = imageMat->cols/2 +1;
                int ys2 = 0;
                int xe2 = imageMat->cols;
                int ye2 = imageMat->rows/2;

                int xs3 = 0;
                int ys3 = imageMat->rows/2 +1;
                int xe3 = imageMat->cols/2;
                int ye3 = imageMat->rows;

                int xs4 = imageMat->cols/2 +1 ;
                int ys4 = imageMat->rows/2 +1;
                int xe4 = imageMat->cols;
                int ye4 = imageMat->rows;


                std::thread t1(&Filter::ProcessImagePar, filter, imageMat, xs1, ys1, xe1, ye1);
                std::thread t2(&Filter::ProcessImagePar, filter,imageMat, xs2, ys2, xe2, ye2);
                std::thread t3(&Filter::ProcessImagePar, filter, imageMat, xs3, ys3, xe3, ye3);
                std::thread t4(&Filter::ProcessImagePar, filter,imageMat, xs4, ys4, xe4, ye4);

                t1.join();
                t2.join();
                t4.join();
                t3.join();

            }
        }
        else
        {
            //hier wird die geschichte sequenziell ausgeführt!
            filter->ProcessImage(imageMat);
        }

        imageMat = filter->getImageMat();



        _ResultImages[i-1] = _ConvertMatToQImage(imageMat);// Hier das CM::MAt in ein QImage übersetzen

    }

    return &_ResultImages;
}


QImage* Pipe::_ConvertMatToQImage(cv::Mat *image)
{
    QImage* returnImage =nullptr;
    switch(image->Mat::type())
    {
    case CV_8UC4:
        {
            returnImage = new QImage(image->data, image->cols, image->rows, static_cast<int>(image->step), QImage::Format_RGB32);
        }
        break;

    default:
        break;
    }

    return returnImage;
}
