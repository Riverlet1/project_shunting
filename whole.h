#ifndef WHOLE_H
#define WHOLE_H

#include <QByteArray>
#include <QUdpSocket>
#include <QSemaphore>

#include "printercontrol.h"

extern QByteArray wholebuff;  //全局缓冲区，udp收到数据后首先拷贝至此
extern QUdpSocket *recvUdp;  //receive datagram : print , voice

#define DONOTHING 0
#define PRINT 2
#define PLAY_VOICE 3

extern int needTodo;

extern shuntingData rec_shuntingData[50];


//生产者-消费者 全局定义
const int BufferSize = 1024;
extern int voiceNeedPlay[BufferSize];
extern QSemaphore FreeSlots;
extern QSemaphore WaitingVoiceNum;

#endif // WHOLE_H
