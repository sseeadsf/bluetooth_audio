#ifndef SERVERTALK_H
#define SERVERTALK_H

#include <QBluetoothServiceInfo>
#include <QBluetoothAddress>
#include <QBluetoothServer>
#include <QAudioBuffer>

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QMessageBox>
#include <QBluetoothSocket>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QDebug>
#include <QBuffer>
#include <QDataStream>

QT_FORWARD_DECLARE_CLASS(QBluetoothServer)
QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class ServerTalk: public QObject{
    Q_OBJECT

public:
    explicit ServerTalk(QObject *parent = 0);
    ~ServerTalk();

    void startServer(const QBluetoothAddress &local = QBluetoothAddress());

    void stopServer();

    QAudioInput *audio_input;
    QAudioOutput *audio_output;
    QAudioFormat format;

public slots:
    void startTalk();
signals:
    void voiceReceive();
    void clientConnect(const QString&);
    void clientDisconnect(const QString&);
    void messageReceived(const QString&, const QString&);


private slots:
    void clientConnect();
    void clientDisconnected();
    void readSocket();
    void handleStateChangedInput(const QAudio::State&);
    void handleStateChangedOutput(const QAudio::State&);
    void replay();

private:
    QBluetoothServer *rfcomm_server;
    QBluetoothServiceInfo service_info;
    QList<QBluetoothSocket *> client_socket;
    //QByteArray *buff;



};

#endif // SERVER_H
