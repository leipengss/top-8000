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
#include "driver/cupdatedatathread.h"
#include"filemanage/cinirw.h"
#include<QDebug>
#include<QDateTime>
#include<QFileInfo>
#include<QTextCodec>
//static LibModbus *g_LibModbus = new LibModbus;
//全局类
CUpdateDataThread *update_thread;

CUpdateDataThread::CUpdateDataThread()
{
    threaddriver = new Libdriver(CONGFILEPATH);
    m_last_frame = -1;
    m_bCnc_state = false;
    m_bHard_state = false;
    m_bStopped= false;
    memset(&keyboard,0,sizeof(key_bit));
    memset(&mb_key,0,sizeof(key_bit));
    memset(&input,0,sizeof(alrmcheck_bit));
    //
    memset(&m_currentmark,0,sizeof(Mark_pos));
    memset(&m_downcurrentmark,0,sizeof(Mark_pos));
    memset(&m_basicpos,0,sizeof(Mark_pos));
    //ccd
    sd = 0;
    zmove_type = -1;
    ccdmove_type[0] = -1;
    ccdmove_type[1] = -1;
    for(int num=0;num<2;num++)
    {
        memset(m_bEnableMarkChange[num],0,4*sizeof(bool));
        memset(m_isusemark[num],0,4*sizeof(bool));
        memset(m_bdownEnableMarkChange[num],0,4*sizeof(bool));
        memset(m_isdownusemark[num],0,4*sizeof(bool));
        memset(&m_uppcbok[num],0,sizeof(CCDLMR_POS));
        memset(&m_downpcbok[num],0,sizeof(CCDLMR_POS));
        memset(&tj_lmrvalue[num],0,sizeof(CCDLMR_POS));
    }
    //
    plan_cnt = 0;
    comportnum =2;//串口ttys1
    planboard_addr = 0;
    //打开modbus
    if(false==ModbusOpen(0))
    {
       qDebug()<<"ModbusOpen is error!";
    }
  //  connect(g_LibModbus,SIGNAL(sig_ModbusData(uint8_t *, int,bool)),this,SLOT(slot_recdata(uint8_t *, int,bool)));

    //
    updatetimer =new QTimer(this);
    //
    is_sendstart = false;
    is_sendcheck = false;
    is_sendstudy = false;
    is_ccdsuoding = false;
    is_tjlmr = false;
    m_record_light_time=0;
    m_record_downframelight_time=0;
    //
    m_bAxis_state = true;
    m_bCCDY_stop = true;
    m_bFrame_stop = true;
    m_bScanAxis_stop = true;
    //
    m_bFrameOpenning[0] = false;
    m_bFrameClosing[0] = false;
    m_bFrameOpenning[1] = false;
    m_bFrameClosing[1] = false;
    //
    axisname_list.clear();
    m_framedingban = false;
    m_framezjb = false;
    m_berrorframe = true;
    m_framemovepause = false;
    m_isframemoving = false;
}
CUpdateDataThread::~CUpdateDataThread()
{
    stop();
    this->terminate();
    this->wait();
  //  delete g_LibModbus;
    delete threaddriver;
    delete updatetimer;
}
void CUpdateDataThread::stop()
{
    m_bStopped = true;
}

