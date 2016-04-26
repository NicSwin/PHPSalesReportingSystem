#include "phpsrs.h"

PhpSrs::PhpSrs()
{

}

bool PhpSrs::connect(QString url, QString user, QString password, QString database) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    db.setHostName(url);
    db.setDatabaseName(database);
    db.setUserName(user);
    db.setPassword(password);

    int returnVal = db.open();

    if (returnVal == 0) {
        qDebug() << "Failed to connect to root mysql admin";
        qDebug() << db.lastError().databaseText();
        qDebug() << db.lastError().driverText();
        return false;
    }
    else {
        qDebug() << "Connected to " + database;
        return true;
    }
}

bool PhpSrs::getProductsTable()
{
    return db->isValid();
}
