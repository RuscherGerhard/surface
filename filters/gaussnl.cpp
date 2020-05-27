#include "gaussnl.h"
#include <math.h>


GaussNL::GaussNL():
    _FilterRad(1)
{

}

GaussNL::~GaussNL()
{
    //delete (_XImage);
}

void GaussNL::ProcessImage(QImage *imageToProcess)
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


QRgb GaussNL::_FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    QColor returnColor;
    QColor currentColor(ImageToProcess->pixelColor(PixelPosX, PixelPosY));

     qreal red   = 0;
     qreal blue  = 0;
     qreal green = 0;

    qreal weight = 0;

    //Anfangs und Endpixel bestimmen!
    int outStart = (PixelPosX - _FilterRad - 1);
    int outEnd   = (PixelPosX + _FilterRad);
    int start    = ( outStart < 0 ) ? 0 :  outStart;
    int end      = ( outEnd > ImageToProcess->width()) ? ImageToProcess->width() : outEnd;



    for(int i = start; i < end; i++)
    {

       //Die Farbe des aktuellen Filterfensterpixels holen
       QColor pixelColor = ImageToProcess->pixelColor(i,PixelPosY);

       //Den euklidischen Abstand im Ortsraum zwischen dem zu betrachtenden pixel und dem Pixel des Filterfensters ausrechnen
       int xDiff = i - PixelPosX;//Achtung: Da wir y fest halten folgt hier y = 0 also sqrt(0 + (i- PixelposX)*(i- PixelposX))

       //Den Farbabstand der beiden zu betrachtenden Pixel ausrechnen
       int redDiff   = currentColor.red()   - pixelColor.red();
       int blueDiff  = currentColor.blue()  - pixelColor.blue();
       int greenDiff = currentColor.green() - pixelColor.green();
       qreal cDiff = sqrt(redDiff * redDiff + greenDiff * greenDiff + blueDiff * blueDiff);//Der euklidischer Abstand der Farbwerte

       //Gewichtung nach der Gausfunktion, hier aus dem Farblichen und Räumlichen Abstand
       qreal gaussGlocke = _g(xDiff) * _g(cDiff);

       //Die Normierung mitnehmen!
       weight = weight + gaussGlocke;

       //Aufaddieren des aktuellen Farbwertes für die aktuelle Filterfensterposition!
       red   = red   + ( redDiff   )* gaussGlocke;
       green = green + ( greenDiff )* gaussGlocke;
       blue  = blue  + ( blueDiff  )* gaussGlocke;
    }

    //Normieren und setzen
    int intGreen = (green/weight > 255.0) ? 255 : static_cast<int>(green/weight);
    int intRed   = (red/weight   > 255.0) ? 255 : static_cast<int>(red/weight);
    int intBlue  = (blue/weight  > 255.0) ? 255 : static_cast<int>(blue/weight);

    //Ausgabe
    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    return returnColor.rgb();
}


QRgb GaussNL::_FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)
{
    QColor returnColor;
    QColor currentColor(ImageToProcess->pixelColor(PixelPosX, PixelPosY));


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
       QColor pixelColor = ImageToProcess->pixelColor(PixelPosX,i);

       int yDiff = i - PixelPosY;

       int redDiff   = currentColor.red()   - pixelColor.red();
       int blueDiff  = currentColor.blue()  - pixelColor.blue();
       int greenDiff = currentColor.green() - pixelColor.green();
       qreal cDiff = sqrt(redDiff * redDiff + greenDiff * greenDiff + blueDiff * blueDiff);//Der euklidischer Abstand der Farbwerte

       qreal gaussGlocke = _g(yDiff)* _g(cDiff);

       weight = weight + gaussGlocke;

       red   = red   + (redDiff )* gaussGlocke;
       green = green + (greenDiff )* gaussGlocke;
       blue  = blue  + (blueDiff )* gaussGlocke;

    }

    int intGreen = (green/weight > 255) ? 255 : static_cast<int>(green/weight);
    int intRed   = (red/weight   > 255) ? 255 : static_cast<int>(red/weight);
    int intBlue  = (blue/weight  > 255) ? 255 : static_cast<int>(blue/weight);

    returnColor.setGreen(intGreen);
    returnColor.setRed(intRed);
    returnColor.setBlue(intBlue);

    return returnColor.rgb();
}


qreal GaussNL::_g(const qreal &diff)
{
    qreal omega = 1.5;

    qreal val = -diff*diff / 2* omega*omega;

    return exp(val);
}

