#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include<QDebug>

#include "printercontrol.h"
#include "voice.h"
#include "unpack.h"

extern QByteArray wholebuff;  //全局缓冲区，udp收到数据后首先拷贝至此

extern QUdpSocket *recvUdp;  //receive datagram : print , voice

#define DONOTHING 0
#define PRINT 2
#define PLAY_VOICE 3
int needTodo;

extern shuntingData rec_shuntingData[50];


void processTheDatagram()
{

    UnPack(reinterpret_cast<unsigned char *>(wholebuff.data()));
    print_Printer();
    needTodo = DONOTHING;
}

void recvData()
{
    while(recvUdp->hasPendingDatagrams())
    {
        QHostAddress srcAdr;        

        wholebuff.resize(recvUdp->pendingDatagramSize());
        recvUdp->readDatagram(wholebuff.data(), wholebuff.size(), &srcAdr);

        QString msg = wholebuff.data();
        qDebug()<<msg<<" ## "<<srcAdr;
        processTheDatagram();
    }
}



#endif // MAIN_H
