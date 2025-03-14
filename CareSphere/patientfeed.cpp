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
    ui->appointmentsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->appointmentsListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showAppointmentsContextMenu);
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
    // Create an instance of AppointmentDialog for new appointment
    AppointmentDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString date = dialog.getDateLineEdit()->text();
        QString time = dialog.getTimeComboBox()->currentText();
        QString doctorName = dialog.getDoctorNameLineEdit()->text();
        QString location = dialog.getLocationLineEdit()->text();

        // Validate the date format
        if (!dialog.isValidDate(date)) {
            QMessageBox::warning(this, "Invalid Date", "Please enter the date in DD-MM-YYYY format.");
            return;
        }

        // Check if any field is empty
        if (date.isEmpty() || doctorName.isEmpty() || location.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all the fields.");
        } else {
            // Create the appointment details
            QString appointmentDetails = QString("%1 - %2 with Dr. %3 at %4")
                                             .arg(date, time, doctorName, location);

            // Add to the appointments list
            QListWidgetItem *item = new QListWidgetItem("• " + appointmentDetails, ui->appointmentsListWidget);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
    }
}

void patientfeed::editAppointment()
{
    QListWidgetItem *item = ui->appointmentsListWidget->currentItem();
    if (item) {
        QString currentDetails = item->text().mid(2);  // Remove bullet point

        // Parse the current details (assuming it's in the format "DD-MM-YYYY - 10:00 AM with Dr. Smith at Clinic")
        QStringList parts = currentDetails.split(" with ");
        QString dateTimeLocation = parts[0];
        QString doctorAndLocation = parts[1];
        QStringList dateTimeParts = dateTimeLocation.split(" - ");
        QString date = dateTimeParts[0];  // Date in DD-MM-YYYY format
        QString time = dateTimeParts[1];
        QString doctorName = doctorAndLocation.split(" at ")[0];
        QString location = doctorAndLocation.split(" at ")[1];

        // Show the dialog with current values
        AppointmentDialog dialog(this);
        dialog.getDateLineEdit()->setText(date);
        dialog.getTimeComboBox()->setCurrentText(time);
        dialog.getDoctorNameLineEdit()->setText(doctorName);
        dialog.getLocationLineEdit()->setText(location);

        if (dialog.exec() == QDialog::Accepted) {
            QString newDetails = QString("%1 - %2 with Dr. %3 at %4")
            .arg(dialog.getDateLineEdit()->text(), dialog.getTimeComboBox()->currentText(),
                 dialog.getDoctorNameLineEdit()->text(), dialog.getLocationLineEdit()->text());

            // Check if the new details are valid
            if (dialog.getDateLineEdit()->text().isEmpty() || dialog.getDoctorNameLineEdit()->text().isEmpty() || dialog.getLocationLineEdit()->text().isEmpty()) {
                QMessageBox::warning(this, "Input Error", "Please fill in all the fields.");
            } else {
                item->setText("• " + newDetails);  // Update item text
            }
        }
    }
}

void patientfeed::showAppointmentsContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->appointmentsListWidget->viewport()->mapToGlobal(pos);
    QMenu menu;

    QAction *editAction = menu.addAction("Modify Appointment");
    QAction *deleteAction = menu.addAction("Cancel Appointment");

    QAction *selectedAction = menu.exec(globalPos);
    if (selectedAction == editAction) {
        editAppointment();  // Edit appointment
    } else if (selectedAction == deleteAction) {
        cancelAppointment();  // Delete appointment
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
