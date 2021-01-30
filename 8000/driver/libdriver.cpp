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
#include "libdriver.h"
#include "CncWidget.h"
#include"halcmd.h"
#include"filemanage/cinirw.h"
#include<QDebug>
#include<stdio.h>
#include <QVector>
#include<vector>
#include<QTextCodec>
#include <algorithm>//注意要包含该头文件
using namespace std;

#define INPUT  1
#define OUTPUT 0
#define SIG_NAME 0
//轴停止检测延时
#define STOPDEAL 10000 //0.01s
#define STOPDEALCNT 2000//

static QString filepath = "";
static vector<int> inputnum_list;
static vector<int> inputpol_list;
static QVector<QString> insigname_list;
static vector<int> outputnum_list;
static QVector<QString> outsigname_list;

Libdriver::Libdriver(QString conffile)
{
    workpath = "";
    filepath = conffile;
    if(filepath!="/home/u/cnc/configs/ppmc/ppmc.ini")
    {
        filepath ="/home/u/cnc/configs/ppmc/ppmc.ini";
        qDebug()<<"conffile is error,reload filepath";
    }
    memset(&m_output,-1,sizeof(OUTPORT));
    //加载输入输出端口数据
    LoadPort();
}
/***********************CNC接口实现******************************/
void Libdriver::CNC_init()
{
    halcmdnew_init();
    //系统初始化
    cnc_init(0);
}
//机器开
void Libdriver::CNC_Enable()
{
     cnc_machine(0,1,1);
     m_isWarnStop = false;
}
//机器关
void Libdriver::CNC_Disable()
{
    cnc_machine(0,1,0);
}
//1:机器开  0：机器关
int Libdriver::CNC_State()
{
    int ret =-1;
    ret = cnc_machine(0,0,0);
    return ret;
}
//void Libdriver::CNC_Mode(int mode)
//{
//    mode = MANUAL;
//}

//程序运行状态  0为IDLE，1为运行，2为暂停
int Libdriver::CNC_ProgramState()
{
    int ret =-1;
    ret = cnc_get_program_status(0);
    return ret;
}
//程序运行 linenum:程序运行起始行号
void Libdriver::CNC_ProgramStart(int linenum)
{
    if(linenum<0)
    {
        return;
    }
    if(!workpath.isEmpty())
    {
        cnc_open(0,workpath.toStdString().c_str());
        cnc_run(0,linenum);  //运行加工文件
    }
    else
    {
        qDebug()<<"process file is invalid!";
    }
}

