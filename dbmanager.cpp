#include "dbmanager.h"

#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <qdebug.h>

DbManager::DbManager(void)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

QSqlDatabase& DbManager::open(const QString &path)
{
    db.close();
    db.setDatabaseName(path);
    if (!db.open()) {
      qDebug() << "Error: connection with database fail";
    } else {
      qDebug() << "Database: connection ok";
    }

    return db;
}

#if 0
void DbManager::listPacjents(void)
{
    QSqlQuery query("SELECT * FROM pacjent");
    int idName = query.record().indexOf("nazwisko");
    while (query.next()) {
       QString name = query.value(idName).toString();
       qDebug() << name;
    }
}
#endif
