#include "patientfeed.h"
#include "ui_patientfeed.h"

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
