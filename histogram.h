#ifndef HISTOGRAM_H
#define HISTOGRAM_H


#include <qwidget.h>

#include <QtCharts>


namespace Ui {
class Histogram;
}

class Histogram: public QWidget
{
    Q_OBJECT

public:
    Histogram();

    ~Histogram();




    void ShowHisto(QImage* image);
    void ShowHisto(std::vector<QImage*> images);

protected:
    void CloseEvent(QCloseEvent* event);

private:
    QChart _HistoChart;
    QLineSeries* _RedSeries;
    QLineSeries* _GreenSeries;
    QLineSeries* _BlueSeries;

    QChartView* _view;

signals:
    void HistoClosed();
};

#endif // HISTOGRAM_H
