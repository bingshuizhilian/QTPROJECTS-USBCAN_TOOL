#include "configusbcan.h"
#include "ui_configusbcan.h"
#include <QLayout>

ConfigUsbCan::ConfigUsbCan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigUsbCan)
{
    componentsAndLayoutInit();

    //this->setGeometry(parent->geometry());
}

ConfigUsbCan::~ConfigUsbCan()
{
    delete ui;
}

void ConfigUsbCan::componentsAndLayoutInit()
{
    ui->setupUi(this);
    //components
    usbDevList = new QComboBox;
    okBtn = new QPushButton;
    usbDevDescTbl = new QTableWidget;

    //Layout
    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(usbDevList);
    topLayout->addWidget(okBtn);

    auto globleLayout = new QVBoxLayout;
    globleLayout->addLayout(topLayout);
    globleLayout->addWidget(usbDevDescTbl);

    this->setLayout(globleLayout);
    this->setWindowTitle("choose device");
//    this->setGeometry();

}
