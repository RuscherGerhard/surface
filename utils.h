#include <fstream>


#include "filters/boxfilter.h"
#include "filters/input.h"
#include "filters/output.h"

#define DEBUG_PATH "/home/gerdie/SurfaceDebug.txt"

#ifndef UTILS_H
#define UTILS_H


#define ORIGINAL_IMG 0

//Eigentlich mutiert es zur ItemID.....
typedef enum FilterId
{
    OpInput,
    OpBoxFilter,
    OpOutput,
    Edge,
    Undefined
}_FilterId;

typedef struct ProjectInfo
{
    QString* imagePath;
    std::vector<std::vector<FilterId>>* PipePlan;
}_PrtojectInfo;

#endif // UTILS_H
