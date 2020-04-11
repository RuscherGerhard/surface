#include "output.h"



output::output()
{
    _FilterId = OpOutput;
    _Image = nullptr;
}


QImage* output::getImage()
{
    return _Image;
}

FilterId output::getFilterId()
{
    return _FilterId;
}

void output::setFilterId(const FilterId id)
{
    _FilterId = id;
}

void output::ProcessImage(QImage *imageToProcess)
{
    _Image = imageToProcess;
}
