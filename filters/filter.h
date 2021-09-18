#include <QImage>
#include <opencv4/opencv2/core.hpp>

#include <opencv4/opencv2/imgproc.hpp>


#include "../utils.h"

#ifndef FILTER_H
#define FILTER_H

typedef struct bgr{
    u_char blue;
    u_char green;
    u_char red;
    u_char alfa;
}_bgr;

typedef union myColorBgr {
  bgr colorVals;
  u_int color;
}_myColor;



class Filter{
public:

    virtual QImage* getImage()=0;//Deprecated, muss man weg machen
    virtual cv::Mat* getImageMat()=0;

    virtual FilterId getFilterId()=0;

    virtual void setFilterId(const FilterId id)=0;

    virtual void ProcessImage(cv::Mat* imageToProcess)=0;
    virtual void ProcessImage(QImage* imageToProcess)=0;

    virtual void ProcessImagePar(cv::Mat* imageToProcess, const int xS, const int yS, const int xE, const int yE)=0;
    virtual bool getParallel()=0;

    virtual void MakeResultImg(cv::Mat* imgToProcess){_Img = new cv::Mat(*imgToProcess);}


protected:

    bool _Parallel;
    FilterId _FilterId;
    QImage* _Image=nullptr;///< Copy des Resultatbildes
    cv::Mat* _Img=nullptr;


    /**
     * @brief _ConvertToQImg Die Methode konvertiert cm::Mat Objekte nach QImage
     * @param mat   Die Daten/Das Bild
     * @param format    Bildformat, SW oder RGB ....
     * @return  QImage*, ein Zeiger auf das QImage-Bild
     */
    virtual QImage* _ConvertToQImg(cv::Mat* mat, const QImage::Format &format);

    //virtual QColor _FilterWindow(QImage* ImageToProcess, const int PixelPosX, const int PixelPosY)=0;

};

#endif // FILTER_H
