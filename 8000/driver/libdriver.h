#ifndef LIBDRIVER_H
#define LIBDRIVER_H
#include<QString>
#include <QWidget>

//IO端口号
typedef struct
{
  int zg_open;
  int zg_close;
  int frame_open;
  int frame_close;
  int frame2_open;
  int frame2_close;
  int zjb;
  int zjb2;
  int frame_absorb;//框架真空
  int frame2_absorb;
  int frame_air;
  int frame2_air;
  int jt_airin;
  int jt_airout;
  int jt2_airin;//底片真空
  int jt2_airout;
  int Uplb_air_out;
  int Downlb_air_out;
  int UpVacumm_switch_out;
  int DownVacumm_switch_out;//ccd伺服转矩开启
  int UpDP_Vacumm_out;
  int DownDP_Vacumm_out;
  int uplight_motor;
  int power_on;
  int light_down;//ccd伺服异常清除
  int ccd_down;
  int frame_lock;
  int frame2_lock;
  int tb;
  int tb2;
  int cool_out;
  int upxlight_power;
  int upring_light;
  int frame_light;
  int beep_out;
  int lmr_power;
  int wz_power;
  int uccd_power;
  int frame_ct_out;//Z.轴伺服电源
  int BM_absorb_out;//版面真空
  int BM2_absorb_out;
  int BM_absorb2_out;
  int BM2_absorb2_out;
  int BM_absorb3_out;
  int BM2_absorb3_out;
//  int bgkg;//框架曝光位置开关
//  int qjkg;//qj曝光位置开关
//  int TaiMian_Coolwen;
}OUTPORT;

typedef struct
{

}INPUT;

typedef struct
{
  int ccd1;
  int ccd2;
  int ccd3;
  int ccd4;
}CCDok;

typedef struct
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
}YMDtime;

typedef struct
{
  float x1;
  float x2;
  float x3;
  float x4;
  float y1;
  float y2;
  float y3;
  float y4;
}CCD_POS;
//LMR position
typedef struct
{
  float value_l;
  float value_m;
  float value_r;
}CCDLMR_POS;
//二维坐标
typedef struct
{
  float x;
  float y;
}TWO_DIM;
class MarkWidget;

class MarkWidget2: public QWidget
{
    Q_OBJECT
public:
    MarkWidget2(int argc,  char **argv, QWidget* parent=0);
    const char* get_version();
    const char* get_current_project_directory()const;
    QWidget* get_vice_view(int CCDIndex=0);
    int load_project(const char*directory);
    ~MarkWidget2();

signals:
    void project_new(const char* directory);

private slots:
    void emit_project_new(const char* directory);

private:
    MarkWidget* widget;
};

class Libdriver: public QObject
{
    Q_OBJECT
public:
    Libdriver(QString conffile);
    /**************************CNC接口定义*********************************/
    QString workpath;//the path and filename of process file
    //CNC初始化
    void CNC_init();
    void CNC_Enable();
    int  CNC_State();
    void CNC_Disable();
    void CNC_HardDisable();
    bool CNC_HardCheck(int type);//0：光纤 1：ethercat 2：光纤与ethercat
    void CNC_Mode(int mode);
    //CNC程序控制
    void CNC_ProgramStart(int linenum);
    int CNC_ProgramState();
    /**************************IO端口输入输出接口定义***********************/
    OUTPORT m_output;
    INPUT m_input;
    /*portnum:IO编号  -1->invalid
    * type: 1->input 0->output
    */
    int  Get_PortVal(int portnum,int type);
    void  Ctr_Output(int portnum,bool value);
    /**************************轴控制接口定义*********************************/
    bool  m_isWarnStop;//报警停止标志位
    void  Axis_Enable(int axis,bool flg);
    bool  Axis_MovePos(int axis,float speed,float cmdpos);
    //当前轴运动状态
    int   Axis_State(int axis);//0:stop  1:moving  2:error
    //当前轴软限位
    int   Axis_SoftLimit(int axis);//0:no  1:positive -1:negtive
    //
    bool  Axis_OrgLimit(int axis);
    //获取伺服轴的报警状态，1:报警  0：正常
    bool  Axis_Alarm(int axis);
    //延时等待轴回零完成，关闭使能即可退出while
    int   Axis_WaitForHome(int axis);
    //延时等待轴停止，关闭使能即可退出while
    int   Axis_WaitForStop(int axis);
    //轴运动实际位置
    float Axis_CurrentPos(int axis);
    //轴运动指令位置
    float Axis_CmdPos(int axis);
    //轴运动实际速度 addr：轴所在总线板卡的站号 axis：轴号
    float Axis_CurrentVel(int addr,int axis);
    //对应轴编码器反馈位置 addr：轴所在总线板卡的站号 axis：轴号
    float Axis_FeedbackPos(int addr,int axis);
    //获取跟随误差
    float Axis_FollowError(int axis);
    //回零状态
    int Axis_homed(int axis);
    //回零
    void Axis_home(int axis);

