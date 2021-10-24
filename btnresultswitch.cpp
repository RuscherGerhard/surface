#include "btnresultswitch.h"

BtnResultSwitch::BtnResultSwitch(QWidget* myParent, const FilterId id, const char* FilterName):
_MyId(id)
{
    this->setParent(myParent);

    this->setText(FilterName);

    connect(this, SIGNAL(clicked()), this, SLOT(_CallPushedId()) );
}

BtnResultSwitch::~BtnResultSwitch()
{

}

void BtnResultSwitch::_CallPushedId()
{
    emit PushedId(_MyId);
}


