#include "translatemainwindow.h"

#include "core/logging.h"
#include "pages/translatorpage.h"
#include "translate/translatedatabase.h"

#include <QMessageBox>

TranslateMainWindow::TranslateMainWindow(QWidget *parent) : QMainWindow(parent)
{
    ready_ = ensureInfrastructure();
    if (!ready_)
        return;

    setupUi();
}

TranslateMainWindow::~TranslateMainWindow() = default;

bool TranslateMainWindow::ensureInfrastructure()
{
    Logging::init();

    if (!TranslateDatabase::open()) {
        QMessageBox::critical(this, "数据库初始化失败", "无法打开翻译历史 SQLite 数据库，请检查磁盘权限或路径。");
        return false;
    }

    return true;
}

void TranslateMainWindow::setupUi()
{
    setWindowTitle("Toolbox 翻译工具");
    resize(980, 720);

    page_ = new TranslatorPage(this);
    setCentralWidget(page_);
}

bool TranslateMainWindow::isReady() const
{
    return ready_;
}

void TranslateMainWindow::activateFromMessage()
{
    showNormal();
    raise();
    activateWindow();
}

