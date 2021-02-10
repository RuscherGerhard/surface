#include "input.h"
//#include "../utils.h"


Input::Input()
{
    _Image = nullptr;
    _FilterId = (FilterId)0;
}

QImage* Input::getImage()
{
    return _Image;
}

cv::Mat* Input::getImageMat()
{
    return _Img;
}

FilterId Input::getFilterId()
{
    return _FilterId;
}

void Input::setFilterId(const FilterId id)
{
    _FilterId = id;
}

void Input::ProcessImage(QImage* imageToProcess)
{
    _Image = imageToProcess;

    _Img = _ConvertToMat(imageToProcess);
}

void Input::ProcessImage(cv::Mat *imageToProcess)
{
    _Img = imageToProcess;
}


cv::Mat* Input::_ConvertToMat(QImage* img)
{
    QImage::Format format = img->format();

    cv::Mat* returnMat = nullptr;

    switch (format) {
    //case QImage::Format_RGB16{}break;

    case QImage::Format_RGB32:
    {
        returnMat = new cv::Mat(img->width(), img->height(), CV_8UC4, img->bits(), img->bytesPerLine());
        return returnMat;
    }break;
    default:
    {
        return returnMat;
    }break;

    }
    //return new cv::Mat(img->width(), img->height(), 1, img->bits(), img->bytesPerLine());
}
