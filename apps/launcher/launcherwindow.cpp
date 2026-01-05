#include "launcherwindow.h"

#include "core/logging.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QVBoxLayout>

namespace {

QString executableFileName(const QString &baseName)
{
#if defined(Q_OS_WIN)
    return baseName + ".exe";
#else
    return baseName;
#endif
}

QString toolPath(const QString &baseName)
{
    const auto exeName = executableFileName(baseName);
    return QDir(QCoreApplication::applicationDirPath()).filePath(exeName);
}

} // namespace

LauncherWindow::LauncherWindow(QWidget *parent) : QMainWindow(parent)
{
    Logging::init();

    setupUi();
    setupTray();
}

LauncherWindow::~LauncherWindow() = default;

void LauncherWindow::setupUi()
{
    setWindowTitle("Toolbox 启动器");
    resize(520, 260);

    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *root = new QVBoxLayout(central);

    auto *title = new QLabel("选择要打开的工具：", central);
    title->setStyleSheet("font-size: 16px; font-weight: 600;");
    root->addWidget(title);

    auto *btnRow = new QHBoxLayout();
    openPasswordBtn_ = new QPushButton("打开密码管理器", central);
    openTranslateBtn_ = new QPushButton("打开翻译工具", central);
    openPasswordBtn_->setMinimumHeight(44);
    openTranslateBtn_->setMinimumHeight(44);
    btnRow->addWidget(openPasswordBtn_);
    btnRow->addWidget(openTranslateBtn_);
    root->addLayout(btnRow);

    statusLabel_ = new QLabel("就绪", central);
    statusLabel_->setStyleSheet("color: #555;");
    root->addWidget(statusLabel_);

    root->addStretch(1);

    connect(openPasswordBtn_, &QPushButton::clicked, this, [this]() {
        launchTool("ToolboxPassword", "密码管理器");
    });
    connect(openTranslateBtn_, &QPushButton::clicked, this, [this]() {
        launchTool("ToolboxTranslate", "翻译工具");
    });
}

void LauncherWindow::setupTray()
{
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        trayEnabled_ = false;
        return;
    }

    trayIcon_ = new QSystemTrayIcon(this);
    trayIcon_->setToolTip("Toolbox 启动器");
    trayIcon_->setIcon(QApplication::windowIcon());

    auto *menu = new QMenu(this);
    actionShow_ = menu->addAction("打开启动器");
    actionQuit_ = menu->addAction("退出启动器");

    connect(actionShow_, &QAction::triggered, this, [this]() { activateFromMessage(); });
    connect(actionQuit_, &QAction::triggered, qApp, &QCoreApplication::quit);

    trayIcon_->setContextMenu(menu);
    trayIcon_->show();

    connect(trayIcon_, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
            activateFromMessage();
    });
}

void LauncherWindow::closeEvent(QCloseEvent *event)
{
    if (!trayEnabled_) {
        event->accept();
        return;
    }

    hide();
    event->ignore();
    statusLabel_->setText("已最小化到托盘");
}

void LauncherWindow::activateFromMessage()
{
    showNormal();
    raise();
    activateWindow();
}

bool LauncherWindow::launchTool(const QString &baseName, const QString &displayName)
{
    const auto program = toolPath(baseName);
    if (!QFileInfo::exists(program)) {
        QMessageBox::warning(this, "未找到程序", QString("未找到 %1：\n%2").arg(displayName, program));
        return false;
    }

    const auto ok = QProcess::startDetached(program, {}, QCoreApplication::applicationDirPath());
    if (!ok) {
        QMessageBox::warning(this, "启动失败", QString("无法启动 %1，请检查权限或路径。").arg(displayName));
        return false;
    }

    statusLabel_->setText(QString("已尝试启动：%1").arg(displayName));
    return true;
}

