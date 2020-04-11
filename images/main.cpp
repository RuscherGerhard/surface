#include <iostream>

#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <math.h>

using namespace cv;
using namespace std;




unsigned /*very*/ char filter(int zeile, int spalte, Mat* img_src)
{
    char f1 = 0;
    char f2 = 1;
    char f3 = -1;

    char wertMitte = img_src->at<char>(zeile, spalte);
    char wert1 = 0;
    char wert3 = 0;

    char result = 0;

    // senity checks
    if( spalte +1 >= img_src->cols)
    {
        wert1 = img_src->at<char>(zeile, spalte -1);
        result = wert1*f1 + wertMitte*f2;

    }
    else if(spalte - 1 < 0 )
    {
        wert3 = img_src->at<char>(zeile, spalte -1);
        result = wertMitte*f2 + wert3 * f3;
    }
    else
    {
        wert1 = img_src->at<char>(zeile, spalte -1);
        wert3 = img_src->at<char>(zeile, spalte -1);
        result = wert1 * f1 + wertMitte * f2 + wert3 * f3;
    }

    return result;
}


    char aY[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    char aX[3][3] = {{-1,0,1}, {-2,0,2},{-1,0,1}};


char sobelOps(int zeile, int spalte, Mat* img_src)
{
    char result;

    bool obererRand = false;
    bool untererRand = false;
    bool linkerRand = false;
    bool rechterRand = false;

    //Anzahl der Zeilen und Spalten beschaffen
    int img_cols = img_src->cols;
    int img_rows = img_src->rows;


    //wo befindet sich der zu manipulierende punkt?
    if(zeile -1 < 0)
        obererRand = true;
    if(zeile >= img_rows -1)
        untererRand = true;
    if(spalte -1 < 0)
        linkerRand = true;
    if(spalte >= img_cols -1)
        rechterRand = true;

    char x = 0;
    char y = 0;

    if(obererRand && linkerRand)
    {
        //Links Oben.
        for(unsigned int i = 1; i < 3; i++)
        {
            for(unsigned int j = 1; j < 3; j++)
            {
                x = img_src->at<char>(i, j) * aX[i][j] + x;
                y = img_src->at<char>(i, j) * aY[i][j] + y;
            }
        }


    }
    else if(obererRand && rechterRand)
    {
        //Oben Rechts
        //Zeileniteration
        for(unsigned int i = 1; i < 3; i++)
        {
            int zeile_lok = i - 1;
            //Spalteniteration
            for(unsigned int j = 0; j < 2; j++)
            {
                //zeile und spalte berechnen
                int spalte_lok = img_cols -2 + j;

                x = img_src->at<char>(zeile_lok, spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok, spalte_lok) * aY[i][j] + y;
            }
        }
    }
    else if(untererRand && linkerRand)
    {
        //Unten Links
         //Zeileniteration
        for(int i = 0; i < 2; i++)
        {
            int zeile_lok = img_rows -2 + i;
            for(int j = 1; j < 3; j++)
            {
                //Zeile und Spalte berechnen

                int spalte_lok = i -1;
                x = img_src->at<char>(zeile_lok,spalte_lok) * aX[i][j] +x;
                y = img_src->at<char>(zeile_lok,spalte_lok) * aY[i][j] +y;
            }
        }
    }
    else if(untererRand && rechterRand)
    {
        //Unten Rechts
        //Zeileniteration
        for(int i = 0; i < 2; i++)
        {
            //Zeile
            int zeile_lok = img_rows -2 + i;
            for(int j = 0; j < 2; j++)
            {
                int spalte_lok = img_cols -2 +i;
                x = img_src->at<char>(zeile_lok,spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok,spalte_lok) * aY[i][j] + y;
            }
        }

    }
    else if(obererRand)
    {
        //Zeileniteration
        for(int i = 1; i < 3; i++)
        {
            int zeile_lok = zeile - 1 +i;
            //Spalteniteration
            for(int j = 0; j < 3; j++)
            {
                int spalte_lok = spalte-1 + j;

                x = img_src->at<char>(zeile_lok, spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok, spalte_lok) * aY[i][j] + y;
            }
        }
    }
    else if(untererRand)
    {
        for(int i = 0; i < 2; i++)
        {
            int zeile_lok = img_rows - 2 + i;
            for(int j = 0; j < 3; j++)
            {
                int spalte_lok = spalte - 1 + j;
                x = img_src->at<char>(zeile_lok, spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok, spalte_lok) * aY[i][j] + y;
            }
        }
    }
    else if(linkerRand)
    {
        for(int i = 0; i < 3; i++)
        {
            int zeile_lok = zeile -1 +i;
            for(int j = 1; j < 3; j++)
            {
                int spalte_lok = spalte-1 +j;

                x = img_src->at<char>(zeile_lok, spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok, spalte_lok) * aY[i][j] + y;
            }
        }
    }
    else if(rechterRand)
    {
        //Rechter Rand
        for(int i = 0; i < 3; i++)
        {
            int zeile_lok = zeile - 1 +i;
            for(int j = 0; j < 2; j++)
            {
                int spalte_lok = spalte -1 +j;
                x = img_src->at<char>(zeile_lok, spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok, spalte_lok) * aY[i][j] + y;
            }
        }
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            int zeile_lok = zeile - 1 +i;
            for(int j = 0; j < 3; j++)
            {
                int spalte_lok = spalte -1 +j;
                x = img_src->at<char>(zeile_lok, spalte_lok) * aX[i][j] + x;
                y = img_src->at<char>(zeile_lok, spalte_lok) * aY[i][j] + y;
            }
        }

    }
    result = x+y;
    return result;
}


