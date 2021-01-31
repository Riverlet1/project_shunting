#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include<QDebug>

#include "printercontrol.h"
#include "voice.h"
#include "unpack.h"
#include "whole.h"

extern void SleepNoBlock(int msec);

void mainLoop()
{
    if(needTodo == PRINT)
    {
        qDebug() << "im printing...";
        print_Printer();
    }
    else if(needTodo == PLAY_VOICE)
    {
        Voice::playVoice();
    }

    //SleepNoBlock(1000);
    needTodo = DONOTHING;
}

void processTheDatagram()
{
    if(wholebuff.size() >= 512)
    {
        UnPack(reinterpret_cast<unsigned char *>(wholebuff.data()));
    }   
}

void recvData()
{
    while(recvUdp->hasPendingDatagrams())
    {
        QHostAddress srcAdr;

        wholebuff.resize(recvUdp->pendingDatagramSize());
        recvUdp->readDatagram(wholebuff.data(), wholebuff.size(), &srcAdr);

        QString msg(wholebuff.data());
        qDebug()<<msg<<" ## "<<srcAdr;
        processTheDatagram();
    }
}


#endif // MAIN_H
