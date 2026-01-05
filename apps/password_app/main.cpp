#include "passwordmainwindow.h"

#include "core/singleinstance.h"

#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CourseDesign");
    QCoreApplication::setApplicationName("ToolboxPassword");

    QApplication app(argc, argv);

    SingleInstance instance("CourseDesign.Toolbox.Password");
    if (!instance.tryRunAsPrimary()) {
        if (!instance.lastError().isEmpty())
            QMessageBox::critical(nullptr, "启动失败", instance.lastError());
        return 0;
    }

    PasswordMainWindow window;
    QObject::connect(&instance, &SingleInstance::messageReceived, &window, [&window](const QByteArray &) {
        window.activateFromMessage();
    });

    if (!window.isReady())
        return 1;

    window.show();
    return app.exec();
}
