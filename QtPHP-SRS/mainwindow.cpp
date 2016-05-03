#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    phpsrs = new PhpSrs();
    productTable = new QSqlQueryModel(ui->productTableView);
    saleReceiptTable = new QSqlQueryModel(ui->salesReceiptTable);
    individualItemSalesTable = new QSqlQueryModel(ui->individualItemSalesTable);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete phpsrs;
    delete productTable;
}

bool MainWindow::connect(QString url, QString user, QString password, QString database)
{
    bool temp = phpsrs->connect(url, user, password, database);
    //productTable = new QSqlRelationalTableModel(ui->productTableView, QSqlDatabase::database());

    //searchSaleReceipt();
    //searchIndiviualItemSales();
    searchProduct();

    return temp;
}

void MainWindow::on_searchProductButton_clicked()
{
    QString search = ui->productSearchBar->text();
    searchProduct(search);
}

void MainWindow::searchProduct(QString search)
{

    // WARNING UNSAFE, needs to be fixed to use prepared statements

    QSqlDatabase defaultDB = QSqlDatabase::database();
    productTable->setQuery("SELECT * FROM `php-srs`.products WHERE `Name` LIKE '%" + search + "%';", defaultDB);

    ui->productTableView->setModel(productTable);
    ui->productTableView->horizontalHeader()->show();
    ui->productTableView->show();

    // Code that doesn't work
    /*
    QSqlDatabase defaultDB = QSqlDatabase::database();

    QSqlQuery query(defaultDB);
    query.prepare("SELECT * FROM `php-srs`.products WHERE `Name` LIKE CONCAT('%',:VAL,'%')");
    query.bindValue(":VAL", QString("ell"), QSql::Out);
    query.exec();

    qDebug() << query.lastQuery();
    productTable->setQuery(query.executedQuery());

    ui->productTableView->setModel(productTable);
    ui->productTableView->show();
    */
}

void MainWindow::searchSaleReceipt()
{
    QSqlDatabase defaultDB = QSqlDatabase::database();
    saleReceiptTable->setQuery("SELECT * FROM sales_record", defaultDB);

    ui->salesReceiptTable->setModel(saleReceiptTable);
    ui->salesReceiptTable->horizontalHeader()->show();
    ui->salesReceiptTable->show();
}

void MainWindow::searchIndiviualItemSales()
{
    individualItemSalesTable->setQuery("SELECT ids.SaleID, p.Name, ids.Quanity FROM individual_sale ids INNER JOIN products p ON ids.ProductID = p.ProductID;");

    ui->individualItemSalesTable->setModel(individualItemSalesTable);
    ui->individualItemSalesTable->horizontalHeader()->show();
    ui->individualItemSalesTable->show();
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    switch(index) {
    case 0:
        searchProduct();
        break;
    case 1:
        searchSaleReceipt();
        break;
    case 2:
        searchIndiviualItemSales();
        break;
    }
}