/***********************输入输出端口******************************/
void Libdriver::PortNumCopy(bool isintput,int portnumber,QString signame)
{
    if(isintput)
    {

    }
    else
    {
        if(signame=="ZG_open_out")
        {
            m_output.zg_open = portnumber;
        }
        else if(signame=="ZG_close_out")
        {
            m_output.zg_close = portnumber;
        }
        else if(signame=="Frame_open_out")
        {
            m_output.frame_open = portnumber;
        }
        else if(signame=="Frame_close_out")
        {
            m_output.frame_close = portnumber;
        }
        else if(signame=="Frame2_open_out")
        {
            m_output.frame2_open = portnumber;
        }
        else if(signame=="Frame2_close_out")
        {
            m_output.frame2_close = portnumber;
        }
        else if(signame=="ZJB_out")
        {
            m_output.zjb = portnumber;
        }
        else if(signame=="ZJB2_out")
        {
            m_output.zjb2 = portnumber;
        }
        else if(signame=="Frame_absorb_out")
        {
            m_output.frame_absorb = portnumber;
        }
        else if(signame=="Frame2_absorb_out")
        {
            m_output.frame2_absorb = portnumber;
        }
        else if(signame=="Frame_air_out")
        {
            m_output.frame_air = portnumber;
        }
        else if(signame=="Frame2_air_out")
        {
            m_output.frame2_air = portnumber;
        }
        else if(signame=="JT_airin_out")
        {
            m_output.jt_airin = portnumber;
        }
        else if(signame=="JT_airout_out")
        {
            m_output.jt_airout = portnumber;
        }
        else if(signame=="JT2_airin_out")
        {
            m_output.jt2_airin = portnumber;
        }
        else if(signame=="JT2_airout_out")
        {
            m_output.jt2_airout = portnumber;
        }
        else if(signame=="Uplb_air_out")
        {
            m_output.Uplb_air_out = portnumber;
        }
        else if(signame=="Downlb_air_out")
        {
            m_output.Downlb_air_out = portnumber;
        }
        else if(signame=="UpVacumm_switch_out")
        {
            m_output.UpVacumm_switch_out = portnumber;
        }
        else if(signame=="DownVacumm_switch_out")
        {
            m_output.DownVacumm_switch_out = portnumber;
        }
        else if(signame=="UpDP_Vacumm_out")
        {
            m_output.UpDP_Vacumm_out = portnumber;
        }
        else if(signame=="DownDP_Vacumm_out")
        {
            m_output.DownDP_Vacumm_out = portnumber;
        }
        else if(signame=="UpLight_move_out")
        {
            m_output.uplight_motor = portnumber;
        }
        else if(signame=="Power_on_out")
        {
            m_output.power_on = portnumber;
        }
        else if(signame=="Light_down_out")
        {
            m_output.light_down = portnumber;
        }
        else if(signame=="CCD_down_out")
        {
            m_output.ccd_down = portnumber;
        }
        else if(signame=="Frame_lock_out")
        {
            m_output.frame_lock = portnumber;
        }
        else if(signame=="Frame2_lock_out")
        {
            m_output.frame2_lock = portnumber;
        }
        else if(signame=="TB_out")
        {
            m_output.tb = portnumber;
        }
        else if(signame=="TB2_out")
        {
            m_output.tb2 = portnumber;
        }
        else if(signame=="Cool_out")
        {
            m_output.cool_out = portnumber;
        }
        else if(signame=="Xlight_power_out")
        {
            m_output.upxlight_power = portnumber;
        }
        else if(signame=="Ring_light_out")
        {
            m_output.upring_light = portnumber;
        }
        else if(signame=="Frame_light_out")
        {
            m_output.frame_light = portnumber;
        }
        else if(signame=="Beep_out")
        {
            m_output.beep_out = portnumber;
        }
        else if(signame=="LMR_power_out")
        {
            m_output.lmr_power = portnumber;
        }
        else if(signame=="WZ_power_out")
        {
            m_output.wz_power = portnumber;
        }
        else if(signame=="UCCD_power_out")
        {
            m_output.uccd_power = portnumber;
        }
        else if(signame=="Frame_ct_out")
        {
            m_output.frame_ct_out = portnumber;
        }
        else if(signame=="BM_absorb_out")
        {
            m_output.BM_absorb_out = portnumber;
        }
        else if(signame=="BM2_absorb_out")
        {
            m_output.BM2_absorb_out = portnumber;
        }
        else if(signame=="BM_absorb2_out")
        {
            m_output.BM_absorb2_out = portnumber;
        }
        else if(signame=="BM2_absorb2_out")
        {
            m_output.BM2_absorb2_out = portnumber;
        }
        else if(signame=="BM_absorb3_out")
        {
            m_output.BM_absorb3_out = portnumber;
        }
        else if(signame=="BM2_absorb3_out")
        {
            m_output.BM2_absorb3_out = portnumber;
        }/*else if(signame=="Frame_manual")
        {
            m_output.bgkg = portnumber;
        }else if(signame=="Frame_move")
        {
            m_output.qjkg = portnumber;
        }*/
    }
}

