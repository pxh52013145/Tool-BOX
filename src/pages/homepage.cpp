#include "homepage.h"

#include "catalog/folderlistmodel.h"
#include "pages/translatorpage.h"
#include "ui/scanlineoverlay.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QResizeEvent>

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    const auto *root = ToolCatalog::root();
    path_.push_back(root);
    currentFolder_ = root;

    setupUi();
    setFolder(root, true);

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HomePage::updateClock);
    timer->start(1000);
    updateClock();
}

void HomePage::setupUi()
{
    auto *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(12, 12, 12, 12);
    rootLayout->setSpacing(10);

    auto *header = new QWidget(this);
    auto *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(12, 10, 12, 10);

    auto *titleBox = new QVBoxLayout();
    auto *title = new QLabel("CASSETTEOS", header);
    title->setObjectName("CassetteTitle");
    auto *subtitle = new QLabel("SYSTEM_READY // V.0.1", header);
    subtitle->setObjectName("CassetteSubtitle");
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);
    headerLayout->addLayout(titleBox);

    headerLayout->addStretch(1);

    auto *clockBox = new QVBoxLayout();
    clockLabel_ = new QLabel(header);
    clockLabel_->setStyleSheet("font-size: 28px; font-weight: 700;");
    clockLabel_->setAlignment(Qt::AlignRight);
    dateLabel_ = new QLabel(header);
    dateLabel_->setStyleSheet("color: rgba(245,158,11,0.55);");
    dateLabel_->setAlignment(Qt::AlignRight);
    clockBox->addWidget(clockLabel_);
    clockBox->addWidget(dateLabel_);
    headerLayout->addLayout(clockBox);

    header->setStyleSheet("background: rgba(0,0,0,0.35); border-bottom: 4px solid rgba(245,158,11,0.65);");
    rootLayout->addWidget(header);

    auto *window = new QWidget(this);
    window->setObjectName("CassetteWindow");
    auto *windowLayout = new QVBoxLayout(window);
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->setSpacing(0);

    auto *topBar = new QWidget(window);
    topBar->setObjectName("CassetteTopBar");
    auto *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(12, 8, 12, 8);

    auto *status = new QLabel("User: ADMIN // Mem: 64K OK", topBar);
    status->setStyleSheet("color: rgba(245,158,11,0.55); letter-spacing: 2px;");
    topBarLayout->addWidget(status);
    topBarLayout->addStretch(1);

    auto *dot1 = new QLabel(topBar);
    dot1->setFixedSize(10, 10);
    dot1->setStyleSheet("background: rgba(146,64,14,0.6); border: 1px solid rgba(245,158,11,0.35); border-radius: 5px;");
    auto *dot2 = new QLabel(topBar);
    dot2->setFixedSize(10, 10);
    dot2->setStyleSheet("background: rgba(146,64,14,0.6); border: 1px solid rgba(245,158,11,0.35); border-radius: 5px;");
    topBarLayout->addWidget(dot1);
    topBarLayout->addSpacing(6);
    topBarLayout->addWidget(dot2);

    windowLayout->addWidget(topBar);

    workspaceStack_ = new QStackedWidget(window);
    windowLayout->addWidget(workspaceStack_, 1);

    explorerView_ = new QWidget(workspaceStack_);
    auto *explorerLayout = new QVBoxLayout(explorerView_);
    explorerLayout->setContentsMargins(14, 14, 14, 14);
    explorerLayout->setSpacing(10);

    breadcrumbsHost_ = new QWidget(explorerView_);
    breadcrumbsHost_->setLayout(new QHBoxLayout());
    breadcrumbsHost_->layout()->setContentsMargins(0, 0, 0, 0);
    explorerLayout->addWidget(breadcrumbsHost_);

    folderModel_ = new FolderListModel(this);
    folderView_ = new QListView(explorerView_);
    folderView_->setModel(folderModel_);
    folderView_->setViewMode(QListView::IconMode);
    folderView_->setResizeMode(QListView::Adjust);
    folderView_->setMovement(QListView::Static);
    folderView_->setWrapping(true);
    folderView_->setSpacing(6);
    folderView_->setIconSize(QSize(42, 42));
    folderView_->setGridSize(QSize(180, 120));
    folderView_->setUniformItemSizes(true);
    explorerLayout->addWidget(folderView_, 1);

    auto *footer = new QLabel(explorerView_);
    footer->setStyleSheet("color: rgba(245,158,11,0.35);");
    footer->setText("OBJECTS: 0    FREE SPACE: 0KB");
    explorerLayout->addWidget(footer);

    connect(folderView_, &QListView::clicked, this, [this](const QModelIndex &index) {
        const auto *item = folderModel_->itemAt(index);
        if (!item)
            return;
        if (item->type == CatalogItemType::Folder) {
            path_.push_back(item);
            setFolder(item, true);
        } else {
            openTool(item);
        }
    });

    connect(folderModel_, &QAbstractItemModel::modelReset, this, [this, footer]() {
        const auto count = folderModel_->rowCount();
        footer->setText(QString("OBJECTS: %1    FREE SPACE: 0KB").arg(count));
    });

    toolView_ = new QWidget(workspaceStack_);
    auto *toolLayout = new QVBoxLayout(toolView_);
    toolLayout->setContentsMargins(14, 14, 14, 14);
    toolLayout->setSpacing(10);

    toolHeader_ = new QWidget(toolView_);
    auto *toolHeaderLayout = new QHBoxLayout(toolHeader_);
    toolHeaderLayout->setContentsMargins(0, 0, 0, 0);

    toolTitle_ = new QLabel(toolHeader_);
    toolTitle_->setStyleSheet("font-size: 18px; font-weight: 700;");
    toolHeaderLayout->addWidget(toolTitle_);
    toolHeaderLayout->addStretch(1);

    terminateBtn_ = new QPushButton("[X] TERMINATE", toolHeader_);
    connect(terminateBtn_, &QPushButton::clicked, this, &HomePage::closeTool);
    toolHeaderLayout->addWidget(terminateBtn_);

    toolLayout->addWidget(toolHeader_);

    toolContentHost_ = new QWidget(toolView_);
    toolContentHost_->setLayout(new QVBoxLayout());
    toolContentHost_->layout()->setContentsMargins(0, 0, 0, 0);
    toolLayout->addWidget(toolContentHost_, 1);

    workspaceStack_->addWidget(explorerView_);
    workspaceStack_->addWidget(toolView_);
    workspaceStack_->setCurrentWidget(explorerView_);

    rootLayout->addWidget(window, 1);

    overlay_ = new ScanlineOverlay(this);
    overlay_->setGeometry(rect());
    overlay_->raise();
}

