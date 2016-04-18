#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <qstring.h>
#include <qsqldatabase.h>

class DbManager
{
public:
    QSqlDatabase db;
    DbManager(const QString& path);
};

#endif // DBMANAGER_H
