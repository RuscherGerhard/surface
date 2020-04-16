#include "boxfilter.h"

BoxFilter::BoxFilter()
{
}


BoxFilter::~BoxFilter()
{
}

void BoxFilter::ProcessImage(QImage* imageToProcess)
{
    _Image = imageToProcess;
    _Filter(imageToProcess);
}


QImage* BoxFilter::_Filter(QImage* imageToProcess)
{

    return imageToProcess;
}
