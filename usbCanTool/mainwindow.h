#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ConfigUsbCan;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

private:
    ConfigUsbCan *chooseUsbDevDiag;

private slots:
    void on_m_btnUsbDevConfig_clicked();
};

#endif // MAINWINDOW_H
