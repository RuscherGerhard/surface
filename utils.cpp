#include "utils.h"

std::vector<std::vector<int>>* utils::Histogram(QImage* image)
{
    //instaziiren eines Arrays
    std::vector<std::vector<int>>* histo = new std::vector<std::vector<int>>();

    for(int i = 0; i < 3; i++)
    {
        histo->push_back( std::vector<int>(256));
    }

    for(int x = 0; x < image->width(); x++)
    {
        for(int y = 0; y < image->height(); y++)
        {
            QColor color = image->pixel(x,y);

            int red = color.red();
            histo->at(0).at(red)++;

            int green = color.green();
            histo->at(1).at(green)++;

            int blue = color.blue();
            histo->at(2).at(blue)++;

        }
    }

    return histo;
}

/*pipes::pipes()
{
    _AmountOfPipes = static_cast<int>(_Pipes.size());
}*/

////////////////////////////
// Klasse pipeScheme
