#include "launcherwindow.h"

#include "core/singleinstance.h"

#include <QApplication>
#include <QCoreApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CourseDesign");
    QCoreApplication::setApplicationName("ToolboxLauncher");

    QApplication app(argc, argv);

    SingleInstance instance("CourseDesign.Toolbox.Launcher");
    if (!instance.tryRunAsPrimary()) {
        if (!instance.lastError().isEmpty())
            QMessageBox::critical(nullptr, "启动失败", instance.lastError());
        return 0;
    }

    LauncherWindow window;
    QObject::connect(&instance, &SingleInstance::messageReceived, &window, [&window](const QByteArray &) {
        window.activateFromMessage();
    });

    window.show();
    return app.exec();
}
