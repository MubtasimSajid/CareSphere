#include "patientFeed.h"
#include "ui_patientFeed.h"

patientFeed::patientFeed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientFeed)
{
    ui->setupUi(this);
}

patientFeed::~patientFeed()
{
    delete ui;
}
