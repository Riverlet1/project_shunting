#ifndef UNPACK_H
#define UNPACK_H

#include "whole.h"
#include "printercontrol.h"
#include "voice.h"

#define MIN_UNIT_LEN 8  //最小单元指1勾，1语音，1表头，1文本
#define BIAO_TOU 0

extern shuntingData rec_shuntingData[50];

/**
 * @brief UnPackShuntingTable 可以解表头，文本，语音，勾计划
 * @param package 最小解析单元大小为8bytes
 */
void UnPackShuntingTable(unsigned char *package)
{
    static int curVoiceQueueIndex;

    if(package[0] == 0xBF)  //表头
    {
        memset(&rec_shuntingData[BIAO_TOU].plan_num, 0, sizeof(rec_shuntingData[BIAO_TOU].plan_num));
        memcpy(&rec_shuntingData[BIAO_TOU].plan_num, &package[1], 3);

        /**
         * @to do  避免多次重复按按钮，打印好多重复的单子
         */

        memset(&rec_shuntingData[BIAO_TOU].engine_num, 0, sizeof(rec_shuntingData[BIAO_TOU].engine_num));
        memcpy(&rec_shuntingData[BIAO_TOU].engine_num, &package[4], 1);
        memcpy(&rec_shuntingData[BIAO_TOU].diaoduyuan_num, &package[4], 1);

        memset(&rec_shuntingData[BIAO_TOU].group_num, 0 ,sizeof(rec_shuntingData[BIAO_TOU].group_num));
        memcpy(&rec_shuntingData[BIAO_TOU].group_num, &package[5], 1);

        memset(&rec_shuntingData[BIAO_TOU].total_hook_num, 0 ,sizeof(rec_shuntingData[BIAO_TOU].total_hook_num));
        memcpy(&rec_shuntingData[BIAO_TOU].total_hook_num, &package[6], 1);


        memset(&rec_shuntingData[BIAO_TOU].attention_num, 0, sizeof(rec_shuntingData[BIAO_TOU].attention_num));
        memcpy(&rec_shuntingData[BIAO_TOU].attention_num, &package[7], 1);
    }
    else if(package[0] == 0xC7) // 文本
    {
        //unicode attention2
        /*
        memcpy(wstr,&package[2],6);
        setlocale(0, "");
        indexofatten = (package[1]&0x0f)*6;
        wcstombs(attention2+indexofatten,wstr,6);
        */
        wchar_t wstr[8];
        memcpy(wstr, &package[2], 6);
        int sequence = (package[1] & 0x0f) * 6;
        wcstombs(attention2 + sequence, wstr, 6);
    }
    else if(package[0] == 0xC3)  //语音
    {
        if(package[1] == 0xf0 && package[2] == 0x01 && package[3] == 0x06 && package[4] == 0x04)
        {
            if(package[5] >= 0x01 && package[5] <= 160)
            {
                curVoiceQueueIndex = 2;
                if(curVoiceQueueIndex >= BufferSize)
                    curVoiceQueueIndex = curVoiceQueueIndex % BufferSize;
                voiceNeedPlay[curVoiceQueueIndex] = package[5];
                curVoiceQueueIndex++;
            }
        }
    }
    else if(package[0] != 0xBF) //勾计划内容
    {
        int num = package[1];

        memset(&rec_shuntingData[num], 0, sizeof(rec_shuntingData[num]));
        memcpy(&rec_shuntingData[num].jishi, &package[3], 1);

        memcpy(&rec_shuntingData[num].dir, &package[0], 1);

        memcpy(&rec_shuntingData[num].hook_num, &package[1], 1);
        //memset(rec_shuntingData[num].track_num,0,sizeof(rec_shuntingData[num].track_num));
        memcpy(&rec_shuntingData[num].track_num, &package[2], 1);

        memcpy(&rec_shuntingData[num].dir_method, &package[3], 1);

        memcpy(&rec_shuntingData[num].vehicle_num, &package[4], 1);

        memcpy(&rec_shuntingData[num].vehicle_coding, &package[5], 3);
    }

}

/**
 * @brief UnPack 分为语音和调车单分别解析，解析单元为8bytes
 * @param rawBuff 未处理的数据，512Bytes
 */
void UnPack(unsigned char *rawBuff)
{
    int gouNum = rawBuff[6] - 8;
    int biaoTouIndex = 6+8+8+1;
    int attentionIndex = 6+8+8+8+1;
    int gouIndex = 6+64+1;
    unsigned char packageUnit[MIN_UNIT_LEN];

    int voice_index = 7;

    if(rawBuff[0] == 0xd1)//yu yin
    {
       needTodo = PLAY_VOICE;
       memset(packageUnit, 0, MIN_UNIT_LEN);
       memcpy(packageUnit, &rawBuff[voice_index], MIN_UNIT_LEN);
       UnPackShuntingTable(packageUnit);
    }
    else
    {
        needTodo = PRINT;
        //biao tou
        memset(packageUnit, 0, MIN_UNIT_LEN);
        memcpy(packageUnit, &rawBuff[biaoTouIndex], MIN_UNIT_LEN);
        UnPackShuntingTable(packageUnit);
        // zhu yi shi xiang
        for(int i = 0; i < 5; i++)
        {
            memset(packageUnit, 0, MIN_UNIT_LEN);
            memcpy(packageUnit, &rawBuff[attentionIndex + i*MIN_UNIT_LEN], MIN_UNIT_LEN);
            UnPackShuntingTable(packageUnit);
        }
        // gou
        for(int i = 0; i < gouNum; i++)
        {
            memset(packageUnit, 0, MIN_UNIT_LEN);
            memcpy(packageUnit, &rawBuff[gouIndex + MIN_UNIT_LEN*i], MIN_UNIT_LEN);
            UnPackShuntingTable(packageUnit);
        }
    }

}



#endif // UNPACK_H
