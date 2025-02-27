#ifndef LOGIN1_H
#define LOGIN1_H

#include <QWidget>

namespace Ui {
class Login1;
}

class Login1 : public QWidget
{
    Q_OBJECT

public:
    explicit Login1(QWidget *parent = nullptr);
    ~Login1();

private:
    Ui::Login1 *ui;
};

#endif // LOGIN1_H