    /**************************底层系统变量接口定义  type 0:signal 1:pin******/
    YMDtime HardwareTime();
    int VarUInt_Get(int type,QString varname);
    int VarSInt_Get(int type,QString varname);
    int VarBit_Get(int type,QString varname);
    float VarFloat_Get(int type,QString varname);
    int Var_Set(int type,QString varname,QString varvalue);
    /**************************CCD接口定义*********************************/
    //CCD初始化
    void CCD_init(QWidget*pWidget);
    //对标
    void CCDSend_duibiao(int ch,CCD_POS ccd_pos);
    //对位
    void CCDSend_duiwei(int ch,int cycle,int movenum,CCD_POS ccd_pos,CCDLMR_POS lmrpos);
    //核对
    void CCDSend_check(int ch,int cnt);
    //设置亮度和对比度 type=0:亮度 type=1:对比度 ccd_num:CCD序号
    void CCDSend_basicpar(int ch,int type,int ccd_num,int val);
    //ccd 平台移动 移动增量 逆时针为正
    void CCDSend_ptmove(int ch,float val_x,float val_y,float val_a);
    //ccd 校正
    void CCDSend_adjust(int ch,int num,CCDLMR_POS lmrpos);
    //连续识别
    void CCDSend_LX(int ch,bool flg);
    //通知对位开始，解除图像锁定
    void CCDSend_DWSart(int ch);
    //重复对位时获取平台预估位置
    void CCDSend_GetLMRPos(int ch);
    //靶标直径
    void CCDSend_markdia(int ch,float filmdia,float pcbdia);
    //设置公差
    void CCDSend_settolerance(int ch,TWO_DIM film,TWO_DIM center);
    //使用CCD
    void CCDSend_use(int ch,int ccduse[4]);
    //清除补偿预估
    void CCDSend_clear(int ch);
    //当前框架
    void CCDSend_frame(int ch,int frame);
    //设置对位停止条件
    void CCDSend_dwstoppar(int ch,TWO_DIM dwstop);
    //设置距离核对
    void CCDSend_checkdis(int ch,float tol);
    //LMR正负限位
    void CCDSend_LMRLimit(int ch,CCDLMR_POS lmrP,CCDLMR_POS lmrN);
    //LMR与CCD坐标
    void CCDSend_LMRCCDPos(int ch,CCD_POS ccd,CCDLMR_POS lmr);
    /****
      CCD数据反馈
    ****/
    void CCDRec_duibiao(const char *buf,CCD_POS &ccd_pos);
    void CCDRec_duiwei(const char *buf,int &result,CCDLMR_POS &lmrpos,CCDok &isCCDok);
    void CCDRec_check(const char *buf,int &result,CCDok &isCCDok);
    void CCDRec_ptmove(const char *buf,CCDLMR_POS &lmrpos);
    void CCDRec_adjust(const char *buf,int &result,CCDLMR_POS &lmrpos);
    //CCD点动
    void CCDRec_move(const char *buf,int &ccd_num,int &x_move,int &y_move);
    //平台预估位置
    void CCDRec_PreLMRPos(const char *buf,CCDLMR_POS &lmrpos);
private:
    MarkWidget2 *markwidget;
    void LoadPort();
    void PortNumCopy(bool isintput,int portnumber,QString signame);
signals:
    void ccd_receive(const char *buf);//视觉模块接口信号
private slots:
    void CCDRec_cmd(const char* buf);
};

#endif // LIBDRIVER_H
