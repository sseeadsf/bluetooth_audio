#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QGroupBox>
#include <QStringList>
#include <QVector>
#include <QMessageBox>
#include <QFileDialog>
#include <QtBluetooth/QBluetoothTransferManager>
#include <QtBluetooth/QBluetoothTransferRequest>
#include <QFile>
#include <QBluetoothTransferReply>
#include <qbluetoothserver.h>
#include <qbluetoothsocket.h>
#include <qbluetoothlocaldevice.h>

#include "dialogtalk.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();
private slots:
    void search_on_clicked();
    void send_on_clicked();
    void detail_on_clicked();
    void talk_on_clicked();
    void pairingFinished(const QBluetoothAddress&, QBluetoothLocalDevice::Pairing pair);
    void error(QBluetoothLocalDevice::Error error);
    void show_info(const QBluetoothDeviceInfo&);
    void on_table_itemChanged(QTableWidgetItem *m_item);
    void transfer_finish(QBluetoothTransferReply *reply);
    void transfer_error(QBluetoothTransferReply::TransferError error);
    void discoveryFinished();

private:
    QPushButton *btn_search;
    QPushButton *btn_detail;
    QPushButton *btn_send;
    QTableWidget *table;
    QTableWidgetItem *item;
    QLineEdit *line;
    QPushButton *btn_talk;
    DialogTalk *new_dialog;



    QBluetoothDeviceDiscoveryAgent *agent;
    QBluetoothLocalDevice *local_device;
    bool m_error = false;
    bool c_error = false;
    QVector<QBluetoothDeviceInfo> *device_list;
    QBluetoothDeviceInfo select_device;
    QFile *m_file;
    int row;



};

#endif // DIALOG_H
