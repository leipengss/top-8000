#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QLabel>
#include <QMainWindow>
#include <QTimer>
#include<QKeyEvent>
#include"filemanage/cinirw.h"
#include"function/cbaoguangflow.h"
#include"driver/cupdatedatathread.h"
#include"UI/cdlgmessage.h"
#include"UI/caxispositionshow.h"
#include "dlgmove.h"
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CDlgMessage *message;
    DlgMove *dlgmove;
    CIniRW *inifilerw;
    QLabel *statusLabel;
    QTime timelighton[2];
    float m_upbg_time[2];//曝光时间
    float m_downbg_time[2];
    float m_absorb_time;
    float m_absorb_time2;
    unsigned long m_lighttime;
    unsigned long m_downlighttime;
    bool m_is_weihu;//系统维护界面
    int m_manualspeed;
    bool m_isaxis_alarm;
    //底片清洁提示标志位
    bool m_isdpclear[2];
    bool m_isdpexchange[2];
    bool m_bdpclear_show;
    //
    bool m_isAlarmshow;
    // 选中的CCD轴
    int ccdselect;
    bool m_isCCDdw[2];//CCD让位按钮标示
    //靶标切换：0:主靶标  1:次靶标
    bool m_isothermark[2];
    //报警信息字符串
    QString str_alarm;
    //
    int dwccd[4];
    int downdwccd[4];
    //
    CCDLMR_POS lmr_PLimit[2];
    CCDLMR_POS lmr_NLimit[2];
    CCDLMR_POS downlmr_PLimit[2];
    CCDLMR_POS downlmr_NLimit[2];
    //
    CCD_POS  m_ccdbasic;
    //
    alrmcheck_bit alarmswitch_bit;
    axischeck_bit axis_alarm_bit[35];
    //
    QVector<QString> m_axisname;
    //
    int m_zpos_type;
    // 声明
    void keyPressEvent(QKeyEvent *event);
    void init_member();
    //检查参数文件及配置文件是否存在
    bool IsSysFileExist();
    //参数初始化
    void init_para();
    //程序退出时的动作
    void exit_state();
    //判断有无弹窗
    bool NoTopDialog();
    //状态显示
    void showtips(int tipstype, QString text);
    void check_inputalarm();
    void check_axisalarm();
    void check_lightstate();
    //报警后的流程处理
    void alarm_deal();
    void check_softlimit();
    QString limitshow(int axis,int type);
    void check_orglimit();
    void allaxis_disable();
    void check_state();
    //开关框
    void frame_mannual();
    //曝光运行
    bool bg_run();
    //
    void Zpos_check();
    //界面按钮状态
    void btn_state();
    //
    void mark_save(int ch,bool ismainmark);
    void singleMark_save(int index);
    //
    void ccdpar_select();
    //点击CCD显示画面时CCD寸动
    void ccd_step(int ccdnum,int xdir,int ydir);
    //
    void check_regstate();
    void Mark_load(int ch);
    //判断CCD手动条件
    bool isCCD_Move(int axis,bool dir);
    //
    int m_pageindex;
public:
    QTimer *firsttimer;
    QTimer *timer;
    QTimer *timer2;
    QLabel *currentTimeLabel;
    bool m_errorframe;
    //local function
    int axis_select();
    void axis_showpos();
signals:
    void sig_estop();
    void sig_dpclear(int type,int id);
    void sig_setlight(int frameid);
private slots:
    bool slot_alarmshow(bool ismodel,QString errcode);
    bool slot_alarmshow();
    void slot_mbkey(int val);//机台面板按键
    void slot_ccdrec(const char* buf);
    void slot_update();
    void slot_dpclear(int type,int id);
    void slot_setlight(int frameid);
//    void logwrite(int tipstype, QString text);
    //框架锁 5s
    void timeUpdatesKey();
    //系统时间更新
    void timeUpdate();
    //底片
    void click_btn_dp();
    //灯柜
    void click_btn_dg();
    //检测维护
    void click_btn_syscheck();
    //调试
    void click_btn_sysdebug();
    //程序退出
    void click_btn_exit();
    //参数设置
    void click_btn_parameter();
    //复位
    void click_btn_reset();

    void click_btn_mainpage2_exit();

    void click_btn_dp_exit();
    void click_btn_dg_exit();

    void on_btn_language_switch_clicked();
    void on_btn_iopara_clicked();
    void on_btn_cncpara_clicked();
    void on_btn_home_select_clicked();
//    void on_btn_dw_study_clicked();
    void on_btn_change_password_clicked();
    void on_btn_machine_switch_clicked();
    void on_btn_upframe_clear_clicked();
    void on_btn_downframe_clear_clicked();
    void on_btn_lightsys_zero_clicked();
    void on_btn_M0_clicked();
    void on_btn_M1_clicked();
    void on_btn_M2_clicked();
    void on_btn_M3_clicked();
    void on_btn_normalprocess_clicked();
    void on_btn_ccd_down_clicked();
    void on_btn_platform_center_clicked();
    void on_btn_frame_absorb_clicked();
    void on_btn_autoset_clicked();
    void on_btn_mark_check_clicked();
    void on_btn_mark_change_clicked();
    void on_btn_ccd_lowspeed_clicked();
    void on_btn_save_mainmark_clicked();
    void on_btn_save_othermark_clicked();
    void on_btn_jt_dy_clicked();
    void on_btn_ccd_rw_clicked();
    void on_btn_position_down_clicked();
    void on_btn_position_lb_clicked();
    void on_btn_position_dw_clicked();
    void on_btn_position_bg_clicked();
    void on_btn_cool_clicked();
    void on_btn_light_clicked();
    void on_btn_bg_test_clicked();
    void on_btn_frame_change_clicked();
    void on_button_liaohao_clicked();
//    void on_btn_positive_clicked();
//    void on_btn_negtive_clicked();
    void on_btn_positive_pressed();
    void on_btn_positive_released();
    void on_btn_negtive_pressed();
    void on_btn_negtive_released();
    void on_btn_hb_minus_clicked();
    void on_btn_hb_plus_clicked();
    void on_btn_ld_minus_clicked();
    void on_btn_ld_plus_clicked();
//    void on_radio_js_clicked();
//    void on_radio_sd_clicked();
    void on_checkBox_dwok_stateChanged(int arg1);
    void on_checkBox_checkok_stateChanged(int arg1);
    //
    void slot_EStop();
    void on_btn_f1_clicked();
    void on_btn_f2_clicked();
    void on_btn_f3_clicked();
    void on_btn_uplight_clear_clicked();

    void on_btn_dp_load_clicked();
    void on_btn_dp_vacuum_clicked();
    void on_btn_dp_vacuum2_clicked();
    void on_btn_bm_vacuum_clicked();
    void on_btn_oilsetting_clicked();
    void on_btn_axis_showposition_clicked();
    void on_btn_npos_clicked();
    void on_btn_wpos_clicked();
    void on_btn_move_set_clicked();
    void on_btn_taimian_binshuiji_clicked();
    void on_btn_taimian_gufenji_clicked();
    void on_btn_dp_zhengkon_clicked();
    void on_btn_BG_Light_clicked();
    void on_btn_BG_Light_2_clicked();
    void on_btn_taimian_ZKBen_clicked();
    void on_btn_CCD_on_clicked();
    void on_btn_CCD_errClear_clicked();
};
extern     CBaoguangFlow *bgflow;
#endif // MAINWINDOW_H
