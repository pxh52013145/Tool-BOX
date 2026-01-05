#pragma once

#include "passwordentry.h"
#include "passwordgroup.h"

#include <QString>
#include <QStringList>
#include <QVector>

#include <optional>

class PasswordVault;

class PasswordRepository final
{
public:
    explicit PasswordRepository(PasswordVault *vault);

    QString lastError() const;

    QVector<PasswordEntry> listEntries() const;
    QStringList listCategories() const;

    QVector<PasswordGroup> listGroups() const;
    QStringList listAllTags() const;

    std::optional<qint64> createGroup(qint64 parentId, const QString &name);
    bool renameGroup(qint64 groupId, const QString &name);
    bool deleteGroup(qint64 groupId);

    bool addEntry(const PasswordEntrySecrets &secrets);
    bool addEntryWithTimestamps(const PasswordEntrySecrets &secrets, qint64 createdAtSecs, qint64 updatedAtSecs);
    bool updateEntry(const PasswordEntrySecrets &secrets);
    bool moveEntryToGroup(qint64 entryId, qint64 groupId);
    bool deleteEntry(qint64 id);

    std::optional<PasswordEntrySecrets> loadEntry(qint64 id) const;

private:
    void setError(const QString &error) const;

    PasswordVault *vault_ = nullptr;
    mutable QString lastError_;
};
