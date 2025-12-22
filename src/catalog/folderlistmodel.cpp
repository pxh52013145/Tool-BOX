#include "folderlistmodel.h"

#include <QApplication>
#include <QStyle>

FolderListModel::FolderListModel(QObject *parent) : QAbstractListModel(parent) {}

int FolderListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return folder_ ? folder_->children.size() : 0;
}

static QIcon iconForKind(IconKind kind)
{
    if (!qApp)
        return {};

    switch (kind) {
    case IconKind::Folder:
        return qApp->style()->standardIcon(QStyle::SP_DirIcon);
    case IconKind::Tool:
        return qApp->style()->standardIcon(QStyle::SP_ComputerIcon);
    case IconKind::Doc:
        return qApp->style()->standardIcon(QStyle::SP_FileIcon);
    default:
        return {};
    }
}

QVariant FolderListModel::data(const QModelIndex &index, int role) const
{
    const auto *item = itemAt(index);
    if (!item)
        return {};

    switch (role) {
    case Qt::DisplayRole:
        return item->name;
    case Qt::DecorationRole:
        return iconForKind(item->icon);
    case Qt::ToolTipRole:
        return item->description;
    case ItemTypeRole:
        return static_cast<int>(item->type);
    case ItemIdRole:
        return item->id;
    case ToolIdRole:
        return static_cast<int>(item->toolId);
    case ItemPtrRole:
        return QVariant::fromValue(reinterpret_cast<quintptr>(item));
    default:
        return {};
    }
}

const CatalogItem *FolderListModel::currentFolder() const
{
    return folder_;
}

void FolderListModel::setCurrentFolder(const CatalogItem *folder)
{
    beginResetModel();
    folder_ = folder;
    endResetModel();
}

const CatalogItem *FolderListModel::itemAt(const QModelIndex &index) const
{
    if (!index.isValid() || !folder_)
        return nullptr;
    const auto row = index.row();
    if (row < 0 || row >= folder_->children.size())
        return nullptr;
    return folder_->children[row];
}

