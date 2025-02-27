#include "login1.h"
#include "ui_login1.h"

Login1::Login1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login1)
{
    ui->setupUi(this);
}

Login1::~Login1()
{
    delete ui;
}