void CUpdateDataThread::beep()
{
    if(g_beeptype==1)//操作提示
    {
        threaddriver->Ctr_Output(threaddriver->m_output.beep_out,1);
        usleep(2000);
        threaddriver->Ctr_Output(threaddriver->m_output.beep_out,0);
        g_beeptype = 0;
    }
    else if(g_beeptype==2)//报警
    {
        int num = 20;
        while(num>1)
        {
            num--;
            threaddriver->Ctr_Output(threaddriver->m_output.beep_out,1);
            usleep(2000);
            threaddriver->Ctr_Output(threaddriver->m_output.beep_out,0);
            usleep(50000);
        }
        g_beeptype = 0;
    }
}
void CUpdateDataThread::framenum()
{
     g_now_frame = 0;
//    static int state =-1;
//    if(g_ishoming) return;//回零过程中不更新框架编号
//    if((threaddriver->Axis_State(AXIS_W)==0) ||m_isframemoving) return;//框架运动过程中或移框不更新框架编号
//    int up_stop=threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//    int down_stop=threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//    if((down_stop==1)&&(up_stop==0)) state=0;
//    else if((up_stop==1)&&(down_stop==0)) state=1;
//    else state = -1;
//    if(state!=g_now_frame)
//    {
//        g_now_frame = 0;
//    }
}
//面板按键触发
void CUpdateDataThread::keytrigger()
{
    keyboard.key1=threaddriver->VarBit_Get(VAR_SIGNAL,"ProgramRun");
//    keyboard.key2=threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_manual");
//    keyboard.key3=threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_move");
    if(keyboard.key1!=mb_key.key1)
    {
        qDebug()<<"我进来 了";

        if(keyboard.key1)
        {
            g_beeptype = 1;
            emit mbkey(1);
            qDebug()<<"mb_key:run=true";
        }
        mb_key.key1 =keyboard.key1;
    }

//    if(keyboard.key2!=mb_key.key2)
//    {
//        if(keyboard.key2)
//        {
//            g_beeptype = 1;
//            emit mbkey(2);
//            qDebug()<<"mb_key:frame_mannual=true";
//        }
//        mb_key.key2 =keyboard.key2;
//    }

//    if(keyboard.key3!=mb_key.key3)
//    {
//        if(keyboard.key3)
//        {
//            g_beeptype = 1;
//            emit mbkey(3);
//            qDebug()<<"mb_key:frame_change=true";
//        }
//        mb_key.key3 =keyboard.key3;
//    }
}
void CUpdateDataThread::par_update(int frame)
{
    qDebug()<<"par_update...";
    if(frame<0)
    {
       qDebug()<<"par_update:invalid frame!";
       return;
    }
    frame=0;
    //CCD
    threaddriver->CCDSend_frame(0,frame);
    threaddriver->CCDSend_frame(1,frame);
    ccd_sd(sd);
    TWO_DIM xy_ds[2],xy_dperror[2],xy_jf[2];
    float film_dia[2]={0.0,0.0},pcb_dia[2]={0.0,0.0},gc=0.0;
    if(frame==0)
    {
        //上影像
        xy_ds[0].x = g_par.dw_up_jingdu/1000.0;
        xy_ds[0].y = g_par.dw_up_jingdu/1000.0;
        xy_dperror[0].x = g_par.DP_up_error_x/1000.0;
        xy_dperror[0].y = g_par.DP_up_error_y/1000.0;
        xy_jf[0].x = g_par.DP_up_junfen_x/1000.0;
        xy_jf[0].y = g_par.DP_up_junfen_y/1000.0;
        film_dia[0] = g_par.film_up_dia/1000.0;
        pcb_dia[0] = g_par.board_up_dia/1000.0;
        //下影像
        xy_ds[1].x = g_par.Downdw_up_jingdu/1000.0;
        xy_ds[1].y = g_par.Downdw_up_jingdu/1000.0;
        xy_dperror[1].x = g_par.DownDP_up_error_x/1000.0;
        xy_dperror[1].y = g_par.DownDP_up_error_y/1000.0;
        xy_jf[1].x = g_par.DownDP_up_junfen_x/1000.0;
        xy_jf[1].y = g_par.DownDP_up_junfen_y/1000.0;
        film_dia[1] = g_par.Downfilm_up_dia/1000.0;
        pcb_dia[1] = g_par.Downboard_up_dia/1000.0;

        gc = g_par.up_CheckDis_GC1;
        ccd_234 = g_par.ccd_234[0];
        ccd_switch = g_par.up_ccd_select;
        downccd_234 = g_par.downccd_234[0];
        downccd_switch = g_par.up_downccd_select;
        check_othermark[0] = g_par.ch_othermark[0][0];
        check_othermark[1] = g_par.ch_othermark[0][1];
        check_othermark[2] = g_par.ch_othermark[0][2];
        check_othermark[3] = g_par.ch_othermark[0][3];
        check_downothermark[0] = g_par.ch_downothermark[0][0];
        check_downothermark[1] = g_par.ch_downothermark[0][1];
        check_downothermark[2] = g_par.ch_downothermark[0][2];
        check_downothermark[3] = g_par.ch_downothermark[0][3];
        is_checkdown8 = g_par.checkdown8[0];
        is_check8 = g_par.check8[0];
        //胶条充气 抽气时间
        g_jtairin_delay = g_par.JT_airin_delay;
        g_jtairout_delay = g_par.JT_airout_delay;
        //
        g_checkdelay = g_par.CCD_check_time;
        g_downcheckdelay = g_par.DownCCD_check_time;
        //
        g_frameopen_delay =g_par.frame_open_delay ;
        g_frameclose_delay =g_par.frame_close_delay ;
    }
    else if(frame==1)
    {
        //上影像
        xy_ds[0].x = g_par.dw_down_jingdu/1000.0;
        xy_ds[0].y = g_par.dw_down_jingdu/1000.0;
        xy_dperror[0].x = g_par.DP_down_error_x/1000.0;
        xy_dperror[0].y = g_par.DP_down_error_y/1000.0;
        xy_jf[0].x = g_par.DP_down_junfen_x/1000.0;
        xy_jf[0].y = g_par.DP_down_junfen_y/1000.0;
        film_dia[0] = g_par.film_down_dia/1000.0;
        pcb_dia[0] = g_par.board_down_dia/1000.0;
        //下影像
        xy_ds[1].x = g_par.Downdw_down_jingdu/1000.0;
        xy_ds[1].y = g_par.Downdw_down_jingdu/1000.0;
        xy_dperror[1].x = g_par.DownDP_down_error_x/1000.0;
        xy_dperror[1].y = g_par.DownDP_down_error_y/1000.0;
        xy_jf[1].x = g_par.DownDP_down_junfen_x/1000.0;
        xy_jf[1].y = g_par.DownDP_down_junfen_y/1000.0;
        film_dia[1] = g_par.Downfilm_down_dia/1000.0;
        pcb_dia[1] = g_par.Downboard_down_dia/1000.0;

        gc = g_par.down_CheckDis_GC2;

        ccd_234 = g_par.ccd_234[1];
        ccd_switch = g_par.down_ccd_select;
        downccd_234 = g_par.downccd_234[1];
        downccd_switch = g_par.down_downccd_select;
        check_othermark[0] = g_par.ch_othermark[1][0];
        check_othermark[1] = g_par.ch_othermark[1][1];
        check_othermark[2] = g_par.ch_othermark[1][2];
        check_othermark[3] = g_par.ch_othermark[1][3];
        is_check8 = g_par.check8[1];
        check_downothermark[0] = g_par.ch_downothermark[1][0];
        check_downothermark[1] = g_par.ch_downothermark[1][1];
        check_downothermark[2] = g_par.ch_downothermark[1][2];
        check_downothermark[3] = g_par.ch_downothermark[1][3];
        is_checkdown8 = g_par.checkdown8[1];
        //胶条充气 抽气时间
        g_jtairin_delay = g_par.JT2_airin_delay;
        g_jtairout_delay = g_par.JT2_airout_delay;
        g_checkdelay = g_par.CCD_check_time2;
        g_downcheckdelay = g_par.DownCCD_check_time2;
        //
        g_frameopen_delay =g_par.frame2_open_delay ;
        g_frameclose_delay =g_par.frame2_close_delay ;
    }
    for(int channel=0;channel<1;channel++)
    {
        threaddriver->CCDSend_dwstoppar(channel,xy_ds[channel]);
        threaddriver->CCDSend_settolerance(channel,xy_dperror[channel],xy_jf[channel]);
        threaddriver->CCDSend_markdia(channel,film_dia[channel],pcb_dia[channel]);
        threaddriver->CCDSend_checkdis(channel,gc);
    }
    //上方CCD使用
    if(ccd_234==0)
    {
      if(ccd_switch==0)
      {
          used_ccd[0]=1;
          used_ccd[1]=1;
          used_ccd[2]=0;
          used_ccd[3]=0;
      }
      else if(ccd_switch==1)
      {
          used_ccd[0]=0;
          used_ccd[1]=0;
          used_ccd[2]=1;
          used_ccd[3]=1;
      }
      else if(ccd_switch==2)
      {
          used_ccd[0]=1;
          used_ccd[1]=0;
          used_ccd[2]=0;
          used_ccd[3]=1;
      }
      else
      {
          used_ccd[0]=0;
          used_ccd[1]=1;
          used_ccd[2]=1;
          used_ccd[3]=0;
      }
    }
    else if(ccd_234==2)
    {
      used_ccd[0]=1;
      used_ccd[1]=1;
      used_ccd[2]=1;
      used_ccd[3]=1;
    }
    else
    {
      used_ccd[0]=1;
      used_ccd[1]=1;
      used_ccd[2]=1;
      used_ccd[3]=1;
    }
    if(ccd_234!=1)
    {
        threaddriver->CCDSend_use(0,used_ccd);
    }
    else
    {
        int used[4]={1,1,1,0};
        threaddriver->CCDSend_use(0,used);
    }
    //下方CCD使用
    if(downccd_234==0)
    {
      if(downccd_switch==0)
      {
          used_downccd[0]=1;
          used_downccd[1]=1;
          used_downccd[2]=0;
          used_downccd[3]=0;
      }
      else if(downccd_switch==1)
      {
          used_downccd[0]=0;
          used_downccd[1]=0;
          used_downccd[2]=1;
          used_downccd[3]=1;
      }
      else if(downccd_switch==2)
      {
          used_downccd[0]=1;
          used_downccd[1]=0;
          used_downccd[2]=0;
          used_downccd[3]=1;
      }
      else
      {
          used_downccd[0]=0;
          used_downccd[1]=1;
          used_downccd[2]=1;
          used_downccd[3]=0;
      }
    }
    else if(downccd_234==2)
    {
      used_downccd[0]=1;
      used_downccd[1]=1;
      used_downccd[2]=1;
      used_downccd[3]=1;
    }
    else
    {
      used_downccd[0]=1;
      used_downccd[1]=1;
      used_downccd[2]=1;
      used_downccd[3]=1;
    }
    if(downccd_234!=1)
    {
        threaddriver->CCDSend_use(1,used_downccd);
    }
    else
    {
        int downused[4]={1,1,1,0};
        threaddriver->CCDSend_use(1,downused);
    }
    //global parameter
    g_up_lbposition = g_par.up_lb_height;
    g_up_dwposition = g_par.up_max_value - g_par.up_bm_height/1000.000 - g_par.up_dw_height/1000.000;
    g_up_bgposition = g_par.up_max_value - g_par.up_bm_height/1000.000;

    g_down_lbposition = g_par.down_lb_height;
    g_down_dwposition = g_par.down_max_value - g_par.down_bm_height/1000.000 - g_par.down_dw_height/1000.000;
    g_down_bgposition = g_par.down_max_value - g_par.down_bm_height/1000.000;
}
void CUpdateDataThread::check_home()
{
    bool is_homing=false;
    bool is_homed=true;
    for(int i=0;i<=AXIS_W;i++)
    {
        if(g_par.is_usescanlight==0)
        {
            if((i!=AXIS_UPSCAN))
            {
                if(threaddriver->Axis_homed(i)==2)
                {
                    is_homing=true;
                    break;
                }
            }
        }
        else if(g_par.is_usescanlight==1)
        {
            if(threaddriver->Axis_homed(i)==2)
            {
                is_homing=true;
                break;
            }
        }
    }
    for(int i=0;i<=AXIS_W;i++)
    {
        if(g_par.is_usescanlight==0)
        {
            if((i!=AXIS_UPSCAN))
            {
                if(threaddriver->Axis_homed(i)!=1)
                {
                    is_homed=false;
                    break;
                }
            }
        }
        else if(g_par.is_usescanlight==1)
        {
            if(threaddriver->Axis_homed(i)!=1)
            {
                is_homed=false;
                break;
            }
        }
    }
    //正在回零表示还未回零,已经回零表示回零空闲
    if(is_homing) is_homed=false;
    if(is_homed) is_homing=false;
    g_ishoming=is_homing;
    g_ishomed=is_homed;
}
void CUpdateDataThread::axis_pos()
{
    for(int num=0;num<=AXIS_UPSCAN;num++)
    {
        m_axispos[num]=threaddriver->Axis_CurrentPos(num);
        m_axiscmdpos[num]= threaddriver->Axis_CmdPos(num);
        m_axisfeedbackpos[num]= 0;//threaddriver->Axis_FeedbackPos(num);
        m_axisfollowerror[num]=0;//threaddriver->Axis_FollowError(num);
    }

}
void CUpdateDataThread::check_alarminput()
{
   input.Sys_air = threaddriver->VarBit_Get(VAR_SIGNAL,"Sys_air");
   input.Light_power = threaddriver->VarBit_Get(VAR_SIGNAL,"Light_power");
   input.Light_ison = threaddriver->VarBit_Get(VAR_SIGNAL,"Light_ison");
  // input.Light_moveok = threaddriver->VarBit_Get(VAR_SIGNAL,"Light_moveok");
   input.Cool_vel = threaddriver->VarBit_Get(VAR_SIGNAL,"Cool_vel");//冷风机
   input.Air_check = threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
  // input.Air_check2 = threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");
   input.Light_tem = threaddriver->VarBit_Get(VAR_SIGNAL,"Light_tem");
   input.Room_tem = threaddriver->VarBit_Get(VAR_SIGNAL,"Room_tem");
   input.Room_sd = threaddriver->VarBit_Get(VAR_SIGNAL,"Room_sd");
   input.Door_alm = threaddriver->VarBit_Get(VAR_SIGNAL,"Door_alm");
   input.Frame_w_safe = threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");
   input.EstopSig = threaddriver->VarBit_Get(VAR_SIGNAL,"EstopSig");
   input.Ccd13_protect = threaddriver->VarBit_Get(VAR_SIGNAL,"Ccd13_protect");
   input.Ccd24_protect = threaddriver->VarBit_Get(VAR_SIGNAL,"Ccd24_protect");
   input.Ccd_isok = threaddriver->VarBit_Get(VAR_SIGNAL,"Ccd_isok");
   input.z_PLimit = threaddriver->VarBit_Get(VAR_SIGNAL,"ULZmaxlim");
   input.updp_vacuum = threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");
  // input.downdp_vacuum = threaddriver->VarBit_Get(VAR_SIGNAL,"DownDP_vacuum_check");
   input.TaiMian_CoolLiu = threaddriver->VarBit_Get(VAR_SIGNAL,"Left_ccd_locate");//台面冷却流量
   input.TaiMian_Coolwen = threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");//台面冷却温度
}
//返回值 false:moving  true:stop
bool CUpdateDataThread::check_allaxis_state()
{
    bool ret =true;
    if(g_isBgRunning)
    {
        for(int num=0;num<=AXIS_W;num++)
        {
            if(threaddriver->Axis_State(num)==0)//轴运动状态 返回值 0:moving  1:stop
            {
                ret=false;
                break;
            }
        }
    }
    else
    {
        for(int num=0;num<=AXIS_UPSCAN;num++)
        {
            if(threaddriver->Axis_State(num)==0)//轴运动状态 返回值 0:moving  1:stop
            {
                ret=false;
                break;
            }
        }
    }
    return ret;
}