void Libdriver::LoadPort()
{
    inputnum_list.clear();
    inputpol_list.clear();
    insigname_list.clear();
    outputnum_list.clear();
    outsigname_list.clear();
    QSettings *IniRead = new QSettings(filepath, QSettings::IniFormat);
    IniRead->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    foreach (QString group, IniRead->childGroups())
    {
      IniRead->beginGroup(group);
      QString signame;
      int num,pol_value;
      if(group.left(6)=="INPUT_")//input
      {
          foreach (QString key, IniRead->childKeys())
          {
               QTextCodec *codec=QTextCodec::codecForName("UTF-8");
               QString outStr = codec->toUnicode(key.toLatin1());
               if (outStr == "NUM")
               {
                  num = IniRead->value(outStr).toInt();
                  if(num>=0)
                  {
                      signame = IniRead->value("SIGNAL_NAME").toString();
                      insigname_list.push_back(signame);
                      inputnum_list.push_back(num);
                      pol_value = IniRead->value("POL").toInt();
                      inputpol_list.push_back(pol_value);
                      PortNumCopy(true,num,signame);
                  }
               }
          }
      }
      else if(group.left(7)=="OUTPUT_")//output
      {
          foreach (QString key, IniRead->childKeys())
          {
               QTextCodec *codec=QTextCodec::codecForName("UTF-8");
               QString outStr = codec->toUnicode(key.toLatin1());
               if (outStr == "NUM")
               {
                  num = IniRead->value(outStr).toInt();
                  if(num>=0)
                  {
                      signame = IniRead->value("SIGNAL_NAME").toString();
                      outsigname_list.push_back(signame);
                      outputnum_list.push_back(num);
                      PortNumCopy(false,num,signame);
                  }
               }
          }
      }
      IniRead->endGroup();
    }
//    qDebug()<<"outnum="+QString::number(outputnum_list.size(),10);
//    qDebug()<<"innum="+QString::number(inputnum_list.size(),10);
    //读入完成后删除指针
    delete IniRead;
    qDebug()<<"IO is loaded!";
}

int Libdriver::Get_PortVal(int portnum,int type)//type 1:input 0:output
{
    int val = -1;
    if(portnum<0)
    {
      //  qDebug()<<"Get_PortVal:invalid portnum!";
        val = -1;
    }
    else
    {
//        CIniRW file(filepath);
//        QString num = QString::number(portnum,10);
//        QString name = file.ReadIniKeyVal("NUM",num,type,SIG_NAME).toString();//获取对应端口的信号名称
        QString name;
        int cnt=0;
        if(type==1)
        {
            //查询是否有重复定义的端口号
            cnt = count(inputnum_list.begin(),inputnum_list.end(),portnum);
            if(cnt>1)
            {
                qDebug()<<"warn:ppmc duplicate inport!";
                return -1;
            }
            vector<int>::iterator iter=std::find(inputnum_list.begin(),inputnum_list.end(),portnum);//返回的是一个迭代器指针
            int index = std::distance(inputnum_list.begin(), iter);
            name = insigname_list.at(index);
        }
        else if(type==0)
        {
            //查询是否有重复定义的端口号
            cnt = count(outputnum_list.begin(),outputnum_list.end(),portnum);
            if(cnt>1)
            {
                qDebug()<<"warn:ppmc duplicate outport!";
                return -1;
            }
            vector<int>::iterator iter=std::find(outputnum_list.begin(),outputnum_list.end(),portnum);//返回的是一个迭代器指针
            int index = std::distance(outputnum_list.begin(), iter);
            name = outsigname_list.at(index);
        }
        val = halcmdgets_bit(name.toStdString().data());
    }
    return val;
}

void Libdriver::Ctr_Output(int portnum, bool value)
{
    if(portnum<0)
    {
        qDebug()<<"Ctr_Output:invalid outport!";
        return ;
    }
//    CIniRW file(filepath);
//    QString num = QString::number(portnum,10);
//    QString name = file.ReadIniKeyVal("NUM",num,OUTPUT,SIG_NAME).toString();//获取对应端口的信号名称
//    QString val  = QString::number(value,10);
//    qDebug()<<num;
//    qDebug()<<name;
//    QString str;
//    str.sprintf("sudo halcmd sets %s  %d &",name.toStdString().data(),value);
//    system(str.toStdString().data());

    //查询是否有重复定义的端口号
    int cnt = count(outputnum_list.begin(),outputnum_list.end(),portnum);
    if(cnt>1)
    {
        qDebug()<<"warn:ppmc duplicate outport!";
        return ;
    }
    vector<int>::iterator iter=std::find(outputnum_list.begin(),outputnum_list.end(),portnum);//返回的是一个迭代器指针
    int index = std::distance(outputnum_list.begin(), iter);
    QString name = outsigname_list.at(index);
    QString val  = QString::number(value,10);
    halcmdsets(name.toStdString().data(),val.toStdString().data());
}
/***********************轴控制接口 axis:轴编号******************************/
void  Libdriver::Axis_Enable(int axis,bool flg)
{
    char pin_motion[200],enable[200];
    sprintf(pin_motion,"axis-controller.%d.motion-enable",axis);
    sprintf(enable,"%d",flg);
    halcmdsetp(pin_motion,enable);
}

