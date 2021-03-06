#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configusbcan.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chooseUsbDevDiag = new ConfigUsbCan(this);



    ui->textBrowser->append(chooseUsbDevDiag->settings().currentUsbDevExInfo.at(0));
    ui->textBrowser->append(chooseUsbDevDiag->settings().currentUsbDevExInfo.at(1));
    ui->textBrowser->append(chooseUsbDevDiag->settings().currentUsbDevExInfo.at(2));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_btnUsbDevConfig_clicked()
{
    chooseUsbDevDiag->exec();
}