bool CUpdateDataThread::check_scanaxis_state()
{
    bool ret =true;
    if(threaddriver->Axis_State(AXIS_UPSCAN)==0)//轴运动状态 返回值 0:moving  1:stop
    {
        ret=false;
    }
    return ret;
}

bool CUpdateDataThread::check_ccdy_movestate()
{
    bool ret =true;
    for(int num=AXIS_CCD1Y;num<=AXIS_CCD4Y;num=num+2)
    {
        if(threaddriver->Axis_State(num)==0)//轴运动状态 返回值 0:moving  1:stop
        {
            ret=false;
            break;
        }
    }
    return ret;
}
//返回值 0:moving  1:stop
bool CUpdateDataThread::check_frame_movestate()
{
    bool ret =true;
    if(threaddriver->Axis_State(AXIS_W)==0)//轴运动状态 返回值 0:moving  1:stop
    {
        ret=false;
    }
    return ret;
}

void CUpdateDataThread::update()
{
    m_bCnc_state = threaddriver->CNC_State();
    m_bHard_state = threaddriver->CNC_HardCheck(0);//IO板卡通信检测
    beep();//提示和报警
    //更新框架号
    framenum();
    //所有轴位置更新
    axis_pos();
    //检查轴回零情况
    check_home();
    //检查输入端口
    check_alarminput();
    //按键扫描
    keytrigger();
    //检测灯管是否点亮
    m_bLighton = threaddriver->Get_PortVal(threaddriver->m_output.power_on,PORT_OUT);//灯管点亮
    //检测轴运动还是停止
    m_bAxis_state = check_allaxis_state();
    m_bCCDY_stop = check_ccdy_movestate();
    m_bFrame_stop = check_frame_movestate();
    m_bScanAxis_stop = check_scanaxis_state();
    m_bcool_out = threaddriver->Get_PortVal(threaddriver->m_output.cool_out,PORT_OUT);//冷风机检测
  //  m_taimian_bin_out = threaddriver->Get_PortVal(threaddriver->m_output.tb,PORT_OUT);//冰水机检测
    m_taimian_gu_out = threaddriver->Get_PortVal(threaddriver->m_output.tb2,PORT_OUT);//gu风机检测
    m_dp_zhengkon_out= threaddriver->Get_PortVal(threaddriver->m_output.jt2_airin,PORT_OUT);//底片真空
    m_bm_zhengkon_out= threaddriver->Get_PortVal(threaddriver->m_output.BM_absorb_out,PORT_OUT);//版面真空
    m_light_state_out=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_light,PORT_OUT);//曝光室照明电源状态
   // m_taimian_ZKBen_out=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_lock,PORT_OUT);//台面真空泵
    m_ccd_sifu_out=update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.DownVacumm_switch_out,PORT_OUT);//ccd伺服转矩开启

  //  m_ccd_down = threaddriver->Get_PortVal(threaddriver->m_output.ccd_down,PORT_OUT);
    //检测开关框
