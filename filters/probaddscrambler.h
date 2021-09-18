#ifndef PROBADDSCRAMBLER_H
#define PROBADDSCRAMBLER_H
#include "filter.h"

class probAddScrambler : public Filter
{
public:
    probAddScrambler();
    ~probAddScrambler();

    virtual QImage* getImage(){return _Image;}//Deprecated, muss man weg machen
    virtual cv::Mat* getImageMat(){return _Img;}
    virtual FilterId getFilterId(){return _FilterId;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}

    virtual void ProcessImage(QImage* imageToProcess);
    virtual void ProcessImage(cv::Mat* imageToProcess);

    virtual void ProcessImagePar(cv::Mat* imageToProcess, const int xS, const int yS, const int xE, const int yE);


    virtual bool getParallel(){return _Parallel;}
private:



};

#endif // PROBADDSCRAMBLER_H
