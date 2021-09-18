#ifndef OUTPUT_H
#define OUTPUT_H

#include "filter.h"

/*Performt die Transformation von CVMat nach QImage*/
class output : public Filter
{
public:
    output();

    virtual QImage* getImage();
    virtual FilterId getFilterId();
    virtual cv::Mat* getImageMat();

    virtual void setFilterId(const FilterId id);

    //Mit Vorsicht zu genießen, da der Process nur _Image oder _Img setzt. Segfault-Gefahr
    virtual void ProcessImage(QImage* imageToProcess);
    virtual void ProcessImage(cv::Mat* imageToProcess);

    virtual void ProcessImagePar(cv::Mat* imageToProcess, const int xS, const int xE, const int yS, const int yE);

    virtual bool getParallel(){return _Parallel;}

};

#endif // OUTPUT_H
