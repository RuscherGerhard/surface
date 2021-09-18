#ifndef FILTERUTILS_H
#define FILTERUTILS_H
#include <opencv4/opencv2/imgproc.hpp>
typedef struct comp
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alfa;
}_comp;

typedef union myColor
{
    unsigned int color;
    comp components;
}_MyColor;



#endif // FILTERUTILS_H
