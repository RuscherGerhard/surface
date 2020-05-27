#include "gaussfilternl1.h"

/*gaussFilterNL1::gaussFilterNL1()
{

}*/

#include "gaussfilterNL.h"

//#include <QPixmap>
#include <math.h>


GaussFilterNL::GaussFilterNL():
    _FilterRad(1)
{

}

GaussFilterNL::~GaussFilterNL()
{
    //delete (_XImage);
}

void GaussFilterNL::ProcessImage(QImage *imageToProcess)
{
    _Image  = new QImage(*imageToProcess);
    _XImage = QImage(*imageToProcess);

    for( int i = 0; i < imageToProcess->width(); i++)
    {
        for(int j = 0; j < imageToProcess->height(); j++)
        {
            _XImage.setPixel(i,j, _FilterWindowX(imageToProcess, i,j));
        }
    }

    for( int k = 0; k < imageToProcess->width(); k++)
    {
        for(int l = 0; l < imageToProcess->height(); l++)
        {

            _Image->setPixel(k, l, _FilterWindowY(&_XImage, k,l));
        }
    }


}


QRgb GaussFilterNL::_FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    QColor returnColor;

     qreal red   = 0;
     qreal blue  = 0;
     qreal green = 0;

    int weight = 0;

    int outStart = (PixelPosX - _FilterRad - 1);
    int outEnd   = (PixelPosX + _FilterRad);
    int start    = ( outStart < 0 ) ? 0 :  outStart;
    int end      = ( outEnd > ImageToProcess->width()) ? ImageToProcess->width() : outEnd;



    for(int i = start; i < end; i++)
    {

       int xDiff = i - PixelPosX;

       qreal gaussGlocke = _g(xDiff);

       weight = weight + gaussGlocke;

       red   = red   + ImageToProcess->pixelColor(i, PixelPosY).red()  * gaussGlocke;
       green = green + ImageToProcess->pixelColor(i, PixelPosY).green()* gaussGlocke;
       blue  = blue  + ImageToProcess->pixelColor(i, PixelPosY).blue() * gaussGlocke;


    }

    int intGreen = (green/weight > 255) ? 255 : static_cast<int>(green/weight);
    int intRed   = (red/weight > 255) ? 255 : static_cast<int>(red/weight);
    int intBlue  = (blue/weight > 255) ? 255 : static_cast<int>(blue/weight);

    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    return returnColor.rgb();
}


QRgb GaussFilterNL::_FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    QColor returnColor;

     qreal red   = 0;
     qreal blue  = 0;
     qreal green = 0;

     int weight = 0;

    int outStart = (PixelPosY - _FilterRad - 1);
    int outEnd   = (PixelPosY + _FilterRad);
    int start    = ( outStart < 0 ) ? 0 :  outStart;
    int end      = ( outEnd > ImageToProcess->height()) ? ImageToProcess->height() : outEnd;


    for(int i = start; i < end; i++)
    {

       int yDiff = i - PixelPosY;


       qreal gaussGlocke = _g(yDiff);
       weight = weight + gaussGlocke;

       red   = red   + ImageToProcess->pixelColor(PixelPosX, i).red()  * gaussGlocke;
       green = green + ImageToProcess->pixelColor(PixelPosX, i).green()* gaussGlocke;
       blue  = blue  + ImageToProcess->pixelColor(PixelPosX, i).blue() * gaussGlocke;

    }

    int intGreen = (green/weight > 255) ? 255 : static_cast<int>(green/weight);
    int intRed   = (red/weight > 255) ? 255 : static_cast<int>(red/weight);
    int intBlue  = (blue/weight > 255) ? 255 : static_cast<int>(blue/weight);

    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    return returnColor.rgb();
}


qreal GaussFilterNL::_g(const qreal &diff)
{
    qreal omega = 1.5;

    qreal val = -diff*diff / 2* omega*omega;

    return exp(val);
}

