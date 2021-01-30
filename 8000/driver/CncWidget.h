#ifndef CNCWIDGET_H
#define CNCWIDGET_H

#define MAX_CHANNEL 2
//定时获取硬件时间
//#define NEWKEY 1

#include "iniwrite.hh"
#include "parwidget.h"
#include "iowidget.h"
#include "valuewidget.h"
#include "inputdialogwidget.h"
#include "homewidget.h"
#include "buttonwidget.h"
#include "radiobuttonwidget.h"
//#include "openglbox.h"
#include "topkeydialog.h"
#include "toppardialog.h"
#include "toppardialog2.h"
#include "topwritekeydialog.h"
#include "changekeydialog.h"
#include "userpardialog.h"
#include "vardialog.h"
#include "halcmd.h"

#define IDLE 0
#define RUNNING 1
#define PAUSED 2

#define OOFF 0
#define FORWARD 1
#define REVERSE 2
#define INCREASE 3
#define DECREASE 4

#define MANUAL 0
#define AUTO 1
#define MDI 2
#define UNKNOWN 3

#define A_KEY 0x41
#define X_KEY 0x58
#define Y_KEY 0x59
#define Z_KEY 0x5a
#define U_KEY 0x55
#define V_KEY 0x56
#define W_KEY 0x57
#define ESC_KEY 0x01000000
#define F11_KEY 0x0100003a

extern int flag_homed[MAX_CHANNEL][9];   //各个轴的回零状态
extern char operator_display_status[MAX_CHANNEL][256];//cnc_operator_display标志

extern int psystem(const char *format, ...);
extern void cnc_raise_sig(const char *format, ...);

extern int cnc_init(int channel);   //初始化系统
extern int cnc_machine(int channel,int flag, int state);    //获取及设置机器开关
extern int cnc_io_input(int channel,int n);     //获取IO输入值
extern int cnc_io_output(int channel,int n);    //获取IO输出值
extern double cnc_abs_cmd_pos(int channel,int axis);
extern double cnc_abs_act_pos(int channel,int axis);
extern double cnc_rel_cmd_pos(int channel,int axis);
extern double cnc_rel_act_pos(int channel,int axis);
extern double cnc_abs_cmd_endpos(int channel,int axis);
extern double cnc_rel_cmd_endpos(int channel,int axis);
extern int cnc_get_program_status(int channel); //获取运行的状态，0为IDLE，1为运行，2为暂停
extern int cnc_mode(int channel,int flag, int mode);    //获取系统的模式，0为手动，1为自动，2为MDI
extern int cnc_current_vel(int channel);    //获取当前速度
extern int cnc_program_velocity(int channel);   //获取
extern int cnc_joint_homed(int channel,int joint);  //获取轴回零的结果
extern int cnc_joint_limit(int channel,int axis); //获取轴的限位情况，0表示在软限位内，1表示超出最小硬限位，2表示超出最小软限位，3表示超出最大软限位，4表示超出最大硬限位
extern int cnc_program_line(int channel);   //获取程序行号
extern int cnc_shell_input_bit(int channel,int num);    //获取num管脚的值
extern double cnc_set_jog_speed(int channel, int mode, double speed);   //获取及设置手动模式轴运动的默认速度
extern double cnc_set_jog_middlespeed(int channel, int mode, double speed);   //获取及设置手动模式轴运动的中等速度
extern double cnc_set_jog_highspeed(int channel, int mode, double speed);   //获取及设置手动模式轴运动的高速速度
extern int cnc_set_jog_speed_choice(int channel, int mode, int choice); //获取及设置手动模式轴运动的高低速选择
extern double cnc_set_jog_incr(int channel, int mode, double incr); //获取及设置手动模式轴增量运动的增量值
extern int cnc_set_jog_type(int channel, int mode, int type);  //获取及设置手动模式轴运动的模式，0为连续，1为增量，2为手轮
extern int cnc_feed_override(int channel,int flag, int percent);  //获取及设置进给倍率
extern int cnc_home(int channel,int axis);          //回零函数
extern int cnc_joint_homing(int channel,int axis);  //判断是否正在回零
extern int cnc_unhome(int channel,int axis);    //取消回零状态
extern int cnc_abort(int channel);          //退出运行状态
extern double cnc_get_parameters(int channel, int num); //获取#参数
extern int cnc_run(int channel, int line);  //运行加工文件
extern int cnc_open(int channel, const char *filename); //打开加工文件
extern int cnc_task_plan_init(int channel); //初始化解析器
extern int cnc_mdi(int channel,const char *text);   //发送MDI命令
extern int cnc_error(int channel);  //获取错误信息，会退出当前运行状态
extern int cnc_operator_text(int channel);  //获取开发者信息，会退出当前运行状态
extern int cnc_operator_display(int channel);   //获取开发者信息，只显示
extern int cnc_jog(int channel, int axis, int direction);   //手动模式轴运动
extern int cnc_jog_stop(int channel, int axis);    //手动模式轴停止
extern int cnc_estop(int channel, int flag, int state); //急停开关打开
extern float cnc_shell_input_float(int channel, int n); //float pin
//extern int cnc_opengl_x(int channel, int value);    //设置Opengl控件的X坐标
//extern int cnc_opengl_y(int channel, int value);    //设置Opengl控件的Y坐标
//extern int cnc_opengl_width(int channel,int value); //设置Opengl控件的宽度
//extern int cnc_opengl_height(int channel,int value);//设置Opengl控件的高度
//extern int cnc_opengl_showmodal(int channel,int value); //设置Opengl控件的模型的显示
//extern int cnc_opengl_newmodal(int channel,int value);  //设置Opengl控件的模型的生成
//extern int cnc_opengl_clear(int channel,int value); //设置Opengl控件的复位
//extern int cnc_opengl_view(int channel,int value);  //设置Opengl控件的视角
//extern int cnc_opengl_enable(int channel,int value);//设置Opengl控件的可视情况
//extern int cnc_opengl_force_disable(int channel,int value);//设置Opengl控件的可视情况
extern int cnc_shell_input_u32(int channel, int n);//获取u32管脚的值
extern int cnc_shell_input_bit(int channel, int n);//获取bit管脚的值
extern float cnc_shell_input_float(int channel, int n);//获取float管脚的值
extern int cnc_tool(int channel);   //获取当前主轴,sp_tool
extern double cnc_get_tooloffset(int channel);  //获取当前刀具补偿
extern int cnc_get_currenttool(int channel);    //获取当前刀具
extern int cnc_spindle_override(int channel, int flag, int percent);    //主轴倍率
extern double cnc_set_spindle_speed(int channel, int flag, double speed);   //主轴指定转速
extern double cnc_spindle_speed(int channel);   //实际的主轴转速
extern double cnc_pos_offset(int channel, int axis);    //工件偏移值
extern double cnc_float_var(int channel, int flag, int num, double value);  //命名参数
extern int cnc_jog_enable(int channel, int flag, int onoff);    //手轮开关
extern int cnc_mdi_running(int channel, int flag, int value);  //mdirun标志位
extern int cnc_shortcut_channel(int flag, int channel);//快捷键的通道
extern int cnc_bit_out(int channel, int flag, int num, int state);//bit类型输出控制
extern int cnc_spindle(int channel, int flag, int state);//主轴控制

