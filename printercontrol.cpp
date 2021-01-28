#include "printercontrol.h"

//发送缓冲区
char sendBuff[1024];
char recvBuff[1024];
int sendBytes;

extern shuntingData rec_shuntingData[50];
unsigned char shunting_rec_flag = 0;
unsigned char isNeedPrint= 0 ;
//调车单固定的格式

 char title[22]= "调车作业计划单" ;
//unsigned char   title[]="调车作业计划单";
 char second_plan[]="单号:";
 char second_engine[]=" 机车:";
 char second_group[]=" 班组:";
 char second_diaoduyuan[]=" 编制:";
 char second_plan_v[7];
 char second_engine_v[4];
 char second_group_v[5];
 char second_totalhook_v[3];
 char second_diaoduyuan_v[6];
 char second[100];

 char header[]="钩序 股道 方向 甩挂 辆数  车号   记事";
 char record_hook[3];
 char record_track[6];
 char record_dir[3];
 char record_method[3];
 char record_vehnum[3];
 char record_vehcod[8];
 char record_jishi[11];
 char record[100];

 char attention1[36];
 char attention1_att[] = "常规注意事项：";
 char attention1_attall[50];

 char attention2[36];
 char attention2_att[] = "专门注意事项：";
 char attention2_attall[50];

 char last_makeTime[17]="编制时间:";
 char last_startTime[27]="开始时间:";
 char last_endTime[14]="结束时间:";
//unsigned char last_endTime[]="结束时间:";
 char Local_time[18] = "00-00-00-00-00-00";
 int Temp_Hour;
 int Temp_Day;
 char tempTime[2];
 char space[5]="   ";
int record_num=0;
 char haveAnotherPlan= 0 ;
 char old_plan_num[2] = "0";