//    m_bopenframe[0] = threaddriver->Get_PortVal(threaddriver->m_output.frame_open,PORT_OUT);
//    m_bopenframe[1] = threaddriver->Get_PortVal(threaddriver->m_output.frame2_open,PORT_OUT);
//   m_bcloseframe[0] = threaddriver->Get_PortVal(threaddriver->m_output.frame_close,PORT_OUT);
//    m_bcloseframe[1] = threaddriver->Get_PortVal(threaddriver->m_output.frame2_close,PORT_OUT);
    m_dp_zhengkon_out = threaddriver->Get_PortVal(threaddriver->m_output.jt2_airin,PORT_OUT);
//    m_dpvacuum2 = threaddriver->Get_PortVal(threaddriver->m_output.DownDP_Vacumm_out,PORT_OUT);
    if(g_now_frame==0)
    {
        m_bjtdy = threaddriver->Get_PortVal(threaddriver->m_output.jt_airout,PORT_OUT);
        m_bframeabsorb = threaddriver->Get_PortVal(threaddriver->m_output.frame_absorb,PORT_OUT);
        m_bjtcq = threaddriver->Get_PortVal(threaddriver->m_output.jt_airin,PORT_OUT);
      //  m_framedingban = threaddriver->Get_PortVal(threaddriver->m_output.tb,PORT_OUT);
      //  m_framezjb = threaddriver->Get_PortVal(threaddriver->m_output.zjb,PORT_OUT);
    }
