#ifndef VOICE_H
#define VOICE_H

#include <QCoreApplication>
#include <QString>
#include <QSound>
#include <QDebug>

#include <QtCore>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief
 * 利用一个全局变量 voiceNeedPlay 等待播放的语音队列，解包函数往队列里放语音索引，voice类从队列里取语音索引并播放
 *
 * 生产者消费者模式：voice类-消费者，解包-生产者
 */


//生产者-消费者 全局定义
const int BufferSize = 1024;
extern int voiceNeedPlay[BufferSize];
extern QSemaphore FreeBytes;
extern QSemaphore UsedBytes;

class Voice : public QThread
{
    Q_OBJECT

public:
    explicit Voice();
    ~Voice();
    void play();

    QString runPath;  //程序当前运行路径
    QString absDir;  //实际需打开的路径
    QSound *bells;

 //bo fang yu yin xian cheng
    void run() override
    {
        int i = 0;
        while(1)
        {
            if(i >= BufferSize){
                i = i % BufferSize;
            }
            UsedBytes.acquire();
            runPath = QCoreApplication::applicationDirPath();//获取当前exe所在路径
            absDir = runPath + "/XXX.wav";
            int needPlay = 3; //
            absDir.replace(QString("XXX"), QString(needPlay));
            QSound::play(absDir);

            FreeBytes.release();
            i++;
            QThread::sleep(2);
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

