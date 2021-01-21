#include "shapefollower.h"
#include "filterutils.h"


ShapeFollower::ShapeFollower()
{
    _neighbors.resize(8);
    for(unsigned int i = 0; i < 8; i++)
    {
        _neighbors[i] = new QPoint(0,0);
    }
}

ShapeFollower::~ShapeFollower()
{

}



void ShapeFollower::ProcessImage(QImage *imageToProcess)
{
    _Image = new QImage(*imageToProcess);


    _SearchStartingPoint(imageToProcess);
    bool stop = false;

    //QPoint lastP;

    QPoint pa(_p0);
    QPoint qa(_q0);

    myColor shapeColor;
    shapeColor.components.alfa = 255;
    /*shapeColor.components.red = 2;
    shapeColor.components.green = 181;
    shapeColor.components.blue = 160;
    */
    shapeColor.components.red = 255;
    shapeColor.components.green = 255;
    shapeColor.components.blue = 255;


    int stopRad = 1;

    unsigned int stopIter = 10000;//imageToProcess->width();// * imageToProcess->height();
    unsigned int cnt = 0;

    while(stop != true)
    {
        _Neiborhood(&pa,&qa);

        //Den Nachbarn Qi raussuchen, das am weitesten vorne im Vektor sitzt und also den kleinsten index besitzt
        for(unsigned int i = 0; i < _neighbors.size(); i ++)
        {

            //Koordinaten für das zu betrachtende Pixel
            int x = _neighbors[i]->x();
            int y = _neighbors[i]->y();


            bool xCorrect = x < imageToProcess->width() && x >= 0;
            bool yCorrect = y < imageToProcess->height() && y >= 0;

            if( xCorrect && yCorrect)
            {
                //Farbwert, falls 0 haben wir ein Element der Zusammenhangskomponente
                myColor color;
                color.color = imageToProcess->pixel(x,y);


                if(color.components.red == 0)
                {
                    //Bingo
                    _Image->setPixel(x,y, shapeColor.color);


                    //den aktuellen Konturpunkt setzen
                    pa.setX(x);
                    pa.setY(y);

                    //den aktuellen ersten Nabaren setzen
                    if(i == 0)
                    {
                        qa.setX(_neighbors.at(_neighbors.size()-1)->x());
                        qa.setY(_neighbors.at(_neighbors.size()-1)->y());
                    }
                    else
                    {
                        qa.setX(_neighbors.at(i-1)->x());
                        qa.setY(_neighbors.at(i-1)->y());
                    }

                    break;
                }
            }

        }



        /*for(unsigned int i = 0; i < static_cast<unsigned int>(_Image->width()/2); i++)
        {
                _Image->setPixel(i,i, shapeColor.color);
        }*/

        //_ClearNeighborArray();
        /*if(abs(pa.x() - _p0.x()) <= stopRad || abs(pa.y() - _p0.y()) <= stopRad)
        {
            stop =true;
        }*/

        cnt++;

        if(_p0.x() == pa.x() && _p0.y() == pa.y())
        {
            stop = true;
        }
        if(cnt >= stopIter)
        {
            stop = true;
        }
    }
}



void ShapeFollower::_SearchStartingPoint(QImage *image)
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

                for(unsigned int i = 0; i < _neighbors.size(); i++)
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
                }
           }
        }
    }
}


void ShapeFollower::_Neiborhood(QPoint* pi, QPoint* q0)
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


/*
std::vector <QPoint*>* ShapeFollower::_Neiborhood(QPoint* pi, QPoint* q0)
{

    std::vector<QPoint*>* returnVectorPtr = new std::vector<QPoint*>();

    if(q0 == nullptr)
    {
        //ungeordnete nachbarschaft, da kein q0
        int x = pi->x();
        int y = pi->y();

        //q0
        returnVectorPtr->push_back(new QPoint(x+1, y));
        //q1
        returnVectorPtr->push_back(new QPoint(x+1, y-1));
        //q2
        returnVectorPtr->push_back(new QPoint(x, y-1));
        //q3
        returnVectorPtr->push_back(new QPoint(x-1, y-1));
        //q4
        returnVectorPtr->push_back(new QPoint(x-1, y));
        //q5
        returnVectorPtr->push_back(new QPoint(x-1, y+1));
        //q6
        returnVectorPtr->push_back(new QPoint(x, y+1));
        //q7
        returnVectorPtr->push_back(new QPoint(x+1, y+1));
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
        returnVectorPtr->push_back(q0);

        for(unsigned int i = 1; i < 8; i++)
        {
            if(yq == yp-1)
            {
                if(xq == xp-1 )
                {
                    yq = yq - 1;

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
            returnVectorPtr->push_back(new QPoint(xq,yq));
        }
    }

    return returnVectorPtr;
}*/


void ShapeFollower::_ClearNeighborArray()
{
    for(unsigned int i = 0; i < _neighbors.size(); i++)
    {
        QPoint* p = _neighbors.at(i);
        delete p;

        //vec->at(i) = nullptr;
    }
}
