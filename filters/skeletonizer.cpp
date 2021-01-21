#include "skeletonizer.h"

#include "filterutils.h"



Skeletonizer::Skeletonizer()
{
    for(unsigned int i = 0; i < 8; i++)
    {
        QPoint* p = new QPoint();
        _neighbors.push_back(p);
    }
}

Skeletonizer::~Skeletonizer()
{
    //free(_Marks);

    for(int i = 0; i < 8; i++)
    {
        delete (_neighbors[i]);
    }
    _neighbors.resize(0);
}

void Skeletonizer::ProcessImage(QImage *imageToProcess)
{
    _Image = new QImage();
    *_Image = *imageToProcess;
    _scndImage = *imageToProcess;
    _thrdImage = *imageToProcess;

    myColor col;
    col.color = 0;
    col.components.alfa = 255;
    col.components.red = 234;

    _scndImage.setPixel(23,23, col.color);


    int width = _Image->width();
    int height = _Image->height();


    //_Marks = (short*)malloc((size_t)(height * width * sizeof(short))); //sizeof(short)*height)*(sizeof(short)*width));
    //memset(_Marks, 0,(size_t)(height * width * sizeof(short))); //sizeof(short)*height)*(sizeof(short)*width));

    //_Marks.resize(height * width * sizeof(short),0);

    _SearchStartingPoint(imageToProcess);

    bool work = true;

    QPoint qActual;
    QPoint qNeib;


    myColor col0;
    myColor col2;
    myColor col4;
    myColor col6;

    myColor col1;
    myColor col3;
    myColor col5;
    myColor col7;

    int workCnt = 0;

    //_scndImage = *_Image;

    while(work)
    {
        *_Image = _thrdImage;


        //erster Eliminierungsschritt
        for(unsigned int y = _p0.y(); y < height-1; y++)
        {
            for(unsigned int x = _p0.x(); x < width-1; x++)
            {
                qActual.setX(x);
                qActual.setY(y);


                 qNeib.setX(x+1);
                 qNeib.setY(y);

                 _Neiborhood(&qActual, &qNeib);


                 myColor pCol;
                 pCol.color = _Image->pixel(x,y);
                 if(pCol.components.red == 0)
                 {

                     col0.color = _Image->pixel(_neighbors[0]->x(), _neighbors[0]->x());
                     col2.color = _Image->pixel(_neighbors[2]->x(), _neighbors[2]->x());
                     col4.color = _Image->pixel(_neighbors[4]->x(), _neighbors[4]->x());
                     col6.color = _Image->pixel(_neighbors[6]->x(), _neighbors[6]->x());

                     col1.color = _Image->pixel(_neighbors[1]->x(), _neighbors[1]->x());
                     col3.color = _Image->pixel(_neighbors[3]->x(), _neighbors[3]->x());
                     col5.color = _Image->pixel(_neighbors[5]->x(), _neighbors[5]->x());
                     col7.color = _Image->pixel(_neighbors[7]->x(), _neighbors[7]->x());



                     if(col0.components.red + col2.components.red + col6.components.red != 0)
                     {
                         if(col0.components.red + col4.components.red + col6.components.red != 0)
                         {
                               int changeCnt = 0;
                               changeCnt = (col0.color != col1.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col1.color != col2.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col2.color != col3.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col3.color != col4.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col4.color != col5.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col5.color != col6.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col6.color != col7.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col7.color != col0.color) ? changeCnt++ : changeCnt;

                               if(changeCnt == 1)
                               {
                                   int memberCnt = 0;
                                   changeCnt = (col0.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col1.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col2.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col3.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col4.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col5.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col6.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col7.components.red == 0) ? memberCnt++ : memberCnt;

                                   if(2 <= memberCnt <= 6)
                                   {
                                        _scndImage.setPixel(qActual.x(), qActual.y(), col.color);
                                   }else {
                                       int foo1 = 0;
                                    }

                               }else{
                                   int foo2 = 0;
                               }
                         }else{
                             int foo3 = 0;
                         }
                     }else{
                         int foo4 = 0;
                     }
                 }else{
                     int foo5=0;
                 }
            }
        }

        //Zweiter Eliminierungsschritt
        _thrdImage = _scndImage;
        for(unsigned int y = _p0.y(); y < height-1; y++)
        {
            for(unsigned int x = _p0.x(); x < width-1; x++)
            {
                qActual.setX(x);
                qActual.setY(y);


                 qNeib.setX(x+1);
                 qNeib.setY(y);

                 _Neiborhood(&qActual, &qNeib);


                 myColor pCol;
                 pCol.color = _scndImage.pixel(x,y);
                 if(pCol.components.red == 0)
                 {

                     col0.color = _scndImage.pixel(_neighbors[0]->x(), _neighbors[0]->x());
                     col2.color = _scndImage.pixel(_neighbors[2]->x(), _neighbors[2]->x());
                     col4.color = _scndImage.pixel(_neighbors[4]->x(), _neighbors[4]->x());
                     col6.color = _scndImage.pixel(_neighbors[6]->x(), _neighbors[6]->x());

                     col1.color = _scndImage.pixel(_neighbors[1]->x(), _neighbors[1]->x());
                     col3.color = _scndImage.pixel(_neighbors[3]->x(), _neighbors[3]->x());
                     col5.color = _scndImage.pixel(_neighbors[5]->x(), _neighbors[5]->x());
                     col7.color = _scndImage.pixel(_neighbors[7]->x(), _neighbors[7]->x());



                     if(col0.components.red + col2.components.red + col6.components.red != 0)
                     {
                         if(col0.components.red + col4.components.red + col6.components.red != 0)
                         {
                               int changeCnt = 0;
                               changeCnt = (col0.color != col1.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col1.color != col2.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col2.color != col3.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col3.color != col4.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col4.color != col5.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col5.color != col6.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col6.color != col7.color) ? changeCnt++ : changeCnt;
                               changeCnt = (col7.color != col0.color) ? changeCnt++ : changeCnt;

                               if(changeCnt == 1)
                               {
                                   int memberCnt = 0;
                                   changeCnt = (col0.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col1.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col2.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col3.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col4.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col5.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col6.components.red == 0) ? memberCnt++ : memberCnt;
                                   changeCnt = (col7.components.red == 0) ? memberCnt++ : memberCnt;

                                   if(2 <= memberCnt <= 6)
                                   {
                                        _thrdImage.setPixel(qActual.x(), qActual.y(), col.color);
                                   }else {
                                       int foo1 = 0;
                                    }

                               }else{
                                   int foo2 = 0;
                               }
                         }else{
                             int foo3 = 0;
                         }
                     }else{
                         int foo4 = 0;
                     }
                 }else{
                     int foo5=0;
                 }
            }
        }


        workCnt++;

        if(_CompareImages(&_scndImage, &_thrdImage) && workCnt >= 100)
        {
            //*_Image = _scndImage;
            work = false;
        }

    }

}


