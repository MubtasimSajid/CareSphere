#include "reguserdetails.h"
#include "ui_reguserdetails.h"

regUserDetails::regUserDetails(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::regUserDetails)
{
    ui->setupUi(this);
}

regUserDetails::~regUserDetails()
{
    delete ui;
}
