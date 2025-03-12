#ifndef PATIENTFEED_H
#define PATIENTFEED_H

#include <QWidget>

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

private:
    Ui::patientfeed *ui;
};

#endif
