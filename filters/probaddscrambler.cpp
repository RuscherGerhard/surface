#include "probaddscrambler.h"

#include <math.h>

/*
typedef struct bgr{
    u_char blue;
    u_char green;
    u_char red;
    u_char alfa;
}_bgr;

typedef union myColor {
  bgr colorVals;
  u_int color;
};
*/



probAddScrambler::probAddScrambler()
{
        srand(time(nullptr));

        _Parallel = false;

}

probAddScrambler::~probAddScrambler()
{

}

void probAddScrambler::ProcessImage(QImage *imageToProcess)
{
    int width = imageToProcess->width();
    int height = imageToProcess->height();

    _Image = new QImage(*imageToProcess);



    int filterMod(100);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            QColor currentColor  = imageToProcess->pixelColor(i,j);

            int redRand = currentColor.red()  + rand() % filterMod;
            int blueRand = currentColor.blue()   + rand() % filterMod;
            int greenRand = currentColor.green() + rand() % filterMod;

            int scrambleValBlue  =  (blueRand > 255) ? 255 : blueRand;
            int scrambleValGreen =  (greenRand > 255) ? 255 : greenRand;
            int scramblValRed    =  (redRand > 255) ? 255 : redRand;

            QColor scrambColor;
            scrambColor.setRed(scramblValRed);
            scrambColor.setBlue(scrambleValBlue);
            scrambColor.setGreen(scrambleValGreen);

            _Image->setPixelColor(i,j,scrambColor);
        }
    }


}


void probAddScrambler::ProcessImage(cv::Mat *imageToProcess)
{
    //int width = imageToProcess->width();
    //int height = imageToProcess->height();
    int width = imageToProcess->cols;
    int height = imageToProcess->rows;

    _Img = new cv::Mat(*imageToProcess);

    int matTyp = imageToProcess->type();

    u_int filterMod = 100;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            myColorBgr currentColor;
            //QColor currentColor  = imageToProcess->pixelColor(i,j);

            currentColor.color = imageToProcess->at<u_int>(i,j);
            u_int redRand = currentColor.colorVals.red  + rand() % filterMod;
            u_int blueRand = currentColor.colorVals.blue   + rand() % filterMod;
            u_int greenRand = currentColor.colorVals.green + rand() % filterMod;

            u_char scrambleValBlue  =  (blueRand > 255) ? 255 : blueRand;
            u_char scrambleValGreen =  (greenRand > 255) ? 255 : greenRand;
            u_char scrambleValRed    =  (redRand > 255) ? 255 : redRand;

            currentColor.colorVals.green = scrambleValGreen;
            currentColor.colorVals.red = scrambleValRed;
            currentColor.colorVals.green = scrambleValBlue;

            _Img->at<u_int>(i,j) = currentColor.color;

        }
    }

    myColorBgr black;
    black.colorVals.red  =0;
    black.colorVals.blue = 0;
    black.colorVals.green = 0;
    black.colorVals.alfa = 255;

    for(int k = 0; k < _Img->cols; k++)
    {
        _Img->at<int>(k, 100) = black.color;
        _Img->at<int>(k, 101) = black.color;
        _Img->at<int>(k, 102) = black.color;
    }
}

void  probAddScrambler::ProcessImagePar(cv::Mat* imageToProcess, const int xS, const int yS, const int xE, const int yE)
{
    int width = imageToProcess->cols;
    int height = imageToProcess->rows;

    _Img = new cv::Mat(*imageToProcess);

    int matTyp = imageToProcess->type();

    u_int filterMod = 100;

    for(int i = xS; i < xE; i++)
    {
        for(int j = yS; j < yE; j++)
        {
            myColorBgr currentColor;
            //QColor currentColor  = imageToProcess->pixelColor(i,j);

            currentColor.color = imageToProcess->at<u_int>(i,j);
            u_int redRand = currentColor.colorVals.red  + rand() % filterMod;
            u_int blueRand = currentColor.colorVals.blue   + rand() % filterMod;
            u_int greenRand = currentColor.colorVals.green + rand() % filterMod;

            u_char scrambleValBlue  =  (blueRand > 255) ? 255 : blueRand;
            u_char scrambleValGreen =  (greenRand > 255) ? 255 : greenRand;
            u_char scrambleValRed    =  (redRand > 255) ? 255 : redRand;

            currentColor.colorVals.green = scrambleValGreen;
            currentColor.colorVals.red = scrambleValRed;
            currentColor.colorVals.green = scrambleValBlue;

            _Img->at<u_int>(i,j) = currentColor.color;

        }
    }

   /* myColorBgr black;
    black.colorVals.red  =0;
    black.colorVals.blue = 0;
    black.colorVals.green = 0;
    black.colorVals.alfa = 255;

    for(int k = 0; k < _Img->cols; k++)
    {
        _Img->at<int>(k, 100) = black.color;
        _Img->at<int>(k, 101) = black.color;
        _Img->at<int>(k, 102) = black.color;
    }*/

}


