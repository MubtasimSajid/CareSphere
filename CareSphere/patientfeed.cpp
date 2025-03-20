#include "patientfeed.h"
#include "ui_patientfeed.h"
#include "login.h"
#include "patientprofile.h"
#include <qmenu.h>

QSettings settingsUser("CareSphere", "Login System");
QString username = settingsUser.value("username", "").toString();
string strUsername = username.toStdString();

patientfeed::patientfeed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::patientfeed)
{
    ui->setupUi(this);

    loadPrescriptions();
    loadReminders();
    loadAppointments();

    connect(ui->notesPlusButton, &QPushButton::clicked, this, [this]() { addBulletPoint(ui->notesListWidget); });
    ui->notesListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->notesListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showContextMenu);

    connect(ui->appointmentPlusButton, &QPushButton::clicked, this, &patientfeed::addAppointment);
    ui->appointmentsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->appointmentsListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showAppointmentsContextMenu);

    connect(ui->prescriptionsPlusButton, &QPushButton::clicked, this, &patientfeed::addPrescription);
    ui->prescriptionsListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->prescriptionsListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showPrescriptionsContextMenu);

    connect(ui->addRemindersButton, &QPushButton::clicked, this, &patientfeed::addReminder);
    ui->remindersListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->remindersListWidget, &QListWidget::customContextMenuRequested, this, &patientfeed::showRemindersContextMenu);

    vector<string> userNotes = get_User_Notes(strUsername);
    for (const string &note : userNotes) {
        addBulletPoint(true, QString::fromStdString(note));
    }
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

void patientfeed::addBulletPoint(bool fromDB, QString text)
{
    if (!fromDB || text == "") {
        bool ok;
        QString userInput = QInputDialog::getText(this, "Add Notes", "Enter text:", QLineEdit::Normal, "", &ok);

        if (!ok || userInput.trimmed().isEmpty()) {
            return;
        }
        text = userInput.trimmed();
        save_User_Notes(strUsername, text.toStdString());
    }

    for (int i = 0; i < ui->notesListWidget->count(); ++i) {
        QListWidgetItem *existingItem = ui->notesListWidget->item(i);
        if (existingItem->text() == "• " + text) {
            return;
        }
    }

    QListWidgetItem *item = new QListWidgetItem("• " + text, ui->notesListWidget);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
}

