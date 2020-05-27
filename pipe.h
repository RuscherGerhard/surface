#ifndef PIPE_H
#define PIPE_H

//#include <vector>

#include "utils.h"
//#include "filters/filter.h"
#include <QImage>

class Filter;

class Pipe
{
public:
    Pipe(std::vector<FilterId>* ids);
    ~Pipe();

    QImage* ProcessImage(QImage* imageToProcess);




private:
    std::vector<Filter*> _FilterQueue;
    QImage _WorkingCopy;

    /**
     * @brief DeleteFilters:
     *      Löscht alle gespeicherten Filterobjekte und setzten den Filtervektor auf die Länge 0!
     */
    void _DeleteFilters();

    void _AddFilterToPipe(const FilterId id);

};

#endif // PIPE_H
