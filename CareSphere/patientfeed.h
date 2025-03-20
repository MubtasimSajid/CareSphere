#ifndef PATIENTFEED_H
#define PATIENTFEED_H

#include <QWidget>
#include <QInputDialog>
#include <QLineEdit>
#include <QMenu>
#include <QSettings>
#include <QFormLayout>
#include "appointmentdialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <QMessageBox>
#include <QSettings>
#include <qmenu.h>
#include "utilities.h"
#include "prescription.h"
#include "reminders.h"
#include "appointment.h"
#include "ui_patientfeed.h"
#include "login.h"
#include "patientprofile.h"

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
    void addBulletPoint(bool fromDB = false, QString text = "");
    void editBulletPoint();
    void deleteBulletPoint();
    void showContextMenu(const QPoint &pos);
    void addAppointment();
    void editAppointment();
    void cancelAppointment();
    void showAppointmentsContextMenu(const QPoint &pos);
    void addPrescription();
    void editPrescription();
    void deletePrescription();
    void showPrescriptionsContextMenu(const QPoint &pos);
    void addReminder();
    void editReminder();
    void deleteReminder();
    void showRemindersContextMenu(const QPoint &pos);
    void loadPrescriptions();
    void loadReminders();
    void loadAppointments();

private:
    Ui::patientfeed *ui;
    void setupAppointmentForm();
};

#endif
