#include "configusbcan.h"
#include "ui_configusbcan.h"
#include <QLayout>
#include <QDebug>

ConfigUsbCan::ConfigUsbCan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigUsbCan)
{
    componentsAndLayoutInit();

    getUsbDeviceInfo();


}

ConfigUsbCan::~ConfigUsbCan(void)
{
    delete ui;
}

void ConfigUsbCan::componentsAndLayoutInit(void)
{
    ui->setupUi(this);
    //components
    okBtn = new QPushButton(tr("Apply"));
    connect(okBtn, &okBtn->clicked, this, &applyBtnPressed);
    usbDevDescTbl = new QTableWidget(14, 2);
    usbDevDescTbl->setColumnWidth(0, 150);
    usbDevDescTbl->setColumnWidth(1, 150);

    usbDevList = new QComboBox;
//    connect(usbDevList, static_cast<void (QComboBox::*)(const QString&)>(&usbDevList->currentTextChanged), this, &getUsbDeviceInfo);
    connect(usbDevList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &updateUsbInfo);


    //Layout
    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(usbDevList);
    topLayout->addWidget(okBtn);

    auto globleLayout = new QVBoxLayout;
    globleLayout->addLayout(topLayout);
    globleLayout->addWidget(usbDevDescTbl);

    this->setLayout(globleLayout);
    this->setWindowTitle("choose device");
    this->setGeometry(750,350,0,0);
    this->setFixedSize(350,500);
}

int ConfigUsbCan::getUsbDeviceInfo(void)
{
    int r;
    ssize_t cnt;
    libusb_device *dev;
    int i = 0, j = 0;
    uint8_t path[8];
    unsigned char usbInfoBuf[USB_INFO_MAX_LEN] = { 0 };
    libusb_device_handle *devHandle = nullptr;
    int ret;

    r = libusb_init(NULL);
    if (r < 0)
        return r;

    cnt = libusb_get_device_list(NULL, &libusbDevs);
    if (cnt < 0)
        return (int) cnt;

    qDebug("-->%d<-- devices", cnt);

    while ((dev = libusbDevs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            fprintf(stderr, "failed to get device descriptor");
            return r;
        }

        libusbDevsDescs.push_back(desc);

//        devHandle = libusb_open_device_with_vid_pid(NULL, desc.idVendor, desc.idProduct);
        ret = libusb_open(dev, &devHandle);

        qDebug() << ret;

        if(LIBUSB_SUCCESS == ret)
        {
            QStringList tmpInfos;
            memset(usbInfoBuf, 0, USB_INFO_MAX_LEN);
            libusb_get_string_descriptor_ascii(devHandle, desc.iManufacturer, usbInfoBuf, USB_INFO_MAX_LEN);
            tmpInfos.append((const char *)usbInfoBuf);
            memset(usbInfoBuf, 0, USB_INFO_MAX_LEN);
            libusb_get_string_descriptor_ascii(devHandle, desc.iProduct, usbInfoBuf, USB_INFO_MAX_LEN);
            tmpInfos.append((const char *)usbInfoBuf);
            memset(usbInfoBuf, 0, USB_INFO_MAX_LEN);
            libusb_get_string_descriptor_ascii(devHandle, desc.iSerialNumber, usbInfoBuf, USB_INFO_MAX_LEN);
            tmpInfos.append((const char *)usbInfoBuf);
            usbDevExInfos.append(tmpInfos);

            if(!tmpInfos.at(1).isEmpty())
            {
                usbDevList->addItem(tmpInfos.at(1));
            }
            else
            {
                QString tmpStr = "Vid:" + QString::number(desc.idVendor, 16) + ", Pid:" + QString::number(desc.idProduct, 16);

                r = libusb_get_port_numbers(dev, path, sizeof(path));
                if (r > 0)
                {
                    tmpStr += ", Path:" + QString::number(path[0]);

                    for (j = 1; j < r; j++)
                        tmpStr += '.' + QString::number(path[j]);
                }

                usbDevList->addItem(tmpStr);
            }

            printf("%04x:%04x (bus %d, device %d)",
                   desc.idVendor, desc.idProduct,
                   libusb_get_bus_number(dev),
                   libusb_get_device_address(dev));

            r = libusb_get_port_numbers(dev, path, sizeof(path));
            if (r > 0)
            {
                printf(" path: %d", path[0]);
                for (j = 1; j < r; j++)
                    printf(".%d", path[j]);
            }
            printf("\n");

            libusb_config_descriptor *config;
            libusb_get_config_descriptor(dev, 0, &config);
            printf("Interfaces: %d\n",(int)config->bNumInterfaces);
            const libusb_interface *inter;
            const libusb_interface_descriptor *interdesc;
            const libusb_endpoint_descriptor *epdesc;

            for(int i=0; i<(int)config->bNumInterfaces; i++)
            {
                inter =&config->interface[i];
                printf("Number of alternate settings: %d\n",inter->num_altsetting);
                for(int j=0; j<inter->num_altsetting; j++){
                    interdesc =&inter->altsetting[j];
                    printf("Interface Number: %d\n",(int)interdesc->bInterfaceNumber);
                    printf("Number of endpoints: %d\n",(int)interdesc->bNumEndpoints);

                    for(int k=0; k<(int)interdesc->bNumEndpoints; k++){
                        epdesc =&interdesc->endpoint[k];
                        printf("Descriptor Type: %d\n",(int)epdesc->bDescriptorType);
                        printf("EP Address: %d\n",(int)epdesc->bEndpointAddress);
                    }
                }
            }
            printf("\n");
            libusb_free_config_descriptor(config);

            if(devHandle)
            {
                libusb_close(devHandle);
            }

        }
    }

    libusb_free_device_list(libusbDevs, 1);

    libusb_exit(NULL);

    return 0;
}

