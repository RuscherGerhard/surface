#include "segmentator.h"

#include "logger.h"

typedef struct comp
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alfa;
}_comp;

typedef union myColor
{
    unsigned int color;
    comp components;
}_MyColor;

segmentator::segmentator()
{
    peaks = (unsigned int*)malloc(sizeof(unsigned int) * 128);
    memset(peaks,0,128*sizeof(unsigned int));
}

segmentator::~segmentator()
{

}

void segmentator::ProcessImage(QImage *imageToProcess)
{
    _grayImg = *imageToProcess;

    _ColorImgToGrayValImg(imageToProcess);

    _Image = &_grayImg;

    _AlphaBetaSelect();

    _Segmentate();
}


void segmentator::_ColorImgToGrayValImg(QImage *imageToProcess)
{

    for(int y = 0; y < imageToProcess->height(); y++)
    {
        QRgb* scan = (QRgb*)imageToProcess->scanLine(y);
        QRgb* scanGray = (QRgb*)_grayImg.scanLine(y);

        for(int x = 0; x < imageToProcess->width(); x++)
        {
               QRgb* pixel = scan+x;
               QRgb* pixelGray = scanGray+x;

               int val =  qGray(*pixel);

               *pixelGray = QColor(val,val,val,val).rgb();
        }
    }


    //_grayImg = *imageToProcess;
}


void segmentator::_AlphaBetaSelect()
{
    //Baue Histogramm auf
    unsigned int histo[256];
    unsigned int alfa = 0;
    unsigned int beta = 0;

    //unsigned int peaks[128];

    memset(histo,0,sizeof(unsigned int)*256);

    for(int y = 0; y < _grayImg.height(); y++)
    {
        uchar* scan = _grayImg.scanLine(y);

        for(int x = 0; x < _grayImg.width();x++)
        {
            histo[*(scan+(x*4))]++;
        }

    }

    //Alfa und Betta wählen
    unsigned int big = 0;
    for(int i= 0; i < 256; i++)
    {
        if(big < histo[i])
        {
            big = histo[i];
            alfa = i;
        }
    }



    big=0;
    for(int i = 0; i < 256; i++)
    {
        if(big < histo[i])
        {
            if(i != alfa)
            {
                big = histo[i];
                beta = i;
            }

        }
    }


    /*int peakIdx = 0;
    for(int i = 0; i < 256; i++)
    {
        if(i > 0 && i < 256)
        {
            unsigned int eins = histo[i-1];
            unsigned int zwei = histo[i];
            unsigned int drei = histo[i+1];

            if(zwei > eins && zwei > drei)
            {

                peaks[peakIdx] = zwei;
                peakIdx++;
            }
        }
        else if(i == 0)
        {
            unsigned int zwei = histo[i];
            unsigned int drei = histo[i+1];
            if(zwei > drei)
            {
                peaks[peakIdx] = zwei;
                peakIdx++;
            }

        }
        else
        {
            unsigned int eins = histo[i-1];
            unsigned int zwei = histo[i];

            if(zwei > eins)
            {
                peaks[peakIdx] = zwei;
                peakIdx++;
            }

        }


    }*/


    _Alfa = alfa;
    _Beta = beta;

    Logger::instance()->WriteToLogFile(std::to_string(alfa).append(" ").append(std::to_string(beta)).c_str());
}

void segmentator::_Segmentate()
{
    myColor teta;
    teta.components.red = (_Alfa + _Beta) / 2;
    teta.components.green = (_Alfa + _Beta) / 2;
    teta.components.blue = (_Alfa + _Beta) / 2;
    teta.components.alfa = 255;
    QRgb myTeta(teta.color);

    //Logger::instance()->WriteToLogFile(std::to_string(teta).c_str());




    myColor color;
    myColor color2;
    color2.color = 0;
    for(int y = 0; y < _grayImg.height(); y++)
    {
        for(int x = 0; x < _grayImg.width(); x++)
        {
            if(_grayImg.pixel(x,y) < myTeta)
            {
                color.components.red = _Alfa;
                color.components.green = _Alfa;
                color.components.blue = _Alfa;
                color.components.alfa = 255;
                _Image->setPixel(x,y,color.color);
            }
            else
            {
                color.components.red = _Beta;
                color.components.green = _Beta;
                color.components.blue = _Beta;
                color.components.alfa = 255;
                _Image->setPixel(x,y,color.color);

            }



        }
    }

    QRgb rgbCol(color.color);
    for(int i = 0; i < _grayImg.height(); i++)
    {
        for(int j = 0; j < _grayImg.width(); j++)
        {
            if(rgbCol != _grayImg.pixel(i,j))
            {
                _grayImg.setPixel(i,j,color2.color);
            }
        }
    }


}
