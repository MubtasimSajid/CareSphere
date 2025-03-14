#include "patientprofile.h"
#include "ui_patientprofile.h"

patientprofile::patientprofile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientprofile)
{
    ui->setupUi(this);

    QSettings settingsUser("CareSphere", "Login System");
    QString username = settingsUser.value("username", "").toString();
    string strUsername = username.toStdString();
    User currUser = getUser(strUsername);
    currUser.details();
    ui->PatientName->setText(QString::fromStdString(currUser.getName()));
    ui->PatientBloodGroup->setText(QString::fromStdString(currUser.getBloodGroup()));
    ui->PatientDateOfBirth->setText(QString::fromStdString(currUser.getDOB()));
    ui->PatientEmail->setText(QString::fromStdString(currUser.getEmail()));
    ui->PatientPhone->setText(QString::fromStdString(currUser.getPhoneNo()));
    ui->PatientGender->setText(QString::fromStdString(currUser.getGender()));
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
