#include "reguserdetails.h"
#include "ui_reguserdetails.h"

RegUserDetails::RegUserDetails(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RegUserDetails)
{
    ui->setupUi(this);
}

RegUserDetails::~RegUserDetails()
{
    delete ui;
}
