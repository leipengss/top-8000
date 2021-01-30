/*******************************************************************
 *  Copyright(c) 2018-2025 TOP-Exposure system
 *  All rights reserved.
 *  文件名称:
 *  简要描述:
 *
 *  创建日期: 2018/11/06
 *  作者: TOP
 *  说明:
 ******************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<math.h>
//#include<QDateTime>
#include<QFileInfo>
#include<QString>
#include<QDebug>
#include<QMessageBox>
//#include<QTextCodec>
#include<QApplication>
#include<QDesktopWidget>
#include<QSettings>
#include <QtConcurrentRun>
//UI
#include"UI/dlghome.h"
#include"UI/dlgsystemcheck.h"
#include"UI/dlgworkpiece.h"
#include"UI/dlgioparameter.h"
#include"UI/dlgchangepassword.h"
#include"UI/dlglogin.h"
#include"UI/dlglightzero.h"
#include"UI/dlgbgparameter.h"
#include"UI/dlgsuperparasetting.h"
#include"UI/dlgcncparmeter.h"
#include"UI/dlgmaterialmanage.h"
#include"UI/dlgregister.h"
#include"UI/dlglanguage.h"
#include "driver/cncsh.hh"
#include "modbus/lightRTU.h"
#include "modbus/light.h"
#include "dlgparmeter.h"
#include "dlgmove.h"
//global
int  g_settings_kind = 1;
bool g_isRestartSys = false;
int  g_beeptype = 0;
bool g_isBgRunning = false;//曝光运行
bool g_isDownPTDuiWei = false;
bool g_bDownPTDuiWeiOK = false;
bool g_bDownPTDuiWeiExit = false;
int  g_now_frame = 0;//初始化为非法框架
float  g_up_dwposition=0;
float  g_up_lbposition=0;
float  g_up_bgposition=0;
float  g_down_dwposition=0;
float  g_down_lbposition=0;
float  g_down_bgposition=0;
bool g_framechange =false;//true:正在切换 false:未切换或切换完成
bool g_framemannual = false;
bool g_isCCDMoveThread[2] = {false,false};
bool g_isSysRunning = false;
bool g_isPauseRun = false;
bool g_isSysContinueRun = false;
bool g_ishoming =false;
bool g_ishomed =false;
bool g_axis_speed_type =false;
int  g_lightstate =0;
int  g_downlightstate = 0;
int  g_light_errorcnt=0;
bool g_uplighton_error = false;
bool g_autoset =false;
bool g_downautoset =false;
bool g_justbg =false;
bool g_runtest[2] = {false,false};
bool g_downruntest[2] = {false,false};
bool g_bDWTest=false;
bool g_bCHTest=false;
bool g_bDebug_run=false;
bool g_ch_start=false;
int  g_ch_answer=0;
bool g_ch_main=false;
bool g_downch_start=false;
int  g_downch_answer=0;
bool g_downch_main=false;
bool g_dw_start=false;
bool g_downdw_start = false;
int g_dw_answer=0;
int g_downdw_answer =0;
bool g_dw_tj = false;
bool g_downdw_tj = false;
bool g_study_start = false;
int  g_study_answer = 0;
bool g_downstudy_start = false;
int  g_downstudy_answer = 0;
int g_study_num = 0;
int g_downstudy_num = 0;
int g_dw_num = 0;
int g_downdw_num = 0;
int g_dw_continuenum  = 0;
bool g_bChange_mark=false;
int g_downdw_continuenum = 0;
bool g_bDownChange_mark = false;
bool g_reloadpara =false;
bool g_loadmaterial = false;
bool g_isccdsafe =false;
bool g_isPause =false;
bool g_isPauseStart =false;
bool g_isBGwhileStart = false;
//
int g_jtairin_delay = 0;
int g_jtairout_delay = 0;
//
int g_frameopen_delay = 0;
int g_frameclose_delay = 0;
//
int g_checkdelay = 0;
int g_downcheckdelay = 0;
bool g_isScreenStart = false;
//
bool g_isHomeAllThread = false;
//
int g_language = 0;
//
int g_no_dw[2] = {0,0};//是否需要对位
int g_no_ch[2] = {0,0};//是否需要核对
//参数结构体
PAR  g_par;
 CBaoguangFlow *bgflow;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /********参数初始化*******/
    init_member();
    inifilerw =new CIniRW(TOPPARAMETERPATH);
    if(IsSysFileExist())
    {
       init_para();
    }
    else
    {
        exit(0);
    }
    /********CNC模块初始化*******/
    //start updatethread
    update_thread = new CUpdateDataThread();
    update_thread->threaddriver->CNC_init();
    update_thread->axisname_list.swap(m_axisname);
    update_thread->start(QThread::NormalPriority);
    connect(update_thread,SIGNAL(mbkey(int)),this,SLOT(slot_mbkey(int)));
    connect(update_thread,SIGNAL(alarmshow(bool,QString)),this,SLOT(slot_alarmshow(bool,QString)));
    connect(update_thread,SIGNAL(alarmshow()),this,SLOT(slot_alarmshow()));

    connect(update_thread->threaddriver,SIGNAL(ccd_receive(const char*)),this,SLOT(slot_ccdrec(const char*)));
    connect(this,SIGNAL(sig_setlight(int)),this,SLOT(slot_setlight(int)));
    //曝光流程初始化
    bgflow = new CBaoguangFlow;
    /********UI初始化*******/
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//退出时释放内存
    // 隐藏标题栏 任务栏图标
    this->setWindowFlags(Qt::FramelessWindowHint);
    //状态栏
    QFont fontstatus;
    fontstatus.setPointSize(16);
    statusBar()->setFont(fontstatus);
    statusBar()->setSizeGripEnabled(false);//关闭右下角调节窗口大小功能
    statusLabel = new QLabel;
    statusBar()->width();
    statusLabel->setAlignment(Qt::AlignLeft);
    statusLabel->setFont(fontstatus);
    statusLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    statusLabel->setFixedSize(800,30);
    statusBar()->addWidget(statusLabel);
    //CCD模块初始化并显示
    update_thread->threaddriver->CCD_init(ui->frame_ccd);
    //UI初始状态
    ui->status_system->setText(QObject::tr("未复位"));
    ui->status_system->setStyleSheet("background-color:black;color:rgb(200,20,20)");
    ui->frame_light_gif->setStyleSheet("#frame_light_gif{border-image:url(:/images/image/led_black.png)}");
    ui->frame_light_gif_2->setStyleSheet("#frame_light_gif_2{border-image:url(:/images/image/led_black.png)}");
    ui->spin_speed->setValue(m_manualspeed);//设置手动速度
    //show main page
    ui->stackedWidget_main->setCurrentIndex(0);
    ui->stackedWidget_left->setCurrentIndex(0);
    ui->stackedWidget_lightgif->setCurrentIndex(0);
    /********主窗口事件*******/
    connect(ui->btn_systemexit,SIGNAL(clicked()),this,SLOT(click_btn_exit()));
    connect(ui->btn_para_setting,SIGNAL(clicked()),this,SLOT(click_btn_parameter()));
    connect(ui->btn_reset,SIGNAL(clicked()),this,SLOT(click_btn_reset()));
    connect(ui->btn_dg,SIGNAL(clicked()),this,SLOT(click_btn_dg()));
    connect(ui->btn_dp,SIGNAL(clicked()),this,SLOT(click_btn_dp()));
    connect(ui->btn_systemcheck,SIGNAL(clicked()),this,SLOT(click_btn_syscheck()));
    connect(ui->btn_systemdebug,SIGNAL(clicked()),this,SLOT(click_btn_sysdebug()));
    //mainpage2
    connect(ui->btn_mainpage2_exit,SIGNAL(clicked()),this,SLOT(click_btn_mainpage2_exit()));
    //stack left page2
    connect(ui->btn_dp_exit,SIGNAL(clicked()),this,SLOT(click_btn_dp_exit()));
    connect(ui->btn_dg_exit,SIGNAL(clicked()),this,SLOT(click_btn_dg_exit()));
    //急停
    connect(this,SIGNAL(sig_estop()),this,SLOT(slot_EStop()));
    //底片清洁
    connect(this,SIGNAL(sig_dpclear(int,int)),this,SLOT(slot_dpclear(int,int)));
    //底层数据更新定时器
    firsttimer =new QTimer(this);
    firsttimer->start((int)FIRSTTIMER); // 5ms
    connect(firsttimer,SIGNAL(timeout()),this,SLOT(slot_update()));
    //系统时间显示
    currentTimeLabel=new QLabel;
    currentTimeLabel->setFrameStyle(QFrame::Box|QFrame::Raised);
    currentTimeLabel->setAlignment(Qt::AlignRight);
    ui->statusbar->addPermanentWidget(currentTimeLabel);
    timer=new QTimer(this);
    timer->start(500); // 每次发射timeout信号时间间隔为0.5s
    connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));

    timer2=new QTimer(this);
    timer2->start(10000); // 每次发射timeout信号时间间隔为15s
    connect(timer2,SIGNAL(timeout()),this,SLOT(timeUpdatesKey()));
    //消息框初始化
    message = new CDlgMessage(this);
    dlgmove = new DlgMove(this);
    //框架初始化
    bgflow->frame_init();
    //产能显示板计划值
//    update_thread->planboard(5000);
    //开机时蜂鸣器
    g_beeptype = 2;//上电初始化时蜂鸣器持续响几秒
    update_thread->beep();
    //
    ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd1_ok->setText("    ---    ");
    ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd2_ok->setText("    ---    ");
    ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd3_ok->setText("    ---    ");
    ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd4_ok->setText("    ---    ");
    //
    ui->checkBox_checkok->setVisible(false);
    ui->checkBox_dwok->setVisible(false);
    //
    if(g_par.is_usescanlight==1)
    {
       ui->label_scanlight->setEnabled(true);
       ui->radio_upscan->setEnabled(true);
       ui->value_upscan->setEnabled(true);
    }
    else if(g_par.is_usescanlight==0)
    {
       ui->label_scanlight->setEnabled(false);
       ui->radio_upscan->setEnabled(false);
       ui->value_upscan->setEnabled(false);
    }
    //获取注册信息
    g_registertype  = diff_time();
    //版本
    QString name,ver,modifytime;
    name = "TOP-8000";
    CIniRW version(LOGPATH);
    ver = version.ReadIni(name,"version").toString();
    modifytime = version.ReadIni(name,"date").toString();
    QString str = QObject::tr("版本:")+ ver+" "+ modifytime.left(8);//年月日
    ui->label_ver->setText(str);
    //
    if(g_language>1)
    {
        ui->btn_iopara->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_cncpara->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_home_select->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_language_switch->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_systemdebug->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_change_password->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_machine_switch->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_axis_showposition->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_mainpage2_exit->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_dp->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_dg->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_para_setting->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_systemcheck->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        ui->btn_systemexit->setStyleSheet("background:darkgray;font-size:10px;font:bold;");
        //
        ui->label_upoil->setStyleSheet("font-size:10px;font:bold;");
        ui->label_downoil->setStyleSheet("font-size:10px;font:bold;");
        ui->label_upframeair->setStyleSheet("font-size:10px;font:bold;");
        ui->label_upbg_finish->setStyleSheet("font-size:10px;font:bold;");
        ui->label_updw_jubao->setStyleSheet("font-size:10px;font:bold;");
        ui->label_downframeair->setStyleSheet("font-size:10px;font:bold;");
        ui->label_downbg_finish->setStyleSheet("font-size:10px;font:bold;");
        ui->label_downdw_jubao->setStyleSheet("font-size:10px;font:bold;");
        ui->label_light->setStyleSheet("font-size:10px;font:bold;");
        ui->label_power->setStyleSheet("font-size:10px;font:bold;");
        ui->label_count->setStyleSheet("font-size:10px;font:bold;");
        ui->label_upbgtime->setStyleSheet("font-size:10px;font:bold;");
        ui->label_time->setStyleSheet("font-size:10px;font:bold;");
        //
        ui->btn_ccd_down->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_platform_center->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_frame_absorb->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_dp_exit->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_autoset->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_mark_check->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_mark_change->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_ccd_lowspeed->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_save_mainmark->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_save_othermark->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_jt_dy->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_ccd_rw->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_position_down->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_position_lb->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_position_dw->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_position_bg->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_frame_change->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_dp_load->setStyleSheet("background:darkgray;font-size:10px;");
        ui->button_liaohao->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_dp_vacuum->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_dp_vacuum2->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_bm_vacuum->setStyleSheet("background:darkgray;font-size:10px;");
        ui->label_hb->setStyleSheet("background:darkgray;font-size:10px;");
        ui->label_ld->setStyleSheet("background:darkgray;font-size:10px;");
        //
        ui->btn_cool->setStyleSheet("background:darkgray;font-size:10px;");
       // ui->btn_taimian_binshuiji->setStyleSheet("background:darkgray;font-size:10px;");//冰水泵
        ui->btn_taimian_gufenji->setStyleSheet("background:darkgray;font-size:10px;");//鼓风机
        ui->btn_dp_zhengkon->setStyleSheet("background:darkgray;font-size:10px;");//底片真空
      //  ui->btn_taimian_ZKBen->setStyleSheet("background:darkgray;font-size:10px;");//台面真空泵
        ui->btn_CCD_on->setStyleSheet("background:darkgray;font-size:10px;");//CCD伺服转矩开启
        ui->btn_light->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_uplight_clear->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_dg_exit->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_bg_test->setStyleSheet("background:darkgray;font-size:10px;");
        ui->btn_oilsetting->setStyleSheet("background:darkgray;font-size:10px;");
    }
    //油墨类型显示
    slot_setlight(-1);
}

MainWindow::~MainWindow()
{
    exit_state();
    delete update_thread;
    delete currentTimeLabel;
    delete firsttimer;
    delete timer;
    delete inifilerw;
    delete bgflow;
    delete statusLabel;
    delete message;
    delete dlgmove;
    delete ui;
}
//程序退出时的动作
void MainWindow::exit_state()
{
    update_thread->threaddriver->CNC_Disable();
    update_thread->threaddriver->CNC_HardDisable();
}
//判断有无弹窗
bool MainWindow::NoTopDialog()
{
    bool ret = false;
    QWidget *pActiveWindow = QApplication::activeWindow();
    MainWindow *pMain = dynamic_cast<MainWindow*>(pActiveWindow);
    if(pMain && pActiveWindow == this)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}

