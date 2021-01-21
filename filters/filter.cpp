#include "filter.h"

QImage* Filter::_ConvertToQImg(cv::Mat* mat, const QImage::Format &format)
{
    return new QImage(mat->data, mat->cols, mat->rows, mat->step, format);
}