void patientfeed::deleteBulletPoint()
{
    QListWidgetItem *item = ui->notesListWidget->currentItem();
    if (item) {
        QString deletedText = item->text().mid(2);
        Delete_User_Notes(strUsername, deletedText.toStdString());
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

    QString originalText = item->text().mid(2);

    bool ok;
    QString newText = QInputDialog::getText(this, "Edit Notes", "Edit text:",
                                            QLineEdit::Normal, originalText, &ok);

    Update_User_Notes(strUsername, newText.toStdString(), originalText.toStdString());

    if (!ok || newText.isEmpty()) return;

    for (int i = 0; i < ui->notesListWidget->count(); ++i) {
        QListWidgetItem *existingItem = ui->notesListWidget->item(i);
        if (existingItem != item && existingItem->text().mid(2) == newText) {
            QMessageBox::warning(this, "Same Note Found", "This note already exists.");
            return;
        }
    }

    item->setText("• " + newText);
}

void patientfeed::addAppointment()
{
    AppointmentDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString date = dialog.getDateLineEdit()->text();
        QString time = dialog.getTimeComboBox()->currentText();
        QString doctorName = dialog.getDoctorNameLineEdit()->text();
        QString location = dialog.getLocationLineEdit()->text();

        if (!dialog.isValidDate(date)) {
            QMessageBox::warning(this, "Invalid Date", "Please enter the date in DD-MM-YYYY format.");
            return;
        }

        if (date.isEmpty() || doctorName.isEmpty() || location.isEmpty()) {
            QMessageBox::warning(this, "Input Error", "Please fill in all the fields.");
        } else {
            QString appointmentDetails = QString("%1 - %2 with Dr. %3 at %4")
                                             .arg(date, time, doctorName, location);

            Appointment newAppointment(strUsername, doctorName.toStdString(), location.toStdString(), date.toStdString(), time.toStdString());
            Save_User_Appointment(newAppointment);
            QListWidgetItem *item = new QListWidgetItem("• " + appointmentDetails, ui->appointmentsListWidget);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
}

void patientfeed::editAppointment()
{
    QListWidgetItem *item = ui->appointmentsListWidget->currentItem();
    if (!item)
        return;

    QString currentDetails = item->text().mid(2);

    if (!currentDetails.contains(" with ") || !currentDetails.contains(" at "))
    {
        QMessageBox::warning(this, "Invalid Data", "Appointment data is corrupted or incorrectly formatted.");
        return;
    }

    QStringList parts = currentDetails.split(" with ");
    QString dateTimeLocation = parts.value(0);
    QString doctorAndLocation = parts.value(1);

    QStringList dateTimeParts = dateTimeLocation.split(" - ");
    QString date = dateTimeParts.value(0);
    QString time = (dateTimeParts.size() > 1) ? dateTimeParts.value(1) : "";

    QStringList doctorLocationParts = doctorAndLocation.split(" at ");
    QString doctorName = doctorLocationParts.value(0).trimmed();
    QString location = doctorLocationParts.value(1).trimmed();

    if (doctorName.startsWith("Dr. "))
    {
        doctorName = doctorName.mid(4);
    }

    AppointmentDialog dialog(this);

    dialog.getDateLineEdit()->setText(date);
    dialog.getTimeComboBox()->setCurrentText(time);
    dialog.getDoctorNameLineEdit()->setText(doctorName);
    dialog.getLocationLineEdit()->setText(location);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString newDoctorName = dialog.getDoctorNameLineEdit()->text().trimmed();
        QString newLocation = dialog.getLocationLineEdit()->text().trimmed();
        QString newDate = dialog.getDateLineEdit()->text().trimmed();
        QString newTime = dialog.getTimeComboBox()->currentText().trimmed();

        if (newDoctorName.isEmpty() || newLocation.isEmpty() || newDate.isEmpty())
        {
            QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
            return;
        }

        QString newDetails = QString("%1 - %2 with Dr. %3 at %4")
                                 .arg(newDate, newTime, newDoctorName, newLocation);

        item->setText("• " + newDetails);

        Update_User_Appointment(strUsername,
                                newDoctorName.toStdString(), newLocation.toStdString(),
                                newDate.toStdString(), newTime.toStdString(),
                                doctorName.toStdString(), location.toStdString(),
                                date.toStdString(), time.toStdString());
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
        editAppointment();
    } else if (selectedAction == deleteAction) {
        cancelAppointment();
    }
}


void patientfeed::cancelAppointment()
{
    QListWidgetItem *item = ui->appointmentsListWidget->currentItem();
    if (!item)
        return;

    QString currentDetails = item->text().mid(2);

    if (!currentDetails.contains(" with ") || !currentDetails.contains(" at "))
    {
        QMessageBox::warning(this, "Invalid Data", "Appointment data is corrupted or incorrectly formatted.");
        return;
    }

    QStringList parts = currentDetails.split(" with ");
    QString dateTimeLocation = parts.value(0);
    QString doctorAndLocation = parts.value(1);

    QStringList dateTimeParts = dateTimeLocation.split(" - ");
    QString date = dateTimeParts.value(0);
    QString time = (dateTimeParts.size() > 1) ? dateTimeParts.value(1) : "";

    QStringList doctorLocationParts = doctorAndLocation.split(" at ");
    QString doctorName = doctorLocationParts.value(0).trimmed();
    QString location = doctorLocationParts.value(1).trimmed();

    if (doctorName.startsWith("Dr. "))
    {
        doctorName = doctorName.mid(4);
    }


    Delete_User_Appointment(strUsername, doctorName.toStdString(), location.toStdString(), date.toStdString(), time.toStdString());

    delete item;
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

void patientfeed::addPrescription()
{
    bool ok;
    QString doctorName = QInputDialog::getText(this, "Doctor's Name", "Enter Doctor's Name:", QLineEdit::Normal, "", &ok);
    if (!ok || doctorName.isEmpty()) return;

    QStringList medicines;
    for (int i = 0; i < 15; ++i) {
        QString medicine = QInputDialog::getText(this, "Add Medicine", QString("Enter Medicine %1 (or leave empty to stop):").arg(i + 1), QLineEdit::Normal, "", &ok);
        if (!ok || medicine.isEmpty()) break;
        medicines.append(medicine);
    }

    if (medicines.isEmpty()) {
        QMessageBox::warning(this, "No Medicines Added", "You must add at least one medicine.");
        return;
    }

    QString prescriptionDetails = QString("Dr. %1: %2").arg(doctorName, medicines.join(", "));
    QListWidgetItem *item = new QListWidgetItem("\u2022 " + prescriptionDetails, ui->prescriptionsListWidget);
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);

    std::string strDoctorName = doctorName.toStdString();
    std::string strMedicines = medicines.join(", ").toStdString();
    Prescription newPrescription(strUsername, strDoctorName, strMedicines);
    SavePrescription(newPrescription);
}

void patientfeed::loadPrescriptions()
{
    ui->prescriptionsListWidget->clear();

    std::vector<std::string> prescriptions = GetUserPrescriptions(strUsername);

    for (const std::string &prescription : prescriptions) {
        QString prescriptionDetails = QString::fromStdString(prescription);
        QListWidgetItem *item = new QListWidgetItem("\u2022 " + prescriptionDetails, ui->prescriptionsListWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}

void patientfeed::loadReminders()
{
    ui->remindersListWidget->clear();

    std::vector<std::string> reminders = Get_User_Reminders(strUsername);

    for (const std::string &reminder : reminders) {
        QString reminderDetails = QString::fromStdString(reminder);
        QListWidgetItem *item = new QListWidgetItem("\u2022 " + reminderDetails, ui->remindersListWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}

void patientfeed::loadAppointments()
{
    ui->appointmentsListWidget->clear();

    std::vector<std::string> appointments = Get_User_Appointments(strUsername);

    for (const std::string &appointment : appointments) {
        QString appointmentDetails = QString::fromStdString(appointment);
        QListWidgetItem *item = new QListWidgetItem("\u2022 " + appointmentDetails, ui->appointmentsListWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}

void patientfeed::editPrescription()
{
    QListWidgetItem *item = ui->prescriptionsListWidget->currentItem();
    if (!item) return;

    QString currentText = item->text().mid(2);
    QStringList parts = currentText.split(": ");
    QString oldDoctorName = parts.value(0).mid(4);
    QString oldMedicineList = parts.value(1);

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Prescription");
    QVBoxLayout layout(&dialog);

    QFormLayout formLayout;
    QLineEdit doctorNameEdit;
    doctorNameEdit.setText(oldDoctorName);
    formLayout.addRow("Doctor's Name:", &doctorNameEdit);

    QVector<QLineEdit*> medicineEdits;
    QStringList oldMedicines = oldMedicineList.split(", ");
    for (int i = 0; i < 15; ++i) {
        QLineEdit *medicineEdit = new QLineEdit(&dialog);
        if (i < oldMedicines.size()) {
            medicineEdit->setText(oldMedicines[i]);
        }
        formLayout.addRow(QString("Medicine %1:").arg(i + 1), medicineEdit);
        medicineEdits.append(medicineEdit);
    }

    layout.addLayout(&formLayout);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout.addWidget(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QString newDoctorName = doctorNameEdit.text();
        QStringList newMedicines;
        for (auto *edit : medicineEdits) {
            if (!edit->text().isEmpty()) {
                newMedicines.append(edit->text());
            }
        }

        QString newMedicineList = newMedicines.join(", ");

        if (!newDoctorName.isEmpty() && !newMedicines.isEmpty()) {
            QString prescriptionText = QString("Dr. %1: %2").arg(newDoctorName, newMedicineList);
            item->setText("\u2022 " + prescriptionText);

            qDebug() << "Old Doctor Name: " << oldDoctorName;
            qDebug() << "New Doctor Name: " << newDoctorName;
            qDebug() << "Old Medicines: " << oldMedicineList;
            qDebug() << "New Medicines: " << newMedicineList;
        } else {
            QMessageBox::warning(this, "No Fields Should Be Empty", "PLease fill up the required fields.");
            return;
        }
        UpdatePrescription(strUsername, newDoctorName.toStdString(), newMedicineList.toStdString(), oldDoctorName.toStdString(), oldMedicineList.toStdString());
    }
}

void patientfeed::deletePrescription()
{
    QListWidgetItem *item = ui->prescriptionsListWidget->currentItem();

    QString currentText = item->text().mid(2);
    QStringList parts = currentText.split(": ");

    if (parts.size() < 2) {
        QMessageBox::warning(this, "Invalid Data", "Prescription data is incomplete or corrupted.");
        return;
    }

    QString doctorName = parts.value(0).mid(4);
    QString medicineList = parts.value(1);

    DeletePrescription(strUsername, doctorName.toStdString(), medicineList.toStdString());

    if (item) {
        delete item;
    }
}

void patientfeed::showPrescriptionsContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->prescriptionsListWidget->viewport()->mapToGlobal(pos);
    QMenu menu;
    QAction *editAction = menu.addAction("Edit Prescription");
    QAction *deleteAction = menu.addAction("Delete Prescription");

    QAction *selectedAction = menu.exec(globalPos);
    if (selectedAction == editAction) {
        editPrescription();
    } else if (selectedAction == deleteAction) {
        deletePrescription();
    }
}

void patientfeed::addReminder()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Reminder");

    QFormLayout form(&dialog);

    QLineEdit *titleEdit = new QLineEdit(&dialog);
    QLineEdit *dateEdit = new QLineEdit(&dialog);
    QLineEdit *timeEdit = new QLineEdit(&dialog);
    QLineEdit *noteEdit = new QLineEdit(&dialog);

    dateEdit->setPlaceholderText("DD-MM-YYYY");
    timeEdit->setPlaceholderText("HH:MM AM/PM (Optional)");
    noteEdit->setPlaceholderText("Optional note");

    form.addRow("Title:", titleEdit);
    form.addRow("Date:", dateEdit);
    form.addRow("Time:", timeEdit);
    form.addRow("Note:", noteEdit);

    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    form.addRow(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString title = titleEdit->text();
        QString date = dateEdit->text();
        QString time = timeEdit->text();
        QString note = noteEdit->text();

        if (title.isEmpty() || date.isEmpty())
        {
            QMessageBox::warning(this, "Input Error", "Title and Date are required!");
            return;
        }

        QString reminderDetails = QString("• %1 - %2").arg(title, date);
        if (!time.isEmpty())
            reminderDetails.append(" - " + time);
        if (!note.isEmpty())
            reminderDetails.append(" - " + note);

        QListWidgetItem *item = new QListWidgetItem(reminderDetails, ui->remindersListWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        Reminder newReminder(strUsername, title.toStdString(), date.toStdString(), time.toStdString(), note.toStdString());
        Save_User_Reminder(newReminder);
    }
}

void patientfeed::editReminder()
{
    QListWidgetItem *item = ui->remindersListWidget->currentItem();
    if (!item)
        return;

    QString currentText = item->text().mid(2);
    QStringList parts = currentText.split(" - ");

    QString title = parts.value(0);
    QString date = parts.value(1);
    QString time = (parts.size() > 2) ? parts.value(2) : "";
    QString note = (parts.size() > 3) ? parts.value(3) : "";

    QDialog dialog(this);
    dialog.setWindowTitle("Edit Reminder");

    QFormLayout form(&dialog);

    QLineEdit *titleEdit = new QLineEdit(title, &dialog);
    QLineEdit *dateEdit = new QLineEdit(date, &dialog);
    QLineEdit *timeEdit = new QLineEdit(time, &dialog);
    QLineEdit *noteEdit = new QLineEdit(note, &dialog);

    dateEdit->setPlaceholderText("DD-MM-YYYY");
    timeEdit->setPlaceholderText("HH:MM AM/PM (Optional)");
    noteEdit->setPlaceholderText("Optional note");

    form.addRow("Title:", titleEdit);
    form.addRow("Date:", dateEdit);
    form.addRow("Time:", timeEdit);
    form.addRow("Note:", noteEdit);

    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    form.addRow(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString newTitle = titleEdit->text();
        QString newDate = dateEdit->text();
        QString newTime = timeEdit->text();
        QString newNote = noteEdit->text();

        if (newTitle.isEmpty() || newDate.isEmpty())
        {
            QMessageBox::warning(this, "Input Error", "Title and Date are required!");
            return;
        }

        QString newReminderDetails = QString("• %1 - %2").arg(newTitle, newDate);
        if (!newTime.isEmpty())
            newReminderDetails.append(" - " + newTime);
        if (!newNote.isEmpty())
            newReminderDetails.append(" - " + newNote);

        item->setText(newReminderDetails);

        Update_Reminder(strUsername, newTitle.toStdString(), newDate.toStdString(), newTime.toStdString(), newNote.toStdString(), title.toStdString(), date.toStdString(), time.toStdString(), note.toStdString());
    }
}

void patientfeed::deleteReminder()
{
    QListWidgetItem *item = ui->remindersListWidget->currentItem();
    if (!item)
        return;

    QString currentText = item->text().mid(2);
    QStringList parts = currentText.split(" - ");

    QString title = parts.value(0);
    QString date = parts.value(1);
    QString time = (parts.size() > 2) ? parts.value(2) : "";
    QString note = (parts.size() > 3) ? parts.value(3) : "";

    Delete_Reminder(strUsername, title.toStdString(), date.toStdString(), time.toStdString(), note.toStdString());

    delete item;
}

void patientfeed::showRemindersContextMenu(const QPoint &pos)
{
    QPoint globalPos = ui->remindersListWidget->viewport()->mapToGlobal(pos);
    QMenu menu;
    QAction *editAction = menu.addAction("Edit Reminder");
    QAction *deleteAction = menu.addAction("Delete Reminder");

    QAction *selectedAction = menu.exec(globalPos);
    if (selectedAction == editAction) {
        editReminder();
    } else if (selectedAction == deleteAction) {
        deleteReminder();
    }
}

