#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
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
    setupComboBoxes();

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
    QString totalPrice = ui->totalPriceEdit->text();
    if(totalPrice == "") {
        QMessageBox msgBox;
        msgBox.setText("You need to enter a total price.");
        msgBox.exec();
        return;
    }

    QSqlDatabase defaultDB = QSqlDatabase::database();
    QSqlQuery insertSale(defaultDB);

    insertSale.prepare("INSERT INTO sales_record (SalePrice) VALUES (?);");
    insertSale.addBindValue(totalPrice);

    if( !insertSale.exec() ) {
        QMessageBox msgBox;
        msgBox.setText("Error!\nPlease make sure total price is in decimal format.");
        msgBox.exec();
        return;
    }

    QString transactionID = insertSale.lastInsertId().toString();

    bool soldOne = false;

    int p1quanity = ui->p1Quanity->value();
    int p2quanity = ui->p2Quanity->value();
    int p3quanity = ui->p3Quanity->value();
    int p4quanity = ui->p4Quanity->value();
    int p5quanity = ui->p5Quanity->value();

    if(p1quanity > 0) {
        sellItem(ui->combo1, transactionID, p1quanity);
        soldOne = true;
    }
    if(p2quanity > 0) {
        sellItem(ui->combo2, transactionID, p2quanity);
        soldOne = true;
    }
    if(p3quanity > 0) {
        sellItem(ui->combo3, transactionID, p3quanity);
        soldOne = true;
    }
    if(p4quanity > 0) {
        sellItem(ui->combo4, transactionID, p4quanity);
        soldOne = true;
    }
    if(p5quanity > 0) {
        sellItem(ui->combo5, transactionID, p5quanity);
        soldOne = true;
    }

    if(soldOne) {
        QMessageBox msgBox;
        QString msg = "Sale recorded! ID: " + transactionID;
        msgBox.setText(msg);
        msgBox.exec();

        ui->p1Quanity->setValue(0);
        ui->p2Quanity->setValue(0);
        ui->p3Quanity->setValue(0);
        ui->p4Quanity->setValue(0);
        ui->p5Quanity->setValue(0);

        ui->combo1->setCurrentIndex(0);
        ui->combo2->setCurrentIndex(0);
        ui->combo3->setCurrentIndex(0);
        ui->combo4->setCurrentIndex(0);
        ui->combo5->setCurrentIndex(0);

        ui->totalPriceEdit->setText("");
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("You need to enter at least 1 product to make a sale.");
        msgBox.exec();
    }
}

bool MainWindow::sellItem(QComboBox* combo, QString transactionID, int quanity) {

    combo->setModelColumn(0);
    QString itemID = combo->currentText();
    int itemQuanity = quanity;
    combo->setModelColumn(2);

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

    setupComboBoxes();

    return true;
}

double MainWindow::calcTotalPrice()
{
    double totalPrice = 0;

    totalPrice += calcItem(ui->combo1, ui->p1Quanity);
    totalPrice += calcItem(ui->combo2, ui->p2Quanity);
    totalPrice += calcItem(ui->combo3, ui->p3Quanity);
    totalPrice += calcItem(ui->combo4, ui->p4Quanity);
    totalPrice += calcItem(ui->combo5, ui->p5Quanity);

    return totalPrice;
}

double MainWindow::calcItem(QComboBox* combo, QSpinBox* spinBox)
{
    combo->setModelColumn(1);
    double price = combo->currentText().toDouble() * spinBox->value();
    combo->setModelColumn(2);

    return price;
}

void MainWindow::on_FormTabs_tabBarClicked(int index)
{
    switch(index) {
    case 0:
        setupComboBoxes();
        break;
    default:
        break;
    }
}

void MainWindow::setupComboBoxes()
{
    QSqlQueryModel *model = new QSqlQueryModel (ui->combo1);
    model->setQuery ("SELECT ProductID, Price, CONCAT(ProductID, '. ', Name, ' $', Price), Stock FROM `php-srs`.products ORDER BY ProductID;");

    ui->combo1->setModel(model);
    ui->combo1->setModelColumn(2);
    ui->combo2->setModel(model);
    ui->combo2->setModelColumn(2);
    ui->combo3->setModel(model);
    ui->combo3->setModelColumn(2);
    ui->combo4->setModel(model);
    ui->combo4->setModelColumn(2);
    ui->combo5->setModel(model);
    ui->combo5->setModelColumn(2);

    QSqlQueryModel *model2 = new QSqlQueryModel (ui->productAlterCombo);
    model2->setQuery ("SELECT ProductID, Price, CONCAT(ProductID, '. ', Name, ' $', Price, ' Stock: ', Stock), Stock FROM `php-srs`.products ORDER BY ProductID;");

    ui->productAlterCombo->setModel(model2);
    ui->productAlterCombo->setModelColumn(2);
}

void MainWindow::on_totalPriceButton_clicked()
{
    QString text = QString::number(calcTotalPrice());
    ui->totalPriceEdit->setText(text);
}

void MainWindow::on_alterProductButton_clicked()
{
    ui->productAlterCombo->setModelColumn(0);
    QString ID = ui->productAlterCombo->currentText();
    ui->productAlterCombo->setModelColumn(2);

    QString newName = ui->newNameEdit->text();
    QString newPrice = ui->newPriceEdit->text();
    QString newStock = ui->newStockEdit->text();

    if(newName == "") {

    }

    QSqlDatabase defaultDB = QSqlDatabase::database();
    QSqlQuery item1(defaultDB);

    item1.prepare("UPDATE products SET Name = ?, Price = ?, Stock = ? WHERE ProductID = ?;");
    item1.addBindValue(newName);
    item1.addBindValue(newPrice);
    item1.addBindValue(newStock);
    item1.addBindValue(ID);

    if( item1.exec() ) {
        ui->newNameEdit->setText("");
        ui->newPriceEdit->setText("");
        ui->newStockEdit->setText("");

        ui->productAlterCombo->setCurrentIndex(0);

        QMessageBox msgBox;
        msgBox.setText("Altered product!");
        msgBox.exec();

        setupComboBoxes();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Error!\n" + item1.lastError().text());
        msgBox.exec();
    }
}
