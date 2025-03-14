#include "patientfeed.h"
#include "ui_patientfeed.h"
#include "login.h"
#include "patientprofile.h"
#include <qmenu.h>

patientfeed::patientfeed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientfeed)
{
    ui->setupUi(this);

    connect(ui->notesPlusButton, &QPushButton::clicked, this, &patientfeed::addBulletPoint);
    ui->notesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->notesListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showContextMenu);

    connect(ui->appointmentPlusButton, &QPushButton::clicked, this, &patientfeed::addAppointment);
}

patientfeed::~patientfeed()
{
    delete ui;
}

void patientfeed::on_logOutButton_clicked()
{
    Login *loginWindow = new Login();
    loginWindow->show();
    this->close();
}


void patientfeed::on_feedToProfileButton_clicked()
{
    patientprofile *patientProfileWindow = new patientprofile();
    patientProfileWindow->show();
    this->close();
}

void patientfeed::addBulletPoint()
{
    bool ok;
    QString text = QInputDialog::getText(this, "Add Notes", "Enter text:", QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem("• " + text, ui->notesListWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }
}

void patientfeed::deleteBulletPoint()
{
    QListWidgetItem *item = ui->notesListWidget->currentItem();
    if (item) {
        delete item;
    }
}

void patientfeed::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->notesListWidget->viewport()->mapToGlobal(pos);
    QMenu menu;
    QAction *editAction = menu.addAction("Edit");
    QAction *deleteAction = menu.addAction("Delete");

    QAction *selectedAction = menu.exec(globalPos);
    if (selectedAction == editAction) {
        editBulletPoint();
    } else if (selectedAction == deleteAction) {
        deleteBulletPoint();
    }
}

void patientfeed::editBulletPoint()
{
    QListWidgetItem *item = ui->notesListWidget->currentItem();
    if (!item) return;

    bool ok;
    QString text = QInputDialog::getText(this, "Edit Notes", "Edit text:", QLineEdit::Normal, item->text().mid(2), &ok);

    if (ok && !text.isEmpty()) {
        item->setText("• " + text);
    }
}

void patientfeed::addAppointment()
{
    bool ok;
    QStringList timeSlots = {"9:00 AM", "10:00 AM", "11:00 AM", "3:00 PM", "4:00 PM"};
    QString time = QInputDialog::getItem(this, "Select Time", "Select Appointment Time:", timeSlots, 0, false, &ok);

    if (ok && !time.isEmpty()) {
        QString date = QInputDialog::getText(this, "Enter Appointment Date", "Enter the date (YYYY-MM-DD):", QLineEdit::Normal, "", &ok);
        QString doctorName = QInputDialog::getText(this, "Doctor's Name", "Enter Doctor's Name:", QLineEdit::Normal, "", &ok);
        QString location = QInputDialog::getText(this, "Location", "Enter Appointment Location:", QLineEdit::Normal, "", &ok);

        if (ok && !date.isEmpty() && !doctorName.isEmpty() && !location.isEmpty()) {
            QString appointmentDetails = QString("%1 - %2 with Dr. %3 at %4").arg(date, time, doctorName, location);

            QListWidgetItem *item = new QListWidgetItem("• " + appointmentDetails, ui->appointmentsListWidget);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
    }
}

void patientfeed::editAppointment()
{
    QListWidgetItem *item = ui->appointmentsListWidget->currentItem();
    if (item) {
        QString currentDetails = item->text().mid(2);

        bool ok;
        QString newDetails = QInputDialog::getText(this, "Edit Appointment", "Edit appointment details:", QLineEdit::Normal, currentDetails, &ok);

        if (ok && !newDetails.isEmpty()) {
            item->setText("• " + newDetails);
        }
    }
}

void patientfeed::cancelAppointment()
{
    QListWidgetItem *item = ui->appointmentsListWidget->currentItem();
    if (item) {
        delete item;
    }
}

void patientfeed::setupAppointmentForm()
{
    bool ok;
    QStringList timeSlots = {"9:00 AM", "10:00 AM", "11:00 AM", "3:00 PM", "4:00 PM"};
    QString time = QInputDialog::getItem(this, "Select Time", "Select Appointment Time:", timeSlots, 0, false, &ok);

    if (ok && !time.isEmpty()) {
        QString date = QInputDialog::getText(this, "Enter Appointment Date", "Enter the date (YYYY-MM-DD):", QLineEdit::Normal, "", &ok);
        QString doctorName = QInputDialog::getText(this, "Doctor's Name", "Enter Doctor's Name:", QLineEdit::Normal, "", &ok);
        QString location = QInputDialog::getText(this, "Location", "Enter Appointment Location:", QLineEdit::Normal, "", &ok);

        if (ok && !date.isEmpty() && !doctorName.isEmpty() && !location.isEmpty()) {
            QString appointmentDetails = QString("%1 - %2 with Dr. %3 at %4").arg(date, time, doctorName, location);

            QListWidgetItem *item = new QListWidgetItem("• " + appointmentDetails, ui->appointmentsListWidget);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
    }
}

void patientfeed::sendMissedAppointmentNotification(const QString &appointmentDetails)
{

}
