#include "mainwindow.h"
#include "mainifc.h"



int main(int argc, char *argv[])
{
    MainIfc mainIfc(argc, argv);

    return mainIfc.GetQApp()->exec();
}
