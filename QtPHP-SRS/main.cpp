//#include "PHPSRS.h"
#include <iostream>

#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>

#include "mainwindow.h"

#define DBHOST "localhost"
#define USER "root"
#define PASSWORD "bird"
#define DATABASE "php-srs"

#define NUMOFFSET 100
#define COLNAME 200

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const QString url(argc >= 2 ? argv[1] : DBHOST);
    const QString user(argc >= 3 ? argv[2] : USER);
    const QString password(argc >= 4 ? argv[3] : PASSWORD);
    const QString database(argc >= 5 ? argv[4] : DATABASE);

    MainWindow w;
    if(w.connect(url, user, password, database)) {
        w.show();
        return a.exec();
    }
    else {
        qDebug() << "Cannot connect, closing PHP-SRS";
        return 0;
    }

    /*
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
    */
}
