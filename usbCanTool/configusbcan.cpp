#include "configusbcan.h"
#include "ui_configusbcan.h"
#include <QLayout>

ConfigUsbCan::ConfigUsbCan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigUsbCan)
{
    getUsbDeviceInfo();

    componentsAndLayoutInit();

    this->setGeometry(750,350,0,0);
}

ConfigUsbCan::~ConfigUsbCan(void)
{
    delete ui;
}

void ConfigUsbCan::componentsAndLayoutInit(void)
{
    ui->setupUi(this);
    //components
    usbDevList = new QComboBox;
    okBtn = new QPushButton(tr("Apply"));
    connect(usbDevList, &usbDevList->currentIndexChanged, this, &getUsbDeviceInfo);
    usbDevDescTbl = new QTableWidget(14, 2);

    //Layout
    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(usbDevList);
    topLayout->addWidget(okBtn);

    auto globleLayout = new QVBoxLayout;
    globleLayout->addLayout(topLayout);
    globleLayout->addWidget(usbDevDescTbl);

    this->setLayout(globleLayout);
    this->setWindowTitle("choose device");

}

int ConfigUsbCan::getUsbDeviceInfo(void)
{
    libusb_device **devs;
    int r;
    ssize_t cnt;
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];

    r = libusb_init(NULL);
    if (r < 0)
        return r;

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int) cnt;

    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            fprintf(stderr, "failed to get device descriptor");
            return r;
        }

        if(desc.idVendor == USBCAN_VID && desc.idProduct == USBCAN_PID)
            ;//usbDevHandle = devs[i];

        printf("%04x:%04x (bus %d, device %d)",
            desc.idVendor, desc.idProduct,
            libusb_get_bus_number(dev), libusb_get_device_address(dev));

        r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0)
        {
            printf(" path: %d", path[0]);
            for (j = 1; j < r; j++)
                printf(".%d", path[j]);
        }
        printf("\n");
    }

    libusb_free_device_list(devs, 1);

    libusb_exit(NULL);

    return 0;
}


