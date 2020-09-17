#include "gaussfilter.h"
#include <QPixmap>
#include <math.h>
#include "logger.h"
#include "utils.h"
#include "filterutils.h"

GaussFilter::GaussFilter():
    _FilterRad(5)
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


int GaussFilter::_FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
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
    for(int j = start; j <= end; j++)
    {

       qreal xDiff = sqrt((j - PixelPosX)*(j - PixelPosX));

       qreal gaussGlocke = _g(xDiff);


       //weight = weight + gaussGlocke;

       myColor tmp;
       tmp.color = ImageToProcess->pixel(j, PixelPosY);

       red   = red   + tmp.components.red  * gaussGlocke/ weight;
       green = green + tmp.components.green * gaussGlocke/ weight;
       blue  = blue  + tmp.components.blue * gaussGlocke/ weight;


       /*red   = red   + ImageToProcess->pixelColor(j, PixelPosY).red()  * gaussGlocke/ weight;
       green = green + ImageToProcess->pixelColor(j, PixelPosY).green()* gaussGlocke/ weight;
       blue  = blue  + ImageToProcess->pixelColor(j, PixelPosY).blue() * gaussGlocke/ weight;*/

       reds.append(QString::number(red)+", ");

    }



    int intGreen = (green > 255.0) ? 255 : static_cast<int>(green);
    int intRed   = (red > 255.0) ? 255 : static_cast<int>(red);
    int intBlue  = (blue > 255.0) ? 255 : static_cast<int>(blue);

    reds.append(QString::number(intRed));
    reds.append("\n");

    log->WriteToLogFile(reds.toStdString().c_str());

    /*returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);*/

    //return returnColor.rgb();

    myColor color;
    color.components.red = intRed;
    color.components.green = intGreen;
    color.components.blue = intBlue;
    color.components.alfa = 255;

    return color.color;

}


int GaussFilter::_FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
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

    for(int j = start; j <= end; j++)
    {

       qreal yDiff = sqrt((j - PixelPosY)*(j - PixelPosY));

       //Das aktuelle gewicht ausrechnen
       qreal gaussGlocke = _g(yDiff);


       myColor tmp;
       tmp.color = ImageToProcess->pixel(PixelPosX, j);

       red   = red   + tmp.components.red  * gaussGlocke/ weight;
       green = green + tmp.components.green * gaussGlocke/ weight;
       blue  = blue  + tmp.components.blue * gaussGlocke/ weight;


       /*red   = red   + ImageToProcess->pixelColor(PixelPosX, i).red()  * gaussGlocke/weight;
       green = green + ImageToProcess->pixelColor(PixelPosX, i).green()* gaussGlocke/weight;
       blue  = blue  + ImageToProcess->pixelColor(PixelPosX, i).blue() * gaussGlocke/weight;*/

    }

    int intGreen = (green > 25.0) ? 255 : static_cast<int>(green);
    int intRed   = (red > 255.0) ? 255 : static_cast<int>(red);
    int intBlue  = (blue > 255.0) ? 255 : static_cast<int>(blue);

    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    //return returnColor.rgb();

    myColor color;
    color.components.green = intGreen;
    color.components.blue = intBlue;
    color.components.red = intRed;
    color.components.alfa = 255;

    return color.color;
}


double GaussFilter::_g(const qreal &diff)
{
    double omega = 0.4;

    double nenner = 2* omega*omega;
    double zaehler = -diff*diff;

    double val = zaehler / nenner;

    return exp(val);
}
