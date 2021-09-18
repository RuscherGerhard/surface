#ifndef GAUSSFILTER_H
#define GAUSSFILTER_H

#include "filter.h"


class GaussFilter : public Filter
{
public:
    GaussFilter();
    ~GaussFilter();

    virtual QImage* getImage(){return _Image;}
    virtual cv::Mat* getImageMat(){return _Img;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}
    virtual FilterId getFilterId(){return _FilterId;}


    virtual void ProcessImage(QImage* imageToProcess);
    virtual void ProcessImage(cv::Mat* imageToProcess);

    virtual bool getParallel(){return _Parallel;}

private:
    int _FilterRad;

    QImage _XImage;
    cv::Mat* _XImgMat;
    /*Gaußfilter sind Separierbar daher wird einmal vertical und einmal Horizontal gefiltert*/

    int _FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);
    int _FilterWindowX(cv::Mat* ImageToProcess, const int PixelPosX, const int PixelPosY);

    int _FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);
    int _FilterWindowY(cv::Mat* ImageToProcess, const int PixelPosX, const int PixelPosY);

    /**
     * @brief _g : Die Methode errechnet die GaußGlocke
     * @param diff : Eingabe ist der Geometrische Abstand der Pixel im filterfenster die PositionsDiff
     * @return : ein Wert, der entweder dem Roten-Grünen und oder  Blauem Farbspektrum zugeordnet wird.
     *
     */
    double _g(const qreal &diff);

};

#endif // GAUSSFILTER_H
