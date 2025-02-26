#include "caresphere.h"
#include<iostream>

#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CareSphere w;
    w.show();
    cout<<"Hello World"<<endl;

    return a.exec();
}
