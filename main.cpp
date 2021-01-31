#include <QCoreApplication>

#include <QtNetwork>

#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //改变缺省消息处理程序的输出
    //qSetMessagePattern("Message:%{message}  File:%{file}  Line:%{line}  Function:%{function} ");

    recvUdp = new QUdpSocket();
    bool result = recvUdp->bind(QHostAddress::LocalHost, 11001);
    qDebug() << "#### udp recv socket bind result:"<<result <<" ;current port:11001; #####";
    QObject::connect(recvUdp, &QUdpSocket::readyRead, &a, &recvData);

    //避免多次快速点击, 避免多次发送相同语音
    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, &a, &mainLoop);
    timer->start(1000);

    return a.exec();
}

void SleepNoBlock(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


