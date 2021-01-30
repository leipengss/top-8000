#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "cncsh.hh"
#include"driver/cupdatedatathread.h"
#include<QDebug>

long nDayLeave = 0;                 // 许可剩余时间
int real_year;
int real_month;
int real_day;
int nRemainHour = 0;
int ADayPassFlag = 1;
int AHourPassFlag = 1;
int g_registertype = UNREG;
QString reg_info = "";

int diff_time(void);
long int real_time();

long int real_time()
{
    YMDtime hardtime;
    int year,month,day;
    hardtime = update_thread->threaddriver->HardwareTime();
    year = hardtime.year;
    month = hardtime.month;
    day = hardtime.day;
    static int get_realtime=1;
    if(get_realtime==1)
    {
        get_realtime=0;
        real_year=year;
        real_month=month;
        real_day=day;
    }

    if(access("/usr/bin/.new-bittorrent-1.3",0)==-1)
    {
        int new_year=year+(month+2)/13;
        int new_month=(month+2)%13;
        if(new_month<2) new_month=new_month+1;
        int new_day=day;
        FILE *fp;
        fp=fopen("/usr/bin/.new-bittorrent-1.3","w");
        if(fp!=NULL)
        {
            fprintf(fp,"%04d%02d%02d",new_year,new_month,new_day);
            fclose(fp);
        }
        usleep(100*1000);
    }

    time_t now;
    struct tm *timenow;
    time(&now);
    timenow=localtime(&now);

    if(access("/usr/bin/.gtf-bittorrent-1.2",0)==-1)
    {
        FILE *fp;
        fp=fopen("/usr/bin/.gtf-bittorrent-1.2","w");
        if(fp!=NULL)
        {
            fprintf(fp,"%04d%02d%02d",timenow->tm_year+1900,timenow->tm_mon+1,timenow->tm_mday);
            fclose(fp);
        }
        usleep(100*1000);
    }
    timenow->tm_year=year-1900;
    timenow->tm_mon=month-1;
    timenow->tm_mday=day;
    now=mktime(timenow);
    return now;
}

int diff_time(void) //比较密码日期和实际时间
{
    real_time();
    long int texttime;
    int year,month,day;
    cnc_sysnum(0,"");//读可用时间
    QString prt;
    prt.sprintf("limitdate_back=%s",limitdate_back);
    qDebug()<<prt;
    if(!strcmp(limitdate_back,"") || !strcmp(limitdate_back,"OK")|| !strcmp(limitdate_back,"FAIL"))
    return LONGTIME;
    //加密时间
    texttime = atoi(limitdate_back);
    year = texttime/10000;
    month = texttime%10000/100;
    day = texttime%100;
    prt.sprintf("diff_time:year=%d month=%d day=%d",year,month,day);
    qDebug()<<prt;
    time_t now;
    struct tm *timenow;
    time(&now);
    timenow=localtime(&now);
    timenow->tm_year=year-1900;
    timenow->tm_mon=month-1;
    timenow->tm_mday=day;
    now=mktime(timenow);

    //硬件时间
    long int realtime=real_time();
    if(realtime==-1) return 0;
    prt.sprintf("realtime=%d",realtime);
    qDebug()<<prt;
    //计算剩下的时间
    long int left_time=now-realtime;
    static int firsttime = 0;
    if(firsttime == 0)
    {
       firsttime = 1;
       nRemainHour = (left_time / 3600 );
    }
//     std::cout << "[1;34m  nRemainHour   [0m" << nRemainHour << std::endl;
    if(!(left_time<=-60*60*24*1))
    {
        nRemainHour = (left_time / 3600 );
    }
    prt.sprintf("nRemainHour=%d",nRemainHour);
    qDebug()<<prt;
    if(left_time>60*60*24*2&&left_time<=60*60*24*3) return THREE_DAY_LEFT;
    else if(left_time>60*60*24*1&&left_time<=60*60*24*2) return TWO_DAY_LEFT;
    else if(left_time>-60*60*24*1&&left_time<=60*60*24*1) return ONE_DAY_LEFT;
    else if(left_time<=-60*60*24*1) return UNREG;
    else return REGED;
}
