#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    phpsrs = new PhpSrs();
    productTable = nullptr;
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
    productTable = new QSqlRelationalTableModel(ui->productTableView, QSqlDatabase::database());

    return temp;
}

void MainWindow::on_searchProductButton_clicked()
{
    QSqlDatabase defaultDB;

    defaultDB = QSqlDatabase::database();
    productTable->setTable("products");
    productTable->setFilter("Name Like %e%*");

    productTable->select();

    ui->productTableView->setModel(productTable);
    ui->productTableView->show();
}
