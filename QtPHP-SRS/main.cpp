//#include "PHPSRS.h"
#include <iostream>

#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>

#define DBHOST "localhost"
#define USER "root"
#define PASSWORD "bird"
#define DATABASE "php-srs"

#define NUMOFFSET 100
#define COLNAME 200

int main(int argc, const char *argv[])
{
    // initiate url, user, password and database variables
    const QString url(argc >= 2 ? argv[1] : DBHOST);
    const QString user(argc >= 3 ? argv[2] : USER);
    const QString password(argc >= 4 ? argv[3] : PASSWORD);
    const QString database(argc >= 5 ? argv[4] : DATABASE);

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
    }
    else {
        qDebug() << "Connected to " + database;
    }

    QSqlQuery query;
    query.exec("SELECT * FROM products");

    qDebug() << "Products table";
    qDebug() << "-----------------------------";
    while (query.next()) {
       QString productID = query.value(0).toString();
       QString name = query.value(1).toString();
       QString price = query.value(2).toString();
       QString stock = query.value(3).toString();
       qDebug() << productID + " " + name + " " + price + " " + stock;
    }

    return returnVal;
}
