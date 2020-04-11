#include "input.h"
//#include "../utils.h"


Input::Input()
{
    _Image = nullptr;
    _FilterId = (FilterId)0;
}

QImage* Input::getImage()
{
    return _Image;
}

FilterId Input::getFilterId()
{
    return _FilterId;
}

void Input::setFilterId(const FilterId id)
{
    _FilterId = id;
}

void Input::ProcessImage(QImage* imageToProcess)
{
    _Image = imageToProcess;
}
