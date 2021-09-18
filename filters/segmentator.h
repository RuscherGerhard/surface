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

    virtual bool getParallel(){return _Parallel;}

private:
    QImage _grayImg;

    unsigned int* peaks;

    unsigned int _Alfa;
    unsigned int _Beta;

    void _ColorImgToGrayValImg(QImage* imageToProcess);

    void _AlphaBetaSelect();

    void _Segmentate();
};

#endif // SEGMENTATOR_H
