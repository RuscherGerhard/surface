#include "linefindertransversal.h"
#include "filterutils.h"

typedef enum Direction
{
    vertical,
    horizontal,
    mainDiag,
    sndDiag,
    NoDir
}Dirction_;

/*typedef struct comp
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
*/

LineFinderTransversal::LineFinderTransversal()
{

}

LineFinderTransversal::~LineFinderTransversal()
{

}

void LineFinderTransversal::ProcessImage(QImage *imageToProcess)
{
    _Image  = new QImage();
    *_Image = *imageToProcess;
    _grayImg = *imageToProcess;

    _ColorImgToGrayValImg(imageToProcess);

    for(int y = 0; y < _grayImg.height(); y++)
    {
        for(int x = 0; x< _grayImg.width(); x++)
        {
            _LineDetect(x,y, imageToProcess);
        }
    }

}


void LineFinderTransversal::_ColorImgToGrayValImg(QImage *imageToProcess)
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




    *_Image = _grayImg;
}

void LineFinderTransversal::_LineDetect(int posX, int posY, QImage* ImageToProcess)
{
    int teta = 75;

    int rad = 6;

    myColor markColor;
    markColor.components.red = 255;
    markColor.components.green = 255;
    markColor.components.blue = 255;
    markColor.components.alfa = 255;


    for(unsigned int i = vertical; i < NoDir; i++)
    {
        bool colDiff1=false;
        bool colDiff2=false;
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;



        for(int j = 0; j < rad; j++)
        {
            switch(i){
            case vertical:
            {
                x1 = posX;
                y1 = ((posY+j) < ImageToProcess->height()) ? posY+j : ImageToProcess->height();
                x2 = posX;
                y2 = ((posY-j) >= 0) ? posY-j : 0;
            }break;
            case horizontal:{
                x1 = ((posX+j) < ImageToProcess->width()) ? posX + j : ImageToProcess->width();
                y1 = posY;
                x2 = ((posX-j) >= 0) ? posX + j : 0;
                y2 = posY;

            }break;
            case mainDiag:{
                x1 = ((posX+j) < ImageToProcess->width()) ? posX + j: ImageToProcess->width();
                y1 = ((posY+j) < ImageToProcess->height()) ? posY + j: ImageToProcess->height();
                x2 = ((posX-j) >= 0) ? posX-j : 0;
                y2 = ((posY-j) >= 0) ? posY-j : 0;

            }break;
            case sndDiag: {
                x1 = ((posX+j) < ImageToProcess->width()) ? posX + j: ImageToProcess->width();;
                y1 = ((posY-j) >= 0) ? posY-j : 0;;
                x2 = ((posX-j) >= 0) ? posX-j : 0;;
                y2 = ((posY+j) < ImageToProcess->height()) ? posY + j: ImageToProcess->height();

            }break;
            }

            //Die Farbwerte besorgen
            myColor color1;
            color1.color = ImageToProcess->pixel(x1,y1);
            myColor color2;
            color2.color = ImageToProcess->pixel(x2, y2);
            myColor color;
            color.color = ImageToProcess->pixel(posX, posY);

            //Schauen ob die die Wertediff größer dem Kontrastwert sind.
            if(!colDiff1)
                colDiff1 = (color.components.red - color1.components.red) >= teta ? true : false; //(abs(color1.components.red - color.components.red) >= teta)? true : false;
            if(!colDiff2)
                colDiff2 = (color.components.red - color2.components.red) >= teta ? true : false; //(abs(color2.components.red - color.components.red) >= teta)? true : false;


            //Aktuellen Pixel markieren oder nicht....
            if(colDiff1 && colDiff2)
            {
                _Image->setPixel(posX,posY, markColor.color);
                break;
            }
        }
        if(colDiff1 && colDiff2)//Wenn dieses Argument zieht, dann sind wir aus der inneren Schleife ausgebrochen und können aber auch hier schon abbrechen.
            break;
    }

}
