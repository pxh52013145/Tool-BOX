#pragma once

#include <QDateTime>
#include <QString>
#include <QStringList>

struct PasswordEntry final
{
    qint64 id = 0;
    qint64 groupId = 1;
    QString title;
    QString username;
    QString url;
    QString category;
    QStringList tags;
    QDateTime createdAt;
    QDateTime updatedAt;
};

struct PasswordEntrySecrets final
{
    PasswordEntry entry;
    QString password;
    QString notes;
};
