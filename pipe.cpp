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

Pipe::Pipe(std::vector<FilterId>* ids)
{
   _FilterQueue.resize(0);
   _ResultImages.resize(0);

   for(size_t i = 0; i< ids->size(); i++)
   {
       _AddFilterToPipe(ids->at(i));
   }
}

Pipe::~Pipe()
{
    _DeleteFilters();
}


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

    case OpGaussFilter:
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
    _ResultImages.resize(_FilterQueue.size()-2);

    _WorkingCopy = *imageToProcess;
    QImage* image = &_WorkingCopy;


    for(unsigned int i = 0; i < _FilterQueue.size(); i++)
    {
        Filter* filter = _FilterQueue[i];
        filter->ProcessImage(image);
        image = filter->getImage();// Hier das CM::MAt in ein QImage übersetzen

        if((i!=0) && (i != _FilterQueue.size()-1))
            _ResultImages[i-1] = image;
    }

    return &_ResultImages;
}
