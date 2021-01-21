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

    virtual void setFilterId(const FilterId id);

    virtual void ProcessImage(QImage* imageToProcess);

   private:
    //virtual QImage* _ConvertToQImg(cv::Mat* mat);

    cv::Mat* _ConvertToMat(QImage* img);

};

#endif // INPUT_H
