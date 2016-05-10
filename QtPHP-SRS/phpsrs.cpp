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

bool PhpSrs::writeReport(QSqlQueryModel* model, QString fileName)
{
  QString csvTable;

  for(int column = 0; column < model->columnCount(); column++) {
     QString data = model->headerData( column, Qt::Horizontal ).toString();
     csvTable += data;

     if(column != model->columnCount()-1) {
         csvTable += ",";
     }
     else {
         csvTable += "\n";
     }
  }

  for(int row = 0; row < model->rowCount(); row++) {
    csvTable += "";
    for(int column = 0; column < model->columnCount(); column++) {
      QString data = model->data(model->index(row, column), Qt::DisplayRole).toString();
      csvTable += data;

      if(column != model->columnCount()-1) {
          csvTable += ",";
      }
    }
    csvTable += "\n";
  }

  QFile file( fileName );
  if ( file.open(QIODevice::ReadWrite) )  {
      QTextStream stream( &file );
      stream << csvTable;
  }

  return true;
}
