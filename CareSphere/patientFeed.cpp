#include "patientFeed.h"
#include "ui_patientFeed.h"

PatientFeed::PatientFeed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientFeed)
{
    ui->setupUi(this);
}

PatientFeed::~PatientFeed()
{
    delete ui;
}
