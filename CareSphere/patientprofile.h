#ifndef PATIENTPROFILE_H
#define PATIENTPROFILE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class patientprofile;
}

class patientprofile : public QWidget
{
    Q_OBJECT

public:
    explicit patientprofile(QWidget *parent = nullptr);
    ~patientprofile();

private slots:
    void on_detailsBackToPatientFeed_clicked();

private:
    Ui::patientprofile *ui;
};

#endif
