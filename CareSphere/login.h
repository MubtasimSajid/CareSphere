#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <QMainWindow>
#include "registration.h"
using namespace std;

namespace Ui {
class Login;
};

class Login : public QMainWindow {
    Q_OBJECT

private:
    const string filename = "credentials.csv";
    Ui::Login *ui;
    Login *loginSystem;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_loginButton_clicked();
    void on_registerButton_clicked();

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void Register(string name, string password);
    bool verify(string name, string password);
};

#endif