bool  Libdriver::Axis_MovePos(int axis,float speed,float cmdpos)
{
    if(axis<0)
    {
       qDebug()<<"invalid axis";
       return false;
    }
    char pin_speed[200],pin_motion[200],pin_target[200];
    sprintf(pin_speed,"axis-controller.%d.free-vel-lim",axis);
    sprintf(pin_motion,"axis-controller.%d.motion-enable",axis);
    sprintf(pin_target,"axis-controller.%d.free-pos-cmd",axis);
//    qDebug()<<QString(pin_speed)+"="+QString("%1").arg(speed);
//    qDebug()<<QString(pin_target)+"="+QString("%1").arg(cmdpos);
    halcmdsetp(pin_speed,"%f",speed);
    halcmdsetp(pin_motion,"1");
    halcmdsetp(pin_target,"%f",cmdpos);
    usleep(3000);//延时3ms等待底层状态更新
//    QString str;
//    str.sprintf("sudo halcmd setp axis-controller.%d.free-vel-lim %f &",axis,speed);
//    system(str.toStdString().data());
//    str.sprintf("sudo halcmd setp axis-controller.%d.motion-enable %d &",axis,1);
//    system(str.toStdString().data());
    return true;
}
//轴运动状态 返回值 0:moving  1:stop
int   Libdriver::Axis_State(int axis)
{
    int ret = -1;
    char position[200];
    sprintf(position,"axis-controller.%d.in-position",axis);
    ret = halcmdgetp_bit(position);
    return ret;
}

int  Libdriver::Axis_SoftLimit(int axis)
{
    int ret = 0;
    char position[200];
    sprintf(position,"axis-controller.%d.min-pos-limit",axis);
    if(1 == halcmdgetp_bit(position))
    {
        ret = -1;
    }
    sprintf(position,"axis-controller.%d.max-pos-limit",axis);
    if(1 == halcmdgetp_bit(position))
    {
        ret = 1;
    }
    return ret;
}

bool Libdriver::Axis_OrgLimit(int axis)
{
    bool ret = false;
    char position[200];
    sprintf(position,"axis-controller.%d.home-sw-in",axis);
    if(1 == halcmdgetp_bit(position))
    {
        ret = true;
    }
    return ret;
}

bool Libdriver::Axis_Alarm(int axis)
{
    bool ret = false;
    char position[200];
    sprintf(position,"Aux_alarm_%d",axis);
    if(0 == halcmdgets_bit(position))
    {
        qDebug("Axis_Alarm:Aux_alm_%d",axis);
        ret = true;
    }
    return ret;
}

//50秒内未回到原点表明回零错误
int Libdriver::Axis_WaitForHome(int axis)
{
    int w =Axis_homed(axis);
    int num =0;
    int delaycnt=0;
    if(axis==12)//框架轴行程大，回零等待时间长
    {
        delaycnt = 5*STOPDEALCNT;
    }
    else
    {
        delaycnt = STOPDEALCNT;
    }
    int estopSig = VarBit_Get(0,"EstopSig");
    int Ccd_isok = VarBit_Get(0,"Ccd_isok");
    bool ccdalarm = false;
    if((Ccd_isok==0))
    {
        ccdalarm = true;
    }
    while((w!=1)&&(num<delaycnt)&&(estopSig!=0)&&(!ccdalarm)&&(!m_isWarnStop))
    {
        w =Axis_homed(axis);
        estopSig = VarBit_Get(0,"EstopSig");//0:急停
        if((Ccd_isok==0))
        {
            ccdalarm = true;
        }
        usleep((int)STOPDEAL);//0.01s
        num++;
    }
    if(estopSig==0)
    {
        qDebug()<<QObject::tr("急停退出回零!");
        return -1;
    }
    if(ccdalarm)
    {
        qDebug()<<QObject::tr("驱动器异常，退出回零!");
        Axis_Enable(axis,false);
        return -2;
    }
    if(m_isWarnStop)
    {
        qDebug()<<QObject::tr("报警触发急停，退出回零!");
        return -3;
    }
    if(w!=1)
    {
        qDebug()<<QObject::tr("回零超时！");
        Axis_Enable(axis,false);
        return 0;
    }
    return 1;
}

