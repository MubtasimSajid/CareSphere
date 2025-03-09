#ifndef REGUSERDETAILS_H
#define REGUSERDETAILS_H

#include <QWidget>

namespace Ui {
class regUserDetails;
}

class regUserDetails : public QWidget
{
    Q_OBJECT

public:
    explicit regUserDetails(QWidget *parent = nullptr);
    ~regUserDetails();

private:
    Ui::regUserDetails *ui;
};

#endif // REGUSERDETAILS_H
