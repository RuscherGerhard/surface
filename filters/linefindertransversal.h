#ifndef LINEFINDERTRANSVERSAL_H
#define LINEFINDERTRANSVERSAL_H

#include "filter.h"

class LineFinderTransversal : public Filter
{

public:
    LineFinderTransversal();
    ~LineFinderTransversal();

    virtual QImage* getImage(){return _Image;}
    virtual FilterId getFilterId(){return _FilterId;}

    virtual void setFilterId(const FilterId id){_FilterId = id;};

    virtual void ProcessImage(QImage* imageToProcess);

private:

    QImage _grayImg;

    void _ColorImgToGrayValImg(QImage *imageToProcess);

    void _LineDetect(int posX, int posY, QImage* ImageToProcess);


};

#endif // LINEFINDERTRANSVERSAL_H
