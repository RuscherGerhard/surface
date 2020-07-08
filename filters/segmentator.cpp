#include "segmentator.h"

segmentator::segmentator()
{

}

segmentator::~segmentator()
{

}

void segmentator::ProcessImage(QImage *imageToProcess)
{
    _Image = imageToProcess;
}


void segmentator::_ColorImgToGrayValImg(QImage *imageToProcess)
{
    _grayImg = *imageToProcess;
}
