#include "logview.h"
#include "ui_logview.h"
#include "utils.h"

LogView::LogView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogView)
{
    ui->setupUi(this);

    //connect(ui->FilterOperatorBtn, SIGNAL(clicked()), this, SLOT(_OnFilterOperatorBtnPushed()));
}

LogView::~LogView()
{
    delete ui;
}

void LogView::SetTextToBrowser(const QString &string)
{
    ui->textBrowser->clear();

    _LogViewText = string;

    ui->textBrowser->setText(string);
}

/*void LogView::_OnFilterOperatorBtnPushed()
{
    if(ui->FilterOperatorBtn->isChecked())
    {
        //[Filter]
        if(!_LogViewText.contains(FILTER))
        {
            ui->textBrowser->clear();
        }
        else
        {
            int startIdx = _LogViewText.indexOf(FILTER);
            int end = _LogViewText.indexOf("*", startIdx+1);

            QString firstMessage = _LogViewText.
        }

    }
    else
    {
        ui->textBrowser->setText(_LogViewText);
    }
}*/
