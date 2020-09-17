#include "histogram.h"
#include "utils.h"

Histogram::Histogram()
{
    _HistoChart.legend()->hide();
    _HistoChart.setTitle("Histogram");

    _view = new QChartView();
    _view->setRenderHint(QPainter::Antialiasing);

    _view->setParent(this);

    _RedSeries = nullptr;
    _GreenSeries = nullptr;
    _BlueSeries = nullptr;
}

Histogram::~Histogram()
{
    if(_RedSeries != nullptr)
        delete _RedSeries;
}

void Histogram::ShowHisto(QImage *image)
{
    std::vector<std::vector<int>>* histo = utils::Histogram(image);
    if(_RedSeries != nullptr)
        delete _RedSeries;

    if(_BlueSeries != nullptr)
        delete _BlueSeries;

    if(_GreenSeries != nullptr)
        delete _GreenSeries;

    _RedSeries = new QLineSeries();
    QPen redPen(Qt::red);
    _RedSeries->setPen(redPen);

    _GreenSeries = new QLineSeries();
    QPen greenPen(Qt::green);
    _GreenSeries->setPen(greenPen);

    _BlueSeries = new QLineSeries();
    QPen bluePen(Qt::blue);
    _BlueSeries->setPen(bluePen);

    for(unsigned int i = 0; i < histo->at(0).size(); i++)
    {
        _RedSeries->append(i,histo->at(0)[i]);
        _GreenSeries->append(i,histo->at(1)[i]);
        _BlueSeries->append(i,histo->at(2)[i]);

    }

    _HistoChart.addSeries(_RedSeries);
    _HistoChart.addSeries(_GreenSeries);
    _HistoChart.addSeries(_BlueSeries);

    _HistoChart.createDefaultAxes();

    _view->setChart(&_HistoChart);

}

void Histogram::ShowHisto(std::vector<QImage*> images)
{
    for(unsigned int i = 0; i < images.size(); i++)
    {
        ShowHisto(images.at(i));
    }
}

void Histogram::CloseEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);

    emit HistoClosed();
}
