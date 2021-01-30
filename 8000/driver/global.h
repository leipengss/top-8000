#ifndef GLOBAL
#define GLOBAL
//配置文件与参数文件路径
#define CONGFILEPATH         "/home/u/cnc/configs/ppmc/ppmc.ini"
#define CONGFILEBACKUPPATH   "/home/u/cnc/configs/para_backup"
#define CNCPARAMETERPATH     "/home/u/cnc/configs/ppmc/para.xml"
#define TOPPARAMETERPATH     "/home/u/cnc/configs/ppmc/8787.ini"
#define MARKPARAMETERPATH     "/home/u/cnc/configs/ppmc/8787ccd.ini"
#define MACHINECONGFILEPATH  "/home/u/cnc/configs/ppmc/machine.ini"

#define LOGFILEPATH          "/home/u/cnc/configs/ppmc/errorinfo"
#define LOGFILEPATHBACK      "/home/u/cnc/configs/ppmc/errorinfo2"
#define MATERIALPATH         "/home/u/cnc/configs/ppmc/Material.db3"
#define LOGPATH              "/usr/share/runf/log.ini"

//axis number
#define AXIS_L   0
#define AXIS_M   1
#define AXIS_R   2
#define AXIS_Z   3
#define AXIS_CCD1X 4
#define AXIS_CCD1Y 5
#define AXIS_CCD2X 6
#define AXIS_CCD2Y 7
#define AXIS_CCD3X 8
#define AXIS_CCD3Y 9
#define AXIS_CCD4X 10
#define AXIS_CCD4Y 11
#define AXIS_W 12
#define AXIS_UPSCAN 13
//状态栏提示信息类型
#define NULL_TIP    0
#define NORMAL_TIP  1
#define WARN_TIP    2
#define ERROR_TIP   3

//变量类型
#define VAR_PIN     1
#define VAR_SIGNAL  0
//端口类型
#define PORT_IN     1
#define PORT_OUT    0
//平台运动方式
#define PT_HOME       0
#define PT_LBPOSITION 1
#define PT_DWPOSITION 2
#define PT_BGPOSITION 3
//端口检测延时时间
#define DELAYCOUNT   300//3s
#define USLEEPTIME   10000
//主线程定时器时间
#define FIRSTTIMER   5 //5ms
//是否重启系统  1: 重启
extern bool g_isRestartSys;
//密码设置类型 10:低级权限只提供查看  11：初级密码  12：中级密码 13：高级密码 14：调试密码 15：开发者密码
extern int g_settings_kind;
//灯状态0:未打开 1:异常(温度/冷却流量) 2：点亮异常 3：打开正常
extern int  g_lightstate;
extern int  g_downlightstate;
//0：正常且无操作  1:操作提示 2：报警
extern int  g_beeptype;
//点灯曝光标志位 false: 停止状态  true:运行中
extern bool g_isBgRunning;
//下平台对位标志位
extern bool g_isDownPTDuiWei;
extern bool g_bDownPTDuiWeiOK;
extern bool g_bDownPTDuiWeiExit;
//所有轴回零线程标志
extern bool g_isHomeAllThread;
//当前框架
extern int g_now_frame;
//对位测试
extern bool g_runtest[2];
extern bool g_downruntest[2];
extern bool g_bDWTest;
extern bool g_bCHTest;
extern bool g_bDebug_run;
extern bool g_ch_start;
extern bool g_downch_start;
extern int  g_ch_answer;
extern int  g_downch_answer;
extern bool g_ch_main;
extern bool g_downch_main;
extern bool g_dw_start;
extern bool g_downdw_start;
extern int g_dw_answer;
extern int g_downdw_answer;
extern bool g_study_start;
extern int  g_study_answer;
extern bool g_downstudy_start;
extern int  g_downstudy_answer;
extern bool g_dw_tj;
extern bool g_downdw_tj;
//CCD学习第N次
extern int g_study_num;
extern int g_downstudy_num;
//对位第N次
extern int g_dw_num;
extern int g_downdw_num;
//单次对位第N次
extern int g_dw_continuenum;
extern bool g_bChange_mark;
extern int g_downdw_continuenum;
extern bool g_bDownChange_mark;
//点灯异常次数
extern int g_light_errorcnt;
extern bool g_uplighton_error;
//对位位置 离板位置 曝光位置
extern float  g_up_dwposition;
extern float  g_up_lbposition;
extern float  g_up_bgposition;

