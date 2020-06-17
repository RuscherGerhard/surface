#include "probaddscrambler.h"

#include <math.h>


probAddScrambler::probAddScrambler()
{
        srand(time(NULL));
}

probAddScrambler::~probAddScrambler()
{

}

void probAddScrambler::ProcessImage(QImage *imageToProcess)
{
    int width = imageToProcess->width();
    int height = imageToProcess->height();

    _Image = new QImage(*imageToProcess);



    int filterMod(50);

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