//成员变量初始化
void MainWindow::init_member()
{
    m_errorframe =false;
    m_upbg_time[0]=0.0;
    m_upbg_time[1]=0.0;
    m_downbg_time[0]=0.0;
    m_downbg_time[1]=0.0;
    m_absorb_time=0.0;
    m_absorb_time2=0.0;
    g_lightstate=0;
    g_downlightstate = 0;
//    m_islighton=false;
    m_lighttime=0;
    m_downlighttime =0;
    m_is_weihu=false;//系统维护界面
    m_manualspeed = 600;
    ccdselect =0;
    m_isCCDdw[0] = false;
    m_isCCDdw[1] = false;
    m_isothermark[0] = false;
    m_isothermark[1] = false;
    str_alarm = "";
    memset(&alarmswitch_bit,0,sizeof(alrmcheck_bit));
    memset(axis_alarm_bit,0,35*sizeof(axischeck_bit));
    memset(&m_ccdbasic,0,sizeof(CCD_POS));
    m_zpos_type = -1;
    m_isaxis_alarm = false;
    m_isdpclear[0] = false;
    m_isdpclear[1] = false;
    m_isdpexchange[0] = false;
    m_isdpexchange[1] = false;
    m_bdpclear_show = false;
    m_isAlarmshow = false;
    m_pageindex = 0;
}
//检查参数文件及配置文件是否存在
bool MainWindow::IsSysFileExist()
{
    bool ret = true;
    QFileInfo confile = QFileInfo(CONGFILEPATH);
    QFileInfo topfile = QFileInfo(TOPPARAMETERPATH);
    QFileInfo markfile = QFileInfo(MARKPARAMETERPATH);
    QFileInfo machinefile = QFileInfo(MACHINECONGFILEPATH);
    QFileInfo cncparfile = QFileInfo(CNCPARAMETERPATH);
    QFileInfo logfile = QFileInfo(LOGFILEPATH);
    QFile tmpfile;
    if(!logfile.exists())
    {
         if(tmpfile.open(QIODevice::ReadWrite))
         {
            qDebug()<<QObject::tr("创建日志文件");
         }
    }
    if(!confile.exists())
    {
        ret = false;
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数文件丢失:")+confile.fileName());
    }
    if(!topfile.exists())
    {
        ret = false;
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数文件丢失:")+topfile.fileName());
    }
    if(!markfile.exists())
    {
        ret = false;
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数文件丢失:")+markfile.fileName());
    }
    if(!machinefile.exists())
    {
        ret = false;
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数文件丢失:")+machinefile.fileName());
    }
    if(!cncparfile.exists())
    {
        ret = false;
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数文件丢失:")+cncparfile.fileName());
    }
    return ret;
}
//参数初始化
void MainWindow::init_para()
{
    g_par.quick_door_time = inifilerw->ReadIni("TRAJ","ENERGY_TIME_LIMIT").toInt();
    g_par.min_bgtime = inifilerw->ReadIni("TRAJ","LIGHT_TRIGGER_DELAY").toInt();
    g_par.Frame_close_airdelay = inifilerw->ReadIni("TRAJ","AIR_FLOW_TIME").toInt();
    g_par.Frame_close_airdelay2 = inifilerw->ReadIni("TRAJ","AIR_FLOW_TIME2").toInt();
    g_par.Frame_finish_num = inifilerw->ReadIni("TOP","FRAME_FINISH_NUM").toInt();
    g_par.Frame2_finish_num = inifilerw->ReadIni("TOP","FRAME2_FINISH_NUM").toInt();
    g_par.JB_finish_num = inifilerw->ReadIni("TOP","JB_FINISH_NUM").toInt();
    g_par.JB2_finish_num = inifilerw->ReadIni("TOP","JB2_FINISH_NUM").toInt();
    g_par.light_num = inifilerw->ReadIni("TOP","LIGHT_NUM").toInt();
    g_par.light_time = inifilerw->ReadIni("TOP","LIGHT_TIME").toFloat();
    g_par.downlight_num = inifilerw->ReadIni("TOP","DOWNLIGHT_NUM").toInt();
    g_par.downlight_time = inifilerw->ReadIni("TOP","DOWNLIGHT_TIME").toFloat();
    g_par.bg_mode = inifilerw->ReadIni("TRAJ","ENERGY_OR_TIME").toInt();
    g_par.bg_down_time = inifilerw->ReadIni("TRAJ","XIA_DI_PIAN").toInt();
    g_par.bg_down_time2 = inifilerw->ReadIni("TRAJ","XIA_DI_PIAN2").toInt();
    g_par.bg_up_time =  inifilerw->ReadIni("TRAJ","SHANG_DI_PIAN").toInt();
    g_par.bg_up_time2 =  inifilerw->ReadIni("TRAJ","SHANG_DI_PIAN2").toInt();
    //速度参数
    g_par.x_speed = inifilerw->ReadIni("TOP","X_SPEED").toFloat();
    g_par.y_speed = inifilerw->ReadIni("TOP","Y_SPEED").toFloat();
    g_par.z_speed = inifilerw->ReadIni("TOP","Z_SPEED").toFloat();
    g_par.pt_speed = inifilerw->ReadIni("TOP","PT_SPEED").toFloat();
    g_par.downpt_speed = inifilerw->ReadIni("TOP","DOWNPT_SPEED").toFloat();
    g_par.z_midspeed = inifilerw->ReadIni("TOP","Z_MIDSPEED").toFloat();
    g_par.z_minspeed = inifilerw->ReadIni("TOP","Z_MINSPEED").toFloat();
    g_par.w_speed = inifilerw->ReadIni("TOP","W_SPEED").toFloat();
    //行程参数
    g_par.w_movelimit = inifilerw->ReadIni("TOP","W_LENGTH").toFloat();
    //上框
    g_par.up_max_value = inifilerw->ReadIni("TRAJ","ZMIN_DISTANCE").toFloat();//最低高度64
    g_par.up_bm_height = inifilerw->ReadIni("TRAJ","PRODUCT_THICKNESS_UM").toFloat();//板厚
    g_par.up_bm_height_mil = inifilerw->ReadIni("TRAJ","PRODUCT_THICKNESS_MIL").toFloat();
    g_par.up_dw_height = inifilerw->ReadIni("TRAJ","DUIWEI_DISTANCE").toFloat();//对位间隙
    g_par.up_lb_height = inifilerw->ReadIni("TRAJ","LB_DISTANCE").toFloat();//离板高度
    g_par.up_xlength = inifilerw->ReadIni("TRAJ","WIDTH2").toFloat();
    g_par.up_ylength = inifilerw->ReadIni("TRAJ","HEIGHT2").toFloat();
    g_up_lbposition = g_par.up_lb_height;
    g_up_dwposition = g_par.up_max_value - g_par.up_bm_height/1000.000 - g_par.up_dw_height/1000.000;
    g_up_bgposition = g_par.up_max_value - g_par.up_bm_height/1000.000;
    //下框
    g_par.down_max_value = inifilerw->ReadIni("TRAJ","ZMIN_DISTANCE2").toFloat();//最大高度
    g_par.down_bm_height = inifilerw->ReadIni("TRAJ","PRODUCT_THICKNESS_UM2").toFloat();//板厚
    g_par.down_bm_height_mil = inifilerw->ReadIni("TRAJ","PRODUCT_THICKNESS_MIL2").toFloat();
    g_par.down_dw_height = inifilerw->ReadIni("TRAJ","DUIWEI_DISTANCE2").toFloat();//对位间隙
    g_par.down_lb_height = inifilerw->ReadIni("TRAJ","LB_DISTANCE2").toFloat();//离板高度
    g_par.down_xlength = inifilerw->ReadIni("TRAJ","WIDTH").toFloat();
    g_par.down_ylength = inifilerw->ReadIni("TRAJ","HEIGHT").toFloat();
    g_down_lbposition = g_par.down_lb_height;
    g_down_dwposition = g_par.down_max_value - g_par.down_bm_height/1000.000 - g_par.down_dw_height/1000.000;
    g_down_bgposition = g_par.down_max_value - g_par.down_bm_height/1000.000;
    //
    g_par.bg_openzg = inifilerw->ReadIni("TRAJ","BGH_OPEN").toInt();
//    g_par.no_dw = inifilerw->ReadIni("TOP","NO_DW").toInt();
//    g_par.no_ch = inifilerw->ReadIni("TOP","NO_CH").toInt();
    g_par.move_delay = inifilerw->ReadIni("TOP","MOVE_DELAY").toFloat();
    g_par.bg_movedelay = inifilerw->ReadIni("TOP","BG_DELAY").toFloat();

    g_par.frame_close_delay = inifilerw->ReadIni("TRAJ","CLOSE_TIME").toInt();
    g_par.frame2_close_delay = inifilerw->ReadIni("TRAJ","CLOSE_TIME2").toInt();
    g_par.frame_open_delay = inifilerw->ReadIni("TRAJ","OPEN_DELAY").toInt();
    g_par.frame2_open_delay = inifilerw->ReadIni("TRAJ","OPEN_DELAY2").toInt();
    g_par.frame_open_flowdelay = inifilerw->ReadIni("TRAJ","OPEN_FRAME_FLOW_TIME").toInt();
    g_par.frame2_open_flowdelay = inifilerw->ReadIni("TRAJ","OPEN_FRAME_FLOW_TIME2").toInt();
    g_par.frame_close_delay_a = inifilerw->ReadIni("TRAJ","FRAME_CLOSE_DELAY_A").toInt();
    g_par.frame_close_delay_b = inifilerw->ReadIni("TRAJ","FRAME_CLOSE_DELAY_B").toInt();
    g_par.frame2_close_delay_a = inifilerw->ReadIni("TRAJ","FRAME2_CLOSE_DELAY_A").toInt();
    g_par.frame2_close_delay_b = inifilerw->ReadIni("TRAJ","FRAME2_CLOSE_DELAY_B").toInt();
    //us *1000->ms
    g_par.JT_airin_delay = inifilerw->ReadIni("TRAJ","JIAOTIAO_CHONGQI_DELAY_TIME").toInt()*1000;
    g_par.JT_airout_delay = inifilerw->ReadIni("TRAJ","JIAOTIAO_PAIQI_TIME").toInt()*1000;
    g_par.JT2_airin_delay = inifilerw->ReadIni("TRAJ","JIAOTIAO_CHONGQI_DELAY_TIME2").toInt()*1000;
    g_par.JT2_airout_delay = inifilerw->ReadIni("TRAJ","JIAOTIAO_PAIQI_TIME2").toInt()*1000;
    g_par.Frame_select = 0;//inifilerw->ReadIni("TOP","FRAME_SELECT").toInt();
    //
    g_par.TB2_delay = inifilerw->ReadIni("TOP","TB_DELAY2").toInt()*1000;
    g_par.TB_delay = inifilerw->ReadIni("TOP","TB_DELAY").toInt()*1000;
    //
    g_par.bg_up_delaytime = inifilerw->ReadIni("TRAJ","BAO_GUANG_DELAY").toInt();
    g_par.bg_down_delaytime = inifilerw->ReadIni("TRAJ","BAO_GUANG_DELAY2").toInt();
    g_par.bg_up_energy = inifilerw->ReadIni("TRAJ","BAO_GUANG_ENERGY").toFloat();
    g_par.bg_up_energyoveride = inifilerw->ReadIni("TRAJ","ENERGY_OVERRIDE").toFloat();
    g_par.bg_down_energy = inifilerw->ReadIni("TRAJ","BAO_GUANG_ENERGY2").toFloat();
    g_par.bg_down_energyoveride = inifilerw->ReadIni("TRAJ","ENERGY_OVERRIDE2").toFloat();
    g_par.DP_clear_num = inifilerw->ReadIni("TOP","DP_CLEAR_NUM").toInt();
    g_par.DP2_clear_num = inifilerw->ReadIni("TOP","DP2_CLEAR_NUM").toInt();
    g_par.DP_maxclear_num = inifilerw->ReadIni("TRAJ","CLEAN_DIPIAN_COUNT").toInt();
    g_par.DP2_maxclear_num = inifilerw->ReadIni("TRAJ","CLEAN_DIPIAN_COUNT2").toInt();
    g_par.DP_BG_NUM = inifilerw->ReadIni("TOP","DP_BG_NUM").toInt();
    g_par.DP2_BG_NUM = inifilerw->ReadIni("TOP","DP2_BG_NUM").toInt();
    g_par.DP_BG_MAXNUM = inifilerw->ReadIni("TRAJ","DIPIAN_COUNT").toInt();
    g_par.DP2_BG_MAXNUM = inifilerw->ReadIni("TRAJ","DIPIAN_COUNT2").toInt();
    g_par.isbg_finishairkeep = inifilerw->ReadIni("TRAJ","AIR_KEEP").toInt();
    g_par.bg_switchboardtime = inifilerw->ReadIni("TRAJ","QU_FANG_TIME").toInt();

    g_par.ccd_234[0] = inifilerw->ReadIni("TRAJ","CCD_234").toInt();
    g_par.ccd_234[1] = inifilerw->ReadIni("TRAJ","CCD_234_2").toInt();
    g_par.downccd_234[0] = inifilerw->ReadIni("TRAJ","DOWNCCD_234").toInt();
    g_par.downccd_234[1] = inifilerw->ReadIni("TRAJ","DOWNCCD_234_2").toInt();
    g_par.CCD_check_time = inifilerw->ReadIni("TRAJ","CHECK_DELAY").toInt();
    g_par.CCD_check_time2 = inifilerw->ReadIni("TRAJ","CHECK_DELAY2").toInt();
    g_par.DownCCD_check_time = inifilerw->ReadIni("TRAJ","DOWNCHECK_DELAY").toInt();
    g_par.DownCCD_check_time2 = inifilerw->ReadIni("TRAJ","DOWNCHECK_DELAY2").toInt();

    g_par.DW_max_num = inifilerw->ReadIni("TRAJ","DW_MAX_NUM").toInt();
    g_par.DW_max_num2 = inifilerw->ReadIni("TRAJ","DW_MAX_NUM2").toInt();
    g_par.DownDW_max_num = inifilerw->ReadIni("TRAJ","DOWNDW_MAX_NUM").toInt();
    g_par.DownDW_max_num2 = inifilerw->ReadIni("TRAJ","DOWNDW_MAX_NUM2").toInt();
    g_par.check8[0] = inifilerw->ReadIni("TOP","CHECK8").toInt();
    g_par.check8[1] = inifilerw->ReadIni("TOP","CHECK8_2").toInt();
    g_par.checkdown8[0] = inifilerw->ReadIni("TOP","DOWNCHECK8").toInt();
    g_par.checkdown8[1] = inifilerw->ReadIni("TOP","DOWNCHECK8_2").toInt();

    g_par.dw_up_jingdu = inifilerw->ReadIni("TRAJ","DUIWEI_FINISH_PRECISION").toFloat();//要改对位精度
    g_par.dw_down_jingdu = inifilerw->ReadIni("TRAJ","DUIWEI_FINISH_PRECISION2").toFloat();
    g_par.DP_up_error_x = inifilerw->ReadIni("TRAJ","DIPIAN_WUCHA_X").toFloat();
    g_par.DP_up_error_y = inifilerw->ReadIni("TRAJ","DIPIAN_WUCHA_Y").toFloat();
    g_par.DP_down_error_x=inifilerw->ReadIni("TRAJ","DIPIAN_WUCHA_X2").toFloat();
    g_par.DP_down_error_y=inifilerw->ReadIni("TRAJ","DIPIAN_WUCHA_Y2").toFloat();
    g_par.DP_up_junfen_x =inifilerw->ReadIni("TRAJ","JUNFEN_WUCHA_X").toFloat();
    g_par.DP_up_junfen_y =inifilerw->ReadIni("TRAJ","JUNFEN_WUCHA_Y").toFloat();
    g_par.DP_down_junfen_x =inifilerw->ReadIni("TRAJ","JUNFEN_WUCHA_X2").toFloat();
    g_par.DP_down_junfen_y =inifilerw->ReadIni("TRAJ","JUNFEN_WUCHA_Y2").toFloat();
    g_par.film_up_dia =inifilerw->ReadIni("TRAJ","UP_FILM_D").toFloat();
    g_par.film_down_dia =inifilerw->ReadIni("TRAJ","DOWN_FILM_D").toFloat();
    g_par.board_up_dia =inifilerw->ReadIni("TRAJ","BOARD_D").toFloat();
    g_par.board_down_dia =inifilerw->ReadIni("TRAJ","BOARD_D2").toFloat();

    g_par.Downdw_up_jingdu = inifilerw->ReadIni("TRAJ","DOWNDUIWEI_FINISH_PRECISION").toFloat();
    g_par.Downdw_down_jingdu = inifilerw->ReadIni("TRAJ","DOWNDUIWEI_FINISH_PRECISION2").toFloat();
    g_par.DownDP_up_error_x = inifilerw->ReadIni("TRAJ","DOWNDIPIAN_WUCHA_X").toFloat();
    g_par.DownDP_up_error_y = inifilerw->ReadIni("TRAJ","DOWNDIPIAN_WUCHA_Y").toFloat();
    g_par.DownDP_down_error_x=inifilerw->ReadIni("TRAJ","DOWNDIPIAN_WUCHA_X2").toFloat();
    g_par.DownDP_down_error_y=inifilerw->ReadIni("TRAJ","DOWNDIPIAN_WUCHA_Y2").toFloat();
    g_par.DownDP_up_junfen_x =inifilerw->ReadIni("TRAJ","DOWNJUNFEN_WUCHA_X").toFloat();
    g_par.DownDP_up_junfen_y =inifilerw->ReadIni("TRAJ","DOWNJUNFEN_WUCHA_Y").toFloat();
    g_par.DownDP_down_junfen_x =inifilerw->ReadIni("TRAJ","DOWNJUNFEN_WUCHA_X2").toFloat();
    g_par.DownDP_down_junfen_y =inifilerw->ReadIni("TRAJ","DOWNJUNFEN_WUCHA_Y2").toFloat();
    g_par.Downfilm_up_dia =inifilerw->ReadIni("TRAJ","DOWNUP_FILM_D").toFloat();
    g_par.Downfilm_down_dia =inifilerw->ReadIni("TRAJ","DOWNDOWN_FILM_D").toFloat();
    g_par.Downboard_up_dia =inifilerw->ReadIni("TRAJ","DOWNBOARD_D").toFloat();
    g_par.Downboard_down_dia =inifilerw->ReadIni("TRAJ","DOWNBOARD_D2").toFloat();

    g_par.up_ccd_select = inifilerw->ReadIni("TRAJ","CCD_SWITCH").toInt();
    g_par.down_ccd_select = inifilerw->ReadIni("TRAJ","CCD_SWITCH2").toInt();
    g_par.up_downccd_select = inifilerw->ReadIni("TRAJ","DOWNCCD_SWITCH").toInt();
    g_par.down_downccd_select = inifilerw->ReadIni("TRAJ","DOWNCCD_SWITCH2").toInt();
    //
    g_par.full_half = inifilerw->ReadIni("TRAJ","QUANGONG_OR_BANGONG").toInt();
    g_par.auto_manual = inifilerw->ReadIni("TRAJ","AUTO_OR_MANUAL").toInt();
    g_par.dw_up_kxybase = inifilerw->ReadIni("TRAJ","DW_KXY_BASE").toInt();
    g_par.dw_down_kxybase = inifilerw->ReadIni("TRAJ","DW_KXY_BASE2").toInt();
    g_par.up_CheckDis_GC1 = inifilerw->ReadIni("TRAJ","GC1").toInt();
    g_par.down_CheckDis_GC2 = inifilerw->ReadIni("TRAJ","GC2").toInt();
    g_par.isBinary = inifilerw->ReadIni("TOP","EZH").toInt();
    g_par.isopensuperpar = inifilerw->ReadIni("TOP","OpenSuperman").toInt();
    //CCD 基准偏差值
    m_ccdbasic.x1 = inifilerw->ReadIni("TOP","CCD1_5XDIS").toFloat();
    m_ccdbasic.x2 = inifilerw->ReadIni("TOP","CCD2_6XDIS").toFloat();
    m_ccdbasic.x3 = inifilerw->ReadIni("TOP","CCD3_7XDIS").toFloat();
    m_ccdbasic.x4 = inifilerw->ReadIni("TOP","CCD4_8XDIS").toFloat();
    m_ccdbasic.y1 = inifilerw->ReadIni("TOP","CCD1_5YDIS").toFloat();
    m_ccdbasic.y2 = inifilerw->ReadIni("TOP","CCD2_6YDIS").toFloat();
    m_ccdbasic.y3 = inifilerw->ReadIni("TOP","CCD3_7YDIS").toFloat();
    m_ccdbasic.y4 = inifilerw->ReadIni("TOP","CCD4_8YDIS").toFloat();
    //mark parameter
    g_par.ch_othermark[0][0] = inifilerw->ReadIni("TOP","OTHERMARK1_1").toInt();
    g_par.ch_othermark[0][1] = inifilerw->ReadIni("TOP","OTHERMARK1_2").toInt();
    g_par.ch_othermark[0][2] = inifilerw->ReadIni("TOP","OTHERMARK1_3").toInt();
    g_par.ch_othermark[0][3] = inifilerw->ReadIni("TOP","OTHERMARK1_4").toInt();
    g_par.ch_othermark[1][0] = inifilerw->ReadIni("TOP","OTHERMARK2_1").toInt();
    g_par.ch_othermark[1][1] = inifilerw->ReadIni("TOP","OTHERMARK2_2").toInt();
    g_par.ch_othermark[1][2] = inifilerw->ReadIni("TOP","OTHERMARK2_3").toInt();
    g_par.ch_othermark[1][3] = inifilerw->ReadIni("TOP","OTHERMARK2_4").toInt();

    g_par.ch_downothermark[0][0] = inifilerw->ReadIni("TOP","DOWNOTHERMARK1_1").toInt();
    g_par.ch_downothermark[0][1] = inifilerw->ReadIni("TOP","DOWNOTHERMARK1_2").toInt();
    g_par.ch_downothermark[0][2] = inifilerw->ReadIni("TOP","DOWNOTHERMARK1_3").toInt();
    g_par.ch_downothermark[0][3] = inifilerw->ReadIni("TOP","DOWNOTHERMARK1_4").toInt();
    g_par.ch_downothermark[1][0] = inifilerw->ReadIni("TOP","DOWNOTHERMARK2_1").toInt();
    g_par.ch_downothermark[1][1] = inifilerw->ReadIni("TOP","DOWNOTHERMARK2_2").toInt();
    g_par.ch_downothermark[1][2] = inifilerw->ReadIni("TOP","DOWNOTHERMARK2_3").toInt();
    g_par.ch_downothermark[1][3] = inifilerw->ReadIni("TOP","DOWNOTHERMARK2_4").toInt();
    //
    g_par.ccdy_mindis = inifilerw->ReadIni("TOP","CCD_Y_MIN").toFloat();
    g_par.ccdy_maxdis = inifilerw->ReadIni("TOP","CCD_Y_MAX").toFloat();
    //
    g_par.screen_waittime = inifilerw->ReadIni("TOP","SCREEN_WAIT").toInt();
    //上下灯扫描轴行程
    g_par.scan_length[0] = inifilerw->ReadIni("TOP","UPSACN_LEN").toFloat();
    g_par.scan_length[1] = inifilerw->ReadIni("TOP","DOWNSCAN_LEN").toFloat();
    g_par.is_usescanlight = inifilerw->ReadIni("TOP","ISUSESCAN").toInt();
    g_par.light_speed = inifilerw->ReadIni("TOP","LIGHTMOVE").toFloat();
    g_par.is_dpvacuum = inifilerw->ReadIni("TOP","ISDPVACUUM").toInt();
    g_par.ischecksafe = inifilerw->ReadIni("TOP","ISCHECKSAFE").toInt();
    g_par.lightmode = inifilerw->ReadIni("TOP","LIGHTMODE").toInt();
    g_par.islightsetting = inifilerw->ReadIni("TOP","ISLIGHTSETTING").toInt();
    qDebug("is_dpvacuum=%d\n",g_par.is_dpvacuum);
    //轴名称获取
    QString name;
    m_axisname.clear();
    for(int a=0;a<=AXIS_UPSCAN;a++)
    {
        QString tmp;
        tmp.sprintf("%d",a);
        name= inifilerw->ReadIni("AXIS_NAME",tmp).toString();
        m_axisname.append(name);
    }
    //LMR positive Limit
    CIniRW axispar_file(CONGFILEPATH);
    QString group;
    //上平台
    group.sprintf("AUX_%d",AXIS_L);
    lmr_PLimit[0].value_l= axispar_file.ReadIni(group,"MAX_LIMIT").toFloat();
    lmr_NLimit[0].value_l= axispar_file.ReadIni(group,"MIN_LIMIT").toFloat();
    group.sprintf("AUX_%d",AXIS_M);
    lmr_PLimit[0].value_m= axispar_file.ReadIni(group,"MAX_LIMIT").toFloat();
    lmr_NLimit[0].value_m= axispar_file.ReadIni(group,"MIN_LIMIT").toFloat();
    group.sprintf("AUX_%d",AXIS_R);
    lmr_PLimit[0].value_r= axispar_file.ReadIni(group,"MAX_LIMIT").toFloat();
    lmr_NLimit[0].value_r= axispar_file.ReadIni(group,"MIN_LIMIT").toFloat();
    lmr_PLimit[1] = lmr_PLimit[0];
    lmr_NLimit[1] = lmr_NLimit[0];
    //ccd hb ld
    CIniRW ccdfile(MARKPARAMETERPATH);
    //靶点数据
    g_par.main_markpos[0].x1=ccdfile.ReadIni("MAIN_MARK1","ccd1x").toFloat();
    g_par.main_markpos[0].x2=ccdfile.ReadIni("MAIN_MARK1","ccd2x").toFloat();
    g_par.main_markpos[0].x3=ccdfile.ReadIni("MAIN_MARK1","ccd3x").toFloat();
    g_par.main_markpos[0].x4=ccdfile.ReadIni("MAIN_MARK1","ccd4x").toFloat();
    g_par.main_markpos[0].y1=ccdfile.ReadIni("MAIN_MARK1","ccd1y").toFloat();
    g_par.main_markpos[0].y2=ccdfile.ReadIni("MAIN_MARK1","ccd2y").toFloat();
    g_par.main_markpos[0].y3=ccdfile.ReadIni("MAIN_MARK1","ccd3y").toFloat();
    g_par.main_markpos[0].y4=ccdfile.ReadIni("MAIN_MARK1","ccd4y").toFloat();

    g_par.main_markpos[1].x1=ccdfile.ReadIni("MAIN_MARK2","ccd1x").toFloat();
    g_par.main_markpos[1].x2=ccdfile.ReadIni("MAIN_MARK2","ccd2x").toFloat();
    g_par.main_markpos[1].x3=ccdfile.ReadIni("MAIN_MARK2","ccd3x").toFloat();
    g_par.main_markpos[1].x4=ccdfile.ReadIni("MAIN_MARK2","ccd4x").toFloat();
    g_par.main_markpos[1].y1=ccdfile.ReadIni("MAIN_MARK2","ccd1y").toFloat();
    g_par.main_markpos[1].y2=ccdfile.ReadIni("MAIN_MARK2","ccd2y").toFloat();
    g_par.main_markpos[1].y3=ccdfile.ReadIni("MAIN_MARK2","ccd3y").toFloat();
    g_par.main_markpos[1].y4=ccdfile.ReadIni("MAIN_MARK2","ccd4y").toFloat();

    g_par.other_markpos[0].x1=ccdfile.ReadIni("OTHER_MARK1","ccd1x").toFloat();
    g_par.other_markpos[0].x2=ccdfile.ReadIni("OTHER_MARK1","ccd2x").toFloat();
    g_par.other_markpos[0].x3=ccdfile.ReadIni("OTHER_MARK1","ccd3x").toFloat();
    g_par.other_markpos[0].x4=ccdfile.ReadIni("OTHER_MARK1","ccd4x").toFloat();
    g_par.other_markpos[0].y1=ccdfile.ReadIni("OTHER_MARK1","ccd1y").toFloat();
    g_par.other_markpos[0].y2=ccdfile.ReadIni("OTHER_MARK1","ccd2y").toFloat();
    g_par.other_markpos[0].y3=ccdfile.ReadIni("OTHER_MARK1","ccd3y").toFloat();
    g_par.other_markpos[0].y4=ccdfile.ReadIni("OTHER_MARK1","ccd4y").toFloat();

    g_par.other_markpos[1].x1=ccdfile.ReadIni("OTHER_MARK2","ccd1x").toFloat();
    g_par.other_markpos[1].x2=ccdfile.ReadIni("OTHER_MARK2","ccd2x").toFloat();
    g_par.other_markpos[1].x3=ccdfile.ReadIni("OTHER_MARK2","ccd3x").toFloat();
    g_par.other_markpos[1].x4=ccdfile.ReadIni("OTHER_MARK2","ccd4x").toFloat();
    g_par.other_markpos[1].y1=ccdfile.ReadIni("OTHER_MARK2","ccd1y").toFloat();
    g_par.other_markpos[1].y2=ccdfile.ReadIni("OTHER_MARK2","ccd2y").toFloat();
    g_par.other_markpos[1].y3=ccdfile.ReadIni("OTHER_MARK2","ccd3y").toFloat();
    g_par.other_markpos[1].y4=ccdfile.ReadIni("OTHER_MARK2","ccd4y").toFloat();
    //靶点数据2
    g_par.main_downmarkpos[0].x1=ccdfile.ReadIni("MAIN_MARK1","ccd5x").toFloat();
    g_par.main_downmarkpos[0].x2=ccdfile.ReadIni("MAIN_MARK1","ccd6x").toFloat();
    g_par.main_downmarkpos[0].x3=ccdfile.ReadIni("MAIN_MARK1","ccd7x").toFloat();
    g_par.main_downmarkpos[0].x4=ccdfile.ReadIni("MAIN_MARK1","ccd8x").toFloat();
    g_par.main_downmarkpos[0].y1=ccdfile.ReadIni("MAIN_MARK1","ccd5y").toFloat();
    g_par.main_downmarkpos[0].y2=ccdfile.ReadIni("MAIN_MARK1","ccd6y").toFloat();
    g_par.main_downmarkpos[0].y3=ccdfile.ReadIni("MAIN_MARK1","ccd7y").toFloat();
    g_par.main_downmarkpos[0].y4=ccdfile.ReadIni("MAIN_MARK1","ccd8y").toFloat();

    g_par.main_downmarkpos[1].x1=ccdfile.ReadIni("MAIN_MARK2","ccd5x").toFloat();
    g_par.main_downmarkpos[1].x2=ccdfile.ReadIni("MAIN_MARK2","ccd6x").toFloat();
    g_par.main_downmarkpos[1].x3=ccdfile.ReadIni("MAIN_MARK2","ccd7x").toFloat();
    g_par.main_downmarkpos[1].x4=ccdfile.ReadIni("MAIN_MARK2","ccd8x").toFloat();
    g_par.main_downmarkpos[1].y1=ccdfile.ReadIni("MAIN_MARK2","ccd5y").toFloat();
    g_par.main_downmarkpos[1].y2=ccdfile.ReadIni("MAIN_MARK2","ccd6y").toFloat();
    g_par.main_downmarkpos[1].y3=ccdfile.ReadIni("MAIN_MARK2","ccd7y").toFloat();
    g_par.main_downmarkpos[1].y4=ccdfile.ReadIni("MAIN_MARK2","ccd8y").toFloat();

    g_par.other_downmarkpos[0].x1=ccdfile.ReadIni("OTHER_MARK1","ccd5x").toFloat();
    g_par.other_downmarkpos[0].x2=ccdfile.ReadIni("OTHER_MARK1","ccd6x").toFloat();
    g_par.other_downmarkpos[0].x3=ccdfile.ReadIni("OTHER_MARK1","ccd7x").toFloat();
    g_par.other_downmarkpos[0].x4=ccdfile.ReadIni("OTHER_MARK1","ccd8x").toFloat();
    g_par.other_downmarkpos[0].y1=ccdfile.ReadIni("OTHER_MARK1","ccd5y").toFloat();
    g_par.other_downmarkpos[0].y2=ccdfile.ReadIni("OTHER_MARK1","ccd6y").toFloat();
    g_par.other_downmarkpos[0].y3=ccdfile.ReadIni("OTHER_MARK1","ccd7y").toFloat();
    g_par.other_downmarkpos[0].y4=ccdfile.ReadIni("OTHER_MARK1","ccd8y").toFloat();

    g_par.other_downmarkpos[1].x1=ccdfile.ReadIni("OTHER_MARK2","ccd5x").toFloat();
    g_par.other_downmarkpos[1].x2=ccdfile.ReadIni("OTHER_MARK2","ccd6x").toFloat();
    g_par.other_downmarkpos[1].x3=ccdfile.ReadIni("OTHER_MARK2","ccd7x").toFloat();
    g_par.other_downmarkpos[1].x4=ccdfile.ReadIni("OTHER_MARK2","ccd8x").toFloat();
    g_par.other_downmarkpos[1].y1=ccdfile.ReadIni("OTHER_MARK2","ccd5y").toFloat();
    g_par.other_downmarkpos[1].y2=ccdfile.ReadIni("OTHER_MARK2","ccd6y").toFloat();
    g_par.other_downmarkpos[1].y3=ccdfile.ReadIni("OTHER_MARK2","ccd7y").toFloat();
    g_par.other_downmarkpos[1].y4=ccdfile.ReadIni("OTHER_MARK2","ccd8y").toFloat();
    for(int i=0;i<8;i++)
    {
        QString group;
        group.sprintf("CCD_%d",i+1);
        g_par.ccd_upld[0][i]=ccdfile.ReadIni(group,"JS_LD").toInt();
        g_par.ccd_uphb[0][i]=ccdfile.ReadIni(group,"JS_HB").toInt();
        g_par.ccd_upld[1][i]=ccdfile.ReadIni(group,"SD_LD").toInt();
        g_par.ccd_uphb[1][i]=ccdfile.ReadIni(group,"SD_HB").toInt();
        g_par.ccd_downld[0][i]=ccdfile.ReadIni(group,"JS_LD1").toInt();
        g_par.ccd_downhb[0][i]=ccdfile.ReadIni(group,"JS_HB1").toInt();
        g_par.ccd_downld[1][i]=ccdfile.ReadIni(group,"SD_LD1").toInt();
        g_par.ccd_downhb[1][i]=ccdfile.ReadIni(group,"SD_HB1").toInt();
    }
    //加载语言参数
    CIniRW lanconf(MACHINECONGFILEPATH);
    g_language = lanconf.ReadIni("LANGUAGE","language").toInt();
}
/***
 *
 ***/
void MainWindow::on_btn_axis_showposition_clicked()
{
    CAxisPositionShow * dlg_axisposition = new CAxisPositionShow(this);
    dlg_axisposition->show();
}
/***
 * 系统退出
 ***/
void MainWindow::click_btn_exit()
{
    g_beeptype = 1;
    //判断灯
    if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("未熄灯,请先熄灯!"));
        return;
    }
    bool res = slot_alarmshow(true,QObject::tr("确定退出系统?"));
//    QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("确定退出系统?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//    if(rb == QMessageBox::Yes)
    if(res)
    {
        disconnect(this, 0, 0, 0);//断开连接到主框架的信号
        //bgflow->frame_init();//恢复上电状态
        this->close();
    }
}
/***
 * 参数设置
 ***/
void MainWindow::click_btn_parameter()
{
   g_beeptype = 1;
   DlgLogin login(0,QObject::tr("输入密码"),this);
   login.exec();
   if(login.iscorrect)
   {
       if(g_settings_kind!=13)
       {
           qDebug()<<"enter bgparameter!";
           dlgBGParameter bgpar(this);
           bgpar.exec();
       }
       else
       {
           qDebug()<<"enter superparameter!";
           DlgSuperParaSetting superpar(this);
           superpar.exec();
       }
   }

}
/***
 * 复位
 ***/
void MainWindow::click_btn_reset()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
//        qDebug("系统运行中，不能回零\n");
        slot_alarmshow(false,QObject::tr("系统运行中，不能复位!"));
        return;
    }
    if(!update_thread->input.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("未释放急停开关!"));
        return;
    }
    if(true==update_thread->m_bHard_state)
    {
        slot_alarmshow(false,QObject::tr("通信错误,请检查线路后重启系统!"));
        return;
    }
//    if(!update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check"))
//    {
//        slot_alarmshow(false,QObject::tr("上框架未关闭,请先关闭框架！"));
//        return;
//    }
//    if(!update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2"))
//    {
//        slot_alarmshow(false,QObject::tr("下框架未关闭,请先关闭框架!"));
//        return;
//    }
    if(update_thread->input.Sys_air)
    {
        slot_alarmshow(false,QObject::tr("系统空压异常"));
        return;
    }
    DlgHome dlghome(this);
    dlghome.exec();
}
//
void MainWindow::check_regstate()
{
    if(AHourPassFlag == 1)
    {
        AHourPassFlag = 0;
        nDayLeave = nRemainHour / 24;
        if( g_registertype == LONGTIME )
        {
            reg_info = QObject::tr("已注册");
        }
        else if(g_registertype == UNREG)
        {
            reg_info = QObject::tr("已过期");
        }
        else
        {
            if(nDayLeave > 0)
            {
                if(nDayLeave  + 1 > 5)
                   reg_info = QObject::tr("已授权");
                else
                   reg_info =QString::number(nDayLeave+1,10) + QObject::tr("天后过期");
            }
            else
            {
                reg_info =QString::number(nRemainHour,10) + QObject::tr("小时后过期");
            }
        }
        if(g_registertype != LONGTIME)
        {
                if(nDayLeave < 5)
                {
                    switch(nDayLeave)
                    {
                        case FIVE_LEFT:
                             slot_alarmshow(false,QObject::tr("授权时间还剩5天,请联系原厂处理!"));
                             break;
                        case FOUR_LEFT:
                            slot_alarmshow(false,QObject::tr("授权时间还剩4天,请联系原厂处理!"));
                            break;
                        case THREE_LEFT:
                            slot_alarmshow(false,QObject::tr("授权时间还剩3天,请联系原厂处理!"));
                            break;
                        case TWO_LEFT:
                            slot_alarmshow(false,QObject::tr("授权时间还剩2天,请联系原厂处理!"));
                            break;
                        case ONE_LEFT:
                            if(nRemainHour <= 0)
                            {
                                  slot_alarmshow(false,QObject::tr("授权时间到达，请联系原厂处理！"));
                            }
                            else
                            {
                                  slot_alarmshow(false,QObject::tr("授权时间还剩1天,请联系原厂处理!"));
                            }
                            break;
                        default:
                            slot_alarmshow(false,QObject::tr("授权时间到达，请联系原厂处理！"));
                            break;
                    }
                }
        }
        if(g_registertype == UNREG)
        {
            slot_alarmshow(false,QObject::tr("授权时间到达，请联系原厂处理！"));
        }
        if(g_registertype != LONGTIME )
        {
            if(nRemainHour >= 1)
            {
                --nRemainHour;
            }
            else
            {
                g_registertype = UNREG;
            }
        }
    }
}

