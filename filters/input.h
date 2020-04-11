#ifndef INPUT_H
#define INPUT_H

#include "filter.h"

class Input : public Filter
{
public:
    Input();

    virtual QImage* getImage();
    virtual FilterId getFilterId();

    virtual void setFilterId(const FilterId id);

    virtual void ProcessImage(QImage* imageToProcess);

};

#endif // INPUT_H
