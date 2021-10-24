#ifndef BTNRESULTSWITCH_H
#define BTNRESULTSWITCH_H
#include <QPushButton>
#include <utils.h>

class BtnResultSwitch : public QPushButton
{
    Q_OBJECT
public:
    BtnResultSwitch(QWidget* myParent, const FilterId id, const char* FilterName);
    ~BtnResultSwitch();

    int getBtnId(){return _MyId;}
    void setNewBtnId(const int newBtnId){_MyId = newBtnId;}

    void setNewBtnTitle(const char* FilterName){this->setText(FilterName);}

signals:
    void PushedId(int IdToSend);
private:
    int _MyId;

private slots:
    void _CallPushedId();


};

#endif // BTNRESULTSWITCH_H