//
void MainWindow::slot_update()
{
    //检查系统注册状态
    check_regstate();
    //检测IO板卡通信是否正常
    static bool old_hardstate =false;
    static int delaycnt = 0;
    delaycnt++;
    if(delaycnt>600)//上电延时3秒后检测通信
    {
        delaycnt = 601;
        if(true==update_thread->m_bHard_state)
        {
            str_alarm = QObject::tr("通信错误,请检查线路后重启系统!");
            showtips(ERROR_TIP,str_alarm);
            if(!old_hardstate)
            {
                slot_alarmshow(false,str_alarm);
                old_hardstate = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
                if(update_thread->m_bCnc_state==1)
                {
                   emit sig_estop();
                }
            }
        }
        else
        {
            if(old_hardstate)
            {
               old_hardstate = false;
               showtips(NULL_TIP,"");
            }
        }
    }
    //
    check_inputalarm();
    axis_showpos();
    check_lightstate();
    check_state();
    //机器开关状态显示
    if(update_thread->m_bCnc_state==1)
    {
        ui->btn_machine_switch->setText(QObject::tr("机器开"));
        if(g_language>1)
        {
            ui->btn_machine_switch->setStyleSheet("background-color:rgb(0,255,0);font-size:10px;font:bold;");
        }
        else
        {
            ui->btn_machine_switch->setStyleSheet("background-color:rgb(0,255,0);");
        }
    }
    else
    {
        ui->btn_machine_switch->setText(QObject::tr("机器关"));
        if(g_language>1)
        {
            ui->btn_machine_switch->setStyleSheet("background-color:rgb(255,0,0);font-size:10px;font:bold;");
        }
        else
        {
            ui->btn_machine_switch->setStyleSheet("background-color:rgb(255,0,0);");
        }
    }
    //系统状态显示更新
    if(g_ishomed==true)
    {
        if(g_isSysRunning||g_isSysContinueRun||g_isBgRunning||(update_thread->m_bAxis_state==0))//曝光或轴运动
        {
            ui->status_system->setText("RUN");
            ui->status_system->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
        }
        else
        {
            ui->status_system->setText("STOP");
            ui->status_system->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
        }
    }
    else
    {
          ui->status_system->setText(QObject::tr("未复位"));
          ui->status_system->setStyleSheet("background-color:black;color:rgb(200,20,20);font:bold");
    }
    //回零后检测轴报警
    if(g_ishomed==1)
    {
       //检测轴报警
       check_axisalarm();
       //检测轴软件限位
//       check_softlimit();
//       check_orglimit();
    }
    //按钮状态
    btn_state();
    //更新参数
    if(g_reloadpara)
    {
        g_reloadpara = false;
        update_thread->par_update(g_now_frame);
    }
    //框架更新同步更新参数
    if(((update_thread->m_last_frame!=g_now_frame)||(true==g_loadmaterial))&&(g_now_frame<=1)&&(g_now_frame>=0))
    {
       if(update_thread->m_last_frame!=g_now_frame)
       {
           update_thread->m_last_frame=g_now_frame;
           emit sig_setlight(g_now_frame);
       }
       if(g_loadmaterial)  g_loadmaterial = false;
       update_thread->par_update(g_now_frame);
       if((g_now_frame==0)||(g_now_frame==1))
       {
           update_thread->threaddriver->CCDSend_LMRLimit(0,lmr_PLimit[g_now_frame],lmr_NLimit[g_now_frame]);
           Mark_load(0);
           update_thread->threaddriver->CCDSend_LMRLimit(1,downlmr_PLimit[g_now_frame],downlmr_NLimit[g_now_frame]);
           Mark_load(1);
       }
    }
    //灰阶
    ccdpar_select();
    //CCD与LMR坐标上传
    if((g_now_frame==0)||(g_now_frame==1))
    {
        CCDLMR_POS lmr_curpos;
        CCD_POS ccd_curpos;
        ccd_curpos.x1 = update_thread->m_axispos[AXIS_CCD1X];
        ccd_curpos.x2 = update_thread->m_axispos[AXIS_CCD2X];
        ccd_curpos.x3 = update_thread->m_axispos[AXIS_CCD3X];
        ccd_curpos.x4 = update_thread->m_axispos[AXIS_CCD4X];
        ccd_curpos.y1 = update_thread->m_axispos[AXIS_CCD1Y];
        ccd_curpos.y2 = update_thread->m_axispos[AXIS_CCD2Y];
        ccd_curpos.y3 = update_thread->m_axispos[AXIS_CCD3Y];
        ccd_curpos.y4 = update_thread->m_axispos[AXIS_CCD4Y];
        lmr_curpos.value_l = update_thread->m_axispos[AXIS_L];
        lmr_curpos.value_m = update_thread->m_axispos[AXIS_M];
        lmr_curpos.value_r = update_thread->m_axispos[AXIS_R];
        update_thread->threaddriver->CCDSend_LMRCCDPos(0,ccd_curpos,lmr_curpos);
    }


    if(g_now_frame==0)
    {
        ui->value_ld->setText(QVariant(g_par.ccd_upld[update_thread->sd][ccdselect]).toString());
        ui->value_hb->setText(QVariant(g_par.ccd_uphb[update_thread->sd][ccdselect]).toString());
    }
    else if(g_now_frame==1)
    {
        ui->value_ld->setText(QVariant(g_par.ccd_downld[update_thread->sd][ccdselect]).toString());
        ui->value_hb->setText(QVariant(g_par.ccd_downhb[update_thread->sd][ccdselect]).toString());
    }
    else
    {
        ui->value_ld->setText("0");
        ui->value_hb->setText("0");
    }
    //手动速度
    m_manualspeed = ui->spin_speed->value();
    //手动模式非运行模式下屏保动作
    if((1==g_par.auto_manual)&&(false==g_isSysRunning))
    {
        static bool old_isScreenStart = false;
        if(old_isScreenStart!=g_isScreenStart)
        {
            if(g_isScreenStart)
            {
                bgflow->thread_pause(false);
            }
        }
        old_isScreenStart = g_isScreenStart;
    }
    //底片清洁 与 更换菲林
    if(!g_isBgRunning)
    {
        if(g_now_frame==0)
        {
            //清洁
            if((g_par.DP_clear_num>=g_par.DP_maxclear_num)&&(!m_bdpclear_show)
                    &&(g_par.DP_BG_NUM<g_par.DP_BG_MAXNUM))
            {
               m_isdpclear[0] = true;
            }
            static bool old_updpclear = false;
            if(old_updpclear!=m_isdpclear[0])
            {
                if(m_isdpclear[0])
                {
                   emit sig_dpclear(0,0);
                }
            }
            old_updpclear = m_isdpclear[0];
            //更换
            if((g_par.DP_BG_NUM>=g_par.DP_BG_MAXNUM)&&(!m_bdpclear_show)
                    &&(g_par.DP_clear_num<g_par.DP_maxclear_num))
            {
               m_isdpexchange[0] = true;
            }
            static bool old_updpexchange = false;
            if(old_updpexchange!=m_isdpexchange[0])
            {
                if(m_isdpexchange[0])
                {
                   emit sig_dpclear(1,0);
                }
            }
            old_updpexchange = m_isdpexchange[0];
        }
        else if(g_now_frame==1)
        {
            if((g_par.DP2_clear_num>=g_par.DP2_maxclear_num)&&(!m_bdpclear_show)
                    &&(g_par.DP2_BG_NUM<g_par.DP2_BG_MAXNUM))
            {
               m_isdpclear[1] = true;
            }
            static bool old_downdpclear = false;
            if(old_downdpclear!=m_isdpclear[1])
            {
                if(m_isdpclear[1])
                {
                   emit sig_dpclear(0,1);
                }
            }
            old_downdpclear = m_isdpclear[1];
            //更换
            if((g_par.DP2_BG_NUM>=g_par.DP2_BG_MAXNUM)&&(!m_bdpclear_show)
                    &&(g_par.DP2_clear_num<g_par.DP2_maxclear_num))
            {
               m_isdpexchange[1] = true;
            }
            static bool old_downdpexchange = false;
            if(old_downdpexchange!=m_isdpexchange[1])
            {
                if(m_isdpexchange[1])
                {
                   emit sig_dpclear(1,1);
                }
            }
            old_downdpexchange = m_isdpexchange[1];
        }
    }
}
//更新当前靶标
void MainWindow::Mark_load(int ch)
{
    if(g_now_frame<0) return;
    if(ch==0)
    {
        //上方CCD
        if(update_thread->m_isusemark[g_now_frame][0]&& update_thread->m_bEnableMarkChange[g_now_frame][0]
                && (!update_thread->is_check8))
        {
            update_thread->m_currentmark.x1=g_par.other_markpos[g_now_frame].x1;
            update_thread->m_currentmark.y1=g_par.other_markpos[g_now_frame].y1;
        }
        else
        {
            update_thread->m_currentmark.x1=g_par.main_markpos[g_now_frame].x1;
            update_thread->m_currentmark.y1=g_par.main_markpos[g_now_frame].y1;
        }
        if(update_thread->m_isusemark[g_now_frame][1]&& update_thread->m_bEnableMarkChange[g_now_frame][1]
                && (!update_thread->is_check8))
        {
            update_thread->m_currentmark.x2=g_par.other_markpos[g_now_frame].x2;
            update_thread->m_currentmark.y2=g_par.other_markpos[g_now_frame].y2;
        }
        else
        {
            update_thread->m_currentmark.x2=g_par.main_markpos[g_now_frame].x2;
            update_thread->m_currentmark.y2=g_par.main_markpos[g_now_frame].y2;
        }
        if(update_thread->m_isusemark[g_now_frame][2]&& update_thread->m_bEnableMarkChange[g_now_frame][2]
                && (!update_thread->is_check8))
        {
            update_thread->m_currentmark.x3=g_par.other_markpos[g_now_frame].x3;
            update_thread->m_currentmark.y3=g_par.other_markpos[g_now_frame].y3;
        }
        else
        {
            update_thread->m_currentmark.x3=g_par.main_markpos[g_now_frame].x3;
            update_thread->m_currentmark.y3=g_par.main_markpos[g_now_frame].y3;
        }
        if(update_thread->m_isusemark[g_now_frame][3]&& update_thread->m_bEnableMarkChange[g_now_frame][3]
                && (!update_thread->is_check8))
        {
            update_thread->m_currentmark.x4=g_par.other_markpos[g_now_frame].x4;
            update_thread->m_currentmark.y4=g_par.other_markpos[g_now_frame].y4;
        }
        else
        {
            update_thread->m_currentmark.x4=g_par.main_markpos[g_now_frame].x4;
            update_thread->m_currentmark.y4=g_par.main_markpos[g_now_frame].y4;
        }
        qDebug("upccd:mark3x=%.3f mark3y=%.3f",update_thread->m_currentmark.x3,update_thread->m_currentmark.y3);
    }
    else if(ch==1)
    {
        return;
    }
}

//机台面板按键
void MainWindow::slot_mbkey(int val)
{
    if(g_isScreenStart)
    {
        qDebug()<<"Screen protection exits";
        QKeyEvent enter(QEvent::KeyPress,Qt::Key_Alt,Qt::NoModifier);
        QCoreApplication::sendEvent(this,&enter);
        return;
    }
    //第二次按任意键停止单键启动
    if(g_isSysRunning==false)
    {
        g_isPause = false;
        g_isPauseStart = false;
    }
    qDebug()<<"key is pressed!";
    switch(val)
    {
    case 1:
        if(g_registertype==UNREG)
        {
           slot_alarmshow(false,QObject::tr("授权期限已到达,请联系原厂处理!"));
           return;
        }
        if(!update_thread->input.EstopSig)
        {
            slot_alarmshow(false,QObject::tr("未释放急停开关!"));
            return;
        }
        if(update_thread->m_bCnc_state==0)
        {
            slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
            return;
        }
        if(1==update_thread->input.Frame_w_safe)
        {
            slot_alarmshow(false,QObject::tr("检测到安全光闸，不能进行此操作!"));
            return;
        }
        if(NoTopDialog()==false)
        {
            if(m_isAlarmshow)
            {
                slot_alarmshow(false,QObject::tr(""));//退出报警弹窗
            }
            else
            {
                slot_alarmshow(false,QObject::tr("运行前请先回到主界面!"));
                return;
            }
        }
        if(m_pageindex!=0)
        {
            slot_alarmshow(false,QObject::tr("运行前请先回到主界面!"));
            return;
        }
        qDebug("Start:g_isSysRunning=%d,g_isSysContinueRun=%d,g_isPause=%d",g_isSysRunning,g_isSysContinueRun,g_isPause);
        if(g_isPauseRun && g_isSysRunning)
        {
            qDebug("Start:Normal process");
            g_isPauseRun = false;
            return;
        }
        if(g_isPauseStart||g_isPause)
        {
           qDebug("Start:Key is invalid");
           return;
        }
        if((g_isSysRunning==false)&&(g_isPause==false))
        {
            //面板单键启动
            ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd1_ok->setText("    ---    ");
            ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd2_ok->setText("    ---    ");
            ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd3_ok->setText("    ---    ");
            ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd4_ok->setText("    ---    ");
            g_isBGwhileStart = false;
            bg_run();
        }
        else
        {
            //曝光点灯时按下启动键
            if(g_isBgRunning &&(g_par.auto_manual==1)&&(false==g_isBGwhileStart))
            {
                g_isBGwhileStart = true;
                bg_run();
            }
        }
        break;
    case 2:
        if(m_isAlarmshow)
        {
            slot_alarmshow(false,QObject::tr(""));//退出报警弹窗
        }
        qDebug("OpenFrame:g_isSysRunning=%d,g_isSysContinueRun=%d,g_isPause=%d",g_isSysRunning,g_isSysContinueRun,g_isPause);
        if(g_isPauseStart||g_framemannual||g_framechange)
        {
           qDebug("OpenFrame:Key is invalid");
           return;
        }
        if((g_isSysRunning==false)&&(g_isPause==false))
        {
            //面板开关框架按钮
            frame_mannual();
        }
        else
        {
            //暂停运行，但处于点灯运行可以开关框架
            if(g_isPause && g_isBgRunning)
            {
                frame_mannual();
            }
            //第一次按键暂停
            if(g_isPause==false)
            {
                bgflow->thread_pause(false);
            }
//            g_isBgRunning = false;
//            g_isSysRunning = false;
        }
        break;
    case 3:
        if(m_isAlarmshow)
        {
            slot_alarmshow(false,QObject::tr(""));//退出报警弹窗
        }
        if(m_is_weihu)
        {
            slot_alarmshow(false,QObject::tr("运行前请先回到主界面!"));
            return;
        }
        if(g_isPauseStart||g_framemannual||g_isBgRunning)
        {
           qDebug("MoveFrame:Key is invalid");
           return;
        }
        if((g_isSysRunning==false)&&(g_isPause==false))
        {
            //面板移框按钮
            on_btn_frame_change_clicked();
        }
        break;
    }
}
/***
 *进入底片页面开启环形灯,Z轴移动到对位位置
 ***/
void MainWindow::click_btn_dp()
{
   g_beeptype = 1;
   ui->stackedWidget_left->setCurrentIndex(1);
   ui->btn_dg->setEnabled(false);
   //
   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upring_light,true);
   //
   update_thread->threaddriver->CCDSend_LX(0,true);//连续识别
   m_pageindex = 1;
   if((g_now_frame==0)||(g_now_frame==1))
   {
       update_thread->m_uppcbok[g_now_frame].value_l = 0;
       update_thread->m_uppcbok[g_now_frame].value_m = 0;
       update_thread->m_uppcbok[g_now_frame].value_r = 0;
       update_thread->m_downpcbok[g_now_frame].value_l = 0;
       update_thread->m_downpcbok[g_now_frame].value_m = 0;
       update_thread->m_downpcbok[g_now_frame].value_r = 0;
       //清除补偿预估
       update_thread->threaddriver->CCDSend_clear(0);
       update_thread->threaddriver->CCDSend_clear(1);
   }
}
/***
 *
 ***/
void MainWindow::click_btn_dg()
{
   g_beeptype = 1;
   ui->stackedWidget_left->setCurrentIndex(2);
   ui->btn_dp->setEnabled(false);
   m_pageindex = 2;
}
/***
 *
 ***/
void MainWindow::click_btn_syscheck()
{
    g_beeptype = 1;
    DlgLogin login(0,QObject::tr("输入密码"),this);
    login.exec();
    if(login.iscorrect)
    {
        if(g_settings_kind==13)
        {
            m_is_weihu = true;
            DlgSystemcheck dlgsystemcheck(this);
            dlgsystemcheck.exec();
            m_is_weihu = false;
        }
        else
        {
            QMessageBox::information(this,"TOP",QObject::tr("密码错误!"));
        }
    }
}
/***
 *用户密码修改
 ***/
void MainWindow::click_btn_sysdebug()
{
    g_beeptype = 1;
    DlgChangePassword dlgchangepassword(0,this);
    dlgchangepassword.exec();
}
/***
 *
 ***/
void MainWindow::click_btn_mainpage2_exit()
{
   g_beeptype = 1;
   ui->stackedWidget_main->setCurrentIndex(0);
   ui->checkBox_checkok->setVisible(false);
   ui->checkBox_dwok->setVisible(false);
}
/***
 *
 ***/
void MainWindow::click_btn_dp_exit()
{
   g_beeptype = 1;
   ui->stackedWidget_left->setCurrentIndex(0);
   ui->stackedWidget_lightgif->setCurrentIndex(0);
   ui->btn_dg->setEnabled(true);
   //
   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upring_light,false);
   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upxlight_power,false);
   update_thread->threaddriver->CCDSend_LX(0,false);//连续识别
   //关闭框架真空
/*   if(g_now_frame==1)
   {
       int val = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
       if(val==1)
       {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,true);
           usleep(g_par.Frame_close_airdelay2*1000);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
       }
   }
   else*/ if(g_now_frame==0)
   {
       int val = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
       if(val==1)
       {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
           usleep(g_par.Frame_close_airdelay*1000);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
       }
   }
   m_pageindex = 0;
}
/***
 *
 ***/
void MainWindow::click_btn_dg_exit()
{
   g_beeptype = 1;
   ui->stackedWidget_left->setCurrentIndex(0);
   ui->stackedWidget_lightgif->setCurrentIndex(0);
   ui->btn_dp->setEnabled(true);
   m_pageindex = 0;
   //熄灯
   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,false);
   //油墨类型显示
   slot_setlight(-1);
}

void MainWindow::timeUpdate()
{
   //当前日期显示
   QDateTime CurrentTime=QDateTime::currentDateTime();
   QString Timestr=CurrentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示的格式
   currentTimeLabel->setText(Timestr);
   static int hourcnt = 0;
   hourcnt++;//周期0.5s
   //更新时间及 判断是否 是否过期
   if(7200==hourcnt)
   {
       hourcnt = 0;
       AHourPassFlag = 1;
       static int hour=0;
       if(hour>23)
       {
           hour=0;
           QDate realtime=QDate(real_year,real_month,real_day).addDays(1);
           real_year=realtime.year();
           real_month=realtime.month();
           real_day=realtime.day();
       }
       hour++;
       qDebug()<<"pass hour:"+QString::number(hour,10);
   }
}

void MainWindow::on_btn_iopara_clicked()
{
    g_beeptype = 1;
    DlgIOParameter dlgiopara(this);
    dlgiopara.exec();
}

void MainWindow::on_btn_cncpara_clicked()
{
    g_beeptype = 1;
    DlgCNCParmeter cncpar(this);
    cncpar.exec();
}
//选中轴回零
void MainWindow::on_btn_home_select_clicked()
{
    g_beeptype = 1;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
        slot_alarmshow(false,QObject::tr("系统运行中，不能回零!"));
        return;
    }
    if(update_thread->m_bCnc_state==0)
    {
        slot_alarmshow(false,QObject::tr("机器尚未开启，不能回零!"));
        return;
    }
    int axis=axis_select();
    if(AXIS_CCD3Y==axis)
    {
        if(update_thread->threaddriver->Axis_homed(AXIS_CCD1Y)!=1)
        {
            slot_alarmshow(false,QObject::tr("防止CCD1-3Y碰撞，请先回零CCD1Y!"));
            return;
        }
    }
    if(AXIS_CCD4Y==axis)
    {
        if(update_thread->threaddriver->Axis_homed(AXIS_CCD2Y)!=1)
        {
            slot_alarmshow(false,QObject::tr("防止CCD2-4Y碰撞，请先回零CCD2Y!"));
            return;
        }
    }

    //框架回零时先CCD上升 遮光板下降
//    if(axis==AXIS_W)
//    {
//        if(update_thread->threaddriver->Axis_homed(AXIS_Z)!=1)
//        {
//            slot_alarmshow(false,QObject::tr("防止框架碰撞，请先回零平台Z轴!"));
//            return;
//        }
//        //遮光板下降
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,true);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,false);
//        //CCD抬起
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
//        //CCD定位检测
//        int left = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Left_ccd_locate");
//        int right = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Right_ccd_locate");
//        bool val =false;
//        if((left==1)&&(right==1))
//        {
//            val = true;
//        }
//        int num =0;
//        while((val==false)&&(num<DELAYCOUNT))
//        {
//            //CCD定位检测
//            left = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Left_ccd_locate");
//            right = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Right_ccd_locate");
//            if((left==1)&&(right==1))
//            {
//                val = true;
//            }
//            usleep(USLEEPTIME);//10ms
//            num++;
//        }
//        if(val==false)
//        {
//            slot_alarmshow(false,QObject::tr("相机超时未上到位!"));
//            return;
//        }
//        //框架遮光板检测
//        int zg_val =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"ZG_open_check");
//        num =0;
//        while((zg_val!=1)&&(num<DELAYCOUNT))
//        {
//            zg_val =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"ZG_open_check");
//            usleep(USLEEPTIME);//0.01s
//            num++;
//        }
//        int zgclose_val =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"ZG_close_check");
//        if((zg_val!=1)||(zgclose_val!=0))
//        {
//            slot_alarmshow(false,QObject::tr("遮光板下降超时!"));
//            qDebug()<<QObject::tr("遮光板下降超时!");
//            return;
//        }
//    }
    //Z轴向下时释放磁铁
//    if(axis==AXIS_Z)
//    {
//        int ct_flg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT);
//        if(ct_flg==1)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
//            usleep(100000);
//        }
//    }
    update_thread->threaddriver->Axis_home(axis);
}

//void MainWindow::on_btn_dw_study_clicked()
//{
//     g_beeptype = 1;
//     if(update_thread->m_bCnc_state==0)
//     {
//         slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
//         return;
//     }
//     if(g_ishoming||(g_ishomed==0))//正在回零或未回零
//     {
//         slot_alarmshow(false,QObject::tr("正在回零或未回零，不能进行此操作!"));
//         return ;
//     }
//     if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
//     {
//         slot_alarmshow(false,QObject::tr("曝光过程中，不能进行此操作!"));
//         return;
//     }
//     bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//     bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//     if((is_upframeclose==false)||(is_downframeclose==false))
//     {
//         slot_alarmshow(false,QObject::tr("框架未关闭好，不能进行此操作!"));
//         return;
//     }
//     if(g_par.ccd_234[g_now_frame]==3)
//     {
//         slot_alarmshow(false,QObject::tr("请在参数设置中设置对位模式!"));
//         return;
//     }
//     bgflow->ccd_dwtype(0,2);
//}

void MainWindow::on_btn_language_switch_clicked()
{
    g_beeptype = 1;
    DlgLanguage dlglan(this);
    dlglan.exec();
}

void MainWindow::on_btn_change_password_clicked()
{
    g_beeptype = 1;
    DlgChangePassword dlgchangepassword(1,this);
    dlgchangepassword.exec();
}

void MainWindow::on_btn_machine_switch_clicked()
{
    g_beeptype = 1;
    qDebug()<<"cncstate="<<QString::number(update_thread->m_bCnc_state,10);
    if(update_thread->m_bCnc_state==1)
    {
        update_thread->threaddriver->CNC_Disable();
        qDebug()<<"cncclose";
    }
    else if(update_thread->m_bCnc_state==0)
    {
        if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.lmr_power,PORT_OUT)==false
        ||update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.wz_power,PORT_OUT)==false
        ||update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT)==false
        ||update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.uccd_power,PORT_OUT)==false)
        {
            slot_alarmshow(false,QObject::tr("驱动器供电异常，不能进行此操作!"));
            return;
        }
        update_thread->threaddriver->CNC_Enable();
        qDebug()<<"cncstart";
    }
}

void MainWindow::on_btn_upframe_clear_clicked()
{
     g_beeptype = 1;
     DlgWorkpiece dlgworkpiece(true,this);
     dlgworkpiece.exec();
}

void MainWindow::on_btn_downframe_clear_clicked()
{
    g_beeptype = 1;
    DlgWorkpiece dlgworkpiece(false,this);
    dlgworkpiece.exec();
}

void MainWindow::on_btn_lightsys_zero_clicked()
{
    g_beeptype = 1;
    DlgLogin login(11,QObject::tr("输入操作密码"),this);
    login.exec();
    if(login.iscorrect)
    {
       dlgLightZero lightzero(false,this);
       lightzero.exec();
    }
}

void MainWindow::on_btn_uplight_clear_clicked()
{
    on_btn_lightsys_zero_clicked();
}

void MainWindow::singleMark_save(int index)
{
    if(g_now_frame<0) return;
    if(index<4)
    {
        if(0==g_par.check8[g_now_frame])
        {
            update_thread->m_bEnableMarkChange[g_now_frame][index] = !update_thread->m_bEnableMarkChange[g_now_frame][index];
        }
        else if(1==g_par.check8[g_now_frame])
        {
            g_par.ch_othermark[g_now_frame][index]=!g_par.ch_othermark[g_now_frame][index];
            QString keyname,strval;
            strval = QString::number(g_par.ch_othermark[g_now_frame][index],10);
    //        qDebug()<<"index="+QString::number(index,10);
    //        qDebug()<<"val="+QString::number(g_par.ch_othermark[g_now_frame][index],10);
            if(g_now_frame==0)
            {
                keyname = "OTHERMARK1_"+QString::number(index+1,10);
            }
            else if(g_now_frame==1)
            {
                keyname = "OTHERMARK2_"+QString::number(index+1,10);
            }
            inifilerw->WriteIni("TOP",keyname,strval);
        }
    }
    else
    {
        index = index - 4;
        if(0==g_par.checkdown8[g_now_frame])
        {
            update_thread->m_bdownEnableMarkChange[g_now_frame][index] = !update_thread->m_bdownEnableMarkChange[g_now_frame][index];
        }
        else if(1==g_par.checkdown8[g_now_frame])
        {
            g_par.ch_downothermark[g_now_frame][index]=!g_par.ch_downothermark[g_now_frame][index];
            QString keyname,strval;
            strval = QString::number(g_par.ch_downothermark[g_now_frame][index],10);
    //        qDebug()<<"index="+QString::number(index,10);
    //        qDebug()<<"val="+QString::number(g_par.ch_othermark[g_now_frame][index],10);
            if(g_now_frame==0)
            {
                keyname = "DOWNOTHERMARK1_"+QString::number(index+1,10);
            }
            else if(g_now_frame==1)
            {
                keyname = "DOWNOTHERMARK2_"+QString::number(index+1,10);
            }
            inifilerw->WriteIni("TOP",keyname,strval);
        }
    }
}

void MainWindow::on_btn_M0_clicked()
{
    g_beeptype = 1;
    singleMark_save(0);
}

void MainWindow::on_btn_M1_clicked()
{
    g_beeptype = 1;
    singleMark_save(1);
}

void MainWindow::on_btn_M2_clicked()
{
    g_beeptype = 1;
    singleMark_save(2);
}

void MainWindow::on_btn_M3_clicked()
{
    g_beeptype = 1;
    singleMark_save(3);
}

void MainWindow::on_btn_normalprocess_clicked()
{
    g_beeptype = 1;
    if(g_registertype==UNREG)
    {
       slot_alarmshow(false,QObject::tr("授权期限已到达,请联系原厂处理!"));
       return;
    }
    g_isPauseRun = !g_isPauseRun;
//    if(g_isSysRunning==false)
//    {
//        //启动
//        ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
//        ui->lineEdit_ccd1_ok->setText("    ---    ");
//        ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
//        ui->lineEdit_ccd2_ok->setText("    ---    ");
//        ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
//        ui->lineEdit_ccd3_ok->setText("    ---    ");
//        ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
//        ui->lineEdit_ccd4_ok->setText("    ---    ");
//        g_isPause = false;
//        g_isBGwhileStart = false;
//        if(true==bg_run())
//        {
//            g_isPauseRun = true;
//        }
//    }
//    else
//    {
//        g_isPauseRun = false;
//    }
}

