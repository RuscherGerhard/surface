#include "probaddscrambler.h"

#include <math.h>


probAddScrambler::probAddScrambler()
{

}

probAddScrambler::~probAddScrambler()
{

}

void probAddScrambler::ProcessImage(QImage *imageToProcess)
{
    int width = imageToProcess->width();
    int height = imageToProcess->height();

    _Image = new QImage(*imageToProcess);

    srand(time(NULL));

    int filterMod(100);

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            QColor currentColor  = imageToProcess->pixelColor(i,j);

            int redRand = currentColor.blue()   + rand() % filterMod;
            int blueRand = currentColor.green() + rand() % filterMod;
            int greenRand = currentColor.red()  + rand() % filterMod;

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
