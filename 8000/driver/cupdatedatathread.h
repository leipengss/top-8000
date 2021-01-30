#ifndef CUPDATEDATATHREAD_H
#define CUPDATEDATATHREAD_H
#include<QThread>
#include <QTimer>
#include"driver/libdriver.h"
#include "driver/global.h"
//#include"modbus/libmodbus.h"
//通用输入端口异常
typedef struct input_bit
{
    bool Sys_air:1;
    bool Light_power:1;
    bool Light_ison:1;
    bool Light_moveok:1;
    bool Cool_vel:1;
    bool Air_check:1;
    bool Air_check2:1;
    bool Light_tem:1;
    bool Room_tem:1;
    bool Room_sd:1;
    bool Door_alm:1;
    bool Frame_w_safe:1;
    bool EstopSig:1;
    bool Ccd13_protect:1;
    bool Ccd24_protect:1;
    bool Ccd_isok:1;
    bool z_PLimit;
    bool updp_vacuum;
    bool downdp_vacuum;
    bool TaiMian_Coolwen:1;
    bool TaiMian_CoolLiu:1;
}alrmcheck_bit;
//轴端口
typedef struct axisinput_bit
{
    bool alarm:1;
    bool org:1;
    bool hardlimit:1;
    bool softlimit_P:1;
    bool softlimit_N:1;
}axischeck_bit;
//面板按键
typedef struct keyinput_bit
{
    bool key1:1;
    bool key2:1;
    bool key3:1;
}key_bit;
class CUpdateDataThread : public QThread
{
    Q_OBJECT

public:
    CUpdateDataThread();
    ~CUpdateDataThread();
    void stop();
    void run();
//    void update();
public:
    Libdriver *threaddriver;
    int zmove_type;
    int ccdmove_type[2];
    CCDLMR_POS lmrvalue;
    CCDLMR_POS downlmrvalue;
    CCD_POS  ccdvalue;
    CCD_POS  downccdvalue;
    //上一次对位成功时的坐标
    CCDLMR_POS m_uppcbok[2];
    CCDLMR_POS m_downpcbok[2];
    //
    CCDLMR_POS tj_lmrvalue[2];
    bool m_bAxis_state;
    bool m_bCCDY_stop;
    bool m_bFrame_stop;
    bool m_bScanAxis_stop;
    bool m_bcool_out;
  //  bool m_taimian_bin_out;//冰水机
    bool m_taimian_gu_out;   //鼓风机
    bool m_dp_zhengkon_out;//底片真空o11.6
    bool m_bm_zhengkon_out;//版面真空
    bool m_light_state_out;//曝光室照明电源状态
 //   bool m_taimian_ZKBen_out;//台面真空泵
    bool m_ccd_sifu_out;//CCD伺服转矩开启
    bool m_ccd_down;
    bool m_bopenframe[2];
    bool m_bcloseframe[2];
    bool m_bCnc_state;
    bool m_bHard_state;
    bool m_bLighton;
    bool m_bjtdy;
    bool m_framedingban;
    bool m_framezjb;
    bool m_dpvacuum;//原底片真空o12.2
    bool m_dpvacuum2;
    bool m_isxlight_on ;
    bool m_bjtcq;
    bool m_bframeabsorb;
    float m_axispos[35];//AXIS_W+1
    float m_axiscmdpos[35];
    float m_axisfeedbackpos[35];
    float m_axisfollowerror[35];
    alrmcheck_bit input;
    //ccd发送命令开关信号
    bool is_sendstart;
    bool is_sendcheck;
    bool is_sendstudy;
    bool is_ccdsuoding;//CCD锁定图像
    bool is_tjlmr;
    int m_record_light_time;//曝光时点灯持续时间 单位：ms
    int m_record_downframelight_time;
    //框架开关进行中的状态
    bool m_bFrameOpenning[2];
    bool m_bFrameClosing[2];
    //框架错误标志
    bool m_berrorframe;
    //移框前检测光栅，信号有效暂停标志位
    bool m_framemovepause;
    bool m_isframemoving;
    /*********CCD相关参数****************/
    int sd;//0表示解锁,1表示锁定
    //黑白
    void hb_minus(int ccd);
    void hb_plus(int ccd);
    //亮度
    void ld_minus(int ccd);
    void ld_plus(int ccd);
    //锁定
    void ccd_sd(int value);
    int m_last_frame;//记录上一次框架编号
    int used_ccd[4];//每个CCD的使用情况
    int used_downccd[4];
    int ccd_234;//采取哪种使用方案
    int downccd_234;
    int check_othermark[4];
    int check_downothermark[4];
    int is_check8;
    int is_checkdown8;
    bool m_isusemark[2][4];
    bool m_bEnableMarkChange[2][4];//靶标切换使能
    bool m_isdownusemark[2][4];
    bool m_bdownEnableMarkChange[2][4];//靶标切换使能
    //当前使用的靶点
    Mark_pos m_currentmark;
    Mark_pos m_downcurrentmark;
    Mark_pos m_basicpos;
    void par_update(int frame);
    void thread_alarmshow(QString errcode);
    void thread_MoveDuiwei();
    //产能显示
    void planboard(short actval);
    void currentboard(short val);
    short plan_cnt;
    int comportnum;//从1开始 1代表ttys0
    unsigned char planboard_addr;//设备站号
    void beep();
    //返回轴名
    QVector<QString> axisname_list;
    //
    void logwrite(int tipstype, QString text);
private:
    QTimer *updatetimer;
    bool m_bStopped;
    //面板按键
    key_bit keyboard;
    key_bit mb_key;
    void keytrigger();
    //
    int ccd_switch;//采用2个ccd的其中一个方案，0表示CCD1-2，1表示CCD3-4，2表示CCD1-4，3表示CCD2-3
    int downccd_switch;
    void framenum();
    void check_home();
    void axis_pos();
    //检查报警输入端口
    void check_alarminput();
    bool check_allaxis_state();
    bool check_scanaxis_state();
    bool check_ccdy_movestate();
    bool check_frame_movestate();
    bool ModbusOpen(int cominf);
signals:
    void mbkey(int val);
    void alarmshow(bool ismodel,QString errcode);
    void alarmshow();
private slots:
    void update();
   // void slot_recdata(uint8_t * data, int dataLen,bool addNewline);
};
extern CUpdateDataThread *update_thread;
#endif // CUPDATEDATATHREAD_H