void MainWindow::on_btn_ccd_down_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
        slot_alarmshow(false,QObject::tr("曝光过程中，不能进行此操作!"));
        return;
    }
  //  int isccddown = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.ccd_down,PORT_OUT);
    if(g_now_frame==1)
    {
        bool flg =false;
//        if(isccddown==0)
//        {
////            bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
////            bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
////            if((is_upframeclose==false)||(is_downframeclose==false))
////            {
////                slot_alarmshow(false,QObject::tr("框架未关闭好，不能进行此操作!"));
////                return;
////            }
//            flg = true;
//        }
//        else
//        {
//            flg = false;
//        }
      //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,flg);
    }

}

void MainWindow::on_btn_platform_center_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    update_thread->lmrvalue.value_l = 0.000;
    update_thread->lmrvalue.value_m = 0.000;
    update_thread->lmrvalue.value_r = 0.000;
    bgflow->thread_ptlmrmove(0);
}

void MainWindow::on_btn_frame_absorb_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
//    bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((is_upframeclose==true)&&(is_downframeclose==true))
//    {
//        if(m_zpos_type < PT_DWPOSITION)
//        {
//            slot_alarmshow(false,QObject::tr("该平台位置，不能进行此操作!"));
//            return;
//        }
//    }
/*    if(g_now_frame==1)
    {
        int val = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
        if(val==1)
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,true);
            usleep(g_par.Frame_close_airdelay2*1000);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
        }
        else
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,true);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,true);
        }
    }
    else */if(g_now_frame==0)
    {
        int val = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
        if(val==1)
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
            usleep(g_par.Frame_close_airdelay*1000);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
        }
        else
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,true);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,true);
        }
    }
    return;
}

void MainWindow::on_btn_autoset_clicked()
{
    g_beeptype = 1;
    if(update_thread->m_bCnc_state==0)
    {
        slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
        return;
    }
    if(g_ishoming||(g_ishomed==0))//正在回零或未回零
    {
        slot_alarmshow(false,QObject::tr("正在回零或未回零，不能进行此操作!"));
        return ;
    }
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
        slot_alarmshow(false,QObject::tr("曝光过程中，不能进行此操作!"));
        return;
    }
//    bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((is_upframeclose==false)||(is_downframeclose==false))
//    {
//        slot_alarmshow(false,QObject::tr("框架未关闭好，不能进行此操作!"));
//        return;
//    }
    if(1==update_thread->input.Frame_w_safe)
    {
        slot_alarmshow(false,QObject::tr("检测到安全光闸，不能进行此操作!"));
        return;
    }
//    if(alarmswitch_bit.Door_alm)
//    {
//        slot_alarmshow(false,QObject::tr("门盖关闭异常，不能进行此操作!");
//        return;
//    }
    //判断单框模式
    int up_frame_select=0,down_frame_select=0;
    if(g_par.Frame_select==0)//上框
    {
        up_frame_select = 1;
        down_frame_select = 0;
    }
    else if(g_par.Frame_select==1)//下框
    {
        up_frame_select = 0;
        down_frame_select = 1;
    }
    else if(g_par.Frame_select==2)//上下框
    {
        up_frame_select = 1;
        down_frame_select = 1;
    }
    if(((g_now_frame==0)&&(up_frame_select==1))||
       ((g_now_frame==1)&&(down_frame_select==1)))
    {
        if(g_par.is_dpvacuum==1)
        {
            if(g_now_frame==0)
            {
                if(false==update_thread->input.updp_vacuum)
                {
                    slot_alarmshow(false,QObject::tr("上框底片真空未达，不能进行此操作!"));
                    return;
                }
            }
            else if(g_now_frame==1)
            {
                if(false==update_thread->input.downdp_vacuum)
                {
                    slot_alarmshow(false,QObject::tr("下框底片真空未达，不能进行此操作!"));
                    return;
                }
            }
        }
        bool ret = false;
        if(g_par.ccd_234[g_now_frame]==3)
        {
            if(g_now_frame==0)
            {
                ret = slot_alarmshow(true,QObject::tr("上框不执行对位继续运行?"));
            }
            else if(g_now_frame==1)
            {
                ret = slot_alarmshow(true,QObject::tr("下框不执行对位继续运行?"));
            }
            g_runtest[g_now_frame] = ret;
        }
        else
        {
            g_runtest[g_now_frame] = false;
        }
    }
update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
    on_btn_position_down_clicked();
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);

    g_autoset =true;
    g_downautoset = false;
    ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd1_ok->setText("    ---    ");
    ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd2_ok->setText("    ---    ");
    ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd3_ok->setText("    ---    ");
    ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
    ui->lineEdit_ccd4_ok->setText("    ---    ");
    Mark_load(0);
    bgflow->thread_bgrun();
}
//底片架设
void MainWindow::on_btn_dp_load_clicked()
{
    int org_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//检测到曝光框架位置
    if(org_stop==1){
        qDebug()<<"框架w未到达曝光位置";
        slot_alarmshow(false,QObject::tr("框架w尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
    g_beeptype = 1;
    if(g_par.is_dpvacuum==0)  return;
    if(g_ishomed==false) return;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;

    bgflow->thread_dpload();
}
//底片真空
void MainWindow::on_btn_dp_vacuum_clicked()
{
    g_beeptype = 1;
    if(g_par.is_dpvacuum==0)  return;
    if(g_ishomed==false) return;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
//    if(g_now_frame<0)
//    {
//        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
//        return;
//    }
    if(update_thread->m_dp_zhengkon_out)
    {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
    }
    else
    {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,true);
    }
}

void MainWindow::on_btn_dp_vacuum2_clicked()
{
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);//框架曝光位置开关out
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);//框架清洁位置开关out
    return;
    g_beeptype = 1;
    if(g_par.is_dpvacuum==0)  return;
    if(g_ishomed==false) return;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
    if(update_thread->m_dpvacuum2)
    {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownDP_Vacumm_out,false);
    }
    else
    {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownDP_Vacumm_out,true);
    }
}

void MainWindow::on_btn_bm_vacuum_clicked()
{

    if(update_thread->m_bm_zhengkon_out)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,false);//板面吹气 O15.2

        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1
        if(g_par.up_ylength>255){
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2
        }    }
    else
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
        if(g_par.up_ylength>255){
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2
        }
    }


}

void MainWindow::on_btn_mark_check_clicked()
{
    g_beeptype = 1;
    if(g_ishomed==false) return;
//    bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((is_upframeclose==false)||(is_downframeclose==false))
//    {
//        slot_alarmshow(false,QObject::tr("框架未关闭好，不能进行此操作!"));
//        return;
//    }
    int isCCD13YSafe = update_thread->input.Ccd13_protect;
    int isCCD24YSafe = update_thread->input.Ccd24_protect;
    if((0==isCCD13YSafe)&&(0==isCCD24YSafe))
    {
        CCD_POS pos;
        pos.x1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X);
        pos.y1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y);
        pos.x2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X);
        pos.y2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y);
        pos.x3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X);
        pos.y3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y);
        pos.x4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X);
        pos.y4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y);
        update_thread->threaddriver->CCDSend_duibiao(0,pos);
    }
    else
    {
        slot_alarmshow(false,QObject::tr("CCD碰撞，不能进行此操作!"));
    }
}

void MainWindow::on_btn_mark_change_clicked()
{
    g_beeptype = 1;
    if(g_now_frame<0) return;
    if(g_ishomed==false) return;
    qDebug()<<"change mark";
//    bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((is_upframeclose==false)||(is_downframeclose==false))
//    {
//        slot_alarmshow(false,QObject::tr("框架未关闭好，不能进行此操作!"));
//        return;
//    }
    int isCCD13YSafe = update_thread->input.Ccd13_protect;
    int isCCD24YSafe = update_thread->input.Ccd24_protect;
    if((0==isCCD13YSafe)&&(0==isCCD24YSafe))
    {
        qDebug()<<"change mark:load mark!";
        //切换靶标
        if(m_isothermark[0])
        {
            update_thread->m_currentmark.x1=g_par.other_markpos[g_now_frame].x1;
            update_thread->m_currentmark.y1=g_par.other_markpos[g_now_frame].y1;
            update_thread->m_currentmark.x2=g_par.other_markpos[g_now_frame].x2;
            update_thread->m_currentmark.y2=g_par.other_markpos[g_now_frame].y2;
            update_thread->m_currentmark.x3=g_par.other_markpos[g_now_frame].x3;
            update_thread->m_currentmark.y3=g_par.other_markpos[g_now_frame].y3;
            update_thread->m_currentmark.x4=g_par.other_markpos[g_now_frame].x4;
            update_thread->m_currentmark.y4=g_par.other_markpos[g_now_frame].y4;
            update_thread->ccd_sd(1);
        }
        else
        {
            update_thread->m_currentmark.x1=g_par.main_markpos[g_now_frame].x1;
            update_thread->m_currentmark.y1=g_par.main_markpos[g_now_frame].y1;
            update_thread->m_currentmark.x2=g_par.main_markpos[g_now_frame].x2;
            update_thread->m_currentmark.y2=g_par.main_markpos[g_now_frame].y2;
            update_thread->m_currentmark.x3=g_par.main_markpos[g_now_frame].x3;
            update_thread->m_currentmark.y3=g_par.main_markpos[g_now_frame].y3;
            update_thread->m_currentmark.x4=g_par.main_markpos[g_now_frame].x4;
            update_thread->m_currentmark.y4=g_par.main_markpos[g_now_frame].y4;
            update_thread->ccd_sd(0);
        }
        m_isothermark[0] = !m_isothermark[0];
        qDebug()<<"change mark:move!";
        bgflow->thread_ccdmove(0,2);
    }
    else
    {
        slot_alarmshow(false,QObject::tr("CCD碰撞，不能进行此操作!"));
    }
}

void MainWindow::on_btn_ccd_lowspeed_clicked()
{
    g_beeptype = 1;
    if((update_thread->m_isxlight_on==true))
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upxlight_power,false);
    }
    else if((update_thread->m_isxlight_on==false))
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upxlight_power,true);
    }
}
//ismainmark:1 save main 0:save other
void MainWindow::mark_save(int ch,bool ismainmark)
{
    if(g_now_frame<0) return;
    QString ccdx[4],ccdy[4],group;
    Mark_pos ccd_currentpos;
    int offset = 0;
    if(ch==0)
    {
        offset = 0;
        int isCCD13YSafe = update_thread->input.Ccd13_protect;
        int isCCD24YSafe = update_thread->input.Ccd24_protect;
        if((1==isCCD13YSafe)||(1==isCCD24YSafe))
        {
            str_alarm = QObject::tr("CCD防撞信号有效,停止位置不能作为靶标!");
            showtips(WARN_TIP,str_alarm);
            return;
        }
    }
    else if(ch==1)
    {
        offset = 0;//AXIS_CCD5X-AXIS_CCD1X;
    }
    ccd_currentpos.x1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X+offset);
    ccd_currentpos.y1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y+offset);
    ccd_currentpos.x2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X+offset);
    ccd_currentpos.y2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y+offset);
    ccd_currentpos.x3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X+offset);
    ccd_currentpos.y3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y+offset);
    ccd_currentpos.x4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X+offset);
    ccd_currentpos.y4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y+offset);
    if((ccd_currentpos.y1==0)||(ccd_currentpos.y2==0)||(ccd_currentpos.y3==0)||(ccd_currentpos.y4==0))
    {
        str_alarm = QObject::tr("CCD原点位置不能作为靶标!");
        showtips(WARN_TIP,str_alarm);
        return;
    }
    if(ismainmark)
    {
        if(ch==0)
        {
            g_par.main_markpos[g_now_frame].x1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X);
            g_par.main_markpos[g_now_frame].y1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y);
            g_par.main_markpos[g_now_frame].x2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X);
            g_par.main_markpos[g_now_frame].y2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y);
            g_par.main_markpos[g_now_frame].x3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X);
            g_par.main_markpos[g_now_frame].y3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y);
            g_par.main_markpos[g_now_frame].x4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X);
            g_par.main_markpos[g_now_frame].y4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y);
            ccdx[0].sprintf("%.3f",g_par.main_markpos[g_now_frame].x1);
            ccdx[1].sprintf("%.3f",g_par.main_markpos[g_now_frame].x2);
            ccdx[2].sprintf("%.3f",g_par.main_markpos[g_now_frame].x3);
            ccdx[3].sprintf("%.3f",g_par.main_markpos[g_now_frame].x4);
            ccdy[0].sprintf("%.3f",g_par.main_markpos[g_now_frame].y1);
            ccdy[1].sprintf("%.3f",g_par.main_markpos[g_now_frame].y2);
            ccdy[2].sprintf("%.3f",g_par.main_markpos[g_now_frame].y3);
            ccdy[3].sprintf("%.3f",g_par.main_markpos[g_now_frame].y4);
        }
        else if(ch==1)
        {
            g_par.main_downmarkpos[g_now_frame].x1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X+offset);
            g_par.main_downmarkpos[g_now_frame].y1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y+offset);
            g_par.main_downmarkpos[g_now_frame].x2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X+offset);
            g_par.main_downmarkpos[g_now_frame].y2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y+offset);
            g_par.main_downmarkpos[g_now_frame].x3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X+offset);
            g_par.main_downmarkpos[g_now_frame].y3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y+offset);
            g_par.main_downmarkpos[g_now_frame].x4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X+offset);
            g_par.main_downmarkpos[g_now_frame].y4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y+offset);
            ccdx[0].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].x1);
            ccdx[1].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].x2);
            ccdx[2].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].x3);
            ccdx[3].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].x4);
            ccdy[0].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].y1);
            ccdy[1].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].y2);
            ccdy[2].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].y3);
            ccdy[3].sprintf("%.3f",g_par.main_downmarkpos[g_now_frame].y4);
        }
        if(g_now_frame==0)
        {
            group = "MAIN_MARK1";
        }
        else if(g_now_frame==1)
        {
            group = "MAIN_MARK2";
        }
    }
    else
    {
        if(ch==0)
        {
            g_par.other_markpos[g_now_frame].x1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X);
            g_par.other_markpos[g_now_frame].y1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y);
            g_par.other_markpos[g_now_frame].x2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X);
            g_par.other_markpos[g_now_frame].y2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y);
            g_par.other_markpos[g_now_frame].x3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X);
            g_par.other_markpos[g_now_frame].y3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y);
            g_par.other_markpos[g_now_frame].x4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X);
            g_par.other_markpos[g_now_frame].y4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y);
            ccdx[0].sprintf("%.3f",g_par.other_markpos[g_now_frame].x1);
            ccdx[1].sprintf("%.3f",g_par.other_markpos[g_now_frame].x2);
            ccdx[2].sprintf("%.3f",g_par.other_markpos[g_now_frame].x3);
            ccdx[3].sprintf("%.3f",g_par.other_markpos[g_now_frame].x4);
            ccdy[0].sprintf("%.3f",g_par.other_markpos[g_now_frame].y1);
            ccdy[1].sprintf("%.3f",g_par.other_markpos[g_now_frame].y2);
            ccdy[2].sprintf("%.3f",g_par.other_markpos[g_now_frame].y3);
            ccdy[3].sprintf("%.3f",g_par.other_markpos[g_now_frame].y4);
        }
        else if(ch==1)
        {
            g_par.other_downmarkpos[g_now_frame].x1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X+offset);
            g_par.other_downmarkpos[g_now_frame].y1 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y+offset);
            g_par.other_downmarkpos[g_now_frame].x2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X+offset);
            g_par.other_downmarkpos[g_now_frame].y2 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y+offset);
            g_par.other_downmarkpos[g_now_frame].x3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X+offset);
            g_par.other_downmarkpos[g_now_frame].y3 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y+offset);
            g_par.other_downmarkpos[g_now_frame].x4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X+offset);
            g_par.other_downmarkpos[g_now_frame].y4 = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y+offset);
            ccdx[0].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].x1);
            ccdx[1].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].x2);
            ccdx[2].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].x3);
            ccdx[3].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].x4);
            ccdy[0].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].y1);
            ccdy[1].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].y2);
            ccdy[2].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].y3);
            ccdy[3].sprintf("%.3f",g_par.other_downmarkpos[g_now_frame].y4);
        }
        if(g_now_frame==0)
        {
            group = "OTHER_MARK1";
        }
        else if(g_now_frame==1)
        {
            group = "OTHER_MARK2";
        }
    }
    CIniRW markfile(MARKPARAMETERPATH);
    if(ch==0)
    {
        markfile.WriteIni(group,"ccd1x",ccdx[0]);
        markfile.WriteIni(group,"ccd2x",ccdx[1]);
        markfile.WriteIni(group,"ccd3x",ccdx[2]);
        markfile.WriteIni(group,"ccd4x",ccdx[3]);
        markfile.WriteIni(group,"ccd1y",ccdy[0]);
        markfile.WriteIni(group,"ccd2y",ccdy[1]);
        markfile.WriteIni(group,"ccd3y",ccdy[2]);
        markfile.WriteIni(group,"ccd4y",ccdy[3]);
    }
    else if(ch==1)
    {
        markfile.WriteIni(group,"ccd5x",ccdx[0]);
        markfile.WriteIni(group,"ccd6x",ccdx[1]);
        markfile.WriteIni(group,"ccd7x",ccdx[2]);
        markfile.WriteIni(group,"ccd8x",ccdx[3]);
        markfile.WriteIni(group,"ccd5y",ccdy[0]);
        markfile.WriteIni(group,"ccd6y",ccdy[1]);
        markfile.WriteIni(group,"ccd7y",ccdy[2]);
        markfile.WriteIni(group,"ccd8y",ccdy[3]);
    }
//    qDebug("save mark:mark3x=%.3f mark3y=%.3f\n",ccdx[2],ccdy[2]);
}

void MainWindow::on_btn_save_mainmark_clicked()
{
    g_beeptype = 1;
    if(g_ishomed==false) return;
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
    ui->btn_save_mainmark->setEnabled(false);
    mark_save(0,true);
    usleep(1000);
    ui->btn_save_mainmark->setEnabled(true);
}

void MainWindow::on_btn_save_othermark_clicked()
{
    g_beeptype = 1;
    if(g_ishomed==false) return;
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
    ui->btn_save_othermark->setEnabled(false);
    mark_save(0,false);
    usleep(1000);
    ui->btn_save_othermark->setEnabled(true);
}

void MainWindow::on_btn_jt_dy_clicked()
{
    g_beeptype = 1;
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
    ui->btn_jt_dy->setEnabled(false);
    if(g_now_frame==0)
    {
        if(update_thread->m_bjtdy) update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
        else update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
    }
    usleep(1000);
    ui->btn_jt_dy->setEnabled(true);
}



//判断CCD手动条件 dir=0正向  1：负向
bool MainWindow::isCCD_Move(int axis,bool dir)
{
    bool ret = true;
    int isCCD13YSafe = update_thread->input.Ccd13_protect;
    int isCCD24YSafe = update_thread->input.Ccd24_protect;
    update_thread->threaddriver->CCDSend_LX(0,true);//连续识别
    switch(axis)
    {
    case AXIS_CCD1X:
    case AXIS_CCD3X:
        if(isCCD13YSafe)
        {
           slot_alarmshow(true,QObject::tr("CCD碰撞，请先移至安全距离!"));
           ret = false;
        }
        break;
    case AXIS_CCD2X:
    case AXIS_CCD4X:
        if(isCCD24YSafe)
        {
           slot_alarmshow(true,QObject::tr("CCD碰撞，请先移至安全距离!"));
           ret = false;
        }
        break;
    case AXIS_CCD1Y:
        if(isCCD13YSafe && dir)
        {
           slot_alarmshow(true,QObject::tr("CCD碰撞，请先移至安全距离!"));
           ret = false;
        }
        break;
    case AXIS_CCD2Y:
        if(isCCD24YSafe && dir)
        {
           slot_alarmshow(true,QObject::tr("CCD碰撞，请先移至安全距离!"));
           ret = false;
        }
        break;
    case AXIS_CCD3Y:
        if(isCCD13YSafe && (dir==false))
        {
           slot_alarmshow(true,QObject::tr("CCD碰撞，请先移至安全距离!"));
           ret = false;
        }
        break;
    case AXIS_CCD4Y:
        if(isCCD24YSafe && (dir==false))
        {
           slot_alarmshow(true,QObject::tr("CCD碰撞，请先移至安全距离!"));
           ret = false;
        }
        break;
    }
    return ret;
}

void MainWindow::on_btn_ccd_rw_clicked()
{
    g_beeptype = 1;
    if(g_now_frame<0) return;
    if(update_thread->m_bCnc_state==0) return;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    //检测框架关闭到位
//    int up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    int down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((0==up_aircheck)||(0==down_aircheck))
//    {
//        slot_alarmshow(false,QObject::tr("框架未关闭，请先关闭框架!"));
//        return;
//    }
    if(false==m_isCCDdw[0])
    {
       bgflow->thread_ccdmove(0,0);
    }
    else
    {
        if(update_thread->m_isusemark[g_now_frame][0]&&(!update_thread->is_check8))
        {
            update_thread->m_currentmark.x1=g_par.other_markpos[g_now_frame].x1;
            update_thread->m_currentmark.y1=g_par.other_markpos[g_now_frame].y1;
        }
        else
        {
            update_thread->m_currentmark.x1=g_par.main_markpos[g_now_frame].x1;
            update_thread->m_currentmark.y1=g_par.main_markpos[g_now_frame].y1;
        }
        if(update_thread->m_isusemark[g_now_frame][1]&&(!update_thread->is_check8))
        {
            update_thread->m_currentmark.x2=g_par.other_markpos[g_now_frame].x2;
            update_thread->m_currentmark.y2=g_par.other_markpos[g_now_frame].y2;
        }
        else
        {
            update_thread->m_currentmark.x2=g_par.main_markpos[g_now_frame].x2;
            update_thread->m_currentmark.y2=g_par.main_markpos[g_now_frame].y2;
        }
        if(update_thread->m_isusemark[g_now_frame][2]&&(!update_thread->is_check8))
        {
            update_thread->m_currentmark.x3=g_par.other_markpos[g_now_frame].x3;
            update_thread->m_currentmark.y3=g_par.other_markpos[g_now_frame].y3;
        }
        else
        {
            update_thread->m_currentmark.x3=g_par.main_markpos[g_now_frame].x3;
            update_thread->m_currentmark.y3=g_par.main_markpos[g_now_frame].y3;
        }
        if(update_thread->m_isusemark[g_now_frame][3]&&(!update_thread->is_check8))
        {
            update_thread->m_currentmark.x4=g_par.other_markpos[g_now_frame].x4;
            update_thread->m_currentmark.y4=g_par.other_markpos[g_now_frame].y4;
        }
        else
        {
            update_thread->m_currentmark.x4=g_par.main_markpos[g_now_frame].x4;
            update_thread->m_currentmark.y4=g_par.main_markpos[g_now_frame].y4;
        }
        qDebug()<<"ccddw:load mark!";
        bgflow->thread_ccdmove(0,2);
    }
}

void MainWindow::on_btn_position_down_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    bgflow->thread_ptzmove(PT_HOME);
}

void MainWindow::on_btn_position_lb_clicked()
{
//    int org_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//检测到曝光框架位置
//    if(org_stop==1){
//        qDebug()<<"框架w未到达曝光位置";
//        slot_alarmshow(false,QObject::tr("框架w尚未到达正确停止位置，不能进行此操作!"));
//        return;
//    }
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    bgflow->thread_ptzmove(PT_LBPOSITION);
}

void MainWindow::on_btn_position_dw_clicked()
{
//    int org_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//检测到曝光框架位置
//    if(org_stop==1){
//        qDebug()<<"框架w未到达曝光位置";
//        slot_alarmshow(false,QObject::tr("框架w尚未到达正确停止位置，不能进行此操作!"));
//        return;
//    }
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    bgflow->thread_ptzmove(PT_DWPOSITION);
}

void MainWindow::on_btn_position_bg_clicked()
{
//    int org_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//检测到曝光框架位置
//    if(org_stop==1){
//        qDebug()<<"框架w未到达曝光位置";
//        slot_alarmshow(false,QObject::tr("框架w尚未到达正确停止位置，不能进行此操作!"));
//        return;
//    }
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(alarmswitch_bit.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("急停未释放!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    bgflow->thread_ptzmove(PT_BGPOSITION);
}

void MainWindow::on_btn_cool_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return ;
    int powersate = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
    int state = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.cool_out,PORT_OUT);
    if(state==0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.cool_out,true);
    }
    else if(state==1&&powersate==0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.cool_out,false);
    }
}

void MainWindow::on_btn_light_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
        slot_alarmshow(false,QObject::tr("曝光测试中，请稍候!"));
        return;
    }
    int poweron_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
    if(0==poweron_out)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("确定点灯?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::No)
        {
            return;
        }
        if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.cool_out,PORT_OUT))
        {
            slot_alarmshow(false,QObject::tr("请先打开冷风机!"));
            return;
        }
        if(alarmswitch_bit.Cool_vel)
        {
            slot_alarmshow(false,QObject::tr("冷却液流量异常，请检查!"));
            return;
        }
//        if(alarmswitch_bit.Light_power)
//        {
//            slot_alarmshow(false,QObject::tr("灯电源异常!");
//            return;
//        }
        if(alarmswitch_bit.Light_tem)
        {
            slot_alarmshow(false,QObject::tr("灯管温度异常，请检查!"));
            return;
        }
        if(g_par.is_usescanlight==1)
        {
            int up_innerlimit,up_outlimit;
            up_innerlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_innerlimit");
            up_outlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_outlimit");
            if((up_innerlimit!=1)&&(up_outlimit!=1))
            {
                update_thread->threaddriver->Axis_MovePos(AXIS_UPSCAN,g_par.light_speed,0);
            }
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,true);
        }
        else if(g_par.is_usescanlight==0)
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,true);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uplight_motor,true);
        }
    }
    else if(1==poweron_out)
    {
        QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("确定熄灯?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::No)
        {
            return;
        }
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,false);
        if(g_par.is_usescanlight==0)
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uplight_motor,false);
    }
}


void MainWindow::on_btn_bg_test_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
        slot_alarmshow(false,QObject::tr("曝光过程中，不能进行此操作!"));
        return;
    }
    if(1==update_thread->input.Frame_w_safe)
    {
        slot_alarmshow(false,QObject::tr("检测到安全光闸，不能进行此操作!"));
        return;
    }
    if(alarmswitch_bit.Door_alm)
    {
        slot_alarmshow(false,QObject::tr("门盖关闭异常，不能进行此操作!"));
        return;
    }
    if(alarmswitch_bit.Room_tem)
    {
        slot_alarmshow(false,QObject::tr("曝光室温度异常，不能进行此操作!"));
        return;
    }
    if(alarmswitch_bit.Cool_vel)
    {
        slot_alarmshow(false,QObject::tr("冷却液流量异常，请检查!"));
        return;
    }
    if(alarmswitch_bit.TaiMian_CoolLiu)
    {
        slot_alarmshow(false,QObject::tr("台面冷却液流量异常，请检查!"));
        return;
    }
    if(alarmswitch_bit.TaiMian_Coolwen)
    {
        slot_alarmshow(false,QObject::tr("台面冷却温度异常，请检查!"));
        return;
    }
