#include <fstream>


#include "filters/boxfilter.h"
#include "filters/input.h"
#include "filters/output.h"
#include "filters/gaussfilter.h"

#define DEBUG_PATH "/home/gerdie/SurfaceDebug.txt"

#ifndef UTILS_H
#define UTILS_H


#define ORIGINAL_IMG 0


typedef enum Error_Codes
{
    ERROR_SUCCESS,
    TOTAL_ERROR,
    NO_IMG_FOUND,
    NO_PIPES_FOUND
}_Error_Codes;

//Eigentlich mutiert es zur ItemID.....
typedef enum FilterId
{
    OpInput,
    OpOutput,
    Edge,
    OpBoxFilter,
    OpGaussFilter,
    Undefined
}_FilterId;



/**
 * TODO
 * Die Zeiger aus dem Projectinfo gegen Vollwertige Instanzen austauschen und Übergabe der Info durch Referenzübergabe
 * ersetzen!
 */
typedef struct ProjectInfo
{
    QString* imagePath;
    std::vector<std::vector<FilterId>>* PipePlan;
}_PrtojectInfo;



#endif // UTILS_H
