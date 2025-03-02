#ifndef PATIENTFEED_H
#define PATIENTFEED_H

#include <QWidget>

namespace Ui {
class patientFeed;
}

class patientFeed : public QWidget
{
    Q_OBJECT

public:
    explicit patientFeed(QWidget *parent = nullptr);
    ~patientFeed();

private:
    Ui::patientFeed *ui;
};

#endif // PATIENTFEED_H
