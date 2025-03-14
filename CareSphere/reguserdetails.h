#ifndef REGUSERDETAILS_H
#define REGUSERDETAILS_H

#include <QWidget>
#include <QString>
#include "ui_reguserdetails.h"
#include "registration.h"
#include "login.h"
#include "user.h"
#include <QMessageBox>
#include <QSettings>

using namespace std;

namespace Ui {
class RegUserDetails;
}

class RegUserDetails : public QWidget
{
    Q_OBJECT

public:
    explicit RegUserDetails(QWidget *parent = nullptr);
    ~RegUserDetails();

private slots:
    void on_detailsBackToReg_clicked();
    bool validateInputs(const QString &name, const QString &email,
                        const QString &phone, const QString &gender,
                        const QDate &dob, const QString &bloodGroup);
    void on_registerDetailsButton_clicked();

private:
    Ui::RegUserDetails *ui;
};

#endif
