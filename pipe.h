#ifndef PIPE_H
#define PIPE_H

#include "utils.h"
#include <QImage>

//#include "filters/filter.h"
#include <opencv4/opencv2/imgproc.hpp>

class Filter;

class Pipe
{
public:
    Pipe(std::vector<FilterId>* ids);
    ~Pipe();

    std::vector<QImage*>* ProcessImage(QImage* imageToProcess);




private:
    std::vector<Filter*> _FilterQueue;
    QImage _WorkingCopy;
    std::vector<QImage*> _ResultImages;


    /**
     * @brief DeleteFilters:
     *      Löscht alle gespeicherten Filterobjekte und setzten den Filtervektor auf die Länge 0!
     */
    void _DeleteFilters();

    void _AddFilterToPipe(const FilterId id);

    QImage* _ConvertMatToQImage(cv::Mat* image);

};

#endif // PIPE_H
