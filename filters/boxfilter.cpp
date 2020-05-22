#include "boxfilter.h"

BoxFilter::BoxFilter()
{
    _FilterRad = 2;
}


BoxFilter::~BoxFilter()
{
}

void BoxFilter::ProcessImage(QImage* imageToProcess)
{
    _Image = imageToProcess;



    for( int i = 0; i < imageToProcess->width(); i++)
    {
        for(int j = 0; j < imageToProcess->height(); j++)
        {
            /*if(i == j)
                imageToProcess->setPixel(i,j, Qt::green);
                */
            _Image->setPixel( i,j, _FilterWindow(imageToProcess, i,j).rgb());

        }
    }

}


QColor BoxFilter::_FilterWindow(QImage* imageToProcess,const int PixelPosX, const int PixelPosY)
{
    QColor returnColor;// = imageToProcess->pixel(PixelPosX, PixelPosY);

    qreal egalisierungsDivisor = (_FilterRad*2+1)*(_FilterRad*2+1);


    int startX = 0;
    int endX = 0;
    int startY = 0;
    int endY = 0;

    //Für x die Zeile machen
    startX = ( (PixelPosX - (_FilterRad+1)) < 0) ? 0 : PixelPosX - _FilterRad - 1;
    endX = ( (PixelPosX + _FilterRad) > imageToProcess->width()) ? (imageToProcess->width()-1) : (PixelPosX + _FilterRad);

    //Für Y die Spalte machen
    startY = ( (PixelPosY - (_FilterRad+1)) < 0) ? 0: PixelPosY - _FilterRad - 1;
    endY = (PixelPosY + _FilterRad) > imageToProcess->height() ? (imageToProcess->height() -1) : PixelPosY + _FilterRad;


    qreal red(0);
    qreal green(0);
    qreal blue(0);
    qreal alpha = QColor(imageToProcess->pixel(0,0)).alphaF();



    for( int i = startX; i < endX; i++)
    {
        for(int j = startY; j < endY; j++)
        {

            QColor color = QColor(imageToProcess->pixel(i,j));

            red = red + color.redF();
            blue = blue + color.blueF();
            green = green + color.greenF();

        }

    }


    returnColor.setRedF(0);
    returnColor.setBlueF(0);
    returnColor.setGreenF(0);


    qreal rationedRed = red/egalisierungsDivisor;
    /*if(rationedRed <= 0)
    {
        rationedRed = QColor(imageToProcess->pixel(PixelPosX, PixelPosY)).redF();
    }*/

    returnColor.setRedF(rationedRed);
    returnColor.setBlueF(blue/egalisierungsDivisor);
    returnColor.setGreenF(green/egalisierungsDivisor);
    returnColor.setAlphaF(alpha);
    return returnColor;
}