//    if(alarmswitch_bit.Light_power)
//    {
//        slot_alarmshow(false,QObject::tr("灯电源异常!");
//        return;
//    }
    if(alarmswitch_bit.Light_tem)
    {
        slot_alarmshow(false,QObject::tr("灯管温度异常，请检查!"));
        return;
    }
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.cool_out,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("请先打开冷风机!"));
        return;
    }
//    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.tb,PORT_OUT))
//    {
//        slot_alarmshow(false,QObject::tr("台面冰水泵未开启，不能进行此操作!"));
//        return ;
//    }
    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.tb2,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("板面鼓风机未开启，不能进行此操作!"));
        return ;
    }
//    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_lock,PORT_OUT))
//    {
//        slot_alarmshow(false,QObject::tr("台面真空泵未开启，不能进行此操作!"));
//        return ;
//    }
    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.DownVacumm_switch_out,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("CCD伺服转矩未开启，不能进行此操作!"));
        return ;
    }
    //曝光时点灯异常超过三次需解锁才能运行
    if(g_light_errorcnt>=3)
    {
        DlgLogin login(0,QObject::tr("点灯异常超过三次，请输入密码解锁"),this);
        login.exec();
        g_light_errorcnt =0;
        return;
    }
    bgflow->thread_bgtest();
}

void MainWindow::on_btn_oilsetting_clicked()
{
    g_beeptype = 1;
    pid_t status;
    if(0==g_par.lightmode)
    {
      //  status = system("sudo /home/u/top/v1/lightSource &");
        light   ligh;
        ligh.exec();
    }
    else if(1==g_par.lightmode)
    {
        //status = system("sudo /home/u/top/v2/lightSource &");
 //       QtConcurrent::run(this,&MainWindow::on_btn_taimian_ZKBen_clicked);
        lightRTU   lightrtu;
        lightrtu.exec();
    }
    if(-1 == status)
    {
        qDebug("system shell is error");
    }
}
//框架交换
void MainWindow::on_btn_frame_change_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isSysContinueRun||g_framechange||(update_thread->m_bAxis_state==0))
    {
        qDebug()<<"frame change is forbided!";
        return;
    }
    qDebug()<<"frame change";
    if(!update_thread->input.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("未释放急停开关!"));
        return;
    }
    if(update_thread->m_bCnc_state==0)
    {
        slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
        return;
    }
    if(g_ishomed==0)
    {
        slot_alarmshow(false,QObject::tr("未回零，不能进行此操作!"));
        return;
    }
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
    {
        slot_alarmshow(false,QObject::tr("曝光过程中，不能进行此操作!"));
        return;
    }
    if(g_now_frame<0)
    {
        slot_alarmshow(false,QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
        return;
    }
//    bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((is_upframeclose==false)||(is_downframeclose==false))
//    {
//        slot_alarmshow(false,QObject::tr("框架未关闭好，不能进行此操作!"));
//        return;
//    }
    if(1==update_thread->input.Frame_w_safe)
    {
        slot_alarmshow(false,QObject::tr("检测到安全光闸，不能进行此操作!"));
        return;
    }
    if(alarmswitch_bit.Door_alm)
    {
        slot_alarmshow(false,QObject::tr("门盖关闭异常，不能进行此操作!"));
        return;
    }
    //灯柜页面执行交换框则判断灯管状态
    if(m_pageindex == 2)
    {
        if(update_thread->m_bLighton)
        {
            bool res = slot_alarmshow(true,QObject::tr("灯管未熄灭，确定继续执行?"));
            if(!res) return;
        }
    }
    bgflow->thread_framechange();
    return;
}

void MainWindow::on_button_liaohao_clicked()
{
    g_beeptype = 1;
    DlgMaterialManage dlgmaterial(this);
    dlgmaterial.exec();
}

int MainWindow::axis_select()
{
    if(ui->radio_ptz->isChecked()) return AXIS_Z;
    else if(ui->radio_ptm->isChecked()) return AXIS_M;
    else if(ui->radio_ptr->isChecked()) return AXIS_R;
    else if(ui->radio_ptl->isChecked()) return AXIS_L;
    else if(ui->radio_upscan->isChecked()) return AXIS_UPSCAN;
    else if(ui->radio_ccd1x->isChecked()) return AXIS_CCD1X;
    else if(ui->radio_ccd1y->isChecked()) return AXIS_CCD1Y;
    else if(ui->radio_ccd2x->isChecked()) return AXIS_CCD2X;
    else if(ui->radio_ccd2y->isChecked()) return AXIS_CCD2Y;
    else if(ui->radio_ccd3x->isChecked()) return AXIS_CCD3X;
    else if(ui->radio_ccd3y->isChecked()) return AXIS_CCD3Y;
    else if(ui->radio_ccd4x->isChecked()) return AXIS_CCD4X;
    else if(ui->radio_ccd4y->isChecked()) return AXIS_CCD4Y;
    else if(ui->radio_w->isChecked()) return AXIS_W;
    else return -1;
}
void MainWindow::axis_showpos()
{
    QString str;
    str.sprintf("%.3f",update_thread->m_axispos[AXIS_Z]);
    ui->value_ptz->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_Z)==1)
    {
        ui->value_ptz->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ptz->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_M]);
    ui->value_ptm->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_M)==1)
    {
        ui->value_ptm->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ptm->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_L]);
    ui->value_ptl->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_L)==1)
    {
        ui->value_ptl->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ptl->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_R]);
    ui->value_ptr->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_R)==1)
    {
        ui->value_ptr->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ptr->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_UPSCAN]);
    ui->value_upscan->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_UPSCAN)==1)
    {
        ui->value_upscan->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_upscan->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD1X]);
    ui->value_ccd1x->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD1X)==1)
    {
        ui->value_ccd1x->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd1x->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD1Y]);
    ui->value_ccd1y->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD1Y)==1)
    {
        ui->value_ccd1y->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd1y->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD2X]);
    ui->value_ccd2x->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD2X)==1)
    {
        ui->value_ccd2x->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd2x->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD2Y]);
    ui->value_ccd2y->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD2Y)==1)
    {
        ui->value_ccd2y->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd2y->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD3X]);
    ui->value_ccd3x->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD3X)==1)
    {
        ui->value_ccd3x->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd3x->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD3Y]);
    ui->value_ccd3y->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD3Y)==1)
    {
        ui->value_ccd3y->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd3y->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD4X]);
    ui->value_ccd4x->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD4X)==1)
    {
        ui->value_ccd4x->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd4x->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD4Y]);
    ui->value_ccd4y->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_CCD4Y)==1)
    {
        ui->value_ccd4y->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_ccd4y->setStyleSheet("color:red;");
    }

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_W]);
    ui->value_w->setText(str);
    if(update_thread->threaddriver->Axis_homed(AXIS_W)==1)
    {
        ui->value_w->setStyleSheet("color:green;");
    }
    else
    {
        ui->value_w->setStyleSheet("color:red;");
    }
}

// 实现
void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_F1:
        on_btn_f1_clicked();
        break;
    case Qt::Key_F2:
        on_btn_f2_clicked();
        break;
    case Qt::Key_F3:
        on_btn_f3_clicked();
        break;
    default:
        break;
    }
}

void MainWindow::on_btn_positive_pressed()
{
    if(update_thread->m_bCnc_state==0)
    {
        slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
        return;
    }
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(ui->radio_continue->isChecked())
    {
        int axis = axis_select();
////        qDebug()<<"btn_press+ axis="+QString::number(num,10);
        float speed = ui->spin_speed->value()/60.0;
        if(false==isCCD_Move(axis,true))
        {
            return;
        }
        if(axis==AXIS_W)
        {
            //遮光板下降
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,false);
        }
        update_thread->threaddriver->Axis_MovePos(axis,speed,100000);
    }
    else if(ui->radio_increment->isChecked())
    {
        g_beeptype = 1;
        int axis = axis_select();
//        qDebug()<<"btn_positive axis="+QString::number(num,10);
        float speed = ui->spin_speed->value()/60.0;
        float inc= ui->spin_inc->value();
        float currentpos = update_thread->m_axispos[axis];
        update_thread->threaddriver->Axis_MovePos(axis,speed,currentpos+inc);
//            update_thread->threaddriver->Axis_WaitForStop(axis);

    }
}

void MainWindow::on_btn_positive_released()
{
    if(ui->radio_continue->isChecked())
    {
        int axis = axis_select();
//        qDebug()<<"btn_release+ axis="+QString::number(axis,10);
        update_thread->threaddriver->Axis_Enable(axis,false);
    }
}

void MainWindow::on_btn_negtive_pressed()
{
    if(update_thread->m_bCnc_state==0)
    {
        slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
        return;
    }
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return;
    if(ui->radio_continue->isChecked())
    {
        int axis = axis_select();
        float speed = ui->spin_speed->value()/60.0;
        if(false==isCCD_Move(axis,false))
        {
            return;
        }
        if(axis==AXIS_W)
        {
            //遮光板下降
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,false);
        }
        //Z轴向下时释放磁铁
//        if(axis==AXIS_Z)
//        {
//            int ct_flg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT);
//            if(ct_flg==1)
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
//                usleep(20000);
//            }
//        }
        update_thread->threaddriver->Axis_MovePos(axis,speed,-100000);
    }
    else if(ui->radio_increment->isChecked())
    {
        g_beeptype = 1;
        int axis = axis_select();
//        qDebug()<<"btn_negtive axis="+QString::number(num,10);
        float speed = ui->spin_speed->value()/60.0;
        float inc= ui->spin_inc->value();
        float currentpos = update_thread->m_axispos[axis];
        update_thread->threaddriver->Axis_MovePos(axis,speed,currentpos-inc);
//            update_thread->threaddriver->Axis_WaitForStop(axis);
    }
}

void MainWindow::on_btn_negtive_released()
{
    if(ui->radio_continue->isChecked())
    {
        int axis = axis_select();
        update_thread->threaddriver->Axis_Enable(axis,false);
    }
}

void MainWindow::showtips(int tipstype, QString text)
{
    QString str_data;
    switch(tipstype){
    case NULL_TIP://null
        str_data = "";
        str_alarm = str_data;
        statusLabel->setStyleSheet("color:darkgray");
        statusLabel->setText(str_data);
        break;
    case NORMAL_TIP://normal
        str_data = "NORMAL:"+text;
        statusBar()->showMessage(str_data,3000);
        break;
    case WARN_TIP://warn yellow
        str_data = "WARN:"+text;
        statusLabel->setStyleSheet("color:yellow");
        statusLabel->setText(str_data);
//        statusBar()->showMessage(data,0);
        break;
    case ERROR_TIP://error red
        str_data = "ERROR:"+text;
        statusLabel->setStyleSheet("color:red");
        statusLabel->setText(str_data);
//        statusBar()->showMessage(data,0);
        break;
    }
}
//void MainWindow::logwrite(int tipstype, QString text)
//{
//    QString str_data;
//    QDateTime CurrentTime=QDateTime::currentDateTime();
//    QString Timestr=CurrentTime.toString("yyyy-MM-dd hh:mm:ss "); //设置显示的格式
//    switch(tipstype){
//    case NORMAL_TIP:
//        str_data = "NORMAL:"+Timestr+text;
//        break;
//    case WARN_TIP:
//        str_data = "WARN:"+Timestr+text;
//        break;
//    case ERROR_TIP:
//        str_data = "ERROR:"+Timestr+text;
//        g_beeptype = 2;//所有报警状态下蜂鸣器响
//        break;
//    }
//    QFile log(LOGFILEPATH);
//    if(log.size()>500*1000)
//    {
//        QFile::remove(LOGFILEPATHBACK);
//        log.copy(LOGFILEPATHBACK);
//        log.remove();
//    }
//    if(log.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
//    {
//        QTextStream in(&log);
//        in.setCodec(QTextCodec::codecForName("UTF-8"));
//        in<<str_data<<endl;
//        log.close();
//    }
//}

void MainWindow::check_lightstate()
{
    //上灯
    if(update_thread->m_bLighton ==true)//灯点亮状态
    {
        if(g_isBgRunning)
        {
            if(alarmswitch_bit.Light_ison||alarmswitch_bit.Light_power)//点亮异常
            {
                if(g_uplighton_error)
                {
                    g_lightstate = 2;
                }
                else
                {
                    g_lightstate = 3;
                }
            }
            else
            {
                g_lightstate = 3;
            }
        }
        else
        {
            if(alarmswitch_bit.Light_ison||alarmswitch_bit.Light_power)//点亮异常
            {
                g_lightstate = 2;
            }
            else
            {
                g_lightstate = 3;
            }
        }
    }
    else
    {
        g_lightstate = 0;
    }
    //异常报警
    if(alarmswitch_bit.Cool_vel||alarmswitch_bit.Light_tem)
    {
        g_lightstate = 1;
    }
    switch (g_lightstate)
    {
    case 0:
        ui->frame_light_gif->setStyleSheet("#frame_light_gif{border-image:url(:/images/image/led_black.png)}");
        ui->frame_light_gif_2->setStyleSheet("#frame_light_gif_2{border-image:url(:/images/image/led_black.png)}");
        break;
    case 1:
    case 2:
        ui->frame_light_gif->setStyleSheet("#frame_light_gif{border-image:url(:/images/image/led_red.png)}");
        ui->frame_light_gif_2->setStyleSheet("#frame_light_gif_2{border-image:url(:/images/image/led_red.png)}");
        break;
    case 3:
        ui->frame_light_gif->setStyleSheet("#frame_light_gif{border-image:url(:/images/image/led_green.png)}");
        ui->frame_light_gif_2->setStyleSheet("#frame_light_gif_2{border-image:url(:/images/image/led_green.png)}");
        break;
    }
    if(alarmswitch_bit.Light_power)
    {
        if(g_language>1)
        {
            ui->lineEdit_lightpowerstate->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:10px;font:bold;");
            ui->lineEdit_lightpowerstate_2->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:10px;font:bold;");
        }
        else
        {
            ui->lineEdit_lightpowerstate->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
            ui->lineEdit_lightpowerstate_2->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
        }
        ui->lineEdit_lightpowerstate->setText(QObject::tr("异常"));
        ui->lineEdit_lightpowerstate_2->setText(QObject::tr("异常"));
    }
    else
    {
        if(g_language>1)
        {
            ui->lineEdit_lightpowerstate->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:10px;font:bold;");
            ui->lineEdit_lightpowerstate_2->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:10px;font:bold;");
        }
        else
        {
            ui->lineEdit_lightpowerstate->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
            ui->lineEdit_lightpowerstate_2->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
        }
        ui->lineEdit_lightpowerstate->setText(QObject::tr("正常"));
        ui->lineEdit_lightpowerstate_2->setText(QObject::tr("正常"));
    }
}
void MainWindow::Zpos_check()
{
    float now_value = 0.00;
    float lb_wz,dw_wz,bg_wz;
    if(g_now_frame==0)
    {
        now_value = fabs(update_thread->threaddriver->Axis_CurrentPos(AXIS_Z));
        lb_wz = g_up_lbposition;
        dw_wz = g_up_dwposition;
        bg_wz = g_up_bgposition;
    }
    else if(g_now_frame==1)
    {
        now_value = fabs(update_thread->threaddriver->Axis_CurrentPos(AXIS_Z));
        lb_wz = g_down_lbposition;
        dw_wz = g_down_dwposition;
        bg_wz = g_down_bgposition;
    }
    else
    {
        m_zpos_type = -1;
        return;
    }
    if(now_value==0.00)
    {
        m_zpos_type = PT_HOME;
    }
    else if(fabs(now_value-lb_wz)<=0.01)
    {
        m_zpos_type = PT_LBPOSITION;
    }
    else if(fabs(now_value-dw_wz)<=0.01)
    {
        m_zpos_type = PT_DWPOSITION;
    }
    else if(fabs(now_value-bg_wz)<=0.01)
    {
        m_zpos_type = PT_BGPOSITION;
    }
    else
    {
        m_zpos_type = -1;
    }
}
//界面按钮状态
void MainWindow::btn_state()
{
    //有轴处于运动状态，屏蔽按钮操作
    bool state=true;
    if((update_thread->m_bAxis_state==false)||g_ishoming||(g_ishomed==false)||g_isSysRunning||g_isBgRunning||g_isSysContinueRun
            ||g_isCCDMoveThread[0]||g_isCCDMoveThread[1]||g_framechange||g_isDownPTDuiWei)
    {
        state = false;
    }
    if(g_ishoming)
    {
        //增量 连续的+ -
        ui->btn_negtive->setEnabled(false);
        ui->btn_positive->setEnabled(false);
    }
    else
    {
        ui->btn_negtive->setEnabled(true);
        ui->btn_positive->setEnabled(true);
    }
    if(g_ishoming||(g_ishomed==false)||g_isSysRunning||g_isBgRunning||g_isSysContinueRun
            ||g_isCCDMoveThread[0]||g_isCCDMoveThread[1]||g_framechange||g_isDownPTDuiWei)
    {

        //手动CCD
//        ui->btn_ccdx_n->setEnabled(false);
//        ui->btn_ccdx_p->setEnabled(false);
//        ui->btn_ccdy_n->setEnabled(false);
//        ui->btn_ccdy_p->setEnabled(false);
    }
    else
    {
//        ui->btn_ccdx_n->setEnabled(true);
//        ui->btn_ccdx_p->setEnabled(true);
//        ui->btn_ccdy_n->setEnabled(true);
//        ui->btn_ccdy_p->setEnabled(true);
    }
    //复位
    if((g_ishomed==0)&&(g_ishoming==0))
    {
        ui->btn_reset->setEnabled(true);
    }
    else
    {
        ui->btn_reset->setEnabled(state);
    }
    //清零
    ui->btn_upframe_clear->setEnabled(state);
    ui->btn_downframe_clear->setEnabled(state);
    //冷风机
    if(update_thread->m_bcool_out)
    {
        if(g_language>1)
        {
            ui->btn_cool->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_cool->setStyleSheet("background:green;");
        }
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_cool->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_cool->setStyleSheet("background:darkgray;");
        }
    }
    if(g_ishomed==false)
    {
        ui->btn_cool->setEnabled(true);
    }
    else
    {
        ui->btn_cool->setEnabled(state);
    }
    //底片真空
    if(update_thread->m_dp_zhengkon_out)
    {
            ui->btn_dp_zhengkon->setStyleSheet("background:green;");//底片真空

    }
    else
    {
            ui->btn_dp_zhengkon->setStyleSheet("background:darkgray;");//底片真空

    }
    //CCD伺服转矩
    if(update_thread->m_ccd_sifu_out)
    {
            ui->btn_CCD_on->setStyleSheet("background:green;");//

    }
    else
    {
            ui->btn_CCD_on->setStyleSheet("background:darkgray;");//

    }
    //台面真空泵
//    if(update_thread->m_taimian_ZKBen_out)
//    {
//            ui->btn_taimian_ZKBen->setStyleSheet("background:green;");//t台面真空泵

//    }
//    else
//    {
//            ui->btn_taimian_ZKBen->setStyleSheet("background:darkgray;");//台面真空泵

//    }
    //曝光室灯源姿态
    if(update_thread->m_light_state_out)
    {
            ui->btn_BG_Light->setStyleSheet("background:green;");//真空

    }
    else
    {
            ui->btn_BG_Light->setStyleSheet("background:darkgray;");//真空

    }
    //台面鼓风机
    if(update_thread->m_taimian_gu_out)
    {
        if(g_language>1)
        {
            ui->btn_taimian_gufenji->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_taimian_gufenji->setStyleSheet("background:green;");
        }
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_taimian_gufenji->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_taimian_gufenji->setStyleSheet("background:darkgray;");
        }
    }
//    if(g_ishomed==false)
//    {
//        ui->btn_taimian_gufenji->setEnabled(true);
//    }
//    else
//    {
//        ui->btn_taimian_gufenji->setEnabled(state);
//    }
    //台面冰水机
