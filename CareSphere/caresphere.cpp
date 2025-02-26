#include "caresphere.h"
#include "./ui_caresphere.h"

CareSphere::CareSphere(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CareSphere)
{
    ui->setupUi(this);
}

CareSphere::~CareSphere()
{
    delete ui;
}
