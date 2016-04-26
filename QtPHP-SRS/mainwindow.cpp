#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    phpsrs = new PhpSrs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::connect(QString url, QString user, QString password, QString database)
{
    return phpsrs->connect(url, user, password, database);
}
