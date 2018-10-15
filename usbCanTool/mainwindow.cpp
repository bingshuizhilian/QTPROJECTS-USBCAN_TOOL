#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ConfigUsbCan *a = new ConfigUsbCan(this);

    a->exec();

}

MainWindow::~MainWindow()
{
    delete ui;
}

