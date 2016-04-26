#ifndef PHPSRS_H
#define PHPSRS_H

#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRelationalTableModel>
#include <QTableView>
#include <QSqlTableModel>

class PhpSrs
{    
private:
    QSqlDatabase* db = nullptr;
    QSqlRelationalTableModel* model = nullptr;


public:
    PhpSrs();
    bool connect(QString url, QString user, QString password, QString database);

    bool getProductsTable();
};

#endif // PHPSRS_H
