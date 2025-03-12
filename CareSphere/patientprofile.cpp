#include "patientprofile.h"
#include "ui_patientprofile.h"

patientProfile::patientProfile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientProfile)
{
    ui->setupUi(this);
}

patientProfile::~patientProfile()
{
    delete ui;
}
