#include "gaussfilter.h"

GaussFilter::GaussFilter()
{

}

GaussFilter::~GaussFilter()
{
    delete (_XImage);
}

void GaussFilter::ProcessImage(QImage *imageToProcess)
{
    _Image = new QImage(*imageToProcess);

    //_XImage(_Image->size());

    for( int i = 0; i < imageToProcess->width(); i++)
    {
        for(int j = 0; j < imageToProcess->height(); j++)
        {
            if(i == j)
                _Image->setPixel(i,j, Qt::green);

            //_Image->setPixel( i,j, _FilterWindow(imageToProcess, i,j).rgb());

        }
    }

}


QColor GaussFilter::_FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    return Qt::green;
}


QColor GaussFilter::_FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    return Qt::green;
}
