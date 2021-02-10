#include "output.h"



output::output()
{
    _FilterId = OpOutput;
    _Image = nullptr;
}


QImage* output::getImage()
{
    return _Image;
}

cv::Mat* output::getImageMat()
{
    return _Img;
}

FilterId output::getFilterId()
{
    return _FilterId;
}

void output::setFilterId(const FilterId id)
{
    _FilterId = id;
}

void output::ProcessImage(QImage *imageToProcess)
{
    _Image = imageToProcess;
}

void output::ProcessImage(cv::Mat *imageToProcess)
{
    _Img = imageToProcess;
}
