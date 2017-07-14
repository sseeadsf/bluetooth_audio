#ifndef DIALOGTALK_H
#define DIALOGTALK_H

#include <QDialog>
#include <QPushButton>
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
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>

#include "clienttalk.h"
#include "servertalk.h"


namespace Ui {
class Dialog;
}

class DialogTalk : public QDialog
{
    Q_OBJECT

public:
    DialogTalk();

private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);
    void discoveryFinished();
    void search_on_clicked();
    void service_select(QTableWidgetItem *item_selected);
    void clientConnected(const QString &name);
    void clientDisconnected(const QString &name);
    void connected(const QString &name);

private:

    ServerTalk *server;
    QList<ClientTalk *> clients;
    QVector<QBluetoothServiceInfo> *device_list;
    QPushButton *btn_search;
    QBluetoothServiceDiscoveryAgent *agent;
    QBluetoothServiceInfo *service;
    QTableWidget *table;
    QTableWidgetItem *item;
    QPushButton *btn_talk;
    int row;
    QBluetoothLocalDevice *local;


};

#endif // DIALOGTALK_H
