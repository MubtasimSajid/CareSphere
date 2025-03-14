#include "patientprofile.h"
#include "ui_patientprofile.h"

patientprofile::patientprofile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientprofile)
{
    ui->setupUi(this);

    QSettings settingsUser("CareSphere", "Login System");
    QString username = settingsUser.value("username", "").toString();
    std::string strUsername = username.toStdString();
    User currUser = getUser(strUsername);
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
