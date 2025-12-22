#pragma once

#include "catalog/toolcatalog.h"

#include <QWidget>

class QLabel;
class QListView;
class QPushButton;
class QStackedWidget;
class QToolButton;
class FolderListModel;
class ScanlineOverlay;

class HomePage final : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUi();
    void rebuildBreadcrumbs();
    void setFolder(const CatalogItem *folder, bool clearTool);
    void openTool(const CatalogItem *item);
    QWidget *createToolWidget(ToolId id, const CatalogItem *item);
    void closeTool();
    void updateClock();

    QVector<const CatalogItem *> path_;
    const CatalogItem *currentFolder_ = nullptr;

    QLabel *clockLabel_ = nullptr;
    QLabel *dateLabel_ = nullptr;
    QWidget *breadcrumbsHost_ = nullptr;
    FolderListModel *folderModel_ = nullptr;
    QListView *folderView_ = nullptr;

    QWidget *toolHeader_ = nullptr;
    QLabel *toolTitle_ = nullptr;
    QPushButton *terminateBtn_ = nullptr;
    QWidget *toolContentHost_ = nullptr;
    QStackedWidget *workspaceStack_ = nullptr;
    QWidget *explorerView_ = nullptr;
    QWidget *toolView_ = nullptr;
    QWidget *activeToolWidget_ = nullptr;

    ScanlineOverlay *overlay_ = nullptr;
};
