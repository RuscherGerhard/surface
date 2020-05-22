#include <QImage>
#include "../utils.h"

#ifndef FILTER_H
#define FILTER_H
class Filter{
public:

    virtual QImage* getImage()=0;
    virtual FilterId getFilterId()=0;

    virtual void setFilterId(const FilterId id)=0;

    virtual void ProcessImage(QImage* imageToProcess)=0;


protected:
    FilterId _FilterId;
    QImage* _Image=nullptr;///< Copy des Resultatbildes

    //virtual QColor _FilterWindow(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)=0;

};

#endif // FILTER_H
