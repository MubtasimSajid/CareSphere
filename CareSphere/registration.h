#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QString>
#include "reguserdetails.h"

using namespace std;

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_regBackToLoginButton_clicked();
    bool registerUser(string name, string password);
    void on_registerPushButton_clicked();

private:
    Ui::Registration *ui;
    const string filename = "credentials.csv";
};

#endif