int Libdriver::Axis_WaitForStop(int axis)
{
    int flg =Axis_State(axis);
    int num =0;
    int estopSig = VarBit_Get(0,"EstopSig");
    while((flg!=1)&&(num<STOPDEALCNT)&&(estopSig!=0)&&(!m_isWarnStop))
    {
        flg =Axis_State(axis);
        estopSig = VarBit_Get(0,"EstopSig");//0:急停
        usleep((int)STOPDEAL);//0.01s
        num++;
    }
    if(estopSig==0)
    {
        qDebug()<<QObject::tr("急停轴停止!");
        return -1;
    }
    if(m_isWarnStop)
    {
        qDebug()<<QObject::tr("报警触发急停，轴停止!");
        return -2;
    }
    if(flg!=1)
    {
        qDebug()<<QObject::tr("轴停止超时！");
        return 0;
    }
//    qDebug()<<QString::number(axis,10)+QObject::tr("轴停止!")+QString::number(flg,10);
    return 1;
}
//轴运动实际位置
float Libdriver::Axis_CurrentPos(int axis)
{
    float ret = 2^128;
    char buf[200];
    sprintf(buf,"axis-controller.%d.joint-pos-cmd",axis);
    ret = halcmdgetp_float(buf);
    return ret;
}
//轴运动指令位置
float Libdriver::Axis_CmdPos(int axis)
{
    float ret = 2^128;
    char buf[200];
    sprintf(buf,"axis-controller.%d.free-pos-cmd",axis);
    ret = halcmdgetp_float(buf);
    return ret;
}

//轴运动实际速度  addr：轴所在总线板卡的站号 axis：轴号
float Libdriver::Axis_CurrentVel(int addr,int axis)
{
    float ret = 2^128;
    char buf[200];
    sprintf(buf,"pcie.%d.encoder.%d.velocity",addr,axis);
    ret = halcmdgetp_float(buf);
    return ret;
}
//轴反馈位置，addr：轴所在总线板卡的站号 axis：轴号
float Libdriver::Axis_FeedbackPos(int addr,int axis)
{
    float ret = 2^128;
    char buf[200];
    sprintf(buf,"pcie.%d.encoder.%d.position",addr,axis);
    ret = halcmdgetp_float(buf);
    return ret;
}
//获取跟随误差
float Libdriver::Axis_FollowError(int axis)
{
    float ret = 2^128;
    char buf[200];
    sprintf(buf,"axis-controller.%d.f-error",axis);
    ret = halcmdgetp_float(buf);
    return ret;
}
//回零状态 0未回零 1：已经回零 2：正在回零
int Libdriver::Axis_homed(int axis)
{
    int ret = 0;
    QString str,str1;
    str.sprintf("axis-controller.%d.homed",axis);
    str1.sprintf("axis-controller.%d.homing",axis);
    if(1==VarBit_Get(1,str))
    {
        ret = 1;
    }
    else if(1==VarBit_Get(1,str1))
    {
        ret = 2;
    }
    return ret;
}
//轴回零
void Libdriver::Axis_home(int axis)
{
    QString str;
    str.sprintf("axis-controller.%d.home-start",axis);
    qDebug()<<str;
    Var_Set(1,str,"0");
    usleep(5*1000);
    str.sprintf("axis-controller.%d.home-start",axis);
    qDebug()<<str;
    Var_Set(1,str,"1");
}