void print_Printer(void)
{
    int i = 1;
    int j = 0;
    long k;
    int hour;
    int min;
    int sec;
    unsigned char tmp;

    //用于发送数据
    TcpSocketClient *tcpClient = new TcpSocketClient();

    print_init(sendBuff);
    tcpClient->sendData(sendBuff, 2);

    shunting_rec_flag = 1;
    if(shunting_rec_flag == 0)
    {
        char noplan[] = "无调车计划单";
        sendBytes = print_cn_title(noplan,sizeof(noplan), sendBuff);
        tcpClient->sendData(sendBuff, sendBytes);
    }
    else  //先把第二行和表头打印出来
    {

        //代码65-74定义调车单接收时间
        k = ( (int)rec_shuntingData[0].plan_num[2] )*65536
            + ( (int)rec_shuntingData[0].plan_num[1] )*256
            + ( (int)rec_shuntingData[0].plan_num[0] );
        sprintf(second_plan_v,"%06d",k);

        memcpy(last_makeTime+9,second_plan_v,2);     //hour
        last_makeTime[11] = ':';
        memcpy(last_makeTime+12,second_plan_v+2,2);  //minute
        last_makeTime[14] = ':';
        memcpy(last_makeTime+15,second_plan_v+4,2);  //second
        // 		strcat(last_makeTime,tempTime);
        // 		strcat(last_makeTime,":");
        // 		sprintf(tempTime,"%d",min);
        // 		strcat(last_makeTime,tempTime);
        // 		strcat(last_makeTime,":");
        // 		sprintf(tempTime,"%d",sec);
        // 		strcat(last_makeTime,tempTime);

        //代码84定义调车单打印时间
        memcpy(last_startTime+9,Local_time,sizeof(Local_time));
        //86-93定义机车号
        j = (int)(rec_shuntingData[0].engine_num[0]&0x0f);
        if(j == 0)
            memcpy(second_engine_v,"--",2);
        else if(j==1)
        {
            j=195;
            sprintf(second_engine_v,"%d",j);
        }
        //95-125定义打印班组和调车员数组
        j = (int)rec_shuntingData[0].group_num[0];
        switch(j)
        {
        case 0: memcpy(second_group_v,"----",4);break;
        case 1: memcpy(second_group_v,"甲白",4);break;
        case 2: memcpy(second_group_v,"甲夜",4);break;
        case 3: memcpy(second_group_v,"乙白",4);break;
        case 4: memcpy(second_group_v,"乙夜",4);break;
        case 5: memcpy(second_group_v,"丙白",4);break;
        case 6: memcpy(second_group_v,"丙夜",4);break;
        case 7: memcpy(second_group_v,"丁白",4);break;
        case 8: memcpy(second_group_v,"丁夜",4);break;
        default : memcpy(second_group_v,"----",4);break;
        }

        j = (int)rec_shuntingData[0].diaoduyuan_num[0];
        switch((j&0xf0)/0x0f)
        {
        case 0: memcpy(second_diaoduyuan_v,"----",4);break;
        case 1: memcpy(second_diaoduyuan_v,"李东山",6);break;
        case 2: memcpy(second_diaoduyuan_v,"葸伟  ",6);break;
        case 3: memcpy(second_diaoduyuan_v,"胥晓东",6);break;
        case 4: memcpy(second_diaoduyuan_v,"杨方德",6);break;
        case 5: memcpy(second_diaoduyuan_v,"储文学",6);break;
        case 6: memcpy(second_diaoduyuan_v,"调度06",6);break;
        case 7: memcpy(second_diaoduyuan_v,"调度07",6);break;
        case 8: memcpy(second_diaoduyuan_v,"调度08",6);break;
        case 9: memcpy(second_diaoduyuan_v,"调度10",6);break;
        default:memcpy(second_diaoduyuan_v,"----",4);break;
        }
        //126-136串接表头数据待打印
        strcpy(second, second_plan);
        strcat(second, second_plan_v);
        strcat(second, second_engine);
        strcat(second, second_engine_v);
        strcat(second, second_group);
        strcat(second, second_group_v);
        strcat(second, second_diaoduyuan);
        strcat(second, second_diaoduyuan_v);
        sendBytes = print_cn_title(title,sizeof(title), sendBuff);
        tcpClient->sendData(sendBuff, sendBytes);
        sendBytes = print_cn(second,sizeof(second), sendBuff);
        tcpClient->sendData(sendBuff, sendBytes);
        sendBytes = print_cn(header,sizeof(header), sendBuff);
        tcpClient->sendData(sendBuff, sendBytes);

        //139-297打印每勾计划
        while(i <= rec_shuntingData[0].total_hook_num[0])
        {
            j = (int)rec_shuntingData[i].jishi[0];
            //switch(j&0x1F)
            switch(j&0x3F)
            {
            case 0: memcpy(record_jishi,"---",3);break;
            case 1: memcpy(record_jishi,"有车线",6);break;
            case 2: memcpy(record_jishi,"全部",8);break;
            case 3: memcpy(record_jishi,"配位",4);break;
            case 4: memcpy(record_jishi,"过磅",4);break;
            case 5: memcpy(record_jishi,"过磅取样",8);break;
            case 6: memcpy(record_jishi,"取样",4);break;
            case 7: memcpy(record_jishi,"单机转",6);break;
            case 8: memcpy(record_jishi,"空线编成",4);break;
            case 9: memcpy(record_jishi,"有车编成",8);break;
            case 10: memcpy(record_jishi,"有车不挂",8);break;
            case 11: memcpy(record_jishi,"卸完回空",6);break;
            case 12: memcpy(record_jishi,"转头",4);break;
            case 13: memcpy(record_jishi,"限速10",6);break;
            case 14: memcpy(record_jishi,"限速15",6);break;
            case 15: memcpy(record_jishi,"禁溜放",6);break;
            case 16: memcpy(record_jishi,"超限",4);break;
            case 17: memcpy(record_jishi,"跨装",4);break;
            case 18: memcpy(record_jishi,"易燃爆",6);break;
            case 19: memcpy(record_jishi,"危险品",6);break;
            case 20: memcpy(record_jishi,"空车",4);break;
            case 21: memcpy(record_jishi,"重车",4);break;
            case 22: memcpy(record_jishi,"关门车",4);break;
            case 23: memcpy(record_jishi,"检修车",6);break;
            case 24: memcpy(record_jishi,"须洗罐",6);break;
            case 25: memcpy(record_jishi,"一级超限",8);break;
            case 26: memcpy(record_jishi,"超长车辆",8);break;
            case 27: memcpy(record_jishi,"重点车",6);break;
            case 28: memcpy(record_jishi,"列车试风",8);break;
            case 29: memcpy(record_jishi,"限速车辆",8);break;
            case 30: memcpy(record_jishi,"开嘉北",6);break;
            case 31: memcpy(record_jishi,"开嘉兴",8);break;
            case 32: memcpy(record_jishi,"开嘉东",8);break;
            case 33: memcpy(record_jishi,"单机回站",8);break;
            case 34: memcpy(record_jishi,"单机接单",8);break;
            case 35: memcpy(record_jishi,"毒品车",8);break;
            case 36: memcpy(record_jishi,"长达平板",8);break;
            case 37: memcpy(record_jishi,"扣修",8);break;
            case 38: memcpy(record_jishi,"预留1",8);break;
            case 39: memcpy(record_jishi,"预留2",8);break;
            default : memcpy(record_jishi,"---",3);break;
            }

            j = (int)rec_shuntingData[i].hook_num[0];
            sprintf(record_hook,"%d",j);

            j = (int)rec_shuntingData[i].track_num[0];
            switch(j)
            {
            case 0: memcpy(record_track,"-----",5);break;
            case 1: memcpy(record_track,"1G   ",5);break;
            case 2: memcpy(record_track,"2G   ",5);break;
            case 3: memcpy(record_track,"3G   ",5);break;
            case 4: memcpy(record_track,"4G   ",5);break;
            case 5: memcpy(record_track,"5G   ",5);break;
            case 6: memcpy(record_track,"6G   ",5);break;
            case 7: memcpy(record_track,"7G   ",5);break;
            case 8: memcpy(record_track,"8G   ",5);break;
            case 9: memcpy(record_track,"9G    ",5);break;
            case 10: memcpy(record_track,"10G  ",5);break;
            case 11: memcpy(record_track,"11G  ",5);break;
            case 12: memcpy(record_track,"12G  ",5);break;
            case 13: memcpy(record_track,"13G  ",5);break;
            case 14: memcpy(record_track,"4衡东",5);break;
            case 15: memcpy(record_track,"4衡西",5);break;
            case 16: memcpy(record_track,"翻1  ",5);break;
            case 17: memcpy(record_track,"翻2  ",5);break;
            case 18: memcpy(record_track,"M1G  ",5);break;
            case 19: memcpy(record_track,"M2G  ",5);break;
            case 20: memcpy(record_track,"M3G  ",5);break;
            case 21: memcpy(record_track,"M4G  ",5);break;
            case 22: memcpy(record_track,"M5G  ",5);break;
            case 23: memcpy(record_track,"M6G  ",5);break;
            case 24: memcpy(record_track,"1#衡 ",5);break;
            case 25: memcpy(record_track,"N2G  ",5);break;
            case 26: memcpy(record_track,"2#衡 ",5);break;
            case 27: memcpy(record_track,"L1G  ",5);break;
            case 28: memcpy(record_track,"L2G东",5);break;
            case 29: memcpy(record_track,"L2G西",5);break;
            case 30: memcpy(record_track,"L3G东",5);break;
            case 31: memcpy(record_track,"L3G西",5);break;
            case 32: memcpy(record_track,"L4G  ",5);break;
            case 33: memcpy(record_track,"L5G  ",5);break;
            case 34: memcpy(record_track,"L6G  ",5);break;
            case 35: memcpy(record_track,"S1G  ",5);break;
            case 36: memcpy(record_track,"S2G  ",5);break;
            case 37: memcpy(record_track,"机东 ",5);break;
            case 38: memcpy(record_track,"机西 ",5);break;
            case 39: memcpy(record_track,"机1G ",5);break;
            case 40: memcpy(record_track,"机2G ",5);break;
            case 41: memcpy(record_track,"H1G  ",5);break;
            case 42: memcpy(record_track,"H2G  ",5);break;
            case 43: memcpy(record_track,"H7G  ",5);break;
            case 44: memcpy(record_track,"H1衡 ",5);break;
            case 45: memcpy(record_track,"H9   ",5);break;
            case 46: memcpy(record_track,"宏翻 ",5);break;
            case 47: memcpy(record_track,"H2衡 ",5);break;
            case 48: memcpy(record_track,"装卸 ",5);break;
            case 49: memcpy(record_track,"洗罐 ",5);break;

            default : memcpy(record_track,"-----",5);break;
            }

            j = (int)(rec_shuntingData[i].dir[0]);
            //			switch((j&0XE0)%0X1F)
            switch(j & 0X0F)
            {
            case 0: memcpy(record_dir,"--",2);break;
            case 1: memcpy(record_dir,"东",2);break;
            case 2: memcpy(record_dir,"西",2);break;
            case 3: memcpy(record_dir,"南",2);break;
            case 4: memcpy(record_dir,"北",2);break;
            default : memcpy(record_dir,"--",2);break;
            }
            //j = (int)(rec_shuntingData[i].dir_method[0]);
            tmp = rec_shuntingData[i].dir_method[0];
            tmp = (tmp & 0xC0) >> 6 ;
            tmp = tmp & 0x03;
            j= (int) tmp;
            //switch(j%16)
            switch(j%16)
            {
            case 3: memcpy(record_method,"  ",2);break;
            case 0: memcpy(record_method,"+ ",2);break;
            case 1: memcpy(record_method,"- ",2);break;
            case 2: memcpy(record_method,"x ",2);break;
            default : memcpy(record_method,"  ",2);break;
            }

            j = (int)rec_shuntingData[i].vehicle_num[0];
            sprintf(record_vehnum,"%2d",j);

            k = ( (int)rec_shuntingData[i].vehicle_coding[2] )*65536
                + ( (int)rec_shuntingData[i].vehicle_coding[1] )*256
                + ( (int)rec_shuntingData[i].vehicle_coding[0] );
            if(k == 0)
            {
                memset(record_vehcod,'0',7);
            }
            else
                sprintf(record_vehcod,"%7d",k);

            memset(record,0,sizeof(record));
            strcpy(record,record_hook);
            strcat(record,space);
            strcat(record,record_track);
            strcat(record,space);
            strcat(record,record_dir);
            strcat(record,space);
            strcat(record,record_method);
            strcat(record,"  ");
            strcat(record,record_vehnum);
            strcat(record,"  ");
            strcat(record,record_vehcod);
            strcat(record," ");
            strcat(record,record_jishi);
            sendBytes = print_cn(record,sizeof(record), sendBuff);
            tcpClient->sendData(sendBuff, sendBytes);
            memset(record_track,0,sizeof(record_track))	;
            memset(record_hook,0,sizeof(record_hook))	;
            memset(record_dir,0,sizeof(record_dir))	;
            memset(record_method,0,sizeof(record_method))	;
            memset(record_vehnum,0,sizeof(record_vehnum))	;
            memset(record_vehcod,0,sizeof(record_vehcod))	;
            memset(record_jishi,0,sizeof(record_jishi))	;

            i++;
        }

        j = (int)rec_shuntingData[0].attention_num[0];
        memset(attention1,0,sizeof(attention1))	;

        switch(j)
        {
        case 0: memcpy(attention1,"-----",5);break;
        case 1: memcpy(attention1,"严格控制速度确保作业安全",24);break;
        case 2: memcpy(attention1,"一度停车",8);break;
        case 3: memcpy(attention1,"认真执行一度停车制度",20);break;
        case 4: memcpy(attention1,"厂房挂车，注意安全",18);break;
        case 5: memcpy(attention1,"选好上下车地点",14);break;
        case 6: memcpy(attention1,"岔线作业采取防溜保安全",22);break;
        case 7: memcpy(attention1,"注意安全",8);break;
        case 8: memcpy(attention1,"岔线作业加强瞭望确认",20);break;
        case 9: memcpy(attention1,"停留车辆采取好防溜措施",22);break;
        case 10: memcpy(attention1,"溜放场作业，注意防溜",20);break;
        case 11: memcpy(attention1,"雾天作业严格控制速度",20);break;
        case 12: memcpy(attention1,"风雨雪天气严格控制速度",22);break;
        case 13: memcpy(attention1,"对货位",6);break;
        default : memcpy(attention1,"-----",5);break;
        }

    }
    //执行打印注意事项编制时间开始时间
    memset(attention1_attall,0,sizeof(attention1_attall));
    strcat(attention1_attall,attention1_att);
    strcat(attention1_attall,attention1);
    sendBytes = print_cn(attention1_attall,sizeof(attention1_attall), sendBuff);
    tcpClient->sendData(sendBuff, sendBytes);
    memset(attention2_attall,0,sizeof(attention2_attall));
    strcat(attention2_attall,attention2_att);
    strcat(attention2_attall,attention2);
    sendBytes = print_cn(attention2_attall,sizeof(attention2_attall), sendBuff);
    tcpClient->sendData(sendBuff, sendBytes);

    strcat(last_makeTime,rec_shuntingData[0].makeTime);
    sendBytes = print_cn(last_makeTime,sizeof(last_makeTime), sendBuff);
    tcpClient->sendData(sendBuff, sendBytes);
    strcat(last_startTime,rec_shuntingData[0].startTime);
    sendBytes = print_cn(last_startTime,sizeof(last_startTime), sendBuff);
    tcpClient->sendData(sendBuff, sendBytes);
    sendBytes = print_cn_end(last_endTime,sizeof(last_endTime), sendBuff);
    tcpClient->sendData(sendBuff, sendBytes);

}

