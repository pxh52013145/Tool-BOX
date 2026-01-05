#include "passwordmainwindow.h"

#include "core/logging.h"
#include "pages/passwordmanagerpage.h"
#include "password/passworddatabase.h"

#include <QMessageBox>

PasswordMainWindow::PasswordMainWindow(QWidget *parent) : QMainWindow(parent)
{
    ready_ = ensureInfrastructure();
    if (!ready_)
        return;

    setupUi();
}

PasswordMainWindow::~PasswordMainWindow() = default;

bool PasswordMainWindow::ensureInfrastructure()
{
    Logging::init();

    if (!PasswordDatabase::open()) {
        QMessageBox::critical(this, "数据库初始化失败", "无法打开密码库 SQLite 数据库，请检查磁盘权限或路径。");
        return false;
    }

    return true;
}

void PasswordMainWindow::setupUi()
{
    setWindowTitle("Toolbox 密码管理器");
    resize(980, 720);

    page_ = new PasswordManagerPage(this);
    setCentralWidget(page_);
}

bool PasswordMainWindow::isReady() const
{
    return ready_;
}

void PasswordMainWindow::activateFromMessage()
{
    showNormal();
    raise();
    activateWindow();
}