void Skeletonizer::_SearchStartingPoint(QImage *image)
{
    for(unsigned int y= 0; y < image->height(); y++)
    {
        for (unsigned int x = 0; x < image->width(); x++)
        {
            myColor color;
            color.color = image->pixel(x,y);
            if( color.components.red == 0)
            {
                QPoint p(x,y);

                _Neiborhood(&p);

                /*for(unsigned int i = 0; i < _neighbors.size(); i++)
                {
                    QPoint* q = _neighbors[i];

                    myColor color;
                    color.color = image->pixel(q->x(), q->y());
                    if( color.components.red != 0)
                    {

                        //P0 und Q0 setzen
                        _p0 = p;

                        _q0 = *q;
                        break;
                    }

                }*/

            _p0 = p;
            break;

           }
        }
    }
}


void Skeletonizer::_Neiborhood(QPoint* pi, QPoint* q0)
{
    //std::vector<QPoint*>* returnVectorPtr = new std::vector<QPoint*>();

    if(q0 == nullptr)
    {
        //ungeordnete nachbarschaft, da kein q0
        int x = pi->x();
        int y = pi->y();

        //q0
        _neighbors[0]->setX(x+1);
        _neighbors[0]->setY(y);
        //q1
        _neighbors[1]->setX(x+1);
        _neighbors[1]->setY(y-1);
        //q2
        _neighbors[2]->setX(x);
        _neighbors[2]->setY(y-1);
        //q3
        _neighbors[3]->setX(x-1);
        _neighbors[3]->setY(y-1);
        //q4
        _neighbors[4]->setX(x-1);
        _neighbors[4]->setY(y);
        //q5
        _neighbors[5]->setX(x-1);
        _neighbors[5]->setY(y+1);
        //q6
        _neighbors[6]->setX(x);
        _neighbors[6]->setY(y+1);
        //q7
        _neighbors[7]->setX(x+1);
        _neighbors[7]->setY(y+1);
    }
    else
    {
        //Geordnete nachbarschaft, da wir mit q0 dich nachbarschaft durchnumerieren können
        int xp = pi->x();
        int yp = pi->y();

        //Aktuelle Koordinate der Nachbarschaft
        int xq = q0->x();
        int yq = q0->y();

        //Q0 direkt als erstes in die nachbarschaft setzen
        _neighbors[0]= q0;

        for(unsigned int i = 1; i < 8; i++)
        {
            if(yq == yp-1)
            {
                if(xq == xp-1 )
                {
                    yq = yq + 1;

                }
                else
                {
                    xq = xq -1;
                    //returnVectorPtr->push_back(new QPoint(xq,yq));
                }
            }
            else if(yq == yp)
            {
                if(xq == xp -1)
                {
                    yq = yq +1;
                    //returnVectorPtr->push_back(new QPoint(xq,yq));
                }
                else
                {
                    yq = yq -1;
                    //returnVectorPtr->push_back(new QPoint(xq,yq));

                }
            }
            else if( yq == yp+1 )
            {
                if(xq == xp+1)
                {
                    yq = yq -1;
                    //returnVectorPtr->push_back(new QPoint(xq, yq));
                }
                else
                {
                    xq = xq +1;
                }
            }
            _neighbors[i]->setX(xq);
            _neighbors[i]->setY(yq);
        }
    }

    //return returnVectorPtr;
}

