#ifndef SKELETONIZER_H
#define SKELETONIZER_H

#include "filter.h"

#include <queue>


class Skeletonizer : public Filter
{
public:
    Skeletonizer();
    ~Skeletonizer();

    virtual QImage* getImage(){return _Image;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}
    virtual FilterId getFilterId(){return _FilterId;}


    virtual void ProcessImage(QImage* imageToProcess);

private:
    std::vector<QPoint*> _neighbors;

    QImage _scndImage;
    QImage _thrdImage;
    //short* _Marks;
    std::vector<short> _Marks;

    QPoint _p0;
    QPoint _q0;

    /**
     * @brief _SearchStartingPoint : Findet die erstbeste Zusammenhangskomponente und Wählt einen Startpunt für die Konturverfolgung und ein q0 nicht Element der Komponente als ersten Nachbaren.
     * @param image : Das Bild, in dem nach einer Zusammenhangskomponente gesucht werden soll
     */
    void _SearchStartingPoint(QImage *image);

    /**
     * @brief _Neiborhood: Liefert einen Vektor mit den 8 Nachbarn von p0. Dieser ist gegen den Uhrzeigersinn angeordnet, beginnend mit q0.
     *                      Wenn q0 nicht angegeben ist, dann wird ein q0 "nicht Element" der Zusammenhangskomponente gewählt.
     * @param p0 : Der Aktuelle Pixel, dessen Nachbarschaft man aufbauen soll
     * @param q0 : Der erste nachbar in der Queue
     * @return vector<QPoint*>* : Ein Zeiger auf den Nachbarschaftsvektor
     */
    //std::vector<QPoint*>* _Neiborhood(QPoint* p0, QPoint* q0=nullptr);
    void _Neiborhood(QPoint* p0, QPoint* q0=nullptr);


    /**
     * @brief _CompareImages : Die Methode returnt true wenn die Bilder gleich sind, false sonst!
     * @param img1 : QImage ptr des ersten zu vergleichenden Bildes.
     * @param img2 _ QImage ptr des zweiten zu vergleichenden Bildes.
     * @return bool : true oder false je nach dem ob die Bilder gleich sind oder nicht.
     */
    bool _CompareImages(QImage* img1, QImage* img2);

    void _Traverse(QImage* imgProcess, std::queue<QPoint*>* queue, unsigned int &recnt);



};

#endif // SKELETONIZER_H