char probabilistic_additive_scrambler(int zeile, int spalte, Mat* img_src)
{
    char s = (rand() % 50);
    float p = (rand() % 60);

    char result = img_src->at<char>(zeile, spalte);

    float p_Ablehn = (60/(s+1));

    if(p_Ablehn < p)
    {
        result = result + s % 50;
    }

    //std::cout << a << " ";

    return result;
}

char box_filter(int zeile, int spalte, Mat* img_src)
{
    int rad = 2;
    int summand = rad +1;

    int M = (2*rad +1);
    float divisor =  (M*M);

    float result = 0;

    //Positionsflaggen
    bool linkerRand  = false;
    bool rechterRand = false;
    bool obererRand  = false;
    bool untererRand = false;

    //Dimensionen des Bildes
    int rows = img_src->rows;
    int cols = img_src->cols;

    if(zeile - summand < 0)
    {
        obererRand = true;
    }
    else if(zeile + summand >= rows)
    {
        untererRand = true;
    }

    if(spalte - summand < 0 )
    {
        linkerRand = true;
    }
    else if(spalte + summand >= cols)
    {
        rechterRand = true;
    }

    //Wertberechnung
    if(obererRand && linkerRand)
    {
        /*int mittler = 1 / summand * summand;

        // Links Oben!
        for(int i = 0; i < summand; i++)//summand = rad+1;
        {
            for(int j = 0; j < summand; j++)
            {
                result = img_src->at<short>(i,j) * mittler;
            }
        }*/

        return img_src->at<char>(zeile, spalte);
    }
    else if(obererRand && rechterRand)
    {
        //Oben Rechts
        /*for(int i = 0; i < summand; i++)//summand = rad+1;
        {
            for(int j = 0; j < summand; j++)
            {
                result = img_src->at<short>(i,j) * mittler;
            }
        }*/
        return img_src->at<char>(zeile, spalte);

    }
    else if(untererRand && linkerRand)
    {
        return img_src->at<char>(zeile, spalte);
    }
    else if(untererRand && rechterRand)
    {
        return img_src->at<char>(zeile, spalte);
    }
    else if(rechterRand)
    {
        return img_src->at<char>(zeile, spalte);
    }
    else if(linkerRand)
    {
        return img_src->at<char>(zeile, spalte);
    }
    else if(obererRand)
    {
        return img_src->at<char>(zeile, spalte);
    }
    else if(untererRand)
    {
        return img_src->at<char>(zeile, spalte);
    }
    else
    {
        //Business as usual!!!!!

        for(int i = zeile-rad; i < zeile+rad; i++)
        {
            for(int j = spalte-rad; j < spalte+rad; j++)
            {
                result = result + img_src->at<char>(zeile, spalte) / divisor;// / 25.0f;
            }
        }

        //result = summe;

        return result;
    }

}

