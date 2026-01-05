#pragma once

#include "passwordgroup.h"

#include <QAbstractItemModel>
#include <QHash>
#include <QVector>

class PasswordGroupModel final : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles
    {
        GroupIdRole = Qt::UserRole + 1,
    };

    explicit PasswordGroupModel(QObject *parent = nullptr);
    ~PasswordGroupModel() override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setGroups(const QVector<PasswordGroup> &groups);
    qint64 groupIdForIndex(const QModelIndex &index) const;
    QModelIndex indexForGroupId(qint64 groupId) const;
    QVector<qint64> descendantGroupIds(qint64 groupId) const;

private:
    struct Node;

    Node *nodeFromIndex(const QModelIndex &index) const;
    void collectDescendants(const Node *node, QVector<qint64> &out) const;

    Node *root_ = nullptr;
    QHash<qint64, Node *> nodesById_;
};

