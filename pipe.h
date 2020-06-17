#ifndef PIPE_H
#define PIPE_H

#include "utils.h"
#include <QImage>

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

};

#endif // PIPE_H
