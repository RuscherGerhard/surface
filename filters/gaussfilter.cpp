#include "gaussfilter.h"
#include <QPixmap>
#include <math.h>
#include "logger.h"
#include "utils.h"

GaussFilter::GaussFilter():
    _FilterRad(2)
{

}

GaussFilter::~GaussFilter()
{
    //delete (_XImage);
}

void GaussFilter::ProcessImage(QImage *imageToProcess)
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


QRgb GaussFilter::_FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    Logger* log = Logger::instance();

    QColor returnColor;

     qreal red   = 0;
     qreal blue  = 0;
     qreal green = 0;

    qreal weight = 0;

    int outStart = (PixelPosX - _FilterRad - 1);
    int outEnd   = (PixelPosX + _FilterRad);
    int start    = ( outStart < 0 ) ? 0 :  outStart;
    int end      = ( outEnd > ImageToProcess->width()) ? ImageToProcess->width() : outEnd;


    for(int i = start; i <= end; i++)
    {
        qreal xDiff = sqrt((i - PixelPosX)*(i - PixelPosX));

        //Das aktuelle gewicht ausrechnen
        qreal gaussGlocke = _g(xDiff);

        //Gewichte aufsummieren
        weight = weight + gaussGlocke;

    }

    QString num;
    num.clear();
    num.append(FILTER);
    num.append("Sum of Weights: ");
    num.append(QString::number(weight));

    log->WriteToLogFile(num.toStdString().c_str());

    QString reds;
    reds.append("Values of red: \n");
    for(int i = start; i <= end; i++)
    {

       qreal xDiff = sqrt((i - PixelPosX)*(i - PixelPosX));

       qreal gaussGlocke = _g(xDiff);



       //weight = weight + gaussGlocke;

       red   = red   + ImageToProcess->pixelColor(i, PixelPosY).red()  * gaussGlocke/ weight;
       green = green + ImageToProcess->pixelColor(i, PixelPosY).green()* gaussGlocke/ weight;
       blue  = blue  + ImageToProcess->pixelColor(i, PixelPosY).blue() * gaussGlocke/ weight;

       reds.append(QString::number(red)+", ");

    }



    int intGreen = (green > 255) ? 255 : static_cast<int>(green);
    int intRed   = (red > 255) ? 255 : static_cast<int>(red);
    int intBlue  = (blue > 255) ? 255 : static_cast<int>(blue);

    reds.append(QString::number(intRed));
    reds.append("\n");

    log->WriteToLogFile(reds.toStdString().c_str());

    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    return returnColor.rgb();
}


QRgb GaussFilter::_FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
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


     for(int i = start; i <= end; i++)
     {
         qreal yDiff = sqrt((i - PixelPosY)*(i - PixelPosY));

         //Das aktuelle gewicht ausrechnen
         qreal gaussGlocke = _g(yDiff);

         //Gewichte aufsummieren
         weight = weight + gaussGlocke;

     }

    for(int i = start; i <= end; i++)
    {

       qreal yDiff = sqrt((i - PixelPosY)*(i - PixelPosY));

       //Das aktuelle gewicht ausrechnen
       qreal gaussGlocke = _g(yDiff);

       red   = red   + ImageToProcess->pixelColor(PixelPosX, i).red()  * gaussGlocke/weight;
       green = green + ImageToProcess->pixelColor(PixelPosX, i).green()* gaussGlocke/weight;
       blue  = blue  + ImageToProcess->pixelColor(PixelPosX, i).blue() * gaussGlocke/weight;

    }

    int intGreen = (green > 255) ? 255 : static_cast<int>(green);
    int intRed   = (red > 255) ? 255 : static_cast<int>(red);
    int intBlue  = (blue > 255) ? 255 : static_cast<int>(blue);

    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    return returnColor.rgb();
}


double GaussFilter::_g(const qreal &diff)
{
    double omega = 0.4;

    double nenner = 2* omega*omega;
    double zaehler = -diff*diff;

    double val = zaehler / nenner;

    return exp(val);
}
