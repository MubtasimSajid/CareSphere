#include "login.h"


Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    loginSystem = nullptr;

    setTabOrder(ui->usernameLineEdit, ui->passwordLineEdit);
    setTabOrder(ui->passwordLineEdit, ui->loginButton);
    setTabOrder(ui->loginButton, ui->registerButton);

    ui->loginButton->setAutoDefault(false);
    ui->loginButton->setDefault(false);

    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
\
    connect(ui->loginButton, &QPushButton::clicked, this, &Login::on_loginButton_clicked);
\
    this->installEventFilter(this);
}

Login::~Login() {
    delete ui;
    delete loginSystem;
}

bool Login::verify(string name, string password) {
    ifstream file(filename);
    string line, uname, pass;

    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, uname, ',');
            getline(ss, pass, ',');

            if (uname == name && decryptPassword(pass) == password) {
                file.close();
                return true;
            }
        }
        file.close();
    } else {
        qInfo() << "Error: Unable to open file for reading!\n";
    }
    return false;
}

void Login::on_loginButton_clicked() {
    ui->loginButton->clearFocus();
    this->setFocus();

    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->errorLabel->setText("Invalid username or password");
        ui->errorLabel->setStyleSheet("color: red;");
        ui->errorLabel->show();
        return;
    }

    std::string user = username.toStdString();
    std::string pass = password.toStdString();

    if (verify(user, pass)) {
        QSettings settingsUser("CareSphere", "Login System");
        settingsUser.remove("username");
        settingsUser.setValue("username", username);
        settingsUser.sync();

        this->hide();

        static patientfeed *patientFeedWindow = nullptr;
        if (!patientFeedWindow) {
            patientFeedWindow = new patientfeed();
        } else {
            patientFeedWindow->close();
            patientFeedWindow = new patientfeed();
        }
        patientFeedWindow->show();
        ui->errorLabel->hide();
    } else {
        ui->errorLabel->setText("Invalid username or password");
        ui->errorLabel->setStyleSheet("color: red;");
        ui->errorLabel->show();
    }
}

bool Login::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            if (ui->loginButton->hasFocus() || ui->usernameLineEdit->hasFocus() || ui->passwordLineEdit->hasFocus()) {
                on_loginButton_clicked();
            } else if (ui->registerButton->hasFocus()) {
                on_registerButton_clicked();
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}


void Login::on_registerButton_clicked()
{
    this->close();
    Registration *registerWindow = new Registration();
    registerWindow->show();
}