/***********************系统底层变量：信号和管脚  type 0:signal 1:pin*****************/
YMDtime Libdriver::HardwareTime()
{
    YMDtime time;
    time.year = 2000+VarUInt_Get(1,"pcie.year");
    time.month = VarUInt_Get(1,"pcie.month");
    time.day = VarUInt_Get(1,"pcie.date");
    time.hour = VarUInt_Get(1,"pcie.hour");
    time.minute = VarUInt_Get(1,"pcie.minute");
    time.second = VarUInt_Get(1,"pcie.second");
    return time;
}

int Libdriver::VarUInt_Get(int type,QString varname)
{
    int ret = 2147483647;//int max value
    if(type==1)
    {
        ret = halcmdgetp_u32(varname.toStdString().data());
    }
    else if(type==0)
    {
        ret = halcmdgets_u32(varname.toStdString().data());
    }
    return ret;
}

int Libdriver::VarSInt_Get(int type,QString varname)
{
    int ret = 2147483647;
    if(type==1)
    {
        ret = halcmdgetp_s32(varname.toStdString().data());
    }
    else if(type==0)
    {
        ret = halcmdgets_s32(varname.toStdString().data());
    }
    return ret;
}

int Libdriver::VarBit_Get(int type,QString varname)
{
    int ret = -1;
    if(type==1)
    {
        ret = halcmdgetp_bit(varname.toStdString().data());
    }
    else if(type==0)
    {
        ret = halcmdgets_bit(varname.toStdString().data());
    }
    return ret;
}

float Libdriver::VarFloat_Get(int type,QString varname)
{
    float ret = 2^128;
    if(type==1)
    {
        ret = halcmdgetp_float(varname.toStdString().data());
    }
    else if(type==0)
    {
        ret = halcmdgets_float(varname.toStdString().data());
    }
    return ret;
}

int Libdriver::Var_Set(int type,QString varname,QString varvalue)
{
    float ret = -1;
    if(type==1)
    {
        ret = halcmdsetp(varname.toStdString().data(),varvalue.toStdString().data());
    }
    else if(type==0)
    {
        ret = halcmdsets(varname.toStdString().data(),varvalue.toStdString().data());
    }
    return ret;
}
//退出HAL，禁止底层操作
void Libdriver::CNC_HardDisable()
{
    exithal();
    exit(1);
}
//检测硬件通信
bool Libdriver::CNC_HardCheck(int type)
{
    bool ret=false;
    char state[200];
    switch(type)
    {
    case 0:
    break;
    case 1:
        sprintf(state,"etc.%d.all-op",0);
        if(0 == halcmdgetp_bit(state))//通信错误
        {
            ret = true;
        }
    break;
    case 2:
    break;
    }
    return ret;
}

/***********************CCD接口实现,channel=0表示CCD1-4，channel=1表示CCD5-8******************************/
void Libdriver::CCD_init(QWidget*pWidget)
{
    // up  ccd show
    markwidget = new MarkWidget2(1,NULL,pWidget);
//    markwidget->resize(pWidget->size());
    QObject::connect(markwidget,SIGNAL(project_new(const char*)),this,SLOT(CCDRec_cmd(const char*)));
}

void Libdriver::CCDRec_cmd(const char* buf)
{
    emit ccd_receive(buf);
    qDebug()<<"receive cmd";
}

