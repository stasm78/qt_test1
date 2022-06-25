#include "mainwindow.h"
#include "form_cpp_1.h"

#include <QApplication>

extern const int g_var1{6};
double g_var2{9.8};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Form_cpp_1 w1;
    w1.show();
    return a.exec();
}
