#ifndef CLIENTTALK_H
#define CLIENTTALK_H

#include <qbluetoothserviceinfo.h>

#include <QtCore/QObject>
#include <QAudioFormat>
#include <qaudioinput.h>
#include <qaudiooutput.h>
#include <qmessagebox.h>
#include <qbluetoothsocket.h>
#include <QDebug>
#include <QBuffer>


QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

//! [declaration]
class ClientTalk : public QObject
{
    Q_OBJECT

public:
    explicit ClientTalk(QObject *parent = 0);
    ~ClientTalk();

    void startClient(const QBluetoothServiceInfo &remoteService);
    void stopClient();




public slots:
    void startTalk();

signals:
    void voiceReceive();
    void connected(const QString &name);
    void disconnected();

private slots:
    void readSocket();
    void connected();
    void handleStateChangedInput(const QAudio::State&);
    void handleStateChangedOutput(const QAudio::State&);

private:
    QBluetoothSocket *socket;
    QByteArray *buff;
    QAudioFormat format;
    QAudioInput *audio_input;
    QAudioOutput *audio_output;

};

#endif // CLIENTTALK_H
