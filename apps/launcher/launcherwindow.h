#pragma once

#include <QMainWindow>

class QAction;
class QLabel;
class QPushButton;
class QSystemTrayIcon;

class LauncherWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit LauncherWindow(QWidget *parent = nullptr);
    ~LauncherWindow() override;

    void activateFromMessage();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void setupUi();
    void setupTray();

    bool launchTool(const QString &baseName, const QString &displayName);

    QPushButton *openPasswordBtn_ = nullptr;
    QPushButton *openTranslateBtn_ = nullptr;
    QLabel *statusLabel_ = nullptr;

    QSystemTrayIcon *trayIcon_ = nullptr;
    QAction *actionShow_ = nullptr;
    QAction *actionQuit_ = nullptr;

    bool trayEnabled_ = true;
};

