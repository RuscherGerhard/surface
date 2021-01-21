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

    virtual void setFilterId(const FilterId id);

    virtual void ProcessImage(QImage* imageToProcess);

};

#endif // OUTPUT_H