//    else if(g_now_frame==1)
//    {
//        m_bjtdy = threaddriver->Get_PortVal(threaddriver->m_output.jt2_airout,PORT_OUT);
//        m_bframeabsorb = threaddriver->Get_PortVal(threaddriver->m_output.frame2_absorb,PORT_OUT);
//        m_bjtcq = threaddriver->Get_PortVal(threaddriver->m_output.jt2_airin,PORT_OUT);
//        m_framedingban = threaddriver->Get_PortVal(threaddriver->m_output.tb2,PORT_OUT);
//        m_framezjb = threaddriver->Get_PortVal(threaddriver->m_output.zjb2,PORT_OUT);
//    }
    m_isxlight_on = update_thread->threaddriver->Get_PortVal(threaddriver->m_output.upxlight_power,PORT_OUT);
    CCD_POS ccdpos;
    CCDLMR_POS  lmrpos;
    ccdpos.x1 = threaddriver->Axis_CurrentPos(AXIS_CCD1X);
    ccdpos.y1 = threaddriver->Axis_CurrentPos(AXIS_CCD1Y);
    ccdpos.x2 = threaddriver->Axis_CurrentPos(AXIS_CCD2X);
    ccdpos.y2 = threaddriver->Axis_CurrentPos(AXIS_CCD2Y);
    ccdpos.x3 = threaddriver->Axis_CurrentPos(AXIS_CCD3X);
    ccdpos.y3 = threaddriver->Axis_CurrentPos(AXIS_CCD3Y);
    ccdpos.x4 = threaddriver->Axis_CurrentPos(AXIS_CCD4X);
    ccdpos.y4 = threaddriver->Axis_CurrentPos(AXIS_CCD4Y);
    lmrpos.value_l = threaddriver->Axis_CurrentPos(AXIS_L);
    lmrpos.value_m = threaddriver->Axis_CurrentPos(AXIS_M);
    lmrpos.value_r = threaddriver->Axis_CurrentPos(AXIS_R);

    //对位
    static bool oldflg_dwstart =false;
    if(oldflg_dwstart!=is_sendstart)
    {
        if(is_sendstart)
        {
            qDebug()<<"ccd send:dw";
            threaddriver->CCDSend_duiwei(0,g_dw_num,g_dw_continuenum,ccdpos,lmrpos);
            is_sendstart = false;
        }
    }
    oldflg_dwstart = is_sendstart;
    //CCD学习
    static bool oldflg_dwstudy =false;
    if(oldflg_dwstudy!=is_sendstudy)
    {
        if(is_sendstudy)
        {
            qDebug()<<"ccd send:dw study";
    //        threaddriver->CCDSend_adjust(g_study_num,lmrpos);
            threaddriver->CCDSend_duiwei(0,0,g_study_num,ccdpos,lmrpos);
            is_sendstudy = false;
        }
    }
    oldflg_dwstudy = is_sendstudy;
    //核对
    static bool oldflg_dwcheck =false;
    if(oldflg_dwcheck!=is_sendcheck)
    {
        if(is_sendcheck)
        {
            qDebug()<<"ccd send:check";
            if(g_ch_main)//主靶标核对
            {
                threaddriver->CCDSend_check(0,g_dw_num);
            }
            else
            {
                threaddriver->CCDSend_check(0,-1);
            }
            is_sendcheck = false;
        }
    }
    oldflg_dwcheck=is_sendcheck;
    //图像锁定
    static bool oldflg_ccdsd =false;
    if(oldflg_ccdsd!=is_ccdsuoding)
    {
        if(is_ccdsuoding)
        {
            qDebug()<<"ccd send:ccd sd";
            threaddriver->CCDSend_LX(0,false);
            is_ccdsuoding = false;
        }
    }
    oldflg_ccdsd = is_ccdsuoding;
    //LMR
    static bool oldflg_lmrpos =false;
    if(oldflg_lmrpos!=is_tjlmr)
    {
        if(is_tjlmr)
        {
            qDebug()<<"ccd send:get lmr pos";
            threaddriver->CCDSend_GetLMRPos(0);
            is_tjlmr = false;
        }
    }
    oldflg_lmrpos = is_tjlmr;
}

