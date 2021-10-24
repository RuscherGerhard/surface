#include "boxfilter.h"
//#include "filterutils.h"


BoxFilter::BoxFilter()
{
    _Parallel = false;
    _FilterRad = 2;
    _egalisierungsDivisor = (_FilterRad*2+1)*(_FilterRad*2+1);
}


BoxFilter::~BoxFilter()
{
}


/*
 *Der QImage Pfad Bestehend aus ProcessImage und dem FilterWindow Algo für das Filterfenster
 * ACHTUNG: nicht Paralleliziert
 *
 */
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
    qreal alpha = 255.0;//QColor(imageToProcess->pixel(0,0)).alpha();


    for( int i = startX; i < endX; i++)
    {
        for(int j = startY; j < endY; j++)
        {

            QColor color = QColor(imageToProcess->pixel(i,j));

            red = red + color.red()/_egalisierungsDivisor;
            blue = blue + color.blue()/_egalisierungsDivisor;
            green = green + color.green()/_egalisierungsDivisor;

        }
    }


    returnColor.setRed(0);
    returnColor.setBlue(0);
    returnColor.setGreen(0);


    //qreal rationedRed = red/egalisierungsDivisor;
    /*if(rationedRed <= 0)
    {
        rationedRed = QColor(imageToProcess->pixel(PixelPosX, PixelPosY)).redF();
    }*/

    /*QColor curColor = imageToProcess->pixelColor(PixelPosX, PixelPosY);
    if(curColor.redF() > red)
    {
        red = curColor.redF();
    }

    if(curColor.greenF() > green)
    {
        green=curColor.greenF();
    }

    if(curColor.blueF() > blue)
    {
        blue=curColor.blueF();
    }*/


    returnColor.setRed(red);
    returnColor.setBlue(blue);
    returnColor.setGreen(green);
    returnColor.setAlpha(alpha);
    return returnColor;
}

/*
 * Der cvMat-Pfad
 * ACHTUNG: Parallelisiert!
 *
 */
myColorBgr BoxFilter::_FilterWindow(cv::Mat* imageToProcess,const int PixelPosX, const int PixelPosY)
{
    myColorBgr returnColor;// = imageToProcess->pixel(PixelPosX, PixelPosY);
    returnColor.color = 0;

    int startX = 0;
    int endX = 0;
    int startY = 0;
    int endY = 0;

    //Die Bilddimensionen besorgen
    int imgWidth = imageToProcess->cols;
    int imgHeight = imageToProcess->rows;

    //Für x die Zeile machen
    startX = ( (PixelPosX - (_FilterRad+1)) < 0) ? 0 : PixelPosX - _FilterRad - 1;
    endX = ( (PixelPosX + _FilterRad) > imgWidth) ? (imgWidth-1) : (PixelPosX + _FilterRad);

    //Für Y die Spalte machen
    startY = ( (PixelPosY - (_FilterRad+1)) < 0) ? 0: PixelPosY - _FilterRad - 1;
    endY = (PixelPosY + _FilterRad) > imgHeight ? (imgHeight -1) : PixelPosY + _FilterRad;


    float red(0);
    float green(0);
    float blue(0);
    float alpha = 255.0;//QColor(imageToProcess->pixel(0,0)).alpha();

    myColorBgr color;
    color.color = 0;

    for( int i = startX; i < endX; i++)
    {
        for(int j = startY; j < endY; j++)
        {

            color.color = imageToProcess->at<u_int>(i,j);

            red = red + color.colorVals.red/_egalisierungsDivisor;
            blue = blue + color.colorVals.blue/_egalisierungsDivisor;
            green = green + color.colorVals.green/_egalisierungsDivisor;

        }
    }




    returnColor.colorVals.red = static_cast<u_int>(red);
    returnColor.colorVals.blue = static_cast<u_int>(blue);
    returnColor.colorVals.green = static_cast<u_int>(green);

    return returnColor;
}


//Processimage mit cv::Mat
void BoxFilter::ProcessImage(cv::Mat* imageToProcess)
{
    _Img = new cv::Mat(*imageToProcess); //Unschön, kostet um die ganzen werte zu Kopieren.... sollte eigentlich nicht bei Gelegenheit raus....



    for( int i = 0; i < imageToProcess->cols; i++)
    {
        for(int j = 0; j < imageToProcess->rows; j++)
        {
            /*if(i == j)
                imageToProcess->setPixel(i,j, Qt::green);
                */
            _Img->at<u_int>(i,j) = _FilterWindow(imageToProcess, i,j).color;

        }
    }

}

void BoxFilter::ProcessImagePar(cv::Mat *imageToProcess, const int xS, const int yS , const int xE, const int yE)
{

    for( int i = xS; i < xE; i++)
    {
        for(int j = yS; j < yE; j++)
        {
            assert(_Img != NULL);
            _Img->at<u_int>(i,j) = _FilterWindow(imageToProcess, i,j).color;
        }
    }

}






