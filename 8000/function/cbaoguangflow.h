#ifndef CBAOGUANGFLOW_H
#define CBAOGUANGFLOW_H
#include"driver/cupdatedatathread.h"
#include<QTime>
////global
#include "driver/global.h"
#define ONCEDW_MAX 80

class CBaoguangFlow
{
public:
    CBaoguangFlow();
    ~CBaoguangFlow();
    void dwpt_lmrmove(int ch,CCDLMR_POS value_lmr,int frame);//平台LMR移动
    void wait_lmrstop(int ch,int frame);
    bool dwpt_zmove(int type,int frame,bool isbreakair);//平台Z移动方式
    void ptz_lrsync(float speed,int axis_l,int axis_r);//左右Z轴停止位置偏差补偿
    void frame_init();//开机时框架状态初始化
    bool framechange();
    bool framechange2();//单建启动
    void thread_dwlightrun();
    void thread_duiweicheck();
    void thread_framechange();
    void thread_bgrun();
    void thread_bgtest();
    void thread_bgcontinuerun();
    void thread_ptzmove(int type);
    void thread_ptlmrmove(int ch);
    void thread_openframe();
    void thread_dpload();
    void estop();//按下急停后的动作
    void open_frame();
    void tb();
    bool isccdsafe(int ch);
    void thread_duiwei();
    void ccd_dwtype(int ch,int type);
    void thread_ccdmove(int ch,int type);//0:回到原点 1：对标
    //move
    bool DuiweiCheck();
    void run();
    void run_move(int ch);
    void run_quit();
    void debug_run();
    void baoguang();
    void ccd_moveposition(bool isCCD34first,bool bwait,Mark_pos mark);
    bool IsSameMark(Mark_pos mark1,Mark_pos mark2);
    void downccd_moveposition(bool bwait,Mark_pos mark);
    bool wait_ccdstop(bool isrw);
    bool wait_downccdstop(bool isrw);
    void thread_pause(bool isjb);
    void scanlight_move(bool bwait,float speed);
    bool wait_scanstop();
    void reject_done();//拒曝后的动作
    //Z轴由原点到离板位置的停止后动作
    void ptz_lbpos();
    bool wait_ptz_stop();
private:
    QStringList List_parmeter,List_parmeterJFx,List_parmeterJFy,List_parmeterWCx,List_parmeterWCy,List_parmeterDWJD;
    bool ccd_rw();//曝光运行中CCD让位
    void pt_check(int axis);//判断方向及每段行程
    bool pt_move(int frame,int axis_l,bool isairbreak);//平台移动
    bool tm_locatecheck();
    int z_step;
    int z_movestep;
    float target_value;
    float lb_wz;
    float dw_wz;
    float bg_wz;
    //对位
    bool dw_start;
    int dw_answer;
    //核对
    int  ch_answer;
    bool ch_main;
    int bgcheck8;
    //对位后得到LMR坐标
    CCDLMR_POS lmr_pos[2];
    //测试时间
    QTime timecnt;
};

#endif // CBAOGUANGFLOW_H
