#ifndef VOICE_H
#define VOICE_H

#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QMediaPlayer>
#include <QSound>
#include <QtCore>
#include <stdio.h>
#include <stdlib.h>

#include "whole.h"

/**
 * @brief
 * 利用一个全局变量 voiceNeedPlay 等待播放的语音队列，解包函数往队列里放语音索引，voice类从队列里取语音索引并播放
 *
 * 生产者消费者模式：voice类-消费者，解包-生产者
 */



class Voice
{

public:
    explicit Voice();
    ~Voice();
    void play();

    QSound *bells;

 //bo fang yu yin xian cheng
    static void playVoice()
    {
        QString runPath;  //程序当前运行路径
        QString absDir;  //实际需打开的路径
        QString index("");
        static int curIndexVoice = 0;
        runPath = QCoreApplication::applicationDirPath();//获取当前exe所在路径
        QMediaPlayer voiceSound;

        if(curIndexVoice >= BufferSize){
            curIndexVoice = curIndexVoice % BufferSize;
        }
        curIndexVoice = 2;
        int needPlay = voiceNeedPlay[curIndexVoice] - 1;
        index.sprintf("/%d.wav", needPlay);
        absDir = runPath + index;
        voiceSound.setMedia(QUrl::fromLocalFile(absDir));
        voiceSound.play();
        curIndexVoice++;

        while(voiceSound.state() == QMediaPlayer::PlayingState)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }

    }
};


typedef struct
{
    int id;
    QString leftName;
    QString rightName;
    QString voiceContent;
    int leftSignalId;
    int rightSignalId;
}Voice_Info;

const int VOICE_NUM = 160;
const int DAOKOU_NUM = 23;

typedef struct
{
    int id;
    int sigEquip;
    int leftPoint;
    int rightPoint;
}DAOKOU;



#endif // VOICE_H

