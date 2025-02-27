#include <QApplication>  // Use QApplication for GUI applications
#include "caresphere.h"  // Include the main window header file

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);  // Use QApplication for GUI applications

    CareSphere w;  // Create the main window
    w.show();      // Show the window

    return a.exec();  // Start the event loop
}