bool Skeletonizer::_CompareImages(QImage* img1, QImage* img2)
{
    std::queue<QPoint*> points;

    unsigned int cnt = 0;

    bool ReturnValue = true;

    for(unsigned int y = 0; y < img1->height(); y++)
    {
        for(unsigned int x = 0; x < img1->width(); x++)
        {
            myColor col1;
            col1.color = img1->pixel(x,y);

            myColor col2;
            col2.color = img2->pixel(x,y);

            col1.components.alfa = 255;
            col2.components.alfa = 255;

            if(col1.color != col2.color)
            {
                ReturnValue = false;
                break;
            }
        }
    }

    return ReturnValue;
    //QPoint* Point = new QPoint(0,0);

    //points.push(&_p0);
    //_Traverse(img1, &points, cnt);
}

void Skeletonizer::_Traverse(QImage* imgProcess, std::queue<QPoint*> *queue, unsigned int &recnt)
{
    recnt++;
    int neighbormarkcnt = 0;

    int queueSize = static_cast<int>(queue->size());

    for(int j = 0; j < queueSize; j++)
    {
        QPoint* p = queue->front();
        queue->pop();

        //Markieren
        int x = p->x();
        int y = p->y();

        _Marks[y*_Image->width()+x] = 1;

        myColor markCol;

        markCol.components.alfa = 254;
        markCol.components.red = 200;
        markCol.components.blue = 125;
        markCol.components.green = 125;

        _Image->setPixel(x,y,markCol.color);

        //Nachbarn besorgen
        _Neiborhood(p);

        int cnt=0;
        bool pushPoint = false;
        bool outOfImg = false;
        bool marked = false;
        bool notBlack = false;

        for(unsigned int i = 0; i < _neighbors.size(); i++)
        {
            //Schaun, ob jeder nachbar auch wirklich in der Zusammenhangskomponenten oder im Bild ist
            QPoint* q = _neighbors[i];



            if(q->x() < _Image->width() && q->x() >= 0 && q->y() < _Image->height() && q->y() >= 0)
            {

                short mark = _Marks[(size_t)(q->y()*_Image->width() + q->x())];
                if(mark == 0)
                {
                    myColor color;
                    color.color = 0;
                    color.color = imgProcess->pixel(q->x(), q->y());

                    //unsigned short foo = color.components.red;

                    if(color.components.red < 1 && color.components.green < 1 && color.components.blue < 1 )
                    {
                        //nur wenn er Punkt zur Zusammenhangskomponenten gehört und im Bild ist, dann wird es in die queue gestopft
                        cnt++;
                        //queue->push(q);
                        pushPoint = true;
                    }
                    else
                    {
                        neighbormarkcnt++;
                        notBlack = true;
                    }
                }
                else {
                    marked = true;
                }
            }
            else {


                outOfImg = true;
            }
            if(pushPoint)
            {
                queue->push(q);
                pushPoint = false;
            }
            else
            {
                pushPoint = false;

                if(outOfImg)
                    outOfImg = false;
                if(notBlack)
                    notBlack = false;
                if(marked)
                    marked = false;
            }

        }
    }

    if(neighbormarkcnt == 8)
        int gaga = 0;

    //Wenn kein punkt mehr in die queue gepusht wurde, dann abbrechen!
    if(queue->size() == 0)
    {
        unsigned int cnt = 0;
        for(size_t k = 0; k < _Marks.size(); k++)
        {
            cnt = cnt + _Marks[k];
        }
        return;
    }

    /*QPoint* frontP = queue->front();
    queue->pop();*/
    _Traverse(imgProcess, queue, recnt);
}
