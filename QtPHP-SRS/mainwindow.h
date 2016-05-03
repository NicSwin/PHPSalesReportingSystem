#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "phpsrs.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool connect(QString url, QString user, QString password, QString database);
    void searchProduct(QString search = "");
    void searchSaleReceipt();
    void searchIndiviualItemSales();

private slots:
    void on_searchProductButton_clicked();
    void on_tabWidget_tabBarClicked(int index);

private:
    Ui::MainWindow *ui;
    PhpSrs* phpsrs;

    QSqlQueryModel* productTable = nullptr;
    QSqlQueryModel* saleReceiptTable = nullptr;
    QSqlQueryModel* individualItemSalesTable = nullptr;
};

#endif // MAINWINDOW_H
