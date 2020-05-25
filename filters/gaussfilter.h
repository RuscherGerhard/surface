#ifndef GAUSSFILTER_H
#define GAUSSFILTER_H

#include "filter.h"


class GaussFilter : public Filter
{
public:
    GaussFilter();
    ~GaussFilter();

    virtual QImage* getImage(){return _Image;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}
    virtual FilterId getFilterId(){return _FilterId;}


    virtual void ProcessImage(QImage* imageToProcess);

private:
    int _FilterRad;

    QImage _XImage;
    QRgb _FilterWindowX(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);

    QRgb _FilterWindowY(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY);

    /**
     * @brief _g : Die Methode errechnet die GaußGlocke
     * @param diff : Eingabe ist der Geometrische Abstand der Pixel im filterfenster die PositionsDiff
     * @return : ein Wert, der entweder dem Roten-Grünen und oder  Blauem Farbspektrum zugeordnet wird.
     *
     */
    qreal _g(const qreal &diff);

};

#endif // GAUSSFILTER_H
