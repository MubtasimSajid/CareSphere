#include "patientprofile.h"
#include "patientfeed.h"
#include "ui_patientprofile.h"

patientprofile::patientprofile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientprofile)
{
    ui->setupUi(this);
}

patientprofile::~patientprofile()
{
    delete ui;
}

void patientprofile::on_detailsBackToPatientFeed_clicked()
{
    patientfeed *patientFeedWindow = new patientfeed();
    patientFeedWindow->show();
    this->close();
}
