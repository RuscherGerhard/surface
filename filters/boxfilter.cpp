#include "boxfilter.h"

BoxFilter::BoxFilter()
{
}


BoxFilter::~BoxFilter()
{
}

void BoxFilter::ProcessImage(QImage* imageToProcess)
{
    _Image = _Filter(imageToProcess);//= imageToProcess;

}


QImage* BoxFilter::_Filter(QImage* imageToProcess)
{


    for( int i = 0; i < imageToProcess->width(); i++)
    {
        for(int j = 0; j < imageToProcess->height(); j++)
        {
            if(i == j)
                imageToProcess->setPixel(i,j, Qt::green);
        }
    }
    return imageToProcess;
}
