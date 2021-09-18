#ifndef BOXFILTER_H
#define BOXFILTER_H
#include "filter.h"

//union myColorBgr;
class BoxFilter : public Filter
{
public:
    BoxFilter();
    ~BoxFilter();

    virtual QImage* getImage(){return _Image;}
    virtual FilterId getFilterId(){return _FilterId;}
    virtual cv::Mat* getImageMat(){return _Img;}
    virtual void setFilterId(const FilterId id){_FilterId = id;}
    virtual bool getParallel(){return _Parallel;}


    virtual void ProcessImage(QImage* imageTorProcess);
    virtual void ProcessImage(cv::Mat* imageToProcess);

    virtual void ProcessImagePar(cv::Mat* iamgeToProcess, const int xS, const int xE, const int yS, const int yE);



private:

    int _FilterRad;
    qreal _egalisierungsDivisor;

    QColor _FilterWindow(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);

    myColorBgr _FilterWindow(cv::Mat* ImageToProcess, const int PixelPosX, const int PixelPosY);

};

#endif // BOXFILTER_H
