#pragma once

#include "toolcatalog.h"

#include <QAbstractListModel>

class FolderListModel final : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        ItemTypeRole = Qt::UserRole + 1,
        ItemIdRole,
        ToolIdRole,
        ItemPtrRole
    };

    explicit FolderListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    const CatalogItem *currentFolder() const;
    void setCurrentFolder(const CatalogItem *folder);
    const CatalogItem *itemAt(const QModelIndex &index) const;

private:
    const CatalogItem *folder_ = nullptr;
};

