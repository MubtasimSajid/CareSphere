#ifndef REGUSERDETAILS_H
#define REGUSERDETAILS_H

#include <QWidget>
#include <QString>

using namespace std;

namespace Ui {
class RegUserDetails;
}

class RegUserDetails : public QWidget
{
    Q_OBJECT

public:
    explicit RegUserDetails(QWidget *parent = nullptr);
    ~RegUserDetails();

private:
    Ui::RegUserDetails *ui;
};

#endif