extern float  g_down_dwposition;
extern float  g_down_lbposition;
extern float  g_down_bgposition;
//框架切换标志位true:正在切换 false:未切换或切换完成
extern bool g_framechange;
extern bool g_framemannual;
extern bool g_isCCDMoveThread[2];
//运行标志位:false: 停止状态  true:运行中
extern bool g_isSysRunning;
extern bool g_isSysContinueRun;
//
extern bool g_isPause;
extern bool g_isPauseStart;
extern bool g_isPauseRun;
//
extern bool g_isBGwhileStart;
//
extern bool g_ishoming;
extern bool g_ishomed;
//速度类型
extern bool g_axis_speed_type;
//
extern bool g_autoset;
extern bool g_downautoset;
//
extern bool g_justbg;
//参数修改后重新初始化
extern bool g_reloadpara;
//料号加载更新
extern bool g_loadmaterial;
//
extern bool g_isccdsafe;
//胶条充气 抽气
extern int g_jtairin_delay;
extern int g_jtairout_delay;
//框架开关限时
extern int g_frameopen_delay;
extern int g_frameclose_delay;
//核对延时
extern int g_checkdelay;
extern int g_downcheckdelay;
//屏保启动状态
extern bool g_isScreenStart;
//有无弹窗
extern bool g_isPopUpDialog;
//
extern int g_language;
//
extern int g_no_dw[2];//是否需要对位
extern int g_no_ch[2];//是否需要核对
//靶点数据
typedef struct MARK_POS
{
  float x1;
  float x2;
  float x3;
  float x4;
  float y1;
  float y2;
  float y3;
  float y4;
}Mark_pos;
////LMR position
//typedef struct LMR_DWPOS
//{
//  float value_l;
//  float value_m;
//  float value_r;
//}LMR_POS;

