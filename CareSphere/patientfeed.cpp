#include "patientfeed.h"
#include "ui_patientfeed.h"

patientfeed::patientfeed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientfeed)
{
    ui->setupUi(this);
}

patientfeed::~patientfeed()
{
    delete ui;
}

void patientfeed::on_logOutButton_clicked()
{

}

