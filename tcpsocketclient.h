#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QTcpServer>

class TcpSocketClient : public QObject
{     
public:
    TcpSocketClient();
    ~TcpSocketClient();

    QTcpSocket *client;

    void sendData(const char *str, int size = -1);
    void clientRecvData();

private:


private slots:


};

#endif // TCPSOCKET_H
