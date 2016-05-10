#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QSqlRecord>
#include <QMessageBox>

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
    delete saleReceiptTable;
    delete individualItemSalesTable;
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
    QString query = "SELECT ids.SaleID, p.Name, ids.Quanity FROM individual_sale ids INNER JOIN products p ON ids.ProductID = p.ProductID;";
    individualItemSalesTable->setQuery(query);

    ui->individualItemSalesTable->setModel(individualItemSalesTable);
    ui->individualItemSalesTable->horizontalHeader()->show();
    ui->individualItemSalesTable->show();
}

void MainWindow::on_tabs_tabBarClicked(int index)
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

void MainWindow::on_productCSVButton_clicked()
{
    QString fileName( "Product_report_" );
    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyy_MM_dd_hh-mm-ss"));
    fileName += timestamp + ".txt";

    phpsrs->writeReport(productTable, fileName);
}


void MainWindow::on_salesReceiptsCSVButton_clicked()
{
    QString fileName( "Sales_report_" );
    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyy_MM_dd_hh-mm-ss"));
    fileName += timestamp + ".txt";

    phpsrs->writeReport(saleReceiptTable, fileName);
}

void MainWindow::on_individualItemSalesCSVButton_clicked()
{
    QString fileName( "Item_sales_report_" );
    const QDateTime now = QDateTime::currentDateTime();
    const QString timestamp = now.toString(QLatin1String("yyyy_MM_dd_hh-mm-ss"));
    fileName += timestamp + ".txt";

    phpsrs->writeReport(individualItemSalesTable, fileName);
}

void MainWindow::on_addProductButton_clicked()
{
    QSqlDatabase defaultDB = QSqlDatabase::database();

    QSqlQuery query(defaultDB);
    query.prepare("INSERT INTO `php-srs`.Products (Name, Price, Stock) VALUES (?, ?, ?)");
    query.addBindValue( ui->productNameEdit->text(), QSql::Out);
    query.addBindValue( ui->productPriceEdit->text(), QSql::Out);
    query.addBindValue( ui->initialStockEdit->text(), QSql::Out);

    QMessageBox msgBox;


    if( query.exec() ) {
        msgBox.setText(ui->productNameEdit->text()+ " has been added.");
        msgBox.exec();
        ui->productNameEdit->setText("");
        ui->productPriceEdit->setText("");
        ui->initialStockEdit->setText("");
    }
    else {
        msgBox.setText("Error!\n" + query.lastError().text());
        msgBox.exec();
    }

    //productTable->setQuery(query.executedQuery());
}

void MainWindow::on_recordSaleButton_clicked()
{
    QSqlDatabase defaultDB = QSqlDatabase::database();

    QSqlQuery insertSale(defaultDB);

    insertSale.prepare("INSERT INTO sales_record (SalePrice) VALUES (?);");
    insertSale.addBindValue(ui->totalPriceEdit->text());
    insertSale.exec();

    QString transactionID = insertSale.lastInsertId().toString();
    bool soldOne = false;

    if(ui->IDp1->text() != "") {
        sellItem(ui->IDp1->text(), ui->p1Quanity->value(), transactionID);
        soldOne = true;
    }
    if(ui->IDp2->text() != "") {
        sellItem(ui->IDp2->text(), ui->p2Quanity->value(), transactionID);
        soldOne = true;
    }
    if(ui->IDp3->text() != "") {
        sellItem(ui->IDp3->text(), ui->p3Quanity->value(), transactionID);
        soldOne = true;
    }
    if(ui->IDp4->text() != "") {
        sellItem(ui->IDp4->text(), ui->p4Quanity->value(), transactionID);
        soldOne = true;
    }
    if(ui->IDp5->text() != "") {
        sellItem(ui->IDp4->text(), ui->p5Quanity->value(), transactionID);
        soldOne = true;
    }

    QMessageBox msgBox;
    if(soldOne) {
        QString msg = "Sale recorded! ID: " + transactionID;
        msgBox.setText(msg);
        msgBox.exec();

        ui->IDp1->setText("");
        ui->IDp2->setText("");
        ui->IDp3->setText("");
        ui->IDp4->setText("");
        ui->IDp5->setText("");

        ui->p1Quanity->setValue(0);
        ui->p2Quanity->setValue(0);
        ui->p3Quanity->setValue(0);
        ui->p4Quanity->setValue(0);
        ui->p5Quanity->setValue(0);

        ui->totalPriceEdit->setText("");
    }
    else {
        msgBox.setText("You need to enter at least 1 product to make a sale.");
        msgBox.exec();
    }

}

bool MainWindow::sellItem(QString itemID, int itemQuanity, QString transactionID) {
    // Item 1
    QSqlDatabase defaultDB = QSqlDatabase::database();
    QSqlQuery item1(defaultDB);
    item1.prepare("INSERT INTO individual_sale (SaleID, ProductID, Quanity) VALUES (?, ?, ?);"
                  "UPDATE products SET stock = stock - ? WHERE ProductID = ?;");
    item1.addBindValue(transactionID);
    item1.addBindValue(itemID);
    item1.addBindValue(itemQuanity);
    item1.addBindValue(itemQuanity);
    item1.addBindValue(itemID);

    if( !item1.exec() ) {
        QMessageBox msgBox;
        msgBox.setText("Error!\n" + item1.lastError().text());
        msgBox.exec();
        return false;
    }

    return true;
}
