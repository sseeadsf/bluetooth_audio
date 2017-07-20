
#include "clienttalk.h"

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
    format.setSampleSize(8);
    format.setCodec("audio/amr");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));

    audio_output = new QAudioOutput(format, this);
    audio_input = new QAudioInput(format, this);
    audio_output->setBufferSize(1024);
    audio_input->setBufferSize(1024);
}


void ClientTalk::stopClient(){
    delete socket;
    socket = 0;
}


void ClientTalk::readSocket(){

    //QBuffer *buffer;
    QByteArray buff;


    audio_input->stop();

    connect(audio_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedOutput(QAudio::State)));


    while(socket->canReadLine()){
        buff.append(socket->readLine());
        QBuffer audio_buffer(&buff);
        audio_buffer.open(QIODevice::ReadWrite);
        //QDataStream s(buff, QIODevice::ReadWrite);
        audio_output->start(&audio_buffer);
        qDebug() << buff << endl;
    }
    buff.clear();

}


void ClientTalk::startTalk(){
    qDebug() << "Start talk";

    audio_output->stop();
    connect(audio_input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedInput(QAudio::State)));

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

