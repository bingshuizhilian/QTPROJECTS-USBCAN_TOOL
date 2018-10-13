#ifndef CONFIGUSBCAN_H
#define CONFIGUSBCAN_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>

namespace Ui {
class ConfigUsbCan;
}

class ConfigUsbCan : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigUsbCan(QWidget *parent = 0);
    ~ConfigUsbCan();

private:
    Ui::ConfigUsbCan *ui;

//added by me
private:
    QComboBox *usbDevList;
    QPushButton *okBtn;
    QTableWidget *usbDevDescTbl;

private:
    void componentsAndLayoutInit();
};

#endif // CONFIGUSBCAN_H
