#ifndef BOXFILTER_H
#define BOXFILTER_H
#include "filter.h"
class BoxFilter : public Filter
{
public:
    BoxFilter();
    ~BoxFilter();

    virtual QImage* getImage(){return _Image;}
    virtual FilterId getFilterId(){return _FilterId;}
    virtual void setFilterId(const FilterId id){_FilterId = id;}

    virtual void ProcessImage(QImage* imageTorProcess);

private:

    int _FilterRad;
    qreal _egalisierungsDivisor;

    QColor _FilterWindow(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);

};

#endif // BOXFILTER_H
