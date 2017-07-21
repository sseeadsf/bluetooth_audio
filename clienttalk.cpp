
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

    format.setChannelCount(1);
    format.setCodec("audio/amr");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleRate(8000);
    format.setSampleSize(16);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));



    //audio_output->setBufferSize(8192);
    //audio_input->setBufferSize(8192);
}


void ClientTalk::stopClient(){
    delete socket;
    socket = 0;
}


void ClientTalk::readSocket(){
    audio_output = new QAudioOutput(format, this);

    //QBuffer *buffer;

    QByteArray buff;

    connect(audio_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedOutput(QAudio::State)));

    while(socket->canReadLine()){
        buff.append(socket->readLine());
        QBuffer audio_buffer(&buff);
        audio_buffer.open(QIODevice::ReadOnly);
        //QDataStream s(buff, QIODevice::ReadWrite);
        audio_output->start(&audio_buffer);
        //qDebug() << buff << endl;
    }
    buff.clear();
}


void ClientTalk::startTalk(){
    qDebug() << "Start talk";

    //audio_output->stop();
    audio_input = new QAudioInput(format, this);
    connect(audio_input, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChangedInput(QAudio::State)));

    audio_input->start(socket);

    qDebug() << "Talking to server";
}
/*
void ClientTalk::startTalk(){
    audio_input = new QAudioInput(format, this);

    QFile file;

    file.setFileName("/tmp/air.mp3");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTimer::singleShot(3000, audio_input, SLOT(stopRecording()));
    audio_input->start(&file);
}*/


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

