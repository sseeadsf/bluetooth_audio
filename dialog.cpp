#include "dialog.h"



Dialog::Dialog(){
    QVBoxLayout *main_layout = new QVBoxLayout;
    QGroupBox *box1 = new QGroupBox;
    QHBoxLayout *menu = new QHBoxLayout;

    btn_search = new QPushButton(tr("Search"));
    btn_detail = new QPushButton(tr("Detail"));
    btn_send = new QPushButton(tr("Send file"));
    line = new QLineEdit;
    line->setEnabled(false);
    agent = new QBluetoothDeviceDiscoveryAgent;
    local_device = new QBluetoothLocalDevice;
    device_list = new QVector<QBluetoothDeviceInfo>;

    btn_send->setDisabled(true);
    if(local_device->isValid()){
        local_device->powerOn();
        local_device->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    }

    table = new QTableWidget;
    table->setColumnCount(4);
    QStringList list;
    list << tr("Mac ID") << tr("Device Name") << tr("Paired") << tr("Authorized");
    table->setHorizontalHeaderLabels(list);
    table->setColumnWidth(0, 400);
    table->setColumnWidth(1,400);
    table->setColumnWidth(2, 226);
    table->setColumnWidth(3, 226);

    menu->addWidget(btn_search);
    menu->addWidget(btn_detail);
    menu->addStretch(1);
    box1->setLayout(menu);

    QGroupBox *box2 = new QGroupBox;
    QVBoxLayout *box3 = new QVBoxLayout;
    QGroupBox *send_block = new QGroupBox;
    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(line);
    layout->addWidget(btn_send);
    send_block->setLayout(layout);
    box3->addWidget(table);
    box3->addWidget(send_block);
    box2->setLayout(box3);

    QGroupBox *box4 = new QGroupBox;
    QHBoxLayout *bottom = new QHBoxLayout;
    btn_talk = new QPushButton(tr("Move to talking"));
    bottom->addWidget(btn_talk);
    box4->setLayout(bottom);


    main_layout->addWidget(box1);
    main_layout->addWidget(box2);
    main_layout->addWidget(box4);
    setLayout(main_layout);
    setWindowTitle(tr("Bluetooth"));


    //button action
    connect(btn_talk, SIGNAL(clicked()), this, SLOT(talk_on_clicked()));
    connect(btn_search, SIGNAL(clicked()), this, SLOT(search_on_clicked()));
    connect(btn_detail, SIGNAL(clicked()), this, SLOT(detail_on_clicked()));
    connect(btn_send, SIGNAL(clicked()), this, SLOT(send_on_clicked()));

    //bluetooth connect and discover action
    connect(agent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(show_info(QBluetoothDeviceInfo)));
    connect(agent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
    connect(agent, SIGNAL(canceled()), this, SLOT(discoveryFinished()));
    connect(local_device, SIGNAL(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)), this,
            SLOT(pairingFinished(QBluetoothAddress,QBluetoothLocalDevice::Pairing)));
    connect(local_device, SIGNAL(error(QBluetoothLocalDevice::Error)), this,
            SLOT(error(QBluetoothLocalDevice::Error)));
    connect(table, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(on_table_itemChanged(QTableWidgetItem*)));

}

void Dialog::discoveryFinished(){
    QMessageBox mess;
    mess.setText("Scan finished");
    mess.exec();
}

void Dialog::talk_on_clicked(){
    new_dialog = new DialogTalk();
    new_dialog->showMaximized();
}

void Dialog::search_on_clicked(){
    table->setRowCount(0);
    device_list->clear();
    agent->start();
}

