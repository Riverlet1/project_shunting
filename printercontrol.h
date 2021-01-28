#ifndef PRINTERCONTROL_H
#define PRINTERCONTROL_H

#include "tcpsocketclient.h"

void print_Printer(void);
void print_init( char *buff);
int print_cn_title( char *date, int length, char *buff);
int print_cn( char *date, int length,  char *buff);
int print_cn_end( char *date, int length,  char *buff);

/******************** 定义调车单数据长度***************************/
/******************** 以下长度是实际接收的长度*******************/
#define plan_num_Length 3
#define engine_num_Length 1       //机车号，火车头
#define group_num_Length 1
#define total_hook_num_Length 1
#define attention_Length 1

#define jishi_Length 1
#define hook_num_Length 1
#define track_num_Length 1
#define dir_method_Length 1
#define vehicle_num_Length 1      //辆数
#define vehicle_coding_Length 3   //车辆编码

/******************** 定义调车单数据***************************/
typedef struct shuntingData
{
    unsigned char plan_num[plan_num_Length];		//调车单号
    unsigned char engine_num[engine_num_Length];     //机车号
    unsigned char group_num[group_num_Length]; //班组号索引值
    unsigned char total_hook_num[total_hook_num_Length];		//总勾数
    unsigned char attention_num[attention_Length]; //注意事项索引值
    unsigned char diaoduyuan_num[1]; //调度员索引值
    char makeTime[16];
    char startTime[16];

    unsigned char jishi[jishi_Length];         //记事索引值
    unsigned char hook_num[hook_num_Length];		//勾序号
    unsigned char track_num[track_num_Length];		      //股道号索引值
    unsigned char dir_method[dir_method_Length];   //方向和作业方法，都是索引值
    unsigned char dir[dir_method_Length];   //方向和作业方法，都是索引值
    unsigned char vehicle_num[vehicle_num_Length];       //车数
    unsigned char vehicle_coding[vehicle_coding_Length];       //车辆编码

} shuntingData;

#endif // PRINTERCONTROL_H
