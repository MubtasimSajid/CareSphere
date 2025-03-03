#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include <QString>

namespace Ui {
class UserDetails;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr);
    ~Registration();

private slots:
    void on_regBackToLoginButton_clicked();

    void on_registerPushButton_clicked();

private:
    Ui::UserDetails *ui;
};

#endif