void Dialog::show_info(const QBluetoothDeviceInfo &info){
    int row = table->rowCount();
    table->insertRow(row);
    item = new QTableWidgetItem(info.address().toString());
    table->setItem(row, 0, item);

    item = new QTableWidgetItem(info.name());

    table->setItem(row, 1, item);

    table->blockSignals(true);

    item = new QTableWidgetItem;
    QBluetoothLocalDevice::Pairing pair;
    pair = local_device->pairingStatus(info.address());

    if(pair&QBluetoothLocalDevice::Paired ||
            pair&QBluetoothLocalDevice::AuthorizedPaired){
        item->setCheckState(Qt::Checked);
        btn_send->setDisabled(false);
    } else{
        item->setCheckState(Qt::Unchecked);
    }
    table->setItem(row, 2, item);

    item = new QTableWidgetItem;
    if(pair&QBluetoothLocalDevice::AuthorizedPaired){
        item->setCheckState(Qt::Checked);
    } else{
        item->setCheckState(Qt::Unchecked);
    }

    table->setItem(row, 3, item);
    table->blockSignals(false);

    device_list->push_back(info);

}


void Dialog::detail_on_clicked(){

}

void Dialog::on_table_itemChanged(QTableWidgetItem *m_item)
{
    int column = m_item->column();
    row = m_item->row();

        if(m_item->checkState() == Qt::Checked && column == 2){
            //QString str = ui->table->item(row, 0)->text();
            local_device->requestPairing(device_list->at(row).address(), QBluetoothLocalDevice::Paired);
        }

        if(m_item->checkState() == Qt::Unchecked && column == 2){
            local_device->requestPairing(device_list->at(row).address(), QBluetoothLocalDevice::Unpaired);
        }

        if(m_item->checkState() == Qt::Checked && column == 3){
            local_device->requestPairing(device_list->at(row).address(), QBluetoothLocalDevice::AuthorizedPaired);
        }

        if(m_item->checkState() == Qt::Unchecked && column == 3){
            local_device->requestPairing(device_list->at(row).address(), QBluetoothLocalDevice::Unpaired);
        }

}

void Dialog::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pair){
    Q_UNUSED(address);

    QMessageBox *mess = new QMessageBox;

    if(c_error){
        mess->setText("Something went wrong");
        mess->setDefaultButton(QMessageBox::Ok);
        local_device->requestPairing(address, QBluetoothLocalDevice::Unpaired);
    }
    else{
        if(pair&QBluetoothLocalDevice::Paired
                || pair&QBluetoothLocalDevice::AuthorizedPaired){
            mess->setText("Pair success");
            btn_send->setDisabled(false);
        } else{
            mess->setText("Release");
            mess->setDefaultButton(QMessageBox::Ok);
        }
    }
    mess->exec();
}

void Dialog::error(QBluetoothLocalDevice::Error error){
    if (error != QBluetoothLocalDevice::PairingError)
        return;

    c_error = true;
    //pairingFinished(device_list->at(row).address(), QBluetoothLocalDevice::Unpaired);
}


void Dialog::send_on_clicked(){
    if(line->text() == ""){
        line->setText(QFileDialog::getOpenFileName());
    } else{
        QBluetoothTransferManager *transfer = new QBluetoothTransferManager(this);

        QBluetoothTransferRequest request(device_list->at(row).address());
        QFile *file = new QFile(line->text());

        QBluetoothTransferReply *reply = transfer->put(request, file);

        if(reply->error() == QBluetoothTransferReply::NoError){
            connect(reply, SIGNAL(finished(QBluetoothTransferReply*)), this,
                    SLOT(transfer_finish(QBluetoothTransferReply*)));
            connect(reply, SIGNAL(error(QBluetoothTransferReply::TransferError)), this,
                    SLOT(transfer_error(QBluetoothTransferReply::TransferError)));
        }
    }

}

void Dialog::transfer_finish(QBluetoothTransferReply *reply){
    Q_UNUSED(reply);

    if(!m_error){
        QMessageBox mess;
        mess.setText(tr("Send success"));
        mess.exec();
        line->setText(tr(""));
    }
}

void Dialog::transfer_error(QBluetoothTransferReply::TransferError error){
    Q_UNUSED(error);
    QMessageBox mess;
    mess.setText(tr("Something went wrong"));
    mess.exec();
    m_error = true;
}



