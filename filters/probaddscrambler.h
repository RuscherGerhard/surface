#ifndef PROBADDSCRAMBLER_H
#define PROBADDSCRAMBLER_H
#include "filter.h"

class probAddScrambler : public Filter
{
public:
    probAddScrambler();
    ~probAddScrambler();

    virtual QImage* getImage(){return _Image;}
    virtual FilterId getFilterId(){return _FilterId;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}

    virtual void ProcessImage(QImage* imageToProcess);
private:



};

#endif // PROBADDSCRAMBLER_H
