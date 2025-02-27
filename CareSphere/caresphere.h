#ifndef CARESPHERE_H
#define CARESPHERE_H

#include <QMainWindow>
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CareSphere;
}
QT_END_NAMESPACE

class CareSphere : public QMainWindow
{
    Q_OBJECT

public:
    CareSphere(QWidget *parent = nullptr);
    ~CareSphere();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;  // 🔹 Declare eventFilter

private slots:
    void on_loginButton_clicked();

private:
    Ui::CareSphere *ui;
    Login loginSystem;
};
#endif // CARESPHERE_H
