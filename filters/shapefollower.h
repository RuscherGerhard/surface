#ifndef SHAPEFOLLOWER_H
#define SHAPEFOLLOWER_H
#include "filter.h"

class ShapeFollower: public Filter
{
public:
    ShapeFollower();
    ~ShapeFollower();

    virtual QImage* getImage(){return _Image;}

    virtual void setFilterId(const FilterId id){_FilterId = id;}
    virtual FilterId getFilterId(){return _FilterId;}


    virtual void ProcessImage(QImage* imageToProcess);

private:

    QPoint _p0;
    QPoint _q0;

    std::vector<QPoint*> _neighbors;

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
     * @brief _DeleteNeighborArray : Spezialmethode um ein Nachbarschaftsarray abzuräumen.
     * @param vec: std::vector<QPoint*>* ein Zeiger auf einen Nachbarschaftsvektor.
     */
    void _ClearNeighborArray();
};

#endif // SHAPEFOLLOWER_H
