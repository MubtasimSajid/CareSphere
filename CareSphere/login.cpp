#include "login.h"
#include "ui_login.h"
#include <qwidget.h>
#include <QMessageBox>
#include <QKeyEvent>
#include <qdebug.h>

Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    loginSystem = nullptr;

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Connect login button to login function (not necessary)
    connect(ui->loginButton, &QPushButton::clicked, this, &Login::on_loginButton_clicked);

    // Install event filter on the whole window to detect Enter key
    this->installEventFilter(this);

    qInfo() << "Login Code Working";
}

Login::~Login() {
    delete ui;
    delete loginSystem;
}

void Login::registerUser(string name, string password) {
    ifstream fileRead(filename);
    string line, uname, pass;

    if (fileRead.is_open()) {
        while (getline(fileRead, line)) {
            stringstream ss(line);
            getline(ss, uname, ',');
            getline(ss, pass, ',');

            if (uname == name) {
                cout << "Error: Username '" << name << "' already exists. Choose a different one.\n";
                fileRead.close();
                return;
            }
        }
        fileRead.close();
    }

    ofstream fileWrite(filename, ios::out | ios::app);
    if (!fileWrite) {
        cout << "Error: Unable to open file for writing!\n";
        return;
    }
    fileWrite << name << "," << password << "\n";
    fileWrite.flush();
    fileWrite.close();
    cout << "User '" << name << "' registered successfully!\n";
}



bool Login::verify(string name, string password) {
    ifstream file(filename);
    string line, uname, pass;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, uname, ',');
            getline(ss, pass, ',');

            if (uname == name && pass == password) {
                file.close();
                return true;
            }
        }
        file.close();
    } else {
        cout << "Error: Unable to open file for reading!\n";
    }
    return false;
}

void Login::on_loginButton_clicked(){
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Both fields must be filled!");
        return;
    }

    std::string user = username.toStdString();
    std::string pass = password.toStdString();

    if (verify(user, pass)) {
        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");
    } else {
        QMessageBox::information(this, "Login failed", "Invalid username or password");
    }
}

bool Login::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            on_loginButton_clicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}


void Login::on_registerButton_clicked()
{
    Registration *registerWindow = new Registration();
    registerWindow->show();
    this->close();
}
