#ifndef PATIENTPROFILE_H
#define PATIENTPROFILE_H

#include <QWidget>

namespace Ui {
class patientProfile;
}

class patientProfile : public QWidget
{
    Q_OBJECT

public:
    explicit patientProfile(QWidget *parent = nullptr);
    ~patientProfile();

private:
    Ui::patientProfile *ui;
};

#endif // PATIENTPROFILE_H
