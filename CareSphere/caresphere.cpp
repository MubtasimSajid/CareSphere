#include "caresphere.h"
#include "./ui_caresphere.h"
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>

CareSphere::CareSphere(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);

    // Connect login button to login function (not necessary)
    connect(ui->loginButton, &QPushButton::clicked, this, &CareSphere::on_loginButton_clicked);

    // Install event filter on the whole window to detect Enter key
    this->installEventFilter(this);
}

CareSphere::~CareSphere()
{
    delete ui;
}

void CareSphere::on_loginButton_clicked(){
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Both fields must be filled!");
        return;
    }

    std::string user = username.toStdString();
    std::string pass = password.toStdString();

    if (loginSystem.verify(user, pass)) {
        QMessageBox::information(this, "Login Successful", "Welcome, " + username + "!");
    } else {
        QMessageBox::information(this, "Login failed", "Invalid username or password");
    }
}

bool CareSphere::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            on_loginButton_clicked();  // Call login function when Enter is pressed
            return true;  // Mark the event as handled
        }
    }
    return QMainWindow::eventFilter(obj, event);  // Pass to base class
}


void CareSphere::on_registerButton_clicked()
{
    Registration *registerWindow = new Registration();
    registerWindow->show();
    this->close();
}