void CUpdateDataThread::run()
{
    qDebug()<<QObject::tr("CUpdateDataThread!");
//    while(1)
//    {
//        update();
////        //发送更新数据信号
////        emit sig_update();
//        if (m_bStopped)
//            break;
//    }
    updatetimer->start(1); // 1ms
    connect(updatetimer,SIGNAL(timeout()),this,SLOT(update()));
    exit(0);
}
/******************CCD 相关操作**************************/
void CUpdateDataThread::ccd_sd(int value)
{
    sd=value;
    if(g_now_frame==0)
    {
        for(int i=0;i<8;i++)
        {
            if(i<4)
            {
                threaddriver->CCDSend_basicpar(0,0,i,g_par.ccd_upld[sd][i]);
                threaddriver->CCDSend_basicpar(0,1,i,g_par.ccd_uphb[sd][i]);
            }
            else
            {
                threaddriver->CCDSend_basicpar(1,0,i-4,g_par.ccd_upld[sd][i]);
                threaddriver->CCDSend_basicpar(1,1,i-4,g_par.ccd_uphb[sd][i]);
            }
        }
    }
//    else if(g_now_frame==1)
//    {
//        for(int i=0;i<8;i++)
//        {
//            if(i<4)
//            {
//                threaddriver->CCDSend_basicpar(0,0,i,g_par.ccd_downld[sd][i]);
//                threaddriver->CCDSend_basicpar(0,1,i,g_par.ccd_downhb[sd][i]);
//            }
//            else
//            {
//                threaddriver->CCDSend_basicpar(1,0,i-4,g_par.ccd_downld[sd][i]);
//                threaddriver->CCDSend_basicpar(1,1,i-4,g_par.ccd_downhb[sd][i]);
//            }
//        }
//    }
}
void CUpdateDataThread::ld_plus(int ccd)
{
    if(ccd<0||ccd>7) return;
    char buf[1024];
    CIniRW inifile(MARKPARAMETERPATH);
    threaddriver->CCDSend_LX(0,true);//连续识别
    if(g_now_frame==0)
    {
        if(g_par.ccd_upld[sd][ccd]<98) g_par.ccd_upld[sd][ccd]+=2;
        else if(g_par.ccd_upld[sd][ccd]==98) g_par.ccd_upld[sd][ccd]+=1;
        if(ccd<4)
        {
           threaddriver->CCDSend_basicpar(0,0,ccd,g_par.ccd_upld[sd][ccd]);
        }
        else
        {
           threaddriver->CCDSend_basicpar(1,0,ccd-4,g_par.ccd_upld[sd][ccd]);
        }
        sprintf(buf,"CCD_%d",(ccd+1));
        QString val=QString::number(g_par.ccd_upld[sd][ccd],10);
        if(sd==0)
            inifile.WriteIni(buf,"JS_LD",val);
        else
            inifile.WriteIni(buf,"SD_LD",val);
    }
//    else if(g_now_frame==1)
//    {
//        if(g_par.ccd_downld[sd][ccd]<98) g_par.ccd_downld[sd][ccd]+=2;
//        else if(g_par.ccd_downld[sd][ccd]==98) g_par.ccd_downld[sd][ccd]+=1;
//        if(ccd<4)
//        {
//            threaddriver->CCDSend_basicpar(0,0,ccd,g_par.ccd_downld[sd][ccd]);
//        }
//        else
//        {
//            threaddriver->CCDSend_basicpar(1,0,ccd-4,g_par.ccd_downld[sd][ccd]);
//        }
//        sprintf(buf,"CCD_%d",(ccd+1));
//        QString val=QString::number(g_par.ccd_downld[sd][ccd],10);
//        if(sd==0)
//            inifile.WriteIni(buf,"JS_LD1",val);
//        else
//            inifile.WriteIni(buf,"SD_LD1",val);
//    }
}

