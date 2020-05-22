#ifndef GAUSSFILTER_H
#define GAUSSFILTER_H

#include "filter.h"


class GaussFilter : public Filter
{
public:
    GaussFilter();
    ~GaussFilter();

    virtual QImage* getImage(){return _Image;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}
    virtual FilterId getFilterId(){return _FilterId;}


    virtual void ProcessImage(QImage* imageToProcess);

private:
    int _FilterRad;

    QImage* _XImage;
    QColor _FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);

    QColor _FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);

};

#endif // GAUSSFILTER_H
