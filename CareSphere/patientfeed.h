#ifndef PATIENTFEED_H
#define PATIENTFEED_H

#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include "appointmentdialog.h"
#include "QMessageBox"

namespace Ui {
class patientfeed;
}

class patientfeed : public QWidget
{
    Q_OBJECT

public:
    explicit patientfeed(QWidget *parent = nullptr);
    ~patientfeed();

private slots:
    void on_logOutButton_clicked();
    void on_feedToProfileButton_clicked();
    void addBulletPoint();
    void editBulletPoint();
    void deleteBulletPoint();
    void showContextMenu(const QPoint &pos);
    void addAppointment();
    void editAppointment();
    void cancelAppointment();
    void showAppointmentsContextMenu(const QPoint &pos);

private:
    Ui::patientfeed *ui;
    void setupAppointmentForm();
    void sendMissedAppointmentNotification(const QString &appointmentDetails);
};

#endif