//    if(update_thread->m_taimian_bin_out)
//    {
//        if(g_language>1)
//        {
//            ui->btn_taimian_binshuiji->setStyleSheet("background:green;font-size:10px;");
//        }
//        else
//        {
//            ui->btn_taimian_binshuiji->setStyleSheet("background:green;");
//        }
//    }
//    else
//    {
//        if(g_language>1)
//        {
//            ui->btn_taimian_binshuiji->setStyleSheet("background:darkgray;font-size:10px;");
//        }
//        else
//        {
//            ui->btn_taimian_binshuiji->setStyleSheet("background:darkgray;");
//        }
//    }
//    if(g_ishomed==false)
//    {
//        ui->btn_taimian_binshuiji->setEnabled(true);
//    }
//    else
//    {
//        ui->btn_taimian_binshuiji->setEnabled(state);
//    }
    //灯管点亮
    if(update_thread->m_bLighton)
    {
        if(g_language>1)
        {
            ui->btn_light->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_light->setStyleSheet("background:green;");
        }
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_light->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_light->setStyleSheet("background:darkgray;");
        }
    }
    if(g_ishomed==false)
    {
        ui->btn_light->setEnabled(true);
    }
    else
    {
        ui->btn_light->setEnabled(state);
    }
    //曝光测试
    if(g_language>1)
    {
        ui->btn_bg_test->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_bg_test->setStyleSheet("background:darkgray;");
    }
    ui->btn_bg_test->setEnabled(state);
    //
    //油墨设定
    if(g_language>1)
    {
        ui->btn_oilsetting->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_oilsetting->setStyleSheet("background:darkgray;");
    }
    if(1==g_par.islightsetting)
    {
        if(g_ishomed==false)
        {
            ui->btn_oilsetting->setEnabled(true);
        }
        else
        {
            ui->btn_oilsetting->setEnabled(state);
        }
    }
    else
    {
        ui->btn_oilsetting->setEnabled(true);
    }
    //摄影机
    if(1==update_thread->m_ccd_down)
    {
        if(g_language>1)
        {
            ui->btn_ccd_down->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_ccd_down->setStyleSheet("background:green;");
        }
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_ccd_down->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_ccd_down->setStyleSheet("background:darkgray;");
        }
    }
    if(g_now_frame==0)
    {
        ui->btn_ccd_down->setEnabled(false);
    }
    else if(g_now_frame==1)
    {
        ui->btn_ccd_down->setEnabled(state);
    }

    //平台置中
    if(g_language>1)
    {
        ui->btn_platform_center->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_platform_center->setStyleSheet("background:darkgray;");
    }
    ui->btn_platform_center->setEnabled(state);

    //框架真空
    if(update_thread->m_bframeabsorb==1)
    {
        if(g_language>1)
        {
            ui->btn_frame_absorb->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_frame_absorb->setStyleSheet("background:green;");
        }
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_frame_absorb->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_frame_absorb->setStyleSheet("background:darkgray;");
        }
    }
    ui->btn_frame_absorb->setEnabled(state);
    //自动定位
    if(g_language>1)
    {
        ui->btn_autoset->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_autoset->setStyleSheet("background:darkgray;");
    }
    ui->btn_autoset->setEnabled(state);
    //底片架设
    if(g_language>1)
    {
        ui->btn_dp_load->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_dp_load->setStyleSheet("background:darkgray;");
    }
    if(g_par.is_dpvacuum==1)
    {
        ui->btn_dp_load->setEnabled(state);
        ui->btn_dp_vacuum->setEnabled(state);
       // ui->btn_dp_vacuum2->setEnabled(state);
        if(update_thread->m_dp_zhengkon_out)
        {
            if(g_language>1)
            {
                ui->btn_dp_vacuum->setStyleSheet("background:green;font-size:10px;");
            }
            else
            {
                ui->btn_dp_vacuum->setStyleSheet("background:green;");
            }

        }
        else
        {
           if(g_language>1)
           {
               ui->btn_dp_vacuum->setStyleSheet("background:darkgray;font-size:10px;");
           }
           else
           {
               ui->btn_dp_vacuum->setStyleSheet("background:darkgray;");
           }
        }
        if(update_thread->m_bm_zhengkon_out)
        {
            if(g_language>1)
            {
                ui->btn_bm_vacuum->setStyleSheet("background:green;font-size:10px;");
            }
            else
            {
                ui->btn_bm_vacuum->setStyleSheet("background:green;");
            }
        }
        else
        {
           if(g_language>1)
           {
               ui->btn_bm_vacuum->setStyleSheet("background:darkgray;font-size:10px;");
           }
           else
           {
               ui->btn_bm_vacuum->setStyleSheet("background:darkgray;");
           }
        }
    }
    else if(g_par.is_dpvacuum==0)
    {
        ui->btn_dp_load->setEnabled(false);
        ui->btn_dp_vacuum->setEnabled(false);
        ui->btn_bm_vacuum->setEnabled(false);
    }
    //校正靶标
    if(g_language>1)
    {
        ui->btn_mark_check->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_mark_check->setStyleSheet("background:darkgray;");
    }
    ui->btn_mark_check->setEnabled(state);
    //靶标切换
    if(g_language>1)
    {
        ui->btn_mark_change->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_mark_change->setStyleSheet("background:darkgray;");
    }

    ui->btn_mark_change->setEnabled(state);

    //相机高低速
    if((update_thread->m_isxlight_on==true))
    {
        g_axis_speed_type = true;
    }
    else if((update_thread->m_isxlight_on==false))
    {
        g_axis_speed_type = false;
    }
    if(g_axis_speed_type)
    {
        if(g_language>1)
        {
            ui->btn_ccd_lowspeed->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_ccd_lowspeed->setStyleSheet("background:green;");
        }

        ui->btn_ccd_lowspeed->setText(QObject::tr("相机高速"));
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_ccd_lowspeed->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_ccd_lowspeed->setStyleSheet("background:darkgray;");
        }

        ui->btn_ccd_lowspeed->setText(QObject::tr("相机低速"));
    }
    ui->btn_ccd_lowspeed->setEnabled(state);
    //主靶保存
    if(g_language>1)
    {
        ui->btn_save_mainmark->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_save_mainmark->setStyleSheet("background:darkgray;");
    }
    ui->btn_save_mainmark->setEnabled(state);
    //次靶保存
    if(g_language>1)
    {
        ui->btn_save_othermark->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_save_othermark->setStyleSheet("background:darkgray;");
    }
    ui->btn_save_othermark->setEnabled(state);
    //胶条低压
    if(((update_thread->m_bjtdy)&&(!update_thread->m_bjtcq))||((!update_thread->m_bjtdy)&&(update_thread->m_bjtcq)))
    {
        if(g_language>1)
        {
            ui->btn_jt_dy->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_jt_dy->setStyleSheet("background:green;");
        }

        if(update_thread->m_bjtdy)
        {
            ui->btn_jt_dy->setText(QObject::tr("胶条低压"));
        }
        if(update_thread->m_bjtcq)
        {
            ui->btn_jt_dy->setText(QObject::tr("胶条充气"));
        }
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_jt_dy->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_jt_dy->setStyleSheet("background:darkgray;");
        }

        ui->btn_jt_dy->setText(QObject::tr("胶条低压"));
    }
    ui->btn_jt_dy->setEnabled(state);
    //相机让位
    m_isCCDdw[0] = bgflow->isccdsafe(0);
    if(false==m_isCCDdw[0])
    {
        if(g_language>1)
        {
            ui->btn_ccd_rw->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_ccd_rw->setStyleSheet("background:darkgray;");
        }
        ui->btn_ccd_rw->setText(QObject::tr("相机让位"));
    }
    else
    {
        if(g_language>1)
        {
            ui->btn_ccd_rw->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_ccd_rw->setStyleSheet("background:green;");
        }

        ui->btn_ccd_rw->setText(QObject::tr("相机对位"));
    }
    ui->btn_ccd_rw->setEnabled(state);

    m_isCCDdw[1] = bgflow->isccdsafe(1);
    //Z轴位置
    Zpos_check();//检测当前框Z的位置
    switch(m_zpos_type)
    {
    case PT_HOME:
        if(g_language>1)
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:green;font-size:10px;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:darkgray;font-size:10px;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:darkgray;font-size:10px;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:green;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:darkgray;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:darkgray;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:darkgray;");
        }
        break;
    case PT_LBPOSITION:
        if(g_language>1)
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:darkgray;font-size:10px;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:green;font-size:10px;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:darkgray;font-size:10px;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:darkgray;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:green;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:darkgray;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:darkgray;");
        }
        break;
    case PT_DWPOSITION:
        if(g_language>1)
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:darkgray;font-size:10px;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:darkgray;font-size:10px;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:green;font-size:10px;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:darkgray;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:darkgray;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:green;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:darkgray;");
        }
        break;
    case PT_BGPOSITION:
        if(g_language>1)
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:darkgray;font-size:10px;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:darkgray;font-size:10px;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:darkgray;font-size:10px;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            //下降定位
            ui->btn_position_down->setStyleSheet("background:darkgray;");
            //离板位置
            ui->btn_position_lb->setStyleSheet("background:darkgray;");
            //对位位置
            ui->btn_position_dw->setStyleSheet("background:darkgray;");
            //曝光位置
            ui->btn_position_bg->setStyleSheet("background:green;");
        }
        break;
    }
    ui->btn_position_down->setEnabled(state);
    ui->btn_position_lb->setEnabled(state);
    ui->btn_position_dw->setEnabled(state);
    ui->btn_position_bg->setEnabled(state);
    //框架交换
    if(g_language>1)
    {
        ui->btn_frame_change->setStyleSheet("background:darkgray;font-size:10px;");
    }
    else
    {
        ui->btn_frame_change->setStyleSheet("background:darkgray;");
    }

    ui->btn_frame_change->setEnabled(state);
    //料号存取
    ui->button_liaohao->setEnabled(state);
    //dp dg
    if(m_pageindex!=0)
    {
        ui->btn_dg->setEnabled(false);
        ui->btn_dp->setEnabled(false);
    }
    else
    {
        if(g_ishomed==false)
        {
            ui->btn_dg->setEnabled(true);
        }
        else
        {
            ui->btn_dg->setEnabled(state);
        }
        ui->btn_dp->setEnabled(state);
    }
    //归零
    ui->btn_uplight_clear->setEnabled(state);
    //M0 M1 M2 M3
    if(g_now_frame>=0)
    {
        if(0==g_par.check8[g_now_frame])
        {
            if(update_thread->m_bEnableMarkChange[g_now_frame][0])
            {
               ui->btn_M0->setStyleSheet("background:green;");
            }
            else
            {
               ui->btn_M0->setStyleSheet("background:darkgray;");
            }

            if(update_thread->m_bEnableMarkChange[g_now_frame][1])
            {
               ui->btn_M1->setStyleSheet("background:green;");
            }
            else
            {
               ui->btn_M1->setStyleSheet("background:darkgray;");
            }

            if(update_thread->m_bEnableMarkChange[g_now_frame][2])
            {
               ui->btn_M2->setStyleSheet("background:green;");
            }
            else
            {
               ui->btn_M2->setStyleSheet("background:darkgray;");
            }

            if(update_thread->m_bEnableMarkChange[g_now_frame][3])
            {
               ui->btn_M3->setStyleSheet("background:green;");
            }
            else
            {
               ui->btn_M3->setStyleSheet("background:darkgray;");
            }
        }
        else if(1==g_par.check8[g_now_frame])
        {
            if(1==g_par.ch_othermark[g_now_frame][0])
            {
               ui->btn_M0->setStyleSheet("background:green;");
            }
            else if(0==g_par.ch_othermark[g_now_frame][0])
            {
               ui->btn_M0->setStyleSheet("background:darkgray;");
            }

            if(1==g_par.ch_othermark[g_now_frame][1])
            {
               ui->btn_M1->setStyleSheet("background:green;");
            }
            else if(0==g_par.ch_othermark[g_now_frame][1])
            {
               ui->btn_M1->setStyleSheet("background:darkgray;");
            }

            if(1==g_par.ch_othermark[g_now_frame][2])
            {
               ui->btn_M2->setStyleSheet("background:green;");
            }
            else if(0==g_par.ch_othermark[g_now_frame][2])
            {
               ui->btn_M2->setStyleSheet("background:darkgray;");
            }

            if(1==g_par.ch_othermark[g_now_frame][3])
            {
               ui->btn_M3->setStyleSheet("background:green;");
            }
            else if(0==g_par.ch_othermark[g_now_frame][3])
            {
               ui->btn_M3->setStyleSheet("background:darkgray;");
            }
        }
    }
    else
    {
        ui->btn_M0->setEnabled(false);
        ui->btn_M1->setEnabled(false);
        ui->btn_M2->setEnabled(false);
        ui->btn_M3->setEnabled(false);
    }
    ui->btn_M0->setEnabled(state);
    ui->btn_M1->setEnabled(state);
    ui->btn_M2->setEnabled(state);
    ui->btn_M3->setEnabled(state);
    //正常生产
    if(g_isPauseRun)
    {
        ui->btn_normalprocess->setEnabled(true);
        if(g_language>1)
        {
            ui->btn_normalprocess->setStyleSheet("background:green;font-size:10px;");
        }
        else
        {
            ui->btn_normalprocess->setStyleSheet("background:green;");
        }
        ui->btn_normalprocess->setText(QObject::tr("暂停运行"));
    }
    else
    {
        ui->btn_normalprocess->setEnabled(state);
        if(g_language>1)
        {
            ui->btn_normalprocess->setStyleSheet("background:darkgray;font-size:10px;");
        }
        else
        {
            ui->btn_normalprocess->setStyleSheet("background:darkgray;");
        }
        ui->btn_normalprocess->setText(QObject::tr("正常生产"));
    }
    //系统退出 f1 f2 f3
    if((g_ishomed==0)&&(g_ishoming==0))
    {
        ui->btn_systemexit->setEnabled(true);
        //f1 f2 f3
        ui->btn_f1->setEnabled(true);
        ui->btn_f2->setEnabled(true);
        ui->btn_f3->setEnabled(true);
    }
    else
    {
        ui->btn_systemexit->setEnabled(state);
        //f1 f2 f3
        ui->btn_f1->setEnabled(state);
        ui->btn_f2->setEnabled(state);
        ui->btn_f3->setEnabled(state);
    }
    //用户密码  厂商密码
    if(g_settings_kind==14)
    {
       ui->btn_systemdebug->setEnabled(false);
       ui->btn_change_password->setEnabled(false);
    }
    else if(g_settings_kind==15)
    {
        ui->btn_systemdebug->setEnabled(true);
        ui->btn_change_password->setEnabled(true);
    }
    //维护检测
    if((g_ishomed==0)&&(g_ishoming==0))
    {
        ui->btn_systemcheck->setEnabled(true);
        //参数设置
        ui->btn_para_setting->setEnabled(true);
        //IO参数 机床参数
        ui->btn_iopara->setEnabled(true);
        ui->btn_cncpara->setEnabled(true);
        //选中轴回零
        ui->btn_home_select->setEnabled(true);
        //语言切换
        ui->btn_language_switch->setEnabled(true);
    }
    else
    {
        ui->btn_systemcheck->setEnabled(state);
        //参数设置
        ui->btn_para_setting->setEnabled(state);
        //IO参数 机床参数
        ui->btn_iopara->setEnabled(state);
        ui->btn_cncpara->setEnabled(state);
        //选中轴回零
        ui->btn_home_select->setEnabled(state);
        //语言切换
        ui->btn_language_switch->setEnabled(state);
    }
//    //CCD学习
//    ui->btn_dw_study->setEnabled(state);
    //CCD para
//    if(ui->stackedWidget_left->currentIndex()==1)
//    {
//       ui->stackedWidget_left->setEnabled(state);
//    }
//    else
//    {
//       ui->stackedWidget_left->setEnabled(true);
//    }
}

void MainWindow::check_state()
{
    QString str;
    //全功限时检测
    static int quick_door_record=0;
    static int old_open_state=0;
    int open_state=update_thread->m_bLighton;//灯管点亮Power_on_out
    if(g_par.Light_type==1)//LED
    {
      if(open_state==1)
      {
         quick_door_record+=1;
      }
      else
      {
         quick_door_record=0;
      }
      if(quick_door_record>g_par.quick_door_time)//超过快门全功限时则报警
      {
        slot_alarmshow(false,QObject::tr("灯光已达全功限时!"));
        quick_door_record=0;
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,false);
      }
      if(old_open_state==1&&open_state!=1)
      {
        quick_door_record=0;
      }
    }
    else
    {
        quick_door_record=0;
    }
    old_open_state=open_state;
    //计算上框框架真空吸附时间
    static int old_absorb_out=0;
    int absorb_out=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
    int absorb_ok=update_thread->input.Air_check;//框架真空到达Air_check
    if((old_absorb_out==0)&&(absorb_out==1))
    {
        m_absorb_time=0;
    }
    if((absorb_out==1)&&(absorb_ok==0))
    {
        m_absorb_time+=1;
    }
    old_absorb_out=absorb_out;
    str.sprintf("%.2f",m_absorb_time*((int)FIRSTTIMER)/1000.0);
    ui->lineEdit_upairckeck_time->setText(str);
    //计算下框框架真空吸附时间
    static int old_absorb_out2=0;
    int absorb_out2=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
    int absorb_ok2=update_thread->input.Air_check2;//框架真空到达Air_check2
    if((old_absorb_out2==0)&&(absorb_out2==1))
    {
        m_absorb_time2=0;
    }
    if((absorb_out2==1)&&(absorb_ok2==0))
    {
        m_absorb_time2+=1;
    }
    old_absorb_out2=absorb_out2;
    str.sprintf("%.2f",m_absorb_time2*((int)FIRSTTIMER)/1000.0);
    ui->lineEdit_downaircheck_time->setText(str);
    //框架及真空指示灯
 //   int light_state=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_light,PORT_OUT);//框架及真空灯的状态Frame_light_out
    bool light_out;
    if(g_ishomed==1&&g_isBgRunning==false) light_out=true;
    else light_out=false;
//    if(light_state!=light_out&&m_is_weihu!=true)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_light,light_out);
//    }
    //上框曝光完成总数
    int num=g_par.Frame_finish_num/g_par.bg_down_time;
    static int value=g_par.Frame_finish_num;
    ui->lineEdit_uppiece_ok->setText(QString::number(num,10));
    if(value!=num)
    {
       inifilerw->WriteIni("TOP","FRAME_FINISH_NUM",QString::number(num,10));
    }
    value=num;
    //上框底片曝光完成计数
    int dpnum=g_par.DP_BG_NUM;
    static int dpvalue=g_par.DP_BG_NUM;
    if(dpvalue!=dpnum)
    {
       inifilerw->WriteIni("TOP","DP_BG_NUM",QString::number(dpnum,10));
    }
    dpvalue=dpnum;
    //上框底片清洁计数
    int dpclearnum=g_par.DP_clear_num;
    static int dpclearvalue=g_par.DP_clear_num;
    if(dpclearvalue!=dpclearnum)
    {
       inifilerw->WriteIni("TOP","DP_CLEAR_NUM",QString::number(dpclearnum,10));
    }
    dpclearvalue=dpclearnum;
    //jb
    int jbnum=g_par.JB_finish_num;
    ui->lineEdit_uppiece_jb->setText(QString::number(jbnum,10));
    static int jbvalue=g_par.JB_finish_num;
    if(jbvalue!=jbnum)
    {
        inifilerw->WriteIni("TOP","JB_FINISH_NUM",QString::number(jbnum,10));
        if(jbnum>jbvalue)
        {
//            slot_alarmshow(false,QObject::tr("上框对位拒曝!");
            g_beeptype =1;
        }
    }
    jbvalue=jbnum;
    //下框曝光完成总数
    int num2=g_par.Frame2_finish_num;
    ui->lineEdit_downpiece_ok->setText(QString::number(num2,10));
    static int value2=g_par.Frame2_finish_num;
    if(value2!=num2)
    {
       inifilerw->WriteIni("TOP","FRAME2_FINISH_NUM",QString::number(num2,10));
    }
    value2=num2;
    //上框底片曝光完成计数
    int dpnum2=g_par.DP2_BG_NUM;
    static int dpvalue2=g_par.DP2_BG_NUM;
    if(dpvalue2!=dpnum2)
    {
       inifilerw->WriteIni("TOP","DP2_BG_NUM",QString::number(dpnum2,10));
    }
    dpvalue2=dpnum2;
    //下框底片清洁计数
    int dpclearnum2=g_par.DP2_clear_num;
    static int dpclearvalue2=g_par.DP2_clear_num;
    if(dpclearvalue2!=dpclearnum2)
    {
       inifilerw->WriteIni("TOP","DP2_CLEAR_NUM",QString::number(dpclearnum2,10));
    }
    dpclearvalue2=dpclearnum2;
    //jb2
    int jbnum2=g_par.JB2_finish_num;
    ui->lineEdit_downpiece_jb->setText(QString::number(jbnum2,10));
    static int jbvalue2=g_par.JB2_finish_num;
    if(jbvalue2!=jbnum2)
    {
        inifilerw->WriteIni("TOP","JB2_FINISH_NUM",QString::number(jbnum2,10));
        if(jbnum2>jbvalue2)
        {
//            slot_alarmshow(false,QObject::tr("下框对位拒曝!");
            g_beeptype =1;
        }
    }
    jbvalue2=jbnum2;
    //点灯次数
    ui->lineEdit_light_count->setText(QString::number(g_par.light_num,10));
    ui->lineEdit_light_count_2->setText(QString::number(g_par.light_num,10));
    static bool old_state=false;
    bool state =false;
    if(g_lightstate==3) state=true;
    else state=false;
    if(old_state!=state)
    {
        if(state==true)
        {
            g_par.light_num++;
            QString tr_num = QString::number(g_par.light_num,10);
            qDebug()<<"save LIGHT_NUM="+tr_num;
            inifilerw->WriteIni("TOP","LIGHT_NUM",tr_num);
        }
    }
    old_state=state;
    //灯管计时:点亮正常记时开始
    static bool old_light=false;
    bool light=false;
    if(g_lightstate==3) light=true;
    else light=false;
    if((light==true)&&(old_light==false))
    {
        timelighton[0].start();
    }
    if((light==false)&&(old_light==true))//熄灭时保存
    {
       m_lighttime = timelighton[0].elapsed();
       QString tr_val;
//       tr_val.sprintf("%f",m_lighttime*0.9);
//       qDebug()<<"m_lighttime f="+tr_val;
       float tmptime = ((double)m_lighttime*0.9)/1000.0;
       g_par.light_time +=tmptime;
       tr_val.sprintf("%f",g_par.light_time);
       inifilerw->WriteIni("TOP","LIGHT_TIME",tr_val);
       qDebug()<<"save LIGHT_TIME="+tr_val;
    }
    old_light=light;
    str.sprintf("%.1f",g_par.light_time/3600.0);//单位:小时
    ui->lineEdit_light_hour->setText(str);
    ui->lineEdit_light_hour_2->setText(str);
    //下灯
    static bool old_downstate=false;
    bool downstate = false;
    if(g_downlightstate==3) downstate= true;
    else downstate=false;
    if(old_downstate!=downstate)
    {
        if(downstate==true)
        {
            g_par.downlight_num++;
            QString tr_valnum = QString::number(g_par.downlight_num,10);
            qDebug()<<"save DOWNLIGHT_NUM="+tr_valnum;
            inifilerw->WriteIni("TOP","DOWNLIGHT_NUM",tr_valnum);
        }
    }
    old_downstate=downstate;
    //曝光时间
    if(g_par.bg_mode==1)//时间模式
    {
        int time = 0;
        //上下框曝光时间独立计时
        /*if(g_now_frame==0)
        {
            time = (int)(update_thread->m_record_downframelight_time/100.00);
            m_downbg_time[0] = (float)time/10.0;
        }
        else */if(g_now_frame==0)
        {
            time = (int)(update_thread->m_record_light_time/100.00);
            m_upbg_time[0] = (float)time/10.0;
        }
        QString tr_bgval;
        tr_bgval.sprintf("%.1f",m_downbg_time[0]);
        ui->lineEdit_downlight_waittime->setText(tr_bgval);
        ui->lineEdit_downlight_waittime_2->setText(tr_bgval);

        tr_bgval.sprintf("%.1f",m_upbg_time[0]);
        ui->lineEdit_light_waittime->setText(tr_bgval);
        ui->lineEdit_light_waittime_2->setText(tr_bgval);
    }
    else//能量模式
    {

    }
    QString str_down="0.000 mm";
    QString str_lb="0.000 mm";
    QString str_dw="0.000 mm";
    QString str_bg="0.000 mm";
    if(g_now_frame==0)
    {
        str_down ="0.000 mm";
        str_lb.sprintf("%.3f mm",g_up_lbposition);
        str_dw.sprintf("%.3f mm",g_up_dwposition);
        str_bg.sprintf("%.3f mm",g_up_bgposition);
    }
