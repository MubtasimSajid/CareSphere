#include "patientfeed.h"
#include "ui_patientfeed.h"
#include "login.h"
#include "patientprofile.h"

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
    Login *loginWindow = new Login();
    loginWindow->show();
    this->close();
}


void patientfeed::on_feedToProfileButton_clicked()
{
    patientprofile *patientProfileWindow = new patientprofile();
    patientProfileWindow->show();
    this->close();
}
