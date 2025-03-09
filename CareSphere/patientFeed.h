#ifndef PATIENTFEED_H
#define PATIENTFEED_H

#include <QWidget>

namespace Ui {
class PatientFeed;
}

class PatientFeed : public QWidget
{
    Q_OBJECT

public:
    explicit PatientFeed(QWidget *parent = nullptr);
    ~PatientFeed();

private:
    Ui::PatientFeed *ui;
};

#endif
