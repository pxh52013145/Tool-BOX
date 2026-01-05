#pragma once

#include <QSqlDatabase>

class PasswordDatabase final
{
public:
    static bool open();
    static QSqlDatabase db();

private:
    static bool ensureSchema(QSqlDatabase &database);
};

