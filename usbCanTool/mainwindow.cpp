#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "libusb.h"

#include <QDebug>

static void print_devs(libusb_device **devs)
{
    libusb_device *dev;
    int i = 0, j = 0, k = 0;
    uint8_t path[8];

    while ((dev = devs[i++]) != NULL) {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0) {
            fprintf(stderr, "failed to get device descriptor");
            return;
        }

        if(desc.idVendor == 0x0 && desc.idProduct == 0x0)
            k = i;

        printf("%04x:%04x (bus %d, device %d)",
            desc.idVendor, desc.idProduct,
            libusb_get_bus_number(dev), libusb_get_device_address(dev));

        r = libusb_get_port_numbers(dev, path, sizeof(path));
        if (r > 0) {
            printf(" path: %d", path[0]);
            for (j = 1; j < r; j++)
                printf(".%d", path[j]);
        }
        printf("\n");
    }

    libusb_device *devCan = devs[k];
    libusb_device_handle *pCanHandle = nullptr;
    qDebug() << libusb_open(devCan, &pCanHandle);

}

int main1(int argc, char *argv[])
{
    libusb_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init(NULL);
    if (r < 0)
        return r;

    cnt = libusb_get_device_list(NULL, &devs);
    if (cnt < 0)
        return (int) cnt;

    print_devs(devs);
    libusb_free_device_list(devs, 1);

    libusb_exit(NULL);
    return 0;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ConfigUsbCan *a = new ConfigUsbCan(this);

    a->exec();

    main1(0, nullptr);
}

MainWindow::~MainWindow()
{
    delete ui;
}