char mountin_filter(const int zeile, const int spalte, Mat *img_src)
{
    //endergebnis
    char result = 0;

    //Filterradius
    int rad = 1;
    //Schwellwert
    int teta = -100;
    assert(teta <= 0);


    //randpositionsflaggen
    bool oberer_Rand = false;
    bool unterer_Rand = false;
    bool rechter_Rand = false;
    bool linker_Rand = false;

    //dimensionen des Ausgangsbildes
    int rows = img_src->rows;
    int cols = img_src->cols;

    assert(zeile < rows);
    assert(spalte < cols);
    //Farbwert an der Stelle (Zeile, Spalte)
    char initColor = img_src->at<char>(zeile, spalte);

    //Positionsbestimmung
    if((zeile - rad)  < 0)
    {
        oberer_Rand = true;
    }
    if( (spalte-rad) < 0)
    {
        linker_Rand = true;
    }
    if((spalte+rad) > cols)
    {
        rechter_Rand = true;
    }
    if((zeile+rad) > rows)
    {
        unterer_Rand = true;
    }

    //Falls wir eine Randposition haben machen wir garnix weil wir faul sind!
    if(oberer_Rand || unterer_Rand || rechter_Rand || linker_Rand)
    {
        result = initColor;
    }
    else
    {

        char minColor = 128;
        char maxColor = 0;

        for(int i = zeile-rad; i < zeile + rad; i++)
        {
            for(int j = spalte - rad; j< spalte +rad; j++)
            {
                char val = img_src->at<char>(i,j);
                if(minColor > val)
                {
                    minColor = val;
                }
                if(maxColor < val)
                {
                    maxColor = val;
                }
            }
        }


        char gamma_u = 0;
        for(int i = -rad; i <= rad; i++)
        {
            for(int j = -rad; j <= rad; j++)
            {
                gamma_u = gamma_u + (img_src->at<char>(zeile + i, spalte +j) - initColor) * (img_src->at<char>(zeile-i, spalte-j) - initColor);

            }
        }

        //Flankenpunkt
        if(gamma_u < teta)
        {
            result = minColor;
        }
        else
        {
            result = initColor;
        }
    }
    return result;
}




char GaussHelpEuklidicLen(const int &x, const int &y)
{
    int x_2 = x * x;
    int y_2 = y * y;
    int sum = x_2 + y_2;
    char root = static_cast<char>(sqrt(sum));
    return root;
}

//Omega für der Gaußglocke.....
double omega = M_PI / sqrt(log(sqrt(2)));

char GaussFilter(const int &zeile, const int &spalte, Mat* img_src)
{
    char result = 0;

    //Filter Radius
    char rad = 15;

    //Container für die Gewichte
    int amountOfWeights = (rad*2+1)*(rad*2+1);
    int rowColumnLen = (rad*2+1);
    double* weights = (double*)malloc(amountOfWeights*sizeof(double));

    //Summe der Gewichte
    double sumOfWeights = 0.0;

    //Summe der gewichteten Farbwerte
    double SumOfWeightedVals = 0;

    double t = 0.0;
    //Zeilenschleife
    for(int i = 0; i < rowColumnLen; i++)
    {
        int arrayZeilePos = i * rowColumnLen;
        //Spalterschleife
        for(int j = 0; j < rowColumnLen; j++)
        {
            int xVec =  rad+1 - i;
            int yVec = rad+1 - j;

            t = GaussHelpEuklidicLen(xVec, yVec);
            double weight = exp(-(t*t)/(2*omega*omega));//Gaus-Verteilung/Gauss-Glocke

            sumOfWeights = sumOfWeights + weight;

            if(weight == weights[arrayZeilePos +j-1])
            {
                weight = 0;
            }

            weights[arrayZeilePos + j]= weight;




        }
    }

    double aWeight = 0.0;

    int cnt = 0;
    for(int i = 0; i < rowColumnLen; i++)
    {
        int ArrayZeile = rowColumnLen * i;
        for(int j = 0; j < rowColumnLen; j++)
        {
            int ArrayPos = ArrayZeile + j;
            aWeight =  weights[ArrayPos];
            double foo = img_src->at<char>(zeile, spalte) * aWeight;

            SumOfWeightedVals = SumOfWeightedVals + foo ;


        }
    }

    double tmp = SumOfWeightedVals/sumOfWeights;

    result = static_cast<char>(tmp);

    free(weights);//Free'en damit wir kein Mem-Leak bekommen ....

    return result;
}

