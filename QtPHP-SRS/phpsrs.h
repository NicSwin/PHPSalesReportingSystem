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
#include <QFile>
#include <QTextStream>

class PhpSrs
{    
public:
    PhpSrs();
    bool connect(QString url, QString user, QString password, QString database);

    bool getProductsTable();
    bool writeReport(QSqlQueryModel* model, QString fileName);
private:
    QSqlDatabase* db = nullptr;
    QSqlRelationalTableModel* model = nullptr;



};

#endif // PHPSRS_H
