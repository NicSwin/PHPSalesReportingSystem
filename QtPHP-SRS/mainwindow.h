#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>

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

    bool sellItem(QComboBox* combo, QString transactionID, int quanity);
    double calcTotalPrice();
    double calcItem(QComboBox* combo, QSpinBox* spinBox);

    void setupComboBoxes();

private slots:
    void on_searchProductButton_clicked();

    void on_tabs_tabBarClicked(int index);

    void on_productCSVButton_clicked();
    void on_salesReceiptsCSVButton_clicked();
    void on_individualItemSalesCSVButton_clicked();

    void on_addProductButton_clicked();
    void on_recordSaleButton_clicked();

    void on_FormTabs_tabBarClicked(int index);

    void on_totalPriceButton_clicked();

    void on_alterProductButton_clicked();

private:
    Ui::MainWindow *ui;
    PhpSrs* phpsrs;

    QSqlQueryModel* productTable = nullptr;
    QSqlQueryModel* saleReceiptTable = nullptr;
    QSqlQueryModel* individualItemSalesTable = nullptr;
};

#endif // MAINWINDOW_H