//    else if(g_now_frame==1)
//    {
//        str_down ="0.000 mm";
//        str_lb.sprintf("%.3f mm",g_down_lbposition);
//        str_dw.sprintf("%.3f mm",g_down_dwposition);
//        str_bg.sprintf("%.3f mm",g_down_bgposition);
//    }
    //下降位置
    ui->lable_value_z_down->setText(str_down);
    //离板位置
    ui->lable_value_z_lb->setText(str_lb);
    //对位位置
    ui->lable_value_z_dw->setText(str_dw);
    //曝光位置
    ui->lable_value_z_bg->setText(str_bg);
    ui->lineEdit_bgchishu->setText(QString::number(g_par.bg_down_time));//底片分几次曝光
    ui->lineEdit_bgpos->setText(QString::number(g_par.bg_pos));//改为单前曝光位置 第几次
}
void MainWindow::check_axisalarm()
{
    //急停时不需检测，按下急停会触发驱动器断电
    if(0==update_thread->input.EstopSig)
    {
        return;
    }
    if(0==update_thread->input.Ccd_isok)
    {
        str_alarm = QObject::tr("CCD驱动器异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!axis_alarm_bit[AXIS_CCD1Y].alarm)
        {
            axis_alarm_bit[AXIS_CCD1Y].alarm = true;
            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(axis_alarm_bit[AXIS_CCD1Y].alarm)
        {
           axis_alarm_bit[AXIS_CCD1Y].alarm = false;
           showtips(NULL_TIP,"");
        }
    }
    //伺服轴报警检测
    if(true==update_thread->threaddriver->Axis_Alarm(AXIS_W))
    {
        str_alarm = QObject::tr("框架驱动器异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!axis_alarm_bit[AXIS_W].alarm)
        {
            axis_alarm_bit[AXIS_W].alarm = true;
            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(axis_alarm_bit[AXIS_W].alarm)
        {
           axis_alarm_bit[AXIS_W].alarm = false;
           showtips(NULL_TIP,"");
        }
    }
    //
    if(g_par.is_usescanlight==1)
    {
        //if(true==update_thread->threaddriver->Axis_Alarm(AXIS_UPSCAN))
        if(true==update_thread->input.Light_moveok)
        {
            str_alarm = QObject::tr("灯扫描驱动器异常！");
            showtips(ERROR_TIP,str_alarm);
            if(!axis_alarm_bit[AXIS_UPSCAN].alarm)
            {
                axis_alarm_bit[AXIS_UPSCAN].alarm = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
            }
        }
        else
        {
            if(axis_alarm_bit[AXIS_UPSCAN].alarm)
            {
               axis_alarm_bit[AXIS_UPSCAN].alarm = false;
               showtips(NULL_TIP,"");
            }
        }
    }
    else if(g_par.is_usescanlight==0)
    {
        axis_alarm_bit[AXIS_UPSCAN].alarm = false;
    }
    for(int k=0;k<=AXIS_Z;k++)
    {
        if(true==update_thread->threaddriver->Axis_Alarm(k))
        {
            switch(k)
            {
            case AXIS_L:
                str_alarm = QObject::tr("L轴驱动器异常！");
                break;
            case AXIS_M:
                str_alarm = QObject::tr("M轴驱动器异常！");
                break;
            case AXIS_R:
                str_alarm = QObject::tr("R轴驱动器异常！");
                break;
            case AXIS_Z:
                str_alarm = QObject::tr("Z轴驱动器异常！");
                break;
            }
            showtips(ERROR_TIP,str_alarm);
            if(!axis_alarm_bit[k].alarm)
            {
                axis_alarm_bit[k].alarm = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
            }
        }
        else
        {
            if(axis_alarm_bit[k].alarm)
            {
               axis_alarm_bit[k].alarm = false;
               showtips(NULL_TIP,"");
            }
        }
    }
    if(axis_alarm_bit[AXIS_CCD1Y].alarm||axis_alarm_bit[AXIS_UPSCAN].alarm
       ||axis_alarm_bit[AXIS_W].alarm||axis_alarm_bit[AXIS_L].alarm
       ||axis_alarm_bit[AXIS_M].alarm||axis_alarm_bit[AXIS_R].alarm||axis_alarm_bit[AXIS_Z].alarm)
    {

        m_isaxis_alarm = true;
    }
    else
    {
        m_isaxis_alarm = false;
    }
    static bool old_alarmstate =false;
    if(old_alarmstate!=m_isaxis_alarm)
    {
        if(m_isaxis_alarm)
        {
           allaxis_disable();
        }
    }
    old_alarmstate = m_isaxis_alarm;
}
QString MainWindow::limitshow(int axis,int type)
{
    QString axisname = update_thread->axisname_list.at(axis);
    if(type==1)
    {
        str_alarm = QObject::tr("轴正向超出软件行程！");
    }
    else if(type==-1)
    {
        str_alarm = QObject::tr("轴负向超出软件行程！");
    }
    else if(type==0)
    {
        str_alarm = QObject::tr("轴原点限位！");
    }
    str_alarm = axisname+str_alarm;
    return str_alarm;
}

void MainWindow::check_softlimit()
{
    for(int axis=0;axis<=AXIS_UPSCAN;axis++)
    {
        if(1==update_thread->threaddriver->Axis_SoftLimit(axis))
        {
            str_alarm = limitshow(axis,1);
            showtips(ERROR_TIP,str_alarm);
            if(!axis_alarm_bit[AXIS_CCD1Y].softlimit_P)
            {
                axis_alarm_bit[AXIS_CCD1Y].softlimit_P = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
            }
        }
        else if(-1==update_thread->threaddriver->Axis_SoftLimit(axis))
        {
            str_alarm = limitshow(axis,-1);
            showtips(ERROR_TIP,str_alarm);
            if(!axis_alarm_bit[AXIS_CCD1Y].softlimit_N)
            {
                axis_alarm_bit[AXIS_CCD1Y].softlimit_N = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
            }
        }
        else
        {
            if(axis_alarm_bit[AXIS_CCD1Y].softlimit_P)
            {
               axis_alarm_bit[AXIS_CCD1Y].softlimit_P = false;
               showtips(NULL_TIP,"");
            }
            if(axis_alarm_bit[AXIS_CCD1Y].softlimit_N)
            {
               axis_alarm_bit[AXIS_CCD1Y].softlimit_N = false;
               showtips(NULL_TIP,"");
            }
        }
     }
}

void MainWindow::check_orglimit()
{
    if((!g_ishomed)||g_ishoming) return;
    if(!update_thread->m_bCnc_state) return;
    for(int axis=AXIS_CCD1X;axis<=AXIS_CCD4Y;axis++)
    {
        if((0.00==update_thread->threaddriver->Axis_CurrentPos(axis))
                && (1==update_thread->threaddriver->Axis_State(axis)))
        {
            if(false==update_thread->threaddriver->Axis_OrgLimit(axis))
            {
                QString axisname = update_thread->axisname_list.at(axis);
                str_alarm = axisname+QObject::tr("轴原点检测失败触发急停,请复位!");
                showtips(ERROR_TIP,str_alarm);
                if(!axis_alarm_bit[axis].org)
                {
                    axis_alarm_bit[axis].org = true;
                    update_thread->logwrite(ERROR_TIP,str_alarm);
                    if(update_thread->m_bCnc_state==1)
                    {
                       slot_alarmshow(false,str_alarm);
                       emit sig_estop();
                       return;
                    }
                }
            }
            else
            {
                if(axis_alarm_bit[axis].org)
                {
                   axis_alarm_bit[axis].org = false;
                   showtips(NULL_TIP,"");
                }
            }
        }
    }

    if((0.00==update_thread->threaddriver->Axis_CurrentPos(AXIS_Z))
            && (1==update_thread->threaddriver->Axis_State(AXIS_Z)))
    {
        if(false==update_thread->threaddriver->Axis_OrgLimit(AXIS_Z))
        {
            QString axisname = update_thread->axisname_list.at(AXIS_Z);
            str_alarm = axisname+QObject::tr("轴原点检测失败触发急停,请复位!");
            showtips(ERROR_TIP,str_alarm);
            if(!axis_alarm_bit[AXIS_Z].org)
            {
                axis_alarm_bit[AXIS_Z].org = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
                if(update_thread->m_bCnc_state==1)
                {
                   slot_alarmshow(false,str_alarm);
                   emit sig_estop();
                   return;
                }
            }
        }
        else
        {
            if(axis_alarm_bit[AXIS_Z].org)
            {
               axis_alarm_bit[AXIS_Z].org = false;
               showtips(NULL_TIP,"");
            }
        }
    }
}
void MainWindow::allaxis_disable()
{
    for(int num=0;num<=AXIS_UPSCAN;num++)
    {
        update_thread->threaddriver->Axis_Enable(num,false);
    }
    if(update_thread->m_bCnc_state==1)
    {
        qDebug()<<"disable all axis";
        slot_alarmshow(false,QObject::tr("驱动器报警触发急停,请复位!"));
        emit sig_estop();//驱动器报警则急停
    }
}

void MainWindow::check_inputalarm()
{
    if(1==update_thread->input.Sys_air)
    {
        str_alarm = QObject::tr("系统空压异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.Sys_air)
        {
            alarmswitch_bit.Sys_air = true;
            update_thread->logwrite(ERROR_TIP,str_alarm);
            if(update_thread->m_bCnc_state==1)
            {
               slot_alarmshow(false,QObject::tr("系统空压异常触发急停,请复位!"));
               emit sig_estop();
            }
        }
    }
    else
    {
        if(alarmswitch_bit.Sys_air)
        {
           alarmswitch_bit.Sys_air = false;
           showtips(NULL_TIP,"");
        }
    }
    static int uptimecnt = 0;
    if(update_thread->m_bLighton ==true)
    {
        uptimecnt++;
        if(uptimecnt==200)//1s
        {
            uptimecnt = 0;
            if(true==update_thread->input.Light_power)
            {
                str_alarm = QObject::tr("灯电源异常！");
                showtips(ERROR_TIP,str_alarm);
                if(!alarmswitch_bit.Light_power)
                {
                    alarmswitch_bit.Light_power = true;
                    update_thread->logwrite(ERROR_TIP,str_alarm);
                    if(g_isBgRunning)
                    {
                         slot_alarmshow(false,QObject::tr("曝光运行中灯电源异常，请检查灯源!"));
                    }
                }
            }
            else
            {
                if(alarmswitch_bit.Light_power)
                {
                   alarmswitch_bit.Light_power = false;
                   showtips(NULL_TIP,"");
                }
            }
        }
    }
    else
    {
        if(alarmswitch_bit.Light_power)
        {
           alarmswitch_bit.Light_power = false;
           showtips(NULL_TIP,"");
        }
    }
    //灯管点亮状态下每隔1s检测一次,UV灯点灯到点灯异常输出时间为1s
    static int timecnt = 0;
    if(update_thread->m_bLighton ==true)
    {
        timecnt++;
        if(timecnt==200)//1s
        {
            timecnt = 0;
            if(false==update_thread->input.Light_ison)
            {
                str_alarm = QObject::tr("灯点亮异常！");
                showtips(ERROR_TIP,str_alarm);
                if(!alarmswitch_bit.Light_ison)
                {
                    alarmswitch_bit.Light_ison = true;
                    update_thread->logwrite(ERROR_TIP,str_alarm);
                    if(g_isBgRunning)
                    {
                        slot_alarmshow(false,QObject::tr("曝光运行中灯点亮异常，请检查灯源!"));
                    }
                }
            }
            else
            {
                if(alarmswitch_bit.Light_ison)
                {
                   alarmswitch_bit.Light_ison = false;
                   showtips(NULL_TIP,"");
                }
            }
        }
    }
    else
    {
        if(alarmswitch_bit.Light_ison)
        {
           alarmswitch_bit.Light_ison = false;
           showtips(NULL_TIP,"");
        }
        timecnt = 0;
    }
//    if((1==update_thread->input.Light_moveok)&&(g_par.is_usescanlight==0))
//    {
//        str_alarm = QObject::tr("灯摆动异常！");
//        showtips(ERROR_TIP,str_alarm);
//        if(!alarmswitch_bit.Light_moveok)
//        {
//            alarmswitch_bit.Light_moveok = true;

//            update_thread->logwrite(ERROR_TIP,str_alarm);
//        }
//    }
//    else
//    {
//        if(alarmswitch_bit.Light_moveok)
//        {
//           alarmswitch_bit.Light_moveok = false;
//           showtips(NULL_TIP,"");
//        }
//    }

    //上灯下灯同一个冷却系统
    if(1==update_thread->input.Cool_vel)
    {
        str_alarm = QObject::tr("灯管冷却液流量异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.Cool_vel)
        {
            alarmswitch_bit.Cool_vel = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
        if(g_language>1)
        {
            ui->lineEdit_cool_vel->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:8px;");
            ui->lineEdit_cool_vel_2->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:8px;");
        }
        else
        {
            ui->lineEdit_cool_vel->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
            ui->lineEdit_cool_vel_2->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
        }
        ui->lineEdit_cool_vel->setText(QObject::tr("冷却异常"));
        ui->lineEdit_cool_vel_2->setText(QObject::tr("冷却异常"));
    }
    else
    {
        if(alarmswitch_bit.Cool_vel)
        {
           alarmswitch_bit.Cool_vel = false;
           showtips(NULL_TIP,"");
        }
        if(g_language>1)
        {
            ui->lineEdit_cool_vel->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:8px;");
            ui->lineEdit_cool_vel_2->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:8px;");
        }
        else
        {
            ui->lineEdit_cool_vel->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
            ui->lineEdit_cool_vel_2->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
        }
        ui->lineEdit_cool_vel->setText(QObject::tr("冷却正常"));
        ui->lineEdit_cool_vel_2->setText(QObject::tr("冷却正常"));
    }
    int is_airok = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
    if((1==update_thread->input.Air_check)&&(1==is_airok))
    {
        if(alarmswitch_bit.Air_check)
        {
           alarmswitch_bit.Air_check = false;
        }
        if(g_language>1)
        {
            ui->lineEdit_upaircheck_val->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:8px;");
        }
        else
        {
            ui->lineEdit_upaircheck_val->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
        }
        ui->lineEdit_upaircheck_val->setText(QObject::tr("到达"));
    }
    else
    {
        if(!alarmswitch_bit.Air_check)
        {
            alarmswitch_bit.Air_check = true;
            //曝光运行过程中泄真空则退出
            if(g_isBgRunning && (g_now_frame==0)&&(1==is_airok))
            {
                slot_alarmshow(false,QObject::tr("曝光过程中上框架真空泄气，退出运行!"));
                bgflow->thread_pause(true);
            }
        }
        if(g_language>1)
        {
            ui->lineEdit_upaircheck_val->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:8px;");
        }
        else
        {
            ui->lineEdit_upaircheck_val->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
        }
        ui->lineEdit_upaircheck_val->setText(QObject::tr("未达"));
    }
//    int is_airok2 = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
//    if((1==update_thread->input.Air_check2)&&(1==is_airok2))
//    {
//        if(alarmswitch_bit.Air_check2)
//        {
//           alarmswitch_bit.Air_check2 = false;
//        }
//        if(g_language>1)
//        {
//            ui->lineEdit_downaircheck_val->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:8px;");
//        }
//        else
//        {
//            ui->lineEdit_downaircheck_val->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
//        }
//        ui->lineEdit_downaircheck_val->setText(QObject::tr("到达"));
//    }
//    else
//    {
//        if(!alarmswitch_bit.Air_check2)
//        {
//            alarmswitch_bit.Air_check2 = true;
//            //曝光运行过程中泄真空则退出
//            if(g_isBgRunning && (g_now_frame==0)&&(1==is_airok2))
//            {
//                slot_alarmshow(false,QObject::tr("曝光过程中下框架真空泄气，退出运行!"));
//                bgflow->thread_pause(false);
//            }
//        }
//        if(g_language>1)
//        {
//            ui->lineEdit_downaircheck_val->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:8px;");
//        }
//        else
//        {
//            ui->lineEdit_downaircheck_val->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
//        }
//        ui->lineEdit_downaircheck_val->setText(QObject::tr("未达"));
//    }
    //底片真空检测
    if(g_par.is_dpvacuum==1&&update_thread->m_dp_zhengkon_out)
    {
        if(((g_par.Frame_select==0)||(g_par.Frame_select==2))&&
                (g_par.ccd_234[0]!=3))
        {
            if(1==update_thread->input.updp_vacuum)
            {
                if(alarmswitch_bit.updp_vacuum)
                {
                   alarmswitch_bit.updp_vacuum = false;
                }
            }
            else
            {
                if(!alarmswitch_bit.updp_vacuum)
                {
                    alarmswitch_bit.updp_vacuum = true;
                    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
                    {
                        bool notop =   NoTopDialog();
                          if(notop) {
                          slot_alarmshow(false,QObject::tr("上框架底片真空泄气，退出运行!"));
                          }
                        bgflow->thread_pause(true);
                    }
                }
            }
        }
        else
        {
            alarmswitch_bit.updp_vacuum = false;
        }
//        if(((g_par.Frame_select==1)||(g_par.Frame_select==2))&&
//                (g_par.ccd_234[1]!=3))
//        {
//            if(1==update_thread->input.downdp_vacuum)
//            {
//                if(alarmswitch_bit.downdp_vacuum)
//                {
//                   alarmswitch_bit.downdp_vacuum = false;
//                }
//            }
//            else
//            {
//                if(!alarmswitch_bit.downdp_vacuum)
//                {
//                    alarmswitch_bit.downdp_vacuum = true;
//                    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
//                    {
//                        slot_alarmshow(false,QObject::tr("下框架底片真空泄气，退出运行!"));
//                        bgflow->thread_pause(false);
//                    }
//                }
//            }
//        }
//        else
//        {
//            alarmswitch_bit.updp_vacuum = false;
//        }
    }
    else
    {
        alarmswitch_bit.updp_vacuum = false;
        alarmswitch_bit.downdp_vacuum = false;
    }
    if(1==update_thread->input.Light_tem)
    {
        str_alarm = QObject::tr("灯温度异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.Light_tem)
        {
            alarmswitch_bit.Light_tem = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
        if(g_language>1)
        {
            ui->lineEdit_light_tem->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:8px;");
            ui->lineEdit_light_tem_2->setStyleSheet("background-color:black;color:rgb(255, 0, 0);font-size:8px;");
        }
        else
        {
            ui->lineEdit_light_tem->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
            ui->lineEdit_light_tem_2->setStyleSheet("background-color:black;color:rgb(255, 0, 0);");
        }
        ui->lineEdit_light_tem->setText(QObject::tr("温度异常"));       
        ui->lineEdit_light_tem_2->setText(QObject::tr("温度异常"));
    }
    else
    {
        if(alarmswitch_bit.Light_tem)
        {
           alarmswitch_bit.Light_tem = false;
           showtips(NULL_TIP,"");
        }
        if(g_language>1)
        {
            ui->lineEdit_light_tem->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:8px;");
            ui->lineEdit_light_tem_2->setStyleSheet("background-color:black;color:rgb(0, 255, 0);font-size:8px;");
        }
        else
        {
            ui->lineEdit_light_tem->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
            ui->lineEdit_light_tem_2->setStyleSheet("background-color:black;color:rgb(0, 255, 0);");
        }
        ui->lineEdit_light_tem->setText(QObject::tr("温度正常"));
        ui->lineEdit_light_tem_2->setText(QObject::tr("温度正常"));
    }
    if(1==update_thread->input.TaiMian_Coolwen)
    {
        str_alarm = QObject::tr("台面温度异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.TaiMian_Coolwen)
        {
            alarmswitch_bit.TaiMian_Coolwen = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(alarmswitch_bit.TaiMian_Coolwen)
        {
           alarmswitch_bit.TaiMian_Coolwen = false;
           showtips(NULL_TIP,"");
        }
    }
    if(1==update_thread->input.Room_tem)
    {
        str_alarm = QObject::tr("曝光室温度异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.Room_tem)
        {
            alarmswitch_bit.Room_tem = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(alarmswitch_bit.Room_tem)
        {
           alarmswitch_bit.Room_tem = false;
           showtips(NULL_TIP,"");
        }
    }
    if(1==update_thread->input.Room_sd)
    {
        str_alarm = QObject::tr("曝光室湿度异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.Room_sd)
        {
            alarmswitch_bit.Room_sd = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(alarmswitch_bit.Room_sd)
        {
           alarmswitch_bit.Room_sd = false;
           showtips(NULL_TIP,"");
        }
    }

    if(1==update_thread->input.Door_alm)
    {
        str_alarm = QObject::tr("门盖关闭异常！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.Door_alm)
        {
            alarmswitch_bit.Door_alm = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(alarmswitch_bit.Door_alm)
        {
           alarmswitch_bit.Door_alm = false;
           showtips(NULL_TIP,"");
        }
    }
//    static int timedelaycnt = 0;
    int bisopen = 1;
    if(1==g_par.ischecksafe)
    {
//        if(g_now_frame==1)
//        {
//             bisopen = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//        }
//        else if(g_now_frame==0)
//        {
//             bisopen = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//        }
        if(1==update_thread->input.Frame_w_safe)
        {
            str_alarm = QObject::tr("安全光闸异常！");
            showtips(ERROR_TIP,str_alarm);
            if(!alarmswitch_bit.Frame_w_safe)
            {
                alarmswitch_bit.Frame_w_safe = true;

//                update_thread->logwrite(WARN_TIP,str_alarm);
            }
            //框架悬空状态检测
            if(bisopen==0)
            {
                //关框时停止关框，开框时停止开框
                if(update_thread->m_bopenframe[0]&&(update_thread->m_bcloseframe[0]==false))
                {
//                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);
                    update_thread->m_bFrameOpenning[0] = true;
                }
                if(update_thread->m_bopenframe[1]&&(update_thread->m_bcloseframe[1]==false))
                {
//                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,false);
                    update_thread->m_bFrameOpenning[1] = true;
                }
                if(update_thread->m_bcloseframe[0]&&(update_thread->m_bopenframe[0]==false))
                {
                   // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);
                    update_thread->m_bFrameClosing[0]=true;
                }
                if(update_thread->m_bcloseframe[1]&&(update_thread->m_bopenframe[1]==false))
                {
                  //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,false);
                    update_thread->m_bFrameClosing[1]=true;
                }
            }
        }
        else
        {
            if(alarmswitch_bit.Frame_w_safe)
            {
               alarmswitch_bit.Frame_w_safe = false;
               showtips(NULL_TIP,"");
            }
    //        timedelaycnt = 0;
            if(bisopen==0)
            {
                static int UpFrameOpenningdelay = 0;
                if((update_thread->m_bFrameOpenning[0] == true)&&(update_thread->m_bFrameClosing[0] == false))
                {
                    UpFrameOpenningdelay++;
                    if(UpFrameOpenningdelay==10)//50ms
                    {
                        UpFrameOpenningdelay = 0;
                        update_thread->m_bFrameOpenning[0] = false;
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);
                    }
                }
                else
                {
                    UpFrameOpenningdelay = 0;
                }
                static int DownFrameOpenningdelay = 0;
                if((update_thread->m_bFrameOpenning[1] == true)&&(update_thread->m_bFrameClosing[1] == false))
                {
                    DownFrameOpenningdelay++;
                    if(DownFrameOpenningdelay==10)//0.1s
                    {
                        DownFrameOpenningdelay = 0;
                        update_thread->m_bFrameOpenning[1] = false;
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,true);
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,false);
                    }
                }
                else
                {
                    DownFrameOpenningdelay = 0;
                }
                static int UpFrameClosingdelay = 0;
                if((update_thread->m_bFrameClosing[0] == true)&&(update_thread->m_bFrameOpenning[0] == false))
                {
                    UpFrameClosingdelay++;
                    if(UpFrameClosingdelay==10)//0.1s
                    {
                        UpFrameClosingdelay = 0;
                        update_thread->m_bFrameClosing[0] = false;
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);
                    }
                }
                else
                {
                    UpFrameClosingdelay = 0;
                }
                static int DownFrameClosingdelay = 0;
                if((update_thread->m_bFrameClosing[1] == true)&&(update_thread->m_bFrameOpenning[1] == false))
                {
                    DownFrameClosingdelay++;
                    if(DownFrameClosingdelay==10)//0.1s
                    {
                        DownFrameClosingdelay = 0;
                        update_thread->m_bFrameClosing[1] = false;
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,true);
//                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,false);
                    }
                }
                else
                {
                    DownFrameClosingdelay = 0;
                }
            }
            //初始状态
//            if(1==update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check"))
//            {
//                update_thread->m_bFrameOpenning[0] = false;
//                update_thread->m_bFrameClosing[0] = false;
//            }
//            if(1==update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2"))
//            {
//                update_thread->m_bFrameOpenning[1] = false;
//                update_thread->m_bFrameClosing[1] = false;
//            }
        }
    }
    else
    {
        update_thread->m_bFrameOpenning[0] = false;
        update_thread->m_bFrameClosing[0] = false;
        update_thread->m_bFrameOpenning[1] = false;
        update_thread->m_bFrameClosing[1] = false;
    }
    //
    if((update_thread->m_berrorframe && g_ishomed &&(!g_isHomeAllThread))||
            ((g_ishomed==false)&&(g_now_frame!=0)&&(g_now_frame!=1)))
    {
        str_alarm = QObject::tr("框架尚未到达停止位置，请复位！");
        showtips(ERROR_TIP,str_alarm);
        if(!m_errorframe)
        {
            m_errorframe = true;

            update_thread->logwrite(ERROR_TIP,str_alarm);
        }
    }
    else
    {
        if(m_errorframe)
        {
           m_errorframe = false;
           showtips(NULL_TIP,"");
        }
    }
    //急停默认是常闭，与其它端口相反
    if(0==update_thread->input.EstopSig)
    {
        str_alarm = QObject::tr("急停！");
        showtips(ERROR_TIP,str_alarm);
        if(!alarmswitch_bit.EstopSig)
        {
            alarmswitch_bit.EstopSig = true;
            update_thread->logwrite(ERROR_TIP,str_alarm);
            g_beeptype = 2;
            emit sig_estop();
        }
    }
    else
    {
        if(alarmswitch_bit.EstopSig)
        {
           alarmswitch_bit.EstopSig = false;
           showtips(NULL_TIP,"");
        }
    }
    //空闲状态，非原点位置时检测CCD碰撞
    if((!g_isSysRunning)&&(!g_isSysContinueRun))
    {
        //CCD<1.3Y>碰撞检测
        if((1==update_thread->input.Ccd13_protect)&&(true!=update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD3Y)))
        {
//            str_alarm = QObject::tr("CCD<1.3Y>碰撞!");
//            showtips(NORMAL_TIP,str_alarm);
            if(!alarmswitch_bit.Ccd13_protect)
            {
                alarmswitch_bit.Ccd13_protect = true;
//                update_thread->logwrite(NORMAL_TIP,str_alarm);
            }
        }
        else
        {
            if(alarmswitch_bit.Ccd13_protect)
            {
               alarmswitch_bit.Ccd13_protect = false;
//               showtips(NULL_TIP,"");
            }
        }
        //CCD<2.4Y>碰撞检测
        if((1==update_thread->input.Ccd24_protect)&&(true!=update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD4Y)))
        {
//            str_alarm = QObject::tr("CCD<2.4Y>碰撞!");
//            showtips(NORMAL_TIP,str_alarm);
            if(!alarmswitch_bit.Ccd24_protect)
            {
                alarmswitch_bit.Ccd24_protect = true;
//                update_thread->logwrite(NORMAL_TIP,str_alarm);
            }
        }
        else
        {
            if(alarmswitch_bit.Ccd24_protect)
            {
               alarmswitch_bit.Ccd24_protect = false;
//               showtips(NULL_TIP,"");
            }
        }
    }
    else
    {
        if(alarmswitch_bit.Ccd13_protect)
        {
           alarmswitch_bit.Ccd13_protect = false;
//           showtips(NULL_TIP,"");
        }
        if(alarmswitch_bit.Ccd24_protect)
        {
           alarmswitch_bit.Ccd24_protect = false;
//           showtips(NULL_TIP,"");
        }
    }
    //Z极限位置检测报警,正在回零时不检测
    if(!g_ishoming)
    {
        if(1==update_thread->input.z_PLimit)
        {
            str_alarm = QObject::tr("Z轴极限位置停止！");
            showtips(ERROR_TIP,str_alarm);
            if(!alarmswitch_bit.z_PLimit)
            {
                //Z轴停止运动
                update_thread->threaddriver->Axis_Enable(AXIS_Z,false);
                alarmswitch_bit.z_PLimit = true;
                update_thread->logwrite(ERROR_TIP,str_alarm);
                if(update_thread->m_bCnc_state==1)
                {
                   slot_alarmshow(false,QObject::tr("Z轴极限位置触发急停,请复位!"));
                   emit sig_estop();
                }
            }
        }
        else
        {
            if(alarmswitch_bit.z_PLimit)
            {
               alarmswitch_bit.z_PLimit = false;
               showtips(NULL_TIP,"");
            }
        }
    }
    return;
}
void MainWindow::alarm_deal()
{

}

//开关框
void MainWindow::frame_mannual()
{
     if(!g_isBgRunning)
     {
        if(update_thread->m_bAxis_state==0)
           return;//轴运动中
     }
     if(g_framechange ||(!update_thread->m_bFrame_stop)) return;
     if(!update_thread->input.EstopSig)
     {
         slot_alarmshow(false,QObject::tr("未释放急停开关!"));
         return;
     }
     if(g_ishoming)//正在回零
     {
         slot_alarmshow(false,QObject::tr("正在回零，不能进行此操作!"));
         return ;
     }
//     if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun)
//     {
//         slot_alarmshow(false,QObject::tr("曝光过程中，不能进行此操作!");
//         return;
//     }
     if(1==update_thread->input.Frame_w_safe)
     {
         slot_alarmshow(false,QObject::tr("检测到安全光闸，不能进行此操作!"));
         return;
     }
     if(g_now_frame<0)
     {
         slot_alarmshow(false,QObject::tr("框架停止位置不准确，不能进行此操作!"));
         return;
     }
     bgflow->thread_openframe();
}
bool MainWindow::bg_run()
{
    qDebug()<<"baoguang running";
    if(!update_thread->input.EstopSig)
    {
        slot_alarmshow(false,QObject::tr("未释放急停开关!"));
        return false;
    }
    if(update_thread->m_bCnc_state==0)
    {
        slot_alarmshow(false,QObject::tr("机器尚未开启，不能进行此操作!"));
        return false;
    }
    //移框时禁止执行
    if(g_framechange) return false;
    //非点灯曝光时判断
    if(!g_isBgRunning)
    {
       if(update_thread->m_bAxis_state==0)
           return false;//轴运动中
    }
    if(g_ishoming||(g_ishomed==0))//正在回零或未回零
    {
        slot_alarmshow(false,QObject::tr("正在回零或未回零，不能进行此操作!"));
        return false;
    }
    if(alarmswitch_bit.Door_alm)
    {
        slot_alarmshow(false,QObject::tr("门盖关闭异常，不能进行此操作!"));
        return false;
    }
    if(1==update_thread->input.Frame_w_safe)
    {
        slot_alarmshow(false,QObject::tr("检测到安全光闸，不能进行此操作!"));
        return false;
    }
    if(alarmswitch_bit.Cool_vel)
    {
        slot_alarmshow(false,QObject::tr("冷却液流量异常，请检查!"));
        return false;
    }
    if(alarmswitch_bit.Light_power)
    {
        slot_alarmshow(false,QObject::tr("灯电源异常!"));
        return false;
    }
    if(alarmswitch_bit.Light_tem)
    {
        slot_alarmshow(false,QObject::tr("灯管温度异常，请检查!"));
        return false;
    }
    if(alarmswitch_bit.Room_tem)
    {
        slot_alarmshow(false,QObject::tr("曝光室温度异常，不能进行此操作!"));
        return false;
    }
    if(alarmswitch_bit.Room_sd)
    {
        slot_alarmshow(false,QObject::tr("曝光室湿度异常，不能进行此操作!"));
        return false;
    }
    if(alarmswitch_bit.Sys_air)
    {
        slot_alarmshow(false,QObject::tr("系统空压异常，不能进行此操作!"));
        return false;
    }
    if(alarmswitch_bit.TaiMian_CoolLiu)
    {
        slot_alarmshow(false,QObject::tr("台面冷却液流量异常，请检查!"));
        return false;
    }    if(alarmswitch_bit.TaiMian_Coolwen)
    {
        slot_alarmshow(false,QObject::tr("台面冷却温度异常，请检查!"));
        return false;
    }
    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.cool_out,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("冷风机未开启，不能进行此操作!"));
        return false;
    }
//    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.tb,PORT_OUT))
//    {
//        slot_alarmshow(false,QObject::tr("台面冰水泵未开启，不能进行此操作!"));
//        return false;
//    }
    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.tb2,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("板面鼓风机未开启，不能进行此操作!"));
        return false;
    }
//    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_lock,PORT_OUT))
//    {
//        slot_alarmshow(false,QObject::tr("台面真空泵未开启，不能进行此操作!"));
//        return false;
//    }
    if(0==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.DownVacumm_switch_out,PORT_OUT))
    {
        slot_alarmshow(false,QObject::tr("CCD伺服转矩未开启，不能进行此操作!"));
        return false;
    }
    //曝光时点灯异常超过三次需解锁才能运行
    if(g_light_errorcnt>=3)
    {
        DlgLogin login(0,QObject::tr("点灯异常超过三次，请输入密码解锁"),this);
        login.exec();
        g_light_errorcnt =0;
        return false;
    }
    //判断单框模式
    int up_frame_select=0,down_frame_select=0;
    if(g_par.Frame_select==0)//上框
    {
        up_frame_select = 1;
        down_frame_select = 0;
    }
//    else if(g_par.Frame_select==1)//下框
//    {
//        up_frame_select = 0;
//        down_frame_select = 1;
//    }
//    else if(g_par.Frame_select==2)//上下框
//    {
//        up_frame_select = 1;
//        down_frame_select = 1;
//    }
    //运行前清除报警
    showtips(NULL_TIP,"");
    if(((g_now_frame==0)&&(up_frame_select==1))||
       ((g_now_frame==1)&&(down_frame_select==1)))
    {
        if(g_par.is_dpvacuum==1)
        {

                if(false==update_thread->input.updp_vacuum)
                {
                    slot_alarmshow(false,QObject::tr("台面底片真空未达，不能进行此操作!"));
                    return false;
                }

//            else if(g_now_frame==1)
//            {
//                if(false==update_thread->input.downdp_vacuum)
//                {
//                    slot_alarmshow(false,QObject::tr("下框底片真空未达，不能进行此操作!"));
//                    return false;
//                }
//            }
        }
        bool ret = false;
        if(g_par.ccd_234[g_now_frame]==3)
        {
            if(g_now_frame==0)
            {
                ret = slot_alarmshow(true,QObject::tr("上框不执行对位继续运行?"));
            }
            else if(g_now_frame==1)
            {
                ret = slot_alarmshow(true,QObject::tr("下框不执行对位继续运行?"));
            }
            g_runtest[g_now_frame] = ret;
        }
        else
        {
            g_runtest[g_now_frame] = false;
        }
    }
    //    if(g_par.downccd_234[g_now_frame]==3)
    //    {
    //        ret =  slot_alarmshow(true,QObject::tr("下平台不执行对位继续运行?"));
    //        if(ret)
    //        {
    //            g_downruntest[g_now_frame] = true;
    //        }
    //        else
    //        {
    //            g_downruntest[g_now_frame] = false;
    //        }
    //    }
    //    else
    //    {
    //        g_downruntest[g_now_frame] = false;
    //    }
    g_autoset = false;
    g_downautoset = false;
    g_justbg = false;
    if((update_thread->ccd_234!=3)&&(update_thread->downccd_234!=3))
    {
        g_justbg = false;
    }
    else
    {
        g_justbg = true;
    }
    Mark_load(0);
    Mark_load(1);
    if(g_isBGwhileStart)
    {
        bgflow->thread_bgcontinuerun();
    }
    else
    {
        bgflow->thread_bgrun();
    }
    return true;
}

void MainWindow::ccdpar_select()
{
    if(ui->radio_ccd1->isChecked())
    {
        ccdselect=0;
        ui->radio_ccd1->setStyleSheet("background:green;");
        ui->radio_ccd2->setStyleSheet("background:darkgray;");
        ui->radio_ccd3->setStyleSheet("background:darkgray;");
        ui->radio_ccd4->setStyleSheet("background:darkgray;");
    }
    else if(ui->radio_ccd2->isChecked())
    {
        ccdselect=1;
        ui->radio_ccd1->setStyleSheet("background:darkgray;");
        ui->radio_ccd2->setStyleSheet("background:green;");
        ui->radio_ccd3->setStyleSheet("background:darkgray;");
        ui->radio_ccd4->setStyleSheet("background:darkgray;");
    }
    else if(ui->radio_ccd3->isChecked())
    {
        ccdselect=2;
        ui->radio_ccd1->setStyleSheet("background:darkgray;");
        ui->radio_ccd2->setStyleSheet("background:darkgray;");
        ui->radio_ccd3->setStyleSheet("background:green;");
        ui->radio_ccd4->setStyleSheet("background:darkgray;");
    }
    else if(ui->radio_ccd4->isChecked())
    {
        ccdselect=3;
        ui->radio_ccd1->setStyleSheet("background:darkgray;");
        ui->radio_ccd2->setStyleSheet("background:darkgray;");
        ui->radio_ccd3->setStyleSheet("background:darkgray;");
        ui->radio_ccd4->setStyleSheet("background:green;");
    }
}

void MainWindow::on_btn_hb_minus_clicked()
{
    ccdpar_select();
    update_thread->hb_minus(ccdselect);
}

void MainWindow::on_btn_hb_plus_clicked()
{
    ccdpar_select();
    update_thread->hb_plus(ccdselect);
}

void MainWindow::on_btn_ld_minus_clicked()
{
    ccdpar_select();
    update_thread->ld_minus(ccdselect);
}

void MainWindow::on_btn_ld_plus_clicked()
{
    ccdpar_select();
    update_thread->ld_plus(ccdselect);
}

//void MainWindow::on_radio_js_clicked()
//{
//    if(ui->radio_js->isChecked())
//    {
//        update_thread->ccd_sd(0);
//        ui->radio_js->setStyleSheet("background:green;");
//        ui->radio_sd->setStyleSheet("background:darkgray;");
//    }
//    else
//    {
//        update_thread->ccd_sd(1);
//        ui->radio_js->setStyleSheet("background:darkgray;");
//        ui->radio_sd->setStyleSheet("background:green;");
//    }
//}

//void MainWindow::on_radio_sd_clicked()
//{
//    if(ui->radio_sd->isChecked())
//    {
//        update_thread->ccd_sd(1);
//        ui->radio_sd->setStyleSheet("background:green;");
//        ui->radio_js->setStyleSheet("background:darkgray;");
//    }
//    else
//    {
//        update_thread->ccd_sd(0);
//        ui->radio_sd->setStyleSheet("background:darkgray;");
//        ui->radio_js->setStyleSheet("background:green;");
//    }
//}
void MainWindow::ccd_step(int ccdnum,int xdir,int ydir)
{
    int axis= -1;
    if(xdir!=0) axis=AXIS_CCD1X+2*ccdnum;
    else if(ydir!=0) axis=AXIS_CCD1X+2*ccdnum+1;
    else if((xdir==0)&&(ydir==0)) axis= -1;
    qDebug()<<"ccd_step:axis="+QString::number(axis,10);
    update_thread->threaddriver->CCDSend_LX(0,true);//连续识别
    float speed = 300.0;//高速是低速的八倍
    if(axis!=-1)
    {
        qDebug("%.3f\n",g_par.ccdy_mindis);
        float ccdspeed,target;
        if(!g_axis_speed_type) ccdspeed = speed/60.0;
        else ccdspeed = 8*speed/60.0;
        float ccd1x = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1X);
        float ccd2x = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2X);
        float ccd3x = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3X);
        float ccd4x = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4X);
        float ccd1y = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y);
        float ccd2y = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y);
        float ccd3y = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y);
        float ccd4y = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y);
        switch(ccdnum+1)
        {
        case 1:
            //ccd1x
            if(xdir>0)
            {
                if((ccd1y-ccd3y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd1x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);
            }
            else if(xdir<0)
            {
                if((ccd1y-ccd3y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd1x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);
            }
            //ccd1y
            if(ydir>0)
            {
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);
            }
            else if(ydir<0)
            {
                if((ccd1y-ccd3y>-435))
                    update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);

            }
            break;
        case 2:
            //ccd2x
            if(xdir>0)
            {
                if((ccd2y-ccd4y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd2x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);
            }
            else if(xdir<0)
            {
                if((ccd2y-ccd4y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd2x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);
            }
            //ccd2y
            if(ydir>0)
            {
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);
            }
            else if(ydir<0)
            {
                if((ccd2y-ccd4y>-435))
                    update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);

            }
            break;
        case 3:
            //ccd3x
            if(xdir>0)
            {
                if((ccd1y-ccd3y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd3x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);
            }
            else if(xdir<0)
            {
                if((ccd1y-ccd3y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd3x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);
            }
            //ccd3y
            if(ydir>0)
            {
                if((ccd1y-ccd3y>-435))
                    update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);

            }
            else if(ydir<0)
            {
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);
            }
            break;
        case 4:
            //ccd4x
            if(xdir>0)
            {
                if((ccd2y-ccd4y<-435))
                {
                    update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd4x);
                }
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);
            }
            else if(xdir<0)
            {
                if((ccd2y-ccd4y<-435))
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,ccd4x);
                else
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);
            }
            //ccd4y
            if(ydir>0)
            {                if((ccd2y-ccd4y>-435))
                    update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,100000);

            }
            else if(ydir<0)
            {
                update_thread->threaddriver->Axis_MovePos(axis,ccdspeed,-100000);
            }
            break;
        }
    }
    else
    {
        for(int aa=AXIS_CCD1X;aa<=AXIS_CCD4Y;aa++)
        {
            update_thread->threaddriver->Axis_Enable(aa,false);
        }
    }
}

