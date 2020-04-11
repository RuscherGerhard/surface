#include <fstream>


#include "filters/boxfilter.h"
#include "filters/input.h"
#include "filters/output.h"

#define DEBUG_PATH "/home/gerdie/SurfaceDebug.txt"

#ifndef UTILS_H
#define UTILS_H


//Eigentlich mutiert es zur ItemID.....
typedef enum FilterId
{
    OpInput,
    OpBoxFilter,
    OpOutput,
    Edge,
    Undefined
}_FilterId;

#endif // UTILS_H
