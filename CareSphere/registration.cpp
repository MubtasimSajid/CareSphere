#include "registration.h"
#include "login.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserDetails)
{
    ui->setupUi(this);

    ui->regPasswordLineEdit->setEchoMode(QLineEdit::Password);
}

Registration::~Registration()
{
    delete ui;
}

void Registration::on_regBackToLoginButton_clicked()
{
    Login *loginWindow = new Login();
    loginWindow->show();
    this->close();
}


void Registration::on_registerPushButton_clicked()
{
    QString regUsername = ui->regUsernameLineEdit->text();
    QString regPassword = ui->regPasswordLineEdit->text();
}

