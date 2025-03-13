#include "reguserdetails.h"
#include "ui_reguserdetails.h"
#include "registration.h"
#include "patientfeed.h"
#include "user.h"
#include <QMessageBox>

string currentUserName;

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

void RegUserDetails::on_detailsBackToReg_clicked()
{
    Registration *registrationWindow = new Registration();
    registrationWindow->show();
    this->close();
}

void detailsPassing(const string &username) {
    currentUserName = username;
}

void RegUserDetails::on_registerDetailsButton_clicked()
{
    QString name = ui->udNameEdit->text();
    QString phone = ui->udPhoneEdit->text();
    QString email = ui->udEmailEdit->text();
    QString gender = (ui->maleButton->isChecked()) ? "Male" : "Female";
    QDate dob = ui->dateEdit->date();
    QString dobStr = dob.toString();
    QString bloodGroup = ui->bloodGroupDropDown->currentText();

    if (!validateInputs(name, email, phone, gender, dob, bloodGroup)) {
        return;
    }

    string strName = name.toStdString();
    string strPhone = phone.toStdString();
    string strEmail = email.toStdString();
    string strGender = gender.toStdString();
    string strDoB = dobStr.toStdString();
    string strBG = bloodGroup.toStdString();

    User currentUser(currentUserName, strName, strEmail, strPhone, strBG, strGender, strDoB);;

    patientfeed *patientFeedWindow = new patientfeed();
    patientFeedWindow->show();
    this->close();
}

bool RegUserDetails::validateInputs(const QString &name, const QString &email,
                                    const QString &phone, const QString &gender,
                                    const QDate &dob, const QString &bloodGroup)
{
    QRegularExpression nameRegex("^[A-Za-z\\s]+$");
    if (name.isEmpty() || !nameRegex.match(name).hasMatch()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid name.");
        return false;
    }

    QRegularExpression emailRegex(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    if (email.isEmpty() || !emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid email address.");
        return false;
    }

    QRegularExpression phoneRegex("^\\d{11}$");
    if (phone.isEmpty() || !phoneRegex.match(phone).hasMatch()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a valid 11-digit phone number.");
        return false;
    }

    if (gender.isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please select a gender.");
        return false;
    }

    if (dob > QDate::currentDate()) {
        QMessageBox::warning(this, "Validation Error", "Date of Birth cannot be in the future.");
        return false;
    }

    if (bloodGroup.isEmpty() || bloodGroup == "Select your blood group") {
        QMessageBox::warning(this, "Validation Error", "Please select a blood group.");
        return false;
    }

    return true;
}
