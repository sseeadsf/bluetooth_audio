
#include "servertalk.h"

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");


ServerTalk::ServerTalk(QObject *parent): QObject(parent), rfcomm_server(0){

}

ServerTalk::~ServerTalk(){
    stopServer();
}

void ServerTalk::startServer(const QBluetoothAddress &local){
    if(rfcomm_server)
        return;
    rfcomm_server = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);

    connect(rfcomm_server, SIGNAL(newConnection()), this, SLOT(clientConnect()));
    bool result = rfcomm_server->listen(local);

    if(!result){
        QMessageBox box;
        box.setText("Can't create server to "+local.toString());
        box.exec();
        return;
    }

    QBluetoothServiceInfo::Sequence class_id;

    class_id << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));

    service_info.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, class_id);
    class_id.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceClassIds, class_id);
    service_info.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Talking"));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceDescription, tr("Example"));
    service_info.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt.io"));
    service_info.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence public_browse;
    public_browse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    service_info.setAttribute(QBluetoothServiceInfo::BrowseGroupList, public_browse);
    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(rfcomm_server->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    service_info.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,protocolDescriptorList);
    service_info.registerService(local);


    format.setChannelCount(1);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleRate(8000);
    format.setSampleSize(16);

    audio_output = new QAudioOutput(format, this);
    //audio_output->setBufferSize(8192);
    audio_input = new QAudioInput(format, this);
    //audio_input->setBufferSize(8192);

}

void ServerTalk::stopServer(){
    // Unregister service
    service_info.unregisterService();

    // Close sockets
    qDeleteAll(client_socket);

    // Close server
    delete rfcomm_server;
    rfcomm_server = 0;
}

void ServerTalk::startTalk(){

    qDebug() << "Start talk";
    audio_output->stop();

    foreach (QBluetoothSocket *socket, client_socket){

        audio_input->start(socket);
        qDebug() << "Talking to client";
        connect(audio_input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedInput(QAudio::State)));
    }
}

void ServerTalk::readSocket(){

    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    //QBuffer *buffer;

    audio_input->stop();
    QByteArray buff;

    connect(audio_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedOutput(QAudio::State)));

    while(socket->canReadLine()){
        buff.append(socket->readLine());
        QBuffer audio_buffer(&buff);
        audio_buffer.open(QIODevice::WriteOnly);
        //QDataStream s(buff, QIODevice::ReadWrite);
        audio_output->start(&audio_buffer);
        qDebug() << buff << endl;
    }
    buff.clear();
}

void ServerTalk::replay(){

}

void ServerTalk::handleStateChangedInput(const QAudio::State &state){
    switch(state){
        case QAudio::StoppedState:
            if(audio_input->error() != QAudio::NoError){
                QMessageBox mess;
                mess.setText("Something went wrong");
                mess.exec();
            }
            break;
        default:
            break;
    }
}

void ServerTalk::handleStateChangedOutput(const QAudio::State &state){
    switch(state){
        case QAudio::StoppedState:
            if(audio_output->error() != QAudio::NoError){
                QMessageBox mess;
                mess.setText("Something went wrong");
                mess.exec();
            }
            break;
        default:
            break;
    }
}

void ServerTalk::clientConnect(){
    QBluetoothSocket *socket = rfcomm_server->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    client_socket.append(socket);
    emit clientConnect(socket->peerName());
}

void ServerTalk::clientDisconnected(){
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    emit clientDisconnect(socket->peerName());

    client_socket.removeOne(socket);

    socket->deleteLater();
}