bool traversator(Mat* img_src, Mat* img_dst)
{
     assert(img_src->cols == img_dst->cols);
     assert(img_src->rows == img_dst->rows);

    //Eine temporäre (Hilfs-)Matrix erzeugen
    Mat tmp = img_src->clone();

    //die randomisierung lostreten
    //srand(time(NULL));

    /*for(int zeile = 0; zeile < img_src->rows; zeile ++)
    {
        for(int spalte = 0; spalte < img_src->cols; spalte ++)
        {
            char wert = box_filter(zeile, spalte, img_src);
            tmp.at<char>(zeile, spalte) = wert;
        }
    }*/


        /*for(int zeile = 0; zeile < img_src->rows; zeile ++)
        {
            for(int spalte = 0; spalte < img_src->cols; spalte ++)

            {

                assert(img_src != nullptr);
                assert(img_dst != nullptr);
                assert(spalte < img_src->cols);
                assert(zeile < img_src->rows);

                //unsigned char wert = filter(zeile, spalte, img_src);
                //char wert = sobelOps(zeile, spalte, img_src);
                //char wert = GaussFilter(zeile, spalte, img_src);



                char wert = probabilistic_additive_scrambler(zeile, spalte, img_src);
                tmp.at<char>(zeile, spalte) = wert;
                //img_dst->at<char>(zeile, spalte) = wert;
            }
        }*/


        for(int zeile = 0; zeile < tmp.rows; zeile ++)
        {
            for(int spalte = 0; spalte < tmp.cols; spalte ++)

            {

                assert(img_src != nullptr);
                assert(img_dst != nullptr);
                assert(spalte < img_src->cols);
                assert(zeile < img_src->rows);

                //unsigned char wert = filter(zeile, spalte, img_src);
                //char wert = sobelOps(zeile, spalte, img_src);
                char wert = GaussFilter(zeile, spalte, &tmp);


                //char wert = probabilistic_additive_scrambler(zeile, spalte, img_src);
                //tmp.at<char>(zeile, spalte) = wert;
                img_dst->at<char>(zeile, spalte) = wert;
            }
        }



//Mountinfilter
/*    for(int k = 0; k < 10; k++)
    {
        std::cout << k << std::endl;

        for(int zeile = 0; zeile < img_src->rows; zeile ++)
        {
            for(int spalte = 0; spalte < img_src->cols; spalte ++)

            {

                assert(img_src != nullptr);
                assert(img_dst != nullptr);
                assert(spalte < img_src->cols);
                assert(zeile < img_src->rows);

                char wert = mountin_filter(zeile, spalte, &tmp);

                img_dst->at<char>(zeile, spalte) = wert;
            }
        }

        tmp = img_dst->clone();
    }*/


    //nachglätten mit boxfilter
    /*for(int zeile = 0; zeile < img_src->rows; zeile ++)
    {
        for(int spalte = 0; spalte < img_src->cols; spalte ++)
        {
            char wert = box_filter(zeile, spalte, img_src);
            tmp.at<char>(zeile, spalte) = wert;
        }
    }*/

    //*img_dst = tmp.clone();

    return true;
}

int main()
{
    //Bild lesen
    Mat image;
    //image = imread("/home/gerdie/Developement/test/test/pic2.png", IMREAD_GRAYSCALE);
    //image = imread("/home/gerdie/Developement/test/test/pic.JPG", IMREAD_GRAYSCALE);
    image = imread("/home/gerdie/Developement/test/test/hands.JPG", IMREAD_GRAYSCALE);
    //image = imread("/home/gerdie/Developement/test/test/msz.jpg", IMREAD_GRAYSCALE);
    cv::resize(image, image, cv::Size(900,1200));

    if(! image.data)
        {
            std::cout << "Konnte bild nicht finden ....." << std::endl;
            waitKey(0);
            return 1;
        }

    //filter aufrufen
    Mat img_dst;
    img_dst = image.clone();
    if(!traversator(&image, &img_dst))
    {
        std::cout << "Konnte bild nicht Filtern" << std::endl;
        waitKey(0);

        return 1;
    }



    //Bildausgabe
    namedWindow("Ireland Views");
    imshow("Ireland Views", img_dst);

    waitKey(0);

    return 0;
}
