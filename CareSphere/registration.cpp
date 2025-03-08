#include <QMessageBox>
#include "registration.h"
#include "login.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Registration)
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

bool Registration::registerUser(string name, string password) {
    ifstream fileRead(filename);
    string line, uname, pass;

    if (fileRead.is_open()) {
        while (getline(fileRead, line)) {
            stringstream ss(line);
            getline(ss, uname, ',');
            getline(ss, pass, ',');

            if (uname == name) {
                cout << "Error: Username: '" << name << "' already exists. Choose a different one.\n";
                fileRead.close();
                return false;
            }
        }
        fileRead.close();
    }

    ofstream fileWrite(filename, ios::out | ios::app);
    if (!fileWrite) {
        cout << "Error: Unable to open file for writing!";
        return false;
    }
    fileWrite << name << "," << password << "\n";
    fileWrite.flush();
    fileWrite.close();
    cout << "User '" << name << "' registered successfully!\n";
    return true;
}

void Registration::on_registerPushButton_clicked()
{
    QString regUsername = ui->regUsernameLineEdit->text();
    QString regPassword = ui->regPasswordLineEdit->text();

    if (regUsername.isEmpty() || regPassword.isEmpty()) {
        ui->regErrorLabel->setText("Invalid username or password");
        ui->regErrorLabel->setStyleSheet("color: red;");
        ui->regErrorLabel->show();
        return;
    }

    std::string user = regUsername.toStdString();
    std::string pass = regPassword.toStdString();

    if (Registration::registerUser(user, pass)) {
        QMessageBox::information(this, "Login Successful", "Welcome, " + regUsername + "!");
        ui->regErrorLabel->hide();
    } else {
        ui->regErrorLabel->setText("Invalid username or password");
        ui->regErrorLabel->setStyleSheet("color: red;");
        ui->regErrorLabel->show();
    }
}