//初始化打印机
void print_init(char *buf){
    buf[0] = 0x1B;
    buf[1] = 0x40;
}

//打印中文标题
int print_cn_title(char *date, int length, char *buff){
    int i = 0;
    //设定行高距离20；行距范围10，20，30，40，50，60
    buff[i] = 0x1B;
    i = i + 1;
    buff[i] = 0x33;
    i = i + 1;
    buff[i] = 0x60;
    i = i + 1;
    //正常字体大小00；11放大一倍；10宽度放大一倍；01高度放大一倍
    buff[i] = 0x1D;
    i = i + 1;
    buff[i] = 0x21;
    i++;
    buff[i] = 0x11;
    i++;
    //文本左对齐00；01居中；02右对齐
    buff[i] = 0x1B;
    i++;
    buff[i] = 0x61;
    i++;
    buff[i] = 0x01;
    i++;
    //选择中文格式
    buff[i] = 0x1C;
    i++;
    buff[i] = 0x26;
    i++;
    //USART_OUT(USART3,Data);
    memcpy(buff + i, date, length);
    i = i + length;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;

    return i;
}

//打印中文非标题
int print_cn(char *date, int length,  char *buff){
    int i = 0;
    buff[i] = 0x1B;
    i++;
    buff[i] = 0x40;
    i++;
    //设定行高距离20；行距范围10，20，30，40，50，60
    buff[i] = 0x1B;
    i++;
    buff[i] = 0x33;
    i++;
    buff[i] = 0x50;
    i++;
    //正常字体大小00；11放大一倍；10宽度放大一倍；01高度放大一倍
    buff[i] = 0x1D;
    i++;
    buff[i] = 0x21;
    i++;
    buff[i] = 0x00;
    i++;
    //文本左对齐00；01居中；02右对齐
    buff[i] = 0x1B;
    i++;
    buff[i] = 0x61;
    i++;
    buff[i] = 0x00;
    i++;
    //选择中文格式
    buff[i] = 0x1C;
    i++;
    buff[i] = 0x26;
    i++;
    //USART_OUT(USART3,Data);
    memcpy(buff + i, date, length);
    i = i + length;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;

    return i;
}

//打印中文非标题,并切纸
int print_cn_end( char *date, int length,  char *buff){
    int i = 0;
    //设定行高距离20；行距范围10，20，30，40，50，60
    buff[i] = 0x1B;
    i++;
    buff[i] = 0x33;
    i++;
    buff[i] = 0x50;
    i++;
    //正常字体大小00；11放大一倍；10宽度放大一倍；01高度放大一倍
    buff[i] = 0x1D;
    i++;
    buff[i] = 0x21;
    i++;
    buff[i] = 0x00;
    i++;
    //文本左对齐00；01居中；02右对齐
    buff[i] = 0x1B;
    i++;
    buff[i] = 0x61;
    i++;
    buff[i] = 0x00;
    i++;
    //选择中文格式
    buff[i] = 0x1C;
    i++;
    buff[i] = 0x26;
    i++;
    //USART_OUT(USART3,Data);
    memcpy(buff + i, date, length);
    i = i + length;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;
    buff[i] = 0x0D;
    i++;
    buff[i] = 0x0A;
    i++;
    //切纸 esc
    /*buff[i] = 0x1B;
    i++;
    buff[i] = 0x69;
    i++;*/

    return i;
}
