#ifndef APPOINTMENTDIALOG_H
#define APPOINTMENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class AppointmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppointmentDialog(QWidget *parent = nullptr);

    QLineEdit *getDateLineEdit() const;
    QComboBox *getTimeComboBox() const;
    QLineEdit *getDoctorNameLineEdit() const;
    QLineEdit *getLocationLineEdit() const;
    bool isValidDate(const QString &date) const;

private:
    QLineEdit *doctorNameLineEdit;
    QLineEdit *locationLineEdit;
    QLineEdit *dateLineEdit;
    QComboBox *timeComboBox;
    QPushButton *submitButton;

private slots:
    void onSubmit();
};

#endif // APPOINTMENTDIALOG_H
