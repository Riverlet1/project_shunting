#include "tcpsocketclient.h"

TcpSocketClient::TcpSocketClient()
{
    client = new QTcpSocket();
    //当socket接收缓冲区有新数据到来时，会发出readRead()信号，因此为该信号添加槽函数以读取数据
    connect(client, &QTcpSocket::readyRead, this, &TcpSocketClient::clientRecvData);

    QString IP = "localhost";
    int port = 9100;
    client->connectToHost(IP, port);
}

TcpSocketClient::~TcpSocketClient()
{
    client->disconnectFromHost();
    delete client;
}

void TcpSocketClient::sendData(const char *str, int size)
{
    QByteArray data(str, size);
    client->write(data);
    //client->disconnectFromHost();
}


void TcpSocketClient::clientRecvData()
{
    QByteArray buffer;
     //读取缓冲区数据
    buffer = client->readAll();
}
