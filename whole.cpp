#include "whole.h"


shuntingData rec_shuntingData[50];
QByteArray wholebuff;  //全局缓冲区，udp收到数据后首先拷贝至此
QUdpSocket *recvUdp;  //receive datagram : print , voice
int needTodo;

int voiceNeedPlay[BufferSize];
QSemaphore FreeSlots(BufferSize);
QSemaphore WaitingVoiceNum;
