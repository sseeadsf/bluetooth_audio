
#include "clienttalk.h"

#include <qbluetoothsocket.h>

ClientTalk::ClientTalk(QObject *parent)
:   QObject(parent), socket(0)
{
}


ClientTalk::~ClientTalk(){
    stopClient();
}


void ClientTalk::startClient(const QBluetoothServiceInfo &remoteService){
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    socket->connectToService(remoteService);

    format.setChannelCount(2);
    format.setSampleRate(48000);
    format.setSampleSize(16);
    format.setCodec("audio/amr");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));


}


void ClientTalk::stopClient(){
    delete socket;
    socket = 0;
}


void ClientTalk::readSocket(){

    QBuffer *buffer;
    audio_output = new QAudioOutput(format, this);
    connect(audio_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedOutput(QAudio::State)));
    buff = new QByteArray;
    while(socket->canReadLine()){
        buff->append(socket->readLine());
        buffer = new QBuffer(buff);
        buffer->open(QIODevice::ReadOnly);
        //QDataStream s(buff, QIODevice::ReadOnly);
        audio_output->start(buffer);
    }

}


void ClientTalk::startTalk(){
    qDebug() << "Start talk";

    audio_input = new QAudioInput(format, this);
    connect(audio_input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedInput(QAudio::State)));
    audio_input->setBufferSize(8192);
    audio_input->start(socket);

    qDebug() << "Talking to server";
}


void ClientTalk::connected(){
    emit connected(socket->peerName());
}

void ClientTalk::handleStateChangedOutput(const QAudio::State &state){
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

void ClientTalk::handleStateChangedInput(const QAudio::State &state){
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

