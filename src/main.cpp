#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

#include "ui/cassettetheme.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CourseDesign");
    QCoreApplication::setApplicationName("Toolbox");

    QApplication app(argc, argv);
    CassetteTheme::apply();
    MainWindow window;
    window.show();
    return app.exec();
}