void CUpdateDataThread::ld_minus(int ccd)
{
    if(ccd<0||ccd>7) return;
    CIniRW inifile(MARKPARAMETERPATH);
    char buf[1024];
    threaddriver->CCDSend_LX(0,true);//连续识别
    if(g_now_frame==0)
    {
        if(g_par.ccd_upld[sd][ccd]>2) g_par.ccd_upld[sd][ccd]-=2;
        else if(g_par.ccd_upld[sd][ccd]==2) g_par.ccd_upld[sd][ccd]-=1;
        if(ccd<4)
        {
            threaddriver->CCDSend_basicpar(0,0,ccd,g_par.ccd_upld[sd][ccd]);
        }
        else
        {
            threaddriver->CCDSend_basicpar(1,0,ccd-4,g_par.ccd_upld[sd][ccd]);
        }
        sprintf(buf,"CCD_%d",(ccd+1));
        QString val=QString::number(g_par.ccd_upld[sd][ccd],10);
        if(sd==0)
            inifile.WriteIni(buf,"JS_LD",val);
        else
            inifile.WriteIni(buf,"SD_LD",val);
    }
//    else if(g_now_frame==1)
//    {
//        if(g_par.ccd_downld[sd][ccd]>2) g_par.ccd_downld[sd][ccd]-=2;
//        else if(g_par.ccd_downld[sd][ccd]==2) g_par.ccd_downld[sd][ccd]-=1;
//        if(ccd<4)
//        {
//            threaddriver->CCDSend_basicpar(0,0,ccd,g_par.ccd_downld[sd][ccd]);
//        }
//        else
//        {
//            threaddriver->CCDSend_basicpar(1,0,ccd-4,g_par.ccd_downld[sd][ccd]);
//        }
//        sprintf(buf,"CCD_%d",(ccd+1));
//        QString val=QString::number(g_par.ccd_downld[sd][ccd],10);
//        if(sd==0)
//            inifile.WriteIni(buf,"JS_LD1",val);
//        else
//            inifile.WriteIni(buf,"SD_LD1",val);
//    }
}

void CUpdateDataThread::hb_plus(int ccd)
{
    if(ccd<0||ccd>7) return;
    char buf[1024];
    CIniRW inifile(MARKPARAMETERPATH);
    threaddriver->CCDSend_LX(0,true);//连续识别
    if(g_now_frame==0)
    {
        if(g_par.ccd_uphb[sd][ccd]<98) g_par.ccd_uphb[sd][ccd]+=2;
        else if(g_par.ccd_uphb[sd][ccd]==98) g_par.ccd_uphb[sd][ccd]+=1;
        if(ccd<4)
        {
            threaddriver->CCDSend_basicpar(0,1,ccd,g_par.ccd_uphb[sd][ccd]);
        }
        else
        {
            threaddriver->CCDSend_basicpar(1,1,ccd-4,g_par.ccd_uphb[sd][ccd]);
        }

        sprintf(buf,"CCD_%d",(ccd+1));
        QString val=QString::number(g_par.ccd_uphb[sd][ccd],10);
        if(sd==0)
            inifile.WriteIni(buf,"JS_HB",val);
        else
            inifile.WriteIni(buf,"SD_HB",val);
    }
//    else if(g_now_frame==1)
//    {
//        if(g_par.ccd_downhb[sd][ccd]<98) g_par.ccd_downhb[sd][ccd]+=2;
//        else if(g_par.ccd_downhb[sd][ccd]==98) g_par.ccd_downhb[sd][ccd]+=1;
//        if(ccd<4)
//        {
//            threaddriver->CCDSend_basicpar(0,1,ccd,g_par.ccd_downhb[sd][ccd]);
//        }
//        else
//        {
//            threaddriver->CCDSend_basicpar(1,1,ccd-4,g_par.ccd_downhb[sd][ccd]);
//        }

//        sprintf(buf,"CCD_%d",(ccd+1));
//        QString val=QString::number(g_par.ccd_downhb[sd][ccd],10);
//        if(sd==0)
//            inifile.WriteIni(buf,"JS_HB1",val);
//        else
//            inifile.WriteIni(buf,"SD_HB1",val);
//    }
}

