#ifndef SEGMENTATOR_H
#define SEGMENTATOR_H

#include "filter.h"


class segmentator: public Filter
{
public:
    segmentator();
    ~segmentator();

    virtual QImage* getImage(){return _Image;}
    virtual FilterId getFilterId(){return _FilterId;}
    virtual void setFilterId(const FilterId id){_FilterId = id;}

    virtual void ProcessImage(QImage* imageToProcess);

private:
    QImage _grayImg;


    void _ColorImgToGrayValImg(QImage* imageToProcess);
};

#endif // SEGMENTATOR_H