//CCD模块反馈数据处理
void MainWindow::slot_ccdrec(const char* buf)
{
    qDebug("buf=%s\n",buf);
#ifdef DEBUG
    qDebug("ccd_receive: %s\n",buf);
#endif
    if(!strncmp(buf,"MV:",3))
    {
        int ccdnum=-1;
        int xmove=0,ymove=0;
        update_thread->threaddriver->CCDRec_move(buf,ccdnum,xmove,ymove);
        if(ccdnum<0||ccdnum>3) return;
        if(!g_ishomed) return;
        switch(ccdnum)
        {
        case 0:
            ui->radio_ccd1->setChecked(TRUE);
            break;
        case 1:
            ui->radio_ccd2->setChecked(TRUE);
            break;
        case 2:
            ui->radio_ccd3->setChecked(TRUE);
            break;
        case 3:
            ui->radio_ccd4->setChecked(TRUE);
            break;
        }
        ccd_step(ccdnum,xmove,ymove);
    }
    else if((!strncmp(buf,"DW:",3))&&g_dw_start)
    {
        CCDok isCCDok;
        update_thread->threaddriver->CCDRec_duiwei(buf,g_dw_answer,update_thread->lmrvalue,isCCDok);
        qDebug("g_dw_answer=%d\n",g_dw_answer);
        if(g_dw_answer==2)
        {
            if((g_dw_num==0)&&(!g_bChange_mark)&&(!update_thread->is_check8))
            {
                qDebug("g_bChange_mark=%d\n",g_bChange_mark);
                int change=0;
                dwccd[0] = isCCDok.ccd1;
                dwccd[1] = isCCDok.ccd2;
                dwccd[2] = isCCDok.ccd3;
                dwccd[3] = isCCDok.ccd4;
                //判断当前是否使用这个标靶，如果标靶被使用且识别不到且这个标靶开启切换功能，则切换标靶
                for(int i=0;i<4;i++)
                {
                    if(dwccd[i]==0&&update_thread->used_ccd[i]&&update_thread->m_bEnableMarkChange[g_now_frame][i])
                    {
                        change=1;
                        //修改使用主次标靶的状态
                        update_thread->m_isusemark[g_now_frame][i]=!update_thread->m_isusemark[g_now_frame][i];
                    }
                }
                qDebug("is_check8=%d\n",update_thread->is_check8);
                if(change==1)
                {
                    if(update_thread->m_isusemark[g_now_frame][0])
                    {
                        update_thread->m_currentmark.x1=g_par.other_markpos[g_now_frame].x1;
                        update_thread->m_currentmark.y1=g_par.other_markpos[g_now_frame].y1;
                    }
                    else
                    {
                        update_thread->m_currentmark.x1=g_par.main_markpos[g_now_frame].x1;
                        update_thread->m_currentmark.y1=g_par.main_markpos[g_now_frame].y1;
                    }
                    if(update_thread->m_isusemark[g_now_frame][1])
                    {
                        update_thread->m_currentmark.x2=g_par.other_markpos[g_now_frame].x2;
                        update_thread->m_currentmark.y2=g_par.other_markpos[g_now_frame].y2;
                    }
                    else
                    {
                        update_thread->m_currentmark.x2=g_par.main_markpos[g_now_frame].x2;
                        update_thread->m_currentmark.y2=g_par.main_markpos[g_now_frame].y2;
                    }
                    if(update_thread->m_isusemark[g_now_frame][2])
                    {
                        update_thread->m_currentmark.x3=g_par.other_markpos[g_now_frame].x3;
                        update_thread->m_currentmark.y3=g_par.other_markpos[g_now_frame].y3;
                    }
                    else
                    {
                        update_thread->m_currentmark.x3=g_par.main_markpos[g_now_frame].x3;
                        update_thread->m_currentmark.y3=g_par.main_markpos[g_now_frame].y3;
                    }
                    if(update_thread->m_isusemark[g_now_frame][3])
                    {
                        update_thread->m_currentmark.x4=g_par.other_markpos[g_now_frame].x4;
                        update_thread->m_currentmark.y4=g_par.other_markpos[g_now_frame].y4;
                    }
                    else
                    {
                        update_thread->m_currentmark.x4=g_par.main_markpos[g_now_frame].x4;
                        update_thread->m_currentmark.y4=g_par.main_markpos[g_now_frame].y4;
                    }
                    qDebug("x1=%.3f y1=%.3f",update_thread->m_currentmark.x1,update_thread->m_currentmark.y1);
                    qDebug("x2=%.3f y2=%.3f",update_thread->m_currentmark.x2,update_thread->m_currentmark.y2);
                    g_dw_answer=3;
                }
            }
        }
//        bgflow->thread_duiwei();
        g_dw_start = false;
    }
//    else if(!strncmp(buf,"AD:",3))
    else if((!strncmp(buf,"DW:",3))&& g_study_start)
    {
//        update_thread->threaddriver->CCDRec_adjust(buf,g_study_answer,update_thread->lmrvalue);
        CCDok ccdisok;
        update_thread->threaddriver->CCDRec_duiwei(buf,g_study_answer,update_thread->lmrvalue,ccdisok);
        bgflow->thread_duiwei();
//        g_study_start = false;
    }
    else if(!strncmp(buf,"DB:",3))
    {
        if(update_thread->m_bAxis_state==0) return;
        if(!g_ishomed) return;
        update_thread->threaddriver->CCDRec_duibiao(buf,update_thread->ccdvalue);
        bgflow->thread_ccdmove(0,1);
    }
    else if(!strncmp(buf,"MP:",3))
    {
        if(update_thread->m_bAxis_state==0) return;
        if(!g_ishomed) return;
        update_thread->threaddriver->CCDRec_ptmove(buf,update_thread->lmrvalue);
        bgflow->thread_ptlmrmove(0);
    }
    else if(!strncmp(buf,"CH:",3))
    {
        int answer;
        CCDok isCCDok;
        int answerout=1;
        int fail_num=0;
//        qDebug()<<"CCDRec_check!";
        update_thread->threaddriver->CCDRec_check(buf,answer,isCCDok);
        if(!g_ch_main) answer = 1;
        if(answer==0) answerout=0;
        //CCD1
        if(isCCDok.ccd1==0&&update_thread->used_ccd[0]&&g_ch_main)
        {
            ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
            ui->lineEdit_ccd1_ok->setText("FAIL");
            answerout=0;
            fail_num++;
        }
        else if(!g_ch_main)//次靶标
        {
            if(!update_thread->check_othermark[0])
            {
                ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd1_ok->setText("    ---    ");
            }
            else if(isCCDok.ccd1==0)
            {
                ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd1_ok->setText("FAIL");
                answerout=0;
            }
            else
            {
                ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd1_ok->setText("PASS");
            }
        }
        else if(update_thread->used_ccd[0]==0)
        {
            ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd1_ok->setText("    ---    ");
        }
        else
        {
            if(answer==1)
            {
                ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd1_ok->setText("PASS");
            }
            else
            {
                ui->lineEdit_ccd1_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd1_ok->setText("FAIL");
            }
        }
        //CCD 2
        if(isCCDok.ccd2==0&&update_thread->used_ccd[1]&&g_ch_main)
        {
            ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
            ui->lineEdit_ccd2_ok->setText("FAIL");
            answerout=0;
            fail_num++;
        }
        else if(!g_ch_main)
        {
            if(!update_thread->check_othermark[1])
            {
                ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd2_ok->setText("    ---    ");
            }
            else if(isCCDok.ccd2==0)
            {
                ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd2_ok->setText("FAIL");
                answerout=0;
            }
            else
            {
                ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd2_ok->setText("PASS");
            }
        }
        else if(update_thread->used_ccd[1]==0)
        {
            ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd2_ok->setText("    ---    ");
        }
        else
        {
            if(answer==1)
            {
                ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd2_ok->setText("PASS");
            }
            else
            {
                ui->lineEdit_ccd2_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd2_ok->setText("FAIL");
            }
        }
        //CCD 3
        if(isCCDok.ccd3==0&&update_thread->used_ccd[2]&&g_ch_main)
        {
            ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
            ui->lineEdit_ccd3_ok->setText("FAIL");
            answerout=0;
            fail_num++;
        }
        else if(!g_ch_main)
        {
            if(!update_thread->check_othermark[2])
            {
                ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd3_ok->setText("    ---    ");
            }
            else if(isCCDok.ccd3==0)
            {
                ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd3_ok->setText("FAIL");
                answerout=0;
            }
            else
            {
                ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd3_ok->setText("PASS");
            }
        }
        else if(update_thread->used_ccd[2]==0)
        {
            ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd3_ok->setText("    ---    ");
        }
        else
        {
            if(answer==1)
            {
                ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd3_ok->setText("PASS");
            }
            else
            {
                ui->lineEdit_ccd3_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd3_ok->setText("FAIL");
            }
        }
        //CCD 3
        if(isCCDok.ccd4==0&&update_thread->used_ccd[3]&&g_ch_main)
        {
            ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
            ui->lineEdit_ccd4_ok->setText("FAIL");
            answerout=0;
            fail_num++;
        }
        else if(!g_ch_main)
        {
            if(!update_thread->check_othermark[3])
            {
                ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd4_ok->setText("    ---    ");
            }
            else if(isCCDok.ccd4==0)
            {
                ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd4_ok->setText("FAIL");
                answerout=0;
            }
            else
            {
                ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd4_ok->setText("PASS");
            }
        }
        else if(update_thread->used_ccd[3]==0)
        {
            ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
            ui->lineEdit_ccd4_ok->setText("    ---    ");
        }
        else
        {
            if(answer==1)
            {
                ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(33,172,61);font:bold");
                ui->lineEdit_ccd4_ok->setText("PASS");
            }
            else
            {
                ui->lineEdit_ccd4_ok->setStyleSheet("background-color:black;color:rgb(200,200,40);font:bold");
                ui->lineEdit_ccd4_ok->setText("FAIL");
            }
        }
        if(update_thread->ccd_234==1&&g_ch_main)
        {
            if(answer==1&&answerout==0&&fail_num<=1) answerout=1;
        }
        g_ch_answer = answerout;
        g_ch_start = false;
        qDebug()<<"CCDRec_check is ok!";
    }
    else if(!strncmp(buf,"TJ:",3) && g_dw_tj)
    {
        update_thread->threaddriver->CCDRec_PreLMRPos(buf,update_thread->tj_lmrvalue[0]);
        g_dw_tj = false;
        qDebug()<<"LMR TJ is ok!";
    }
    else if(!strncmp(buf,"ALM:",4))
    {

    }
}
bool MainWindow::slot_alarmshow()
{
    dlgmove->exec();
    return true;
}
bool MainWindow::slot_alarmshow(bool ismodel,QString errcode)
{
    bool ret = false;
    static bool isshowmodel = false;
    if(isshowmodel)  return ret;
    if(errcode.isEmpty())
    {
        message->hide();
        ret = true;
        m_isAlarmshow = false;
    }
    else
    {
        message->hide();
        message->move((QApplication::desktop()->width() - message->width())/2,
                    (QApplication::desktop()->height() - message->height())/2);//居中显示
        message->MessageShow(ismodel,errcode);
        m_isAlarmshow = true;
        if(ismodel)
        {
            isshowmodel = true;
            message->raise();
            message->activateWindow();
            int res = message->exec();
            if(res==1)
            {
                ret = true;
            }
            else
            {
                ret = false;
            }
            isshowmodel = false;
        }
        else
        {
            message->raise();
            message->activateWindow();
            message->show();
            ret = true;
        }
        update_thread->logwrite(NORMAL_TIP,errcode);
    }
    return ret;
}

void MainWindow::on_checkBox_dwok_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
    {
        ui->checkBox_dwok->setStyleSheet("background:green;");
        g_bDWTest = true;
        g_no_dw[0] = 1;
    }
    else if(arg1==Qt::Unchecked)
    {
        ui->checkBox_dwok->setStyleSheet("background-color: rgb(223, 223, 223);");
        g_bDWTest = false;
        g_no_dw[0] = 0;
    }
}

void MainWindow::on_checkBox_checkok_stateChanged(int arg1)
{
    if(arg1==Qt::Checked)
    {
        ui->checkBox_checkok->setStyleSheet("background:green;");
        g_bCHTest = true;
        g_no_ch[0] = 1;
    }
    else if(arg1==Qt::Unchecked)
    {
        ui->checkBox_checkok->setStyleSheet("background-color: rgb(223, 223, 223);");
        g_bCHTest = false;
        g_no_ch[0] = 0;
    }
}

void MainWindow::slot_EStop()
{
    qDebug()<<"slot_EStop";
    bgflow->estop();//急停后的动作
}

void MainWindow::on_btn_f1_clicked()
{
    ui->stackedWidget_main->setCurrentIndex(0);
    ui->checkBox_checkok->setVisible(false);
    ui->checkBox_dwok->setVisible(false);
}

void MainWindow::on_btn_f2_clicked()
{
    int mainpage_index = ui->stackedWidget_main->currentIndex();
    if(mainpage_index==0)
    {
        DlgLogin debuglogin(-1,QObject::tr("输入密码"),this);
        debuglogin.exec();
        if(debuglogin.iscorrect)
        {
            ui->stackedWidget_main->setCurrentIndex(1);
            ui->checkBox_checkok->setVisible(true);
            ui->checkBox_dwok->setVisible(true);
        }
    }
}

void MainWindow::on_btn_f3_clicked()
{
    DlgRegister dlgreg(this);
    dlgreg.exec();
}

void MainWindow::slot_dpclear(int type,int id)
{
    bool ret = false;
//    bgflow->thread_pause(false);
    //保存参数
    CIniRW dpfile(TOPPARAMETERPATH);
    m_bdpclear_show = true;
    if(type==0)
    {
        if(id==0)
        {
            m_isdpclear[0] = false;
            ret = slot_alarmshow(false,QObject::tr("请进行上框底片清洁!"));
            if(ret)
            {
                g_par.DP_clear_num = 0;
                dpfile.WriteIni("TOP","DP_CLEAR_NUM",QString::number(g_par.DP_clear_num,10));
            }
            m_bdpclear_show = false;
        }
        else if(id==1)
        {
            m_isdpclear[1] = false;
            ret = slot_alarmshow(false,QObject::tr("请进行下框底片清洁!"));
            if(ret)
            {
                g_par.DP2_clear_num = 0;
                dpfile.WriteIni("TOP","DP2_CLEAR_NUM",QString::number(g_par.DP2_clear_num,10));
            }
            m_bdpclear_show = false;
        }
    }
    else if(type==1)
    {
        if(id==0)
        {
            m_isdpexchange[0] = false;
            ret = slot_alarmshow(false,QObject::tr("请更换上框底片!"));
            if(ret)
            {
                g_par.DP_BG_NUM = 0;
                dpfile.WriteIni("TOP","DP_BG_NUM",QString::number(g_par.DP_BG_NUM,10));
            }
            m_bdpclear_show = false;
        }
        else if(id==1)
        {
            m_isdpexchange[1] = false;
            ret = slot_alarmshow(false,QObject::tr("请更换下框底片!"));
            if(ret)
            {
                g_par.DP2_BG_NUM = 0;
                dpfile.WriteIni("TOP","DP2_BG_NUM",QString::number(g_par.DP2_BG_NUM,10));
            }
            m_bdpclear_show = false;
        }
    }
}

//0:下框   1:上框
void MainWindow::slot_setlight(int frameid)
{
    if(0==g_par.islightsetting)
    {
        ui->lineEdit_upoiltype->setText("---");
        ui->lineEdit_downoiltype->setText("---");
    }
    else if(1==g_par.islightsetting)
    {
        QString upink,lowerink;
        bool  lightisok = true;
        int cnt = 0;
        //油墨类型
        if(0==g_par.lightmode)//
        {
            light ligh;
            upink =ligh.returnlightUP();
            lowerink =  ligh.returnlightLOWER();
            if(frameid==0)
            {
                lightisok=  ligh.returnlightTCP(0);
            }
            else if(frameid==1)
            {
                lightisok= ligh.returnlightTCP(1);
            }
            if(frameid==1||frameid==0)
            {
                while((false==lightisok) &&(cnt<2))
                {
                    lightisok = ligh.returnlightTCP(frameid);
                    cnt++;
                }
            }
        }
        else if(1==g_par.lightmode)//串口
        {
            lightRTU lightRtu;
            upink=lightRtu.returnlightUpInkRTU();
            lowerink= lightRtu.returnlightLowerInkRTU();
//            if(frameid==0)
//            {
//                lightisok = lightRtu.returnlightRTU(0);
//            }
//            else if(frameid==1)
//            {
//                lightisok = lightRtu.returnlightRTU(1);
//            }
//            if(frameid==1||frameid==0)
//            {
//                while((false==lightisok) &&(cnt<2))
//                {
//                    lightisok = lightRtu.returnlightRTU(frameid);
//                    cnt++;
//                }
//            }
        }
//        if(!lightisok)
//        {
//            update_thread->logwrite(ERROR_TIP,QObject::tr("灯源通信错误,请检查!"));
//            slot_alarmshow(false,QObject::tr("灯源通信错误,请检查!"));
//        }
        if(g_language>1)
        {
            ui->lineEdit_upoiltype->setStyleSheet("font-size:8px;");
            ui->lineEdit_downoiltype->setStyleSheet("font-size:8px;");
        }
        ui->lineEdit_upoiltype->setText(upink);
        ui->lineEdit_downoiltype->setText(lowerink);
    }
}

//15s检测到信号 自动锁住
void MainWindow::timeUpdatesKey(){
    bool is_bg=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_manual");//框架曝光位置开关感应器
    if(is_bg){
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);//框架曝光位置开关out
    }
    bool is_clear=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_move");//框架曝光位置开关感应器
    if(is_clear){
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);//框架曝光位置开关out
    }
}

//曝光位置开关
void MainWindow::on_btn_npos_clicked()
{

    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);//框架曝光位置开关out
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);//框架清洁位置开关out
//    int bg_kg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_open,PORT_OUT);
//    qDebug()<<"bgwzkg"<<QString::number(bg_kg);
//        if(bg_kg==1){
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);//框架曝光位置开关out

//        }else{
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);//框架曝光位置开关out

//        }

//        int qj_kg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_close,PORT_OUT);
//        qDebug()<<"qjwzkg"<<QString::number(qj_kg);
//            if(qj_kg==1){
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);//框架曝光位置开关out
//            }else{
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);//框架曝光位置开关out

//            }

//update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);//框架曝光位置开关out
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);//框架清洁位置开关out
//    bool is_clear=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_move");//框架清洁位置开关感应器
}
//框架交换
void MainWindow::on_btn_wpos_clicked()
{
   on_btn_frame_change_clicked();

//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);//框架曝光位置开关out
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);//框架清洁位置开关out
//    int qj_kg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_close,PORT_OUT);
//    qDebug()<<"qjwzkg"<<QString::number(qj_kg);
//        if(qj_kg==1){
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);//框架曝光位置开关out
//        }else{
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);//框架曝光位置开关out

//        }

//        int bg_kg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_open,PORT_OUT);
//        qDebug()<<"bgwzkg"<<QString::number(bg_kg);
//            if(bg_kg==1){
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);//框架曝光位置开关out

//            }else{
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);//框架曝光位置开关out

//            }
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);//框架曝光位置开关out
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);//框架清洁位置开关out
//    bool is_bg=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_manual");//框架曝光位置开关感应器
//    bool is_clear=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_move");//框架清洁位置开关感应器
}
//移动设置
void MainWindow::on_btn_move_set_clicked()
{
    dlgparmeter dlg;
    dlg.exec();
}
//开台面冰水泵
void MainWindow::on_btn_taimian_binshuiji_clicked()
{
//    g_beeptype = 1;
//    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return ;
//    int powersate = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
//    int state = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.tb,PORT_OUT);
//    if(state==0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,true);
//    }
//    else if(state==1&&powersate==0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,false);
//    }
}
//开板面鼓风机
void MainWindow::on_btn_taimian_gufenji_clicked()
{
    g_beeptype = 1;
    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return ;
    int powersate = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
    int state = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.tb2,PORT_OUT);
    if(state==0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,true);
    }
    else if(state==1&&powersate==0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,false);
    }
    //    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,true);//开台面冰水泵
    //    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,true);//开台面鼓风机
}
//底片真空
void MainWindow::on_btn_dp_zhengkon_clicked()
{
    int dp_vacuum=0;
    dp_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");//底片真空达到
    if(dp_vacuum==1){
        bool is_clear=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_move");//框架清洁位置感应器
        if(is_clear){
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);//底片破真空
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);//底片吹气
        }
    }else{
        //int dp_vacuum=0;
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,true);//底片真空
        dp_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");//底片真空达到
       int num =0;
        while((dp_vacuum!=1)&&(num<DELAYCOUNT))
        {
            dp_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");
            usleep(USLEEPTIME);//0.01s
            num++;
        }
        if(dp_vacuum!=1)
        {
            update_thread->thread_alarmshow(QObject::tr("底片真空未达到，请检查底片是否摆放正常！"));
        }
    }

}
//曝光室照明
void MainWindow::on_btn_BG_Light_clicked()
{
    int light_state=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_light,PORT_OUT);//曝光室照明电源状态

    if(light_state){
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_light,false);//关 曝光室照明电源

    }else{
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_light,true);//开 曝光室照明电源

    }
}

void MainWindow::on_btn_BG_Light_2_clicked()
{
   slot_alarmshow();
}
//台面真空泵
void MainWindow::on_btn_taimian_ZKBen_clicked()
{
//            lightRTU   lightrtu;
//            lightrtu.exec();
//    g_beeptype = 1;
//    if(g_isSysRunning||g_isBgRunning||g_isSysContinueRun) return ;
//    int powersate = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
//    int state = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_lock,PORT_OUT);
//    if(state==0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,true);
//    }
//    else if(state==1&&powersate==0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,false);
//    }
}
//ccd伺服转矩开启DownVacumm_switch_out
void MainWindow::on_btn_CCD_on_clicked()
{

    int state = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.DownVacumm_switch_out,PORT_OUT);
    if(state==0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownVacumm_switch_out,true);
    }
    else if(state==1)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownVacumm_switch_out,false);
    }
}
//ccd伺服异常清除light_down
void MainWindow::on_btn_CCD_errClear_clicked()
{
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.light_down,true);
}