void CUpdateDataThread::hb_minus(int ccd)
{
    if(ccd<0||ccd>7) return;
    char buf[1024];
    CIniRW inifile(MARKPARAMETERPATH);
    threaddriver->CCDSend_LX(0,true);//连续识别
    if(g_now_frame==0)
    {
        if(g_par.ccd_uphb[sd][ccd]>2) g_par.ccd_uphb[sd][ccd]-=2;
        else if(g_par.ccd_uphb[sd][ccd]==2) g_par.ccd_uphb[sd][ccd]-=1;
        if(ccd<4)
        {
            threaddriver->CCDSend_basicpar(0,1,ccd,g_par.ccd_uphb[sd][ccd]);
        }
        else
        {
            threaddriver->CCDSend_basicpar(1,1,ccd-4,g_par.ccd_uphb[sd][ccd]);
        }
        sprintf(buf,"CCD_%d",(ccd+1));
        QString val=QString::number(g_par.ccd_uphb[sd][ccd],10);
        if(sd==0)
            inifile.WriteIni(buf,"JS_HB",val);
        else
            inifile.WriteIni(buf,"SD_HB",val);
    }
//    else if(g_now_frame==1)
//    {
//        if(g_par.ccd_downhb[sd][ccd]>2) g_par.ccd_downhb[sd][ccd]-=2;
//        else if(g_par.ccd_downhb[sd][ccd]==2) g_par.ccd_downhb[sd][ccd]-=1;
//        if(ccd<4)
//        {
//            threaddriver->CCDSend_basicpar(0,1,ccd,g_par.ccd_downhb[sd][ccd]);
//        }
//        else
//        {
//            threaddriver->CCDSend_basicpar(1,1,ccd-4,g_par.ccd_downhb[sd][ccd]);
//        }

//        sprintf(buf,"CCD_%d",(ccd+1));
//        QString val=QString::number(g_par.ccd_downhb[sd][ccd],10);
//        if(sd==0)
//            inifile.WriteIni(buf,"JS_HB1",val);
//        else
//            inifile.WriteIni(buf,"SD_HB1",val);
//    }
}
void CUpdateDataThread::thread_alarmshow(QString errcode)
{
    if(g_isPause && g_isSysRunning) return;
    emit alarmshow(false,errcode);
}
//产能显示板计划值
void CUpdateDataThread::planboard(short actval)
{   
//    ModbusDataInfo todata;
//    todata.functionCode = 0x10;
//    todata.slaveID = planboard_addr;
//    todata.startAddr = 0x3609;
//    todata.nums = 1;
//    todata.data[0] = 0;
//    todata.data[1] = 0;
//    todata.data[2] = (actval>>8) & 0xFF;
//    todata.data[3] = actval & 0xFF;
//    g_LibModbus->CopyDataToSendBuff(todata);
//    g_LibModbus->sendModbusRequest();
}

void CUpdateDataThread::currentboard(short val)
{
//    ModbusDataInfo todata;
//    todata.functionCode = 0x10;
//    todata.slaveID = planboard_addr;
//    todata.startAddr = 0x3601;
//    todata.nums = 1;
//    todata.data[0] = 0;
//    todata.data[1] = 0;
//    todata.data[2] = (val>>8) & 0xFF;
//    todata.data[3] = val & 0xFF;
//    g_LibModbus->CopyDataToSendBuff(todata);
//    g_LibModbus->sendModbusRequest();
}

bool CUpdateDataThread::ModbusOpen(int cominf)
{
    bool ret=false;
  //  g_LibModbus->SetModbusConnectionType(COMRTU);
//    ModbusComPortInfo portinfo;
//    switch(cominf)
//    {
//    case 0:
//        portinfo.port = comportnum;
//        portinfo.baudrate = 9600;
//        portinfo.databits = 8;
//        portinfo.stopbits = 1;
//        portinfo.parity = 'N';
//        break;
//    case 1:
//        portinfo.port = comportnum;
//        portinfo.baudrate = 115200;
//        portinfo.databits = 8;
//        portinfo.stopbits = 1;
//        portinfo.parity = 'N';
//        break;
//    }
   // ret = g_LibModbus->SerialModbusCreate(portinfo);
    return ret;
}

//void CUpdateDataThread::slot_recdata(uint8_t * data, int dataLen,bool addNewline)
//{
//    qDebug()<<"monitor slot_recdata";
//    if( dataLen > 0 )
//    {

//    }
//}
//选择功能
void CUpdateDataThread::thread_MoveDuiwei()
{
    if(g_isPause && g_isSysRunning) return;
    emit alarmshow();
}
void CUpdateDataThread::logwrite(int tipstype, QString text)
{
    QString str_data;
    QDateTime CurrentTime=QDateTime::currentDateTime();
    QString Timestr=CurrentTime.toString("yyyy-MM-dd hh:mm:ss "); //设置显示的格式
    switch(tipstype){
    case NORMAL_TIP:
        str_data = "NORMAL:"+Timestr+text;
        break;
    case WARN_TIP:
        str_data = "WARN:"+Timestr+text;
        break;
    case ERROR_TIP:
        str_data = "ERROR:"+Timestr+text;
        g_beeptype = 2;//所有报警状态下蜂鸣器响
        break;
    }
    QFile log(LOGFILEPATH);
    if(log.size()>500*1000)
    {
        QFile::remove(LOGFILEPATHBACK);
        log.copy(LOGFILEPATHBACK);
        log.remove();
    }
    if(log.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
    {
        QTextStream in(&log);
        in.setCodec(QTextCodec::codecForName("UTF-8"));
        in<<str_data<<endl;
        log.close();
    }
}