void ConfigUsbCan::applyBtnPressed()
{

}

void ConfigUsbCan::updateUsbInfo()
{
    auto tItem = new QTableWidgetItem[28];
    //0-13存第一列
    tItem[0].setText("bLength");
    tItem[1].setText("bDescriptorType");
    tItem[2].setText("bcdUSB");
    tItem[3].setText("bDeviceClass");
    tItem[4].setText("bDeviceSubClass");
    tItem[5].setText("bDeviceProtocol");
    tItem[6].setText("bMaxPacketSize0");
    tItem[7].setText("idVendor");
    tItem[8].setText("idProduct");
    tItem[9].setText("bcdDevice");
    tItem[10].setText("iManufacturer");
    tItem[11].setText("iProduct");
    tItem[12].setText("iSerialNumber");
    tItem[13].setText("bNumConfigurations");
    //14-27存第二列

    tItem[14].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bLength));
    tItem[15].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bDescriptorType));
    tItem[16].setText("0x" + QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bcdUSB, 16));
    tItem[17].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bDeviceClass));
    tItem[18].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bDeviceSubClass));
    tItem[19].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bDeviceProtocol));
    tItem[20].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bMaxPacketSize0));
    tItem[21].setText("0x" + QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).idVendor, 16));
    tItem[22].setText("0x" + QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).idProduct, 16));
    tItem[23].setText("0x" + QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bcdDevice, 16));



    tItem[24].setText(usbDevExInfos.at(usbDevList->currentIndex()).at(0));
    tItem[25].setText(usbDevExInfos.at(usbDevList->currentIndex()).at(1));
    tItem[26].setText(usbDevExInfos.at(usbDevList->currentIndex()).at(2));
    tItem[27].setText(QString::number(libusbDevsDescs.at(usbDevList->currentIndex()).bNumConfigurations));

    for(auto cnt = 0; cnt < 14; ++cnt)
    {
        usbDevDescTbl->setItem(cnt,0,&tItem[cnt]);
        usbDevDescTbl->setItem(cnt,1,&tItem[cnt + 14]);
        usbDevDescTbl->item(cnt,0)->setFlags(usbDevDescTbl->item(cnt,0)->flags() & ~Qt::ItemIsEditable);
        usbDevDescTbl->item(cnt,1)->setFlags(usbDevDescTbl->item(cnt,1)->flags() & ~Qt::ItemIsEditable);
    }
}






























