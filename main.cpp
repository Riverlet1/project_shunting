#include <QCoreApplication>

#include <QtNetwork>

#include "main.h"

shuntingData rec_shuntingData[50];
QByteArray wholebuff;  //全局缓冲区，udp收到数据后首先拷贝至此
QUdpSocket *recvUdp;  //receive datagram : print , voice

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //改变缺省消息处理程序的输出
    qSetMessagePattern("Message:%{message}  File:%{file}  Line:%{line}  Function:%{function} ");




    recvUdp = new QUdpSocket();
    bool result = recvUdp->bind(QHostAddress::LocalHost, 11001);
    qDebug() << "#### udp recv socket bind result:"<<result<<" #####";

    QObject::connect(recvUdp, &QUdpSocket::readyRead, &a, &recvData);

    Voice voiceinstance;
    voiceinstance.start();
    //voiceinstance.wait();

    return a.exec();
}



