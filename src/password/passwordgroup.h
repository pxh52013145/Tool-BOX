#pragma once

#include <QtGlobal>
#include <QString>

struct PasswordGroup final
{
    qint64 id = 0;
    qint64 parentId = 0;
    QString name;
};