//对标
void Libdriver::CCDSend_duibiao(int ch,CCD_POS ccd_pos)
{
    char buf[400];
    sprintf(buf,"DB:x1=%.3f,y1=%.3f,x2=%.3f,y2=%.3f,x3=%.3f,y3=%.3f,x4=%.3f,y4=%.3f", \
            ccd_pos.x1,ccd_pos.y1,\
            ccd_pos.x2,ccd_pos.y2,\
            ccd_pos.x3,ccd_pos.y3,\
            ccd_pos.x4,ccd_pos.y4);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//对位
void Libdriver::CCDSend_duiwei(int ch,int cycle,int movenum,CCD_POS ccd_pos,CCDLMR_POS lmrpos)
{
    char buf[400];
    sprintf(buf,"DW:T=%d,N=%d,x1=%.3f,y1=%.3f,x2=%.3f,y2=%.3f,x3=%.3f,y3=%.3f,x4=%.3f,y4=%.3f,L=%.3f,M=%.3f,R=%.3f", \
            cycle,movenum,\
            ccd_pos.x1,ccd_pos.y1,\
            ccd_pos.x2,ccd_pos.y2,\
            ccd_pos.x3,ccd_pos.y3,\
            ccd_pos.x4,ccd_pos.y4,\
            lmrpos.value_l,lmrpos.value_m,lmrpos.value_r);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//真空后核对,核对次数cnt
void Libdriver::CCDSend_check(int ch,int cnt)
{
    char buf[256];
    sprintf(buf,"CH:n=%d",cnt);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//设置亮度和对比度 type=0:亮度 type=1:对比度 ccd_num:CCD序号
void Libdriver::CCDSend_basicpar(int ch,int type,int ccd_num,int val)
{
    char buf[256];
    if(0==type)
    {
        sprintf(buf,"BR:CCD=%d,VAL=%d",ccd_num,val);
    }
    else if(1==type)
    {
        sprintf(buf,"CT:CCD=%d,VAL=%d",ccd_num,val);
    }
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//ccd 平台移动 移动增量 逆时针为正
void Libdriver::CCDSend_ptmove(int ch,float val_x,float val_y,float val_a)
{
    char buf[256];
    sprintf(buf,"MP:X=%.3f,Y=%.3f,A=%.3f",val_x,val_y,val_a);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//ccd 校正
void Libdriver::CCDSend_adjust(int ch,int num,CCDLMR_POS lmrpos)
{
    char buf[256];
    sprintf(buf,"AD:n=%d,L=%.3f,M=%.3f,R=%.3f",num,\
            lmrpos.value_l,lmrpos.value_m,lmrpos.value_r);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//true:连续识别 false:锁定
void Libdriver::CCDSend_LX(int channel,bool flg)
{
    if(flg==true)
    {
        if(channel==0)
        {
            markwidget->load_project("LX:V=1");
        }
        else if(channel==1)
        {
            return;
        }
    }
    else if(flg==false)
    {
        if(channel==0)
        {
            markwidget->load_project("LX:V=0");
        }
        else if(channel==1)
        {
            return;
        }
    }
}
//通知对位开始，解除图像锁定
void Libdriver::CCDSend_DWSart(int ch)
{
    if(ch==0)
    {
        markwidget->load_project("ST");
    }
    else if(ch==1)
    {
        return;
    }
}
//重复对位时获取平台预估位置
void Libdriver::CCDSend_GetLMRPos(int ch)
{
    if(ch==0)
    {
        markwidget->load_project("TJ");
    }
    else if(ch==1)
    {
        return;
    }
}
//靶标直径 filmdia:菲林直径 pcbdia:靶标直径
void Libdriver::CCDSend_markdia(int ch,float filmdia,float pcbdia)
{
    char buf[256];
    sprintf(buf,"DD:D1=%.3f,D2=%.3f",filmdia,pcbdia);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//设置公差 film:菲林  center:中心均分
void Libdriver::CCDSend_settolerance(int ch,TWO_DIM film,TWO_DIM center)
{
    char buf[256];
    sprintf(buf,"GC:fx=%.3f,fy=%.3f,cx=%.3f,cy=%.3f",\
            film.x,film.y,center.x,center.y);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//使用CCD
void Libdriver::CCDSend_use(int ch,int ccduse[4])
{
    char buf[256];
    sprintf(buf,"US:CCD0=%d,CCD1=%d,CCD2=%d,CCD3=%d",\
            ccduse[0],ccduse[1],ccduse[2],ccduse[3]);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//清除补偿预估
void Libdriver::CCDSend_clear(int ch)
{
    if(ch==0)
    {
        markwidget->load_project("CL");
    }
    else if(ch==1)
    {
        return;
    }
}
//当前框架
void Libdriver::CCDSend_frame(int ch,int frame)
{
    char buf[256];
    sprintf(buf,"FR:index=%d",frame);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//设置对位停止条件
void Libdriver::CCDSend_dwstoppar(int ch,TWO_DIM dwstop)
{
    char buf[256];
    sprintf(buf,"DS:x=%.3f,y=%.3f",dwstop.x,dwstop.y);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//设置距离核对
void Libdriver::CCDSend_checkdis(int ch,float tol)
{
    char buf[256];
    sprintf(buf,"CR:val=0,tol=%.3f",tol);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//LMR正负限位
void Libdriver::CCDSend_LMRLimit(int ch,CCDLMR_POS lmrP,CCDLMR_POS lmrN)
{
    char buf[256];
    sprintf(buf,"LM:L=%.3f,%.3f,M=%.3f,%.3f,R=%.3f,%.3f",\
            lmrN.value_l,lmrP.value_l,lmrN.value_m,lmrP.value_m,lmrN.value_r,lmrP.value_r);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
//LMR与CCD坐标
void Libdriver::CCDSend_LMRCCDPos(int ch,CCD_POS ccd,CCDLMR_POS lmr)
{
    char buf[256];
    sprintf(buf,"AX:x1=%lf,y1=%lf,x2=%lf,y2=%lf,x3=%lf,y3=%lf,x4=%lf,y4=%lf,L=%lf,M=%lf,R=%lf",\
            ccd.x1,ccd.y1,ccd.x2,ccd.y2,ccd.x3,ccd.y3,ccd.x4,ccd.y4,lmr.value_l,lmr.value_m,lmr.value_r);
    if(ch==0)
    {
        markwidget->load_project(buf);
    }
    else if(ch==1)
    {
        return;
    }
}
/****
  CCD数据反馈
****/
void Libdriver::CCDRec_duibiao(const char *buf,CCD_POS &ccd_pos)
{
     sscanf(buf,"DB:x1=%f,y1=%f,x2=%f,y2=%f,x3=%f,y3=%f,x4=%f,y4=%f",\
       &ccd_pos.x1,&ccd_pos.y1,&ccd_pos.x2,&ccd_pos.y2,\
            &ccd_pos.x3,&ccd_pos.y3,&ccd_pos.x4,&ccd_pos.y4);
}

void Libdriver::CCDRec_duiwei(const char *buf,int &result,CCDLMR_POS &lmrpos,CCDok &isCCDok)
{
     sscanf(buf,"DW:RES=%d,L=%f,M=%f,R=%f,CCD1=%d,CCD2=%d,CCD3=%d,CCD4=%d",\
            &result,&lmrpos.value_l,&lmrpos.value_m,&lmrpos.value_r,\
            &isCCDok.ccd1,&isCCDok.ccd2,&isCCDok.ccd3,&isCCDok.ccd4);
}

void Libdriver::CCDRec_check(const char *buf,int &result,CCDok &isCCDok)
{
    qDebug()<<"Libdriver:CCDRec_check!";
    sscanf(buf,"CH:RES=%d,CCD1=%d,CCD2=%d,CCD3=%d,CCD4=%d",\
            &result,&isCCDok.ccd1,&isCCDok.ccd2,&isCCDok.ccd3,&isCCDok.ccd4);
}

void Libdriver::CCDRec_ptmove(const char *buf,CCDLMR_POS &lmrpos)
{
     sscanf(buf,"MP:L=%f,M=%f,R=%f",&lmrpos.value_l,&lmrpos.value_m,&lmrpos.value_r);
}

void Libdriver::CCDRec_PreLMRPos(const char *buf,CCDLMR_POS &lmrpos)
{
     sscanf(buf,"TJ:L=%f,M=%f,R=%f",&lmrpos.value_l,&lmrpos.value_m,&lmrpos.value_r);
}

void Libdriver::CCDRec_adjust(const char *buf,int &result,CCDLMR_POS &lmrpos)
{
     sscanf(buf,"AD:RES=%d,L=%f,M=%f,R=%f",&result,&lmrpos.value_l,&lmrpos.value_m,&lmrpos.value_r);
}

//CCD点动
void Libdriver::CCDRec_move(const char *buf,int &ccd_num,int &x_move,int &y_move)
{
     sscanf(buf,"MV:CCD=%d,X=%d,Y=%d",&ccd_num,&x_move,&y_move);
}
