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
    QSqlDatabase defaultDB = QSqlDatabase::database();
    productTable->setQuery("SELECT * FROM `php-srs`.products WHERE `Name` LIKE '%" + search + "%';", defaultDB);

    ui->productTableView->setModel(productTable);
    ui->productTableView->show();
}

void MainWindow::on_productTableView_clicked(const QModelIndex &index)
{
    qDebug() << index.column();
    ui->productTableView->sortByColumn(index.column(), Qt::AscendingOrder);
}
