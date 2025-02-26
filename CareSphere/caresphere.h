#ifndef CARESPHERE_H
#define CARESPHERE_H

#include <QMainWindow>

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

private:
    Ui::CareSphere *ui;
};
#endif // CARESPHERE_H
