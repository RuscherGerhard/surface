#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include "pipe.h"
#include "filters/filter.h"

class PipeManager
{
public:
    PipeManager();
    ~PipeManager();

    /**
     * @brief UpdatePipes:
     *          bekommt einen Zeiger auf einen Vektor von Vektoren mit FilterIDs.
     *          Nach diesen IDs werden die Pipes aufgebaut!
     * @param std::vector< std::vector<FilterId>* >* Pipes
     *          ist ein Zeiger auf einen Vektor mit Zeigern auf Vektoren von FilterIDs.
     */
    void UpdatePipes(std::vector< std::vector<FilterId>>* Pipes);

    std::vector<std::vector<QImage*>*>* ProcessImage(QImage* imageToProcess);

    bool GetPipeExists();

    std::vector<std::vector<FilterId>>* GetPipePlan();//{return &_PipePlan;}

private:

    std::vector<std::vector<QImage*>*> _ResultVector;

    std::vector<Pipe*> _Pipes;///< Beherbergt die Einzelnen Bildverarbeitungspipes.

    std::vector<std::vector<FilterId>> _PipePlan;

    /**
     * @brief _DeletePipes:
     *          Baut alle Bildverarbeitungspipes ab!
     */
    void _DeletePipes();

    void _DeleteResults();
};

#endif // PIPEMANAGER_H
