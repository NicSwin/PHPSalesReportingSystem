#ifndef PHPSRS_H
#define PHPSRS_H

#include <QApplication>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtDebug>
#include <QSqlError>
#include <QSqlQuery>

class PhpSrs
{    
private:
    QSqlDatabase* db = nullptr;

public:
    PhpSrs();
    bool connect(QString url, QString user, QString password, QString database);
};

#endif // PHPSRS_H
