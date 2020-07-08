#include <fstream>
#include <vector>
#include <qstring.h>

#define DEBUG_PATH "/home/gerdie/SurfaceDebug.txt"

#ifndef UTILS_H
#define UTILS_H


#define ORIGINAL_IMG 0


//Log File Comment Tags
#define FILTER "*[Filter]"
#define BOX_FILTER "*[Box Filter]"
#define PIPE_MAN "*[Pipe Manager]"


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
    OpGaussFilterNL,
    OpProbAddScramb,
    OpSegmentator,
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
