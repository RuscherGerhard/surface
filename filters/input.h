#ifndef INPUT_H
#define INPUT_H

#include "filter.h"

/*Input performt die Transformation von QImage nach CVMat*/
class Input : public Filter
{
 public:
    Input();

    virtual QImage* getImage();
    virtual FilterId getFilterId();
    virtual cv::Mat* getImageMat();

    virtual void setFilterId(const FilterId id);

    virtual void ProcessImage(cv::Mat* imageToProcess);
    virtual void ProcessImage(QImage* imageToProcess);

    virtual void ProcessImagePar(cv::Mat* imageToProcess, const int xS, const int xE, const int yS, const int yE);

    virtual bool getParallel(){return _Parallel;}

   private:
    //virtual QImage* _ConvertToQImg(cv::Mat* mat);

    cv::Mat* _ConvertToMat(QImage* img);

};

#endif // INPUT_H
