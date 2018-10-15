#ifndef CONFIGUSBCAN_H
#define CONFIGUSBCAN_H

#include "libusb.h"
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
    ~ConfigUsbCan(void);

private:
    Ui::ConfigUsbCan *ui;

//added by me
private:
    const unsigned short USBCAN_VID = 0x4348;
    const unsigned short USBCAN_PID = 0x5537;
    const unsigned int USB_INFO_MAX_LEN = 256;
private:
    libusb_device_handle *usbDevHandle;

    QComboBox *usbDevList;
    QPushButton *okBtn;
    QTableWidget *usbDevDescTbl;

private:
    void componentsAndLayoutInit(void);

private slots:
    int getUsbDeviceInfo(void);
    void applyBtnPressed(void);
};

#endif // CONFIGUSBCAN_H
