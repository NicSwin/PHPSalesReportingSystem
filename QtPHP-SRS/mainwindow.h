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

private slots:
    void on_searchProductButton_clicked();

private:
    Ui::MainWindow *ui;
    PhpSrs* phpsrs;

    QSqlQueryModel* productTable = nullptr;
};

#endif // MAINWINDOW_H