void HomePage::rebuildBreadcrumbs()
{
    auto *layout = qobject_cast<QHBoxLayout *>(breadcrumbsHost_->layout());
    if (!layout)
        return;

    while (auto *child = layout->takeAt(0)) {
        delete child->widget();
        delete child;
    }

    for (int i = 0; i < path_.size(); i++) {
        const auto *item = path_[i];
        auto *btn = new QToolButton(breadcrumbsHost_);
        btn->setText(item->name);
        btn->setToolButtonStyle(Qt::ToolButtonTextOnly);
        layout->addWidget(btn);

        connect(btn, &QToolButton::clicked, this, [this, i]() {
            if (i < 0 || i >= path_.size())
                return;
            path_.resize(i + 1);
            setFolder(path_.last(), true);
        });

        if (i != path_.size() - 1) {
            auto *sep = new QLabel(">", breadcrumbsHost_);
            sep->setStyleSheet("color: rgba(245,158,11,0.35); padding: 0 4px;");
            layout->addWidget(sep);
        }
    }
    layout->addStretch(1);
}

void HomePage::setFolder(const CatalogItem *folder, bool clearTool)
{
    currentFolder_ = folder;
    folderModel_->setCurrentFolder(folder);
    rebuildBreadcrumbs();

    if (clearTool)
        workspaceStack_->setCurrentWidget(explorerView_);
}

void HomePage::openTool(const CatalogItem *item)
{
    toolTitle_->setText(QString("EXECUTING: %1").arg(item->name));

    if (activeToolWidget_)
        closeTool();

    activeToolWidget_ = createToolWidget(item->toolId, item);
    toolContentHost_->layout()->addWidget(activeToolWidget_);
    workspaceStack_->setCurrentWidget(toolView_);
}

QWidget *HomePage::createToolWidget(ToolId id, const CatalogItem *item)
{
    switch (id) {
    case ToolId::Translator:
        return new TranslatorPage(toolView_);
    case ToolId::PasswordPlaceholder: {
        auto *w = new QWidget(toolView_);
        auto *l = new QVBoxLayout(w);
        l->addWidget(new QLabel("密码管理器：开发中（后续将接入数据库 + 加密 + 导入导出）。", w));
        l->addStretch(1);
        return w;
    }
    case ToolId::Readme: {
        auto *edit = new QPlainTextEdit(toolView_);
        edit->setReadOnly(true);
        edit->setPlainText(item ? item->docContent : QString());
        return edit;
    }
    default: {
        auto *w = new QWidget(toolView_);
        auto *l = new QVBoxLayout(w);
        l->addWidget(new QLabel("UNKNOWN_EXECUTABLE_FORMAT", w));
        l->addStretch(1);
        return w;
    }
    }
}

void HomePage::closeTool()
{
    if (activeToolWidget_) {
        toolContentHost_->layout()->removeWidget(activeToolWidget_);
        delete activeToolWidget_;
        activeToolWidget_ = nullptr;
    }
    workspaceStack_->setCurrentWidget(explorerView_);
}

void HomePage::updateClock()
{
    const auto now = QDateTime::currentDateTime();
    clockLabel_->setText(now.toString("HH:mm:ss"));
    dateLabel_->setText(now.toString("yyyy-MM-dd"));
}

void HomePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (overlay_)
        overlay_->setGeometry(rect());
}
