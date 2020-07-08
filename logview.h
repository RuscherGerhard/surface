#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>

namespace Ui {
class LogView;
}

class LogView : public QWidget
{
    Q_OBJECT

public:
    explicit LogView(QWidget *parent = nullptr);
    ~LogView();

    void SetTextToBrowser(const QString &string);

private:
    Ui::LogView *ui;

    QString _LogViewText;

private slots:
    //void _OnFilterOperatorBtnPushed();
};

#endif // LOGVIEW_H