class CncWidget:public QObject
{
    Q_OBJECT
public:
    CncWidget(int channel_num);
    ~CncWidget();

    int channel;    //通道号
    QTimer *timer_update;

    /*路径*/
    QString ppmcini;
    QString machineini;
    QString clientini;
    QString parameterini;
    QString ppmcorigenini;
    QString ppmclastini;
    QString iolastini;
    QString ioorigenini;
    QString coordinateini;
    QString halvarini;
    //通道数
    int total_channel;
    //机床参数
    int axis_num;
    //当前程序
    QString filepath;
    //程序当前行
    int program_line;
    QStringList program_line_list;
    int total_line;
    //工件计数
    int piece;
    int var3501;
    //手动操作
    double custom_incr_value;  //自定义增量值
    int jogspeedchoice; //进给速度选项，0表示默认速度（低速），1表示高速
    //消息栏
    QStringList message_bar_io1,message_bar_io2;
    //安全屏蔽
    int flag_safe;
    //日志栏
    int type_information,type_warning,type_error;
    //快捷键

    int real_year,real_month,real_day;
    int end_year,end_month,end_day;
    int left_hour;

    QTimer *keytimer;

    char temp[400];
    char test_s[400];//保存从文本中获取的内容
    char final[400];
    char digit[400];//%s的内容
    char back_en2ch[400];

    char* en2ch(const char *source);//读取zh_CN.ini的翻译值
    char* test(const char *str,char *ret,char *parameter);

    //检查安全屏蔽
    int is_safe();
    int is_homing();
    int is_homed();

signals:
    void information(int channel_num,const QString &str);
    void warnning(int channel_num,const QString &str);
    void error(int channel_num,const QString &str);
    void custom_line_changed(int num);
    void feed_slider_changed(int value);
    void spindle_slider_changed(int value);
    void iochanged();
    void tcp_message(const char *text);
public slots:
    void update();
    void clearworkpiece();
    void jogmove(int axis,int direction);
    void jogstop(int axis);
    //top
    void click_top_btn_par();
    void click_top_btn_fix();
    void topendtime();  //更新截至时间
    void click_topkey();
    void changekeydialog();
};

extern CncWidget *cncwidget[MAX_CHANNEL];

#endif // CNCWIDGET_H