//参数项：全局变量
typedef struct parameter
{
    int quick_door_time;  //快门全功限时
    int full_half;
    int auto_manual;
    int up_ccd_select;
    int down_ccd_select;
    int up_downccd_select;
    int down_downccd_select;
    int dw_up_kxybase;
    int dw_down_kxybase;
    int up_CheckDis_GC1;
    int down_CheckDis_GC2;
    int isBinary;
    int ccd_upld[2][8];//亮度，ccd0～ccd3,ccd序号，0～3
    int ccd_uphb[2][8];
    int ccd_downld[2][8];
    int ccd_downhb[2][8];
    int Frame_close_airdelay; //真空后吹气时长
    int Frame_close_airdelay2;
    int JT_airin_delay;//胶条充气延时ms
    int JT_airout_delay;// 胶条抽气延时ms
    int JT2_airin_delay;
    int JT2_airout_delay;
    int TB_delay;
    int TB2_delay;
    int CCD_check_time;
    int CCD_check_time2;
    int DownCCD_check_time;
    int DownCCD_check_time2;
    int Frame_finish_num;//加工结果，框架完成计数
    int JB_finish_num;
    int Frame2_finish_num;//加工结果，框架完成计数
    int JB2_finish_num;
    int light_num;//点灯次数
    int downlight_num;
    float light_time;//点灯时数 s
    float downlight_time;
    int bg_mode;//曝光模式
    int bg_up_time;//上底片曝光时间
    int bg_up_time2;
    int bg_down_time;//下底片曝光时间 改为底片分几次曝光
    int bg_down_time2;
    int DP_clear_num;//底片清洁
    int DP_maxclear_num;
    int DP_BG_NUM;//底片曝光
    int DP2_clear_num;//底片清洁
    int DP2_maxclear_num;
    int DP2_BG_NUM;//底片曝光
    int DP_BG_MAXNUM;
    int DP2_BG_MAXNUM;
    int Light_type;// 灯类型
    float up_max_value;//最高位置
    float up_bm_height;//板厚
    float up_bm_height_mil;
    float up_dw_height;//对位间隙
    float up_lb_height;//离板高度
    float up_xlength;
    float up_ylength;
    float down_max_value;//最高位置
    float down_bm_height;//板厚
    float down_bm_height_mil;
    float down_dw_height;//对位间隙
    float down_lb_height;//离板高度
    float down_xlength;
    float down_ylength;
    float x_speed;
    float y_speed;
    float z_speed;
    float pt_speed;
    float downpt_speed;
    float z_midspeed;//离板到对位位置的速度
    float z_minspeed;//对位到曝光位置的速度
    float w_speed;//框架移动速度
    float w_movelimit;//框架极限位置
    int bg_openzg;//曝光完是否关闭遮光板
    float move_delay;//对位移动延时
    float bg_movedelay;//曝光完成移出延时
    Mark_pos main_markpos[2];//main_markpos[0]:上框  main_markpos[1]:下框
    Mark_pos other_markpos[2];
    Mark_pos main_downmarkpos[2];//main_downmarkpos[0]:上框  main_downmarkpos[1]:下框
    Mark_pos other_downmarkpos[2];
    int ccd_234[2];//CCD使用方案
    int downccd_234[2];
    int frame_close_delay;
    int frame2_close_delay;
    int frame_open_delay;
    int frame2_open_delay;
    int frame_open_flowdelay;
    int frame2_open_flowdelay;
    int frame_close_delay_a;
    int frame_close_delay_b;
    int frame2_close_delay_a;
    int frame2_close_delay_b;
    int DW_max_num;//最大对位次数
    int DW_max_num2;
    int DownDW_max_num;//最大对位次数
    int DownDW_max_num2;
    int check8[2];//是否8点核对
    int checkdown8[2];
    int ch_othermark[2][4];//上框使用次靶核对
    int ch_downothermark[2][4];
    int Frame_select;
    int bg_up_delaytime;
    int bg_down_delaytime;//改为菲林大小
    float bg_up_energy;
    float bg_up_energyoveride;
    float bg_down_energy;
    float bg_down_energyoveride;
    int isbg_finishairkeep;
    int bg_switchboardtime;
    float dw_up_jingdu;//对位精度
    float dw_down_jingdu;
    float Downdw_up_jingdu;
    float Downdw_down_jingdu;
    float DP_up_error_x;//对位误差
    float DP_down_error_x;
    float DP_up_error_y;//对位误差
    float DP_down_error_y;
    float DP_up_junfen_x;//对位均分
    float DP_up_junfen_y;//
    float DP_down_junfen_x;
    float DP_down_junfen_y;
    float film_up_dia;
    float film_down_dia;
    float board_up_dia;
    float board_down_dia;

    float DownDP_up_error_x;
    float DownDP_down_error_x;
    float DownDP_up_error_y;
    float DownDP_down_error_y;
    float DownDP_up_junfen_x;
    float DownDP_up_junfen_y;
    float DownDP_down_junfen_x;
    float DownDP_down_junfen_y;
    float Downfilm_up_dia;
    float Downfilm_down_dia;
    float Downboard_up_dia;
    float Downboard_down_dia;

    float ccdy_mindis;
    float ccdy_maxdis;
    int screen_waittime;
    float scan_length[2];
    int is_usescanlight;
    float light_speed;
    int is_dpvacuum;//底片真空
    int ischecksafe;
    int isopensuperpar;
    int lightmode;
    int islightsetting;
    int min_bgtime;
    int bg_pos;//改为单前曝光位置 第几次
}PAR;
extern PAR g_par;
#endif // GLOBAL

