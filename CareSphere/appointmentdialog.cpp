#include "appointmentdialog.h"
#include <QRegularExpression>
#include <QMessageBox>

AppointmentDialog::AppointmentDialog(QWidget *parent)
    : QDialog(parent)
{
    doctorNameLineEdit = new QLineEdit(this);
    locationLineEdit = new QLineEdit(this);
    dateLineEdit = new QLineEdit(this);
    timeComboBox = new QComboBox(this);

    // Adding time slots to combo box
    timeComboBox->addItems({"9:00 AM", "10:00 AM", "11:00 AM", "3:00 PM", "4:00 PM"});

    submitButton = new QPushButton("Submit", this);
    connect(submitButton, &QPushButton::clicked, this, &AppointmentDialog::onSubmit);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Doctor's Name:"));
    layout->addWidget(doctorNameLineEdit);
    layout->addWidget(new QLabel("Location:"));
    layout->addWidget(locationLineEdit);
    layout->addWidget(new QLabel("Date (DD-MM-YYYY):"));
    layout->addWidget(dateLineEdit);
    layout->addWidget(new QLabel("Time:"));
    layout->addWidget(timeComboBox);
    layout->addWidget(submitButton);

    setLayout(layout);
    setWindowTitle("New Appointment");
}

QLineEdit *AppointmentDialog::getDateLineEdit() const {
    return dateLineEdit;
}

QComboBox *AppointmentDialog::getTimeComboBox() const {
    return timeComboBox;
}

QLineEdit *AppointmentDialog::getDoctorNameLineEdit() const {
    return doctorNameLineEdit;
}

QLineEdit *AppointmentDialog::getLocationLineEdit() const {
    return locationLineEdit;
}

bool AppointmentDialog::isValidDate(const QString &date) const {
    // Define a regular expression for the date format (DD-MM-YYYY)
    QRegularExpression datePattern("(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])-(\\d{4})");

    // Create a match object and check if the input matches the regular expression
    QRegularExpressionMatch match = datePattern.match(date);

    return match.hasMatch();  // Return true if the date matches the format
}

void AppointmentDialog::onSubmit()
{
    QString date = dateLineEdit->text();

    // If date is invalid, show an error message and return
    if (!isValidDate(date)) {
        QMessageBox::warning(this, "Invalid Date", "Please enter the date in DD-MM-YYYY format.");
        return;
    }

    accept();  // Closes the dialog and returns the result
}
