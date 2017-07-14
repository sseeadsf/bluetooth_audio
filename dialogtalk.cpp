#include "dialogtalk.h"

static const QLatin1String uuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

DialogTalk::DialogTalk(){

    //khoi tao doi tuong
    QVBoxLayout *main_layout = new QVBoxLayout;
    btn_search = new QPushButton(tr("Search"));
    table = new QTableWidget;
    table->setColumnCount(3);
    QStringList list;
    list << tr("Device Name") << tr("Uuid") << tr("Connect");
    table->setHorizontalHeaderLabels(list);
    table->setColumnWidth(0, 500);
    table->setColumnWidth(1,700);
    table->setColumnWidth(2, 100);
    btn_talk = new QPushButton(tr("Talk"));
    device_list = new QVector<QBluetoothServiceInfo>();
    local = new QBluetoothLocalDevice;
    agent = new QBluetoothServiceDiscoveryAgent(this);

    //add widget to main layout
    main_layout->addWidget(btn_search);
    main_layout->addWidget(table);
    main_layout->addWidget(btn_talk);
    setLayout(main_layout);

    server = new ServerTalk(this);
    connect(server, SIGNAL(clientConnect(QString)), this, SLOT(clientConnected(QString)));
    connect(server, SIGNAL(clientDisconnect(QString)), this, SLOT(clientDisconnected(QString)));
    connect(btn_talk, SIGNAL(clicked()), server, SLOT(startTalk()));
    //connect(btn_talk, SIGNAL(released()), server, SLOT(readSocket()));
    server->startServer();


    //bluetooth and button action
    connect(agent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)), this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect(agent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
    connect(agent, SIGNAL(canceled()), this, SLOT(discoveryFinished()));
    connect(btn_search, SIGNAL(clicked()), this, SLOT(search_on_clicked()));
    connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), SLOT(service_select(QTableWidgetItem*)));
}


void DialogTalk::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo){
    int row = table->rowCount();
    table->insertRow(row);

    item = new QTableWidgetItem(serviceInfo.device().name());
    table->setItem(row, 0, item);
    item = new QTableWidgetItem(serviceInfo.serviceUuid().toString());
    table->setItem(row, 1, item);
    table->blockSignals(true);

    item = new QTableWidgetItem;
    item->setCheckState(Qt::Unchecked);
    table->setItem(row, 2, item);
    table->blockSignals(false);

    device_list->push_back(serviceInfo);
}


void DialogTalk::search_on_clicked(){
    table->setRowCount(0);
    agent->setUuidFilter(QBluetoothUuid(uuid));
    agent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

void DialogTalk::discoveryFinished(){
    QMessageBox mess;
    mess.setText("Scan finish");
    mess.exec();
}


void DialogTalk::service_select(QTableWidgetItem *item_selected){
    int column = item_selected->column();
    row = item_selected->row();

    if(item_selected->checkState() == Qt::Checked && column == 2){
        btn_talk->disconnect();
        server->disconnect();

        ClientTalk *client = new ClientTalk(this);
        connect(client, SIGNAL(connected(QString)), this, SLOT(connected(QString)));
        connect(btn_talk, SIGNAL(clicked()), client, SLOT(startTalk()));
        client->startClient(device_list->at(row));
        clients.append(client);
    }
    /*
    if(item_selected->checkState() == Qt::Unchecked && column == 2){
        ClientTalk *client = qobject_cast<ClientTalk *>(sender());
        if (client) {
            clients.removeOne(client);
            client->deleteLater();
        }
    }*/
}


void DialogTalk::clientConnected(const QString &names){
    QMessageBox mess;
    mess.setText(names + " connect to server");
    mess.exec();
}

void DialogTalk::clientDisconnected(const QString &name){
    QMessageBox mess;
    mess.setText(name+" disconnect to server");
    mess.exec();
}

void DialogTalk::connected(const QString &name){
    QMessageBox mess;
    mess.setText("join server "+name);
    mess.exec();
}



