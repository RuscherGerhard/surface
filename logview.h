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
};

#endif // LOGVIEW_H
