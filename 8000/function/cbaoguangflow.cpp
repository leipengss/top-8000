#include "cbaoguangflow.h"
#include<math.h>
#include<QDebug>
#include<stdlib.h>
#include "UI/dlgbgparameter.h"
//#include <pthread.h>
#include"filemanage/cinirw.h"
#define FRAMEID 0;
//运行流程子模块线程
static pthread_t runthreadId;
static pthread_t moveframethreadId;
static pthread_t openframethreadId;
static pthread_t pausethreadId;
static pthread_t ccdmovethreadId;
static pthread_t downccdmovethreadId;
static pthread_t dwthreadId;
static pthread_t bgrunthreadId;
static pthread_t bgtestthreadId;
static pthread_t dwcheckthreadId;
static pthread_t bgcontinuethreadId;
static pthread_t ptlmrmovethreadId;
static pthread_t downptlmrmovethreadId;
static pthread_t ptzmovethreadId;
static pthread_t dploadthreadId;
//
static bool m_downccdisstoping=false;
static bool m_ccdisstoping=false;
static bool m_ccdismoving=false;
static bool m_downccdismoving=false;
//
static bool m_isBgTest =false;
static bool m_isAirbreak =false;
//破真空
void air_break(int frameid)
{
    qDebug()<<"break air!";
    if(m_isAirbreak)
    {
        qDebug()<<"It is breaking air!";
        return;
    }
    m_isAirbreak = true;
    frameid=FRAMEID;//默认上框
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//吹气
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
        usleep(g_par.frame_open_flowdelay*1000);
        usleep(g_par.Frame_close_airdelay*1000);
        usleep(g_jtairout_delay);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);

    m_isAirbreak = false;
}
//下菲林对位核对
void *dwcheckthread(void *arg){}

//{
//    CBaoguangFlow dwflow;
//    CCDLMR_POS downlmrpos;
//    downlmrpos.value_l=0;
//    downlmrpos.value_m=0;
//    downlmrpos.value_r=0;
//    g_bDownPTDuiWeiExit = false;
//    update_thread->threaddriver->CCDSend_DWSart(1);
//    //移框后靶标切换，等待停止
//    qDebug()<<QObject::tr("down等待CCD停止");
//    dwflow.wait_downccdstop(false);
//    //压板
//    qDebug()<<QObject::tr("down压板");
//    if(g_now_frame==0)
//    {
//        if(update_thread->m_framedingban)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb,true);
//            usleep(10000);
//            //重复对位时等待顶板完成
//            if(g_downdw_num>0)
//            {
//                usleep(50000);//50ms
//            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,false);
//        }
//        if(false==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.jt_airout,PORT_OUT))
//        {
//            qDebug()<<QObject::tr("down:胶条抽气");
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//            usleep(g_jtairout_delay);
//        }
//    }
//    else if(g_now_frame==1)
//    {
//        if(update_thread->m_framedingban)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb2,true);
//            usleep(10000);
//            if(g_downdw_num>0)
//            {
//                usleep(50000);//50ms
//            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,false);
//        }
//        if(false==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.jt2_airout,PORT_OUT))
//        {
//            qDebug()<<QObject::tr("down:胶条抽气");
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
//            usleep(g_jtairout_delay);
//        }
//    }
//    if(g_downautoset)
//    {
//        //对位
//        g_downdw_num = 0;
//        g_downdw_continuenum=0;
//        g_downdw_start = false;
//        g_downdw_answer  = 0;
//        //核对
//        g_downch_start = false;
//        g_downch_answer = 0;
////        dwflow.dwpt_zmove(PT_HOME,g_now_frame,true);
//        update_thread->tj_lmrvalue[1] = downlmrpos;
//    }
//    dwflow.downccd_moveposition(false,update_thread->m_downcurrentmark);
//    //重复对位时可不置中
//    if(0==g_downdw_num)
//    {
////        qDebug()<<QObject::tr("下平台:LMR回零");
////        dwflow.dwpt_lmrmove(1,downlmrpos,g_now_frame);
////        dwflow.wait_lmrstop(1,g_now_frame);
//        qDebug()<<QObject::tr("下平台:LMR上一次对位位置");
//        dwflow.dwpt_lmrmove(1,update_thread->m_downpcbok[g_now_frame],g_now_frame);
//        dwflow.wait_lmrstop(1,g_now_frame);
//    }
//    else if(g_downdw_num>1)
//    {
//        dwflow.dwpt_lmrmove(1,update_thread->tj_lmrvalue[1],g_now_frame);
//        dwflow.wait_lmrstop(1,g_now_frame);
//    }
//    dwflow.wait_downccdstop(false);
//    qDebug()<<QObject::tr("下平台:开始对位，并等待对位结果");
//    int dccd_234=0;
//    if(g_now_frame==0)
//    {
//        dccd_234 = g_par.downccd_234[0];
//    }
//    else if(g_now_frame==1)
//    {
//        dccd_234 = g_par.downccd_234[1];
//    }
//    if(dccd_234!=3)
//    {
//        int downdw_answer=0;
//        g_bDownChange_mark =0;
//        int num=0;
//        //等待对位结果 0：继续对位 1：成功 2：失败 3：软件限位失败
//        while((downdw_answer==0)&&(g_isPause==false)&&((g_isSysRunning==true)||(g_isSysContinueRun==true)||(g_downautoset==true)))
//        {
//            /*****************
//            ******对位处理*****
//            ******************/
//            dwflow.ccd_dwtype(1,1);
//            num =0;
//            while(g_downdw_start &&(num<20))//等待CCD反馈对位结果 超时1s
//            {
//                usleep(50000);//50ms
//                num++;
//            }
//            if(num<20)
//            {
//                downdw_answer = g_downdw_answer;
//            }
//            else
//            {
//                downdw_answer = 2;
//            }
//            if(g_bDebug_run)
//            {
//                downdw_answer = g_bDWTest;
//                if(downdw_answer==0) downdw_answer=g_downdw_answer;
//            }
//            if((g_no_dw[1]==1)||(g_downruntest[g_now_frame]==true))
//            {
//                downdw_answer =1;
//            }
//            if(downdw_answer==0)//根据视觉反馈坐标偏移后继续对位
//            {
//               dwflow.dwpt_lmrmove(1,update_thread->downlmrvalue,g_now_frame);
//               dwflow.wait_lmrstop(1,g_now_frame);
//               usleep(50000);//0.05s等待轴完全停止
//            }
//            g_downdw_continuenum++;//继续对位次数
//            if((g_downdw_continuenum>200)||(num>=20))//超过单次连续对位次数
//            {
//                downdw_answer = 2;
//                qDebug("g_downdw_continuenum=%d num=%d\n",g_downdw_continuenum,num);
//                num =0;
//                qDebug()<<QObject::tr("对位超时");
//                update_thread->thread_alarmshow(QObject::tr("下平台对位超时!"));
//            }
//            if((downdw_answer==3)&&(g_bDownChange_mark==0))
//            {
//                qDebug()<<QObject::tr("下平台对位失败，切换靶点");
//                g_bDownChange_mark =1;
//                dwflow.dwpt_lmrmove(1,update_thread->m_downpcbok[g_now_frame],g_now_frame);
//                qDebug()<<QObject::tr("下平台靶标切换");
//                dwflow.downccd_moveposition(false,update_thread->m_downcurrentmark);
//                dwflow.wait_lmrstop(1,g_now_frame);
//                dwflow.wait_downccdstop(false);
//                g_downdw_continuenum=0;
//                downdw_answer=0;
//            }
//        }
//        if(downdw_answer==2)
//        {
//            //添加对位失败处理
//            g_bDownPTDuiWeiOK = false;
//            g_bDownPTDuiWeiExit = true;
//            qDebug()<<QObject::tr("下平台对位失败，即将退出");
//            if(g_now_frame==0)
//            {
//                update_thread->thread_alarmshow(QObject::tr("上框下平台对位拒曝！"));
//                g_par.JB_finish_num++;
//            }
//            else if(g_now_frame==1)
//            {
//                update_thread->thread_alarmshow(QObject::tr("下框下平台对位拒曝！"));
//                g_par.JB2_finish_num++;
//            }
//            //保存参数
//            CIniRW filesave(TOPPARAMETERPATH);
//            filesave.WriteIni("TOP","JB_FINISH_NUM",QString::number(g_par.JB_finish_num,10));
//            filesave.WriteIni("TOP","JB2_FINISH_NUM",QString::number(g_par.JB2_finish_num,10));
//            if(!g_downautoset && (g_downdw_num>0)) dwflow.reject_done();
//        }
//        else if(downdw_answer==1)
//        {
//            g_bDownPTDuiWeiOK = true;
//            //记录上一次对位OK的位置
//            update_thread->m_downpcbok[g_now_frame] = update_thread->downlmrvalue;
//        }
//        if(g_downautoset)
//        {
//            qDebug()<<QObject::tr("下平台自动对位完毕");
//        }
//    }
//    else
//    {
//        g_bDownPTDuiWeiOK = true;
//    }
//    qDebug()<<QObject::tr("下平台对位完毕");
//    //状态恢复
//    g_isDownPTDuiWei = false;
//    pthread_cancel(dwcheckthreadId);
//    pthread_exit(NULL);
//}

//对位点灯曝光
void *dwlightrunthread(void *arg)
{
//    qDebug()<<"start dwlightrun thread!";
    qDebug()<<QObject::tr("曝光延时");
    bool iserror = false,zgerror=false;
    usleep(g_par.bg_up_delaytime*1000);
    //遮光板上升
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,false);
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,true);
   // qDebug()<<QObject::tr("检测遮光板上升");
   // int zg_val =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"ZG_close_check");
  //  int num =0;
//    while((zg_val!=1)&&(num<DELAYCOUNT))
//    {
//        zg_val =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"ZG_close_check");
//        usleep(USLEEPTIME);//0.01s
//        num++;
//    }
 //   int zgopen_val =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"ZG_open_check");
//    if((zg_val!=1)||(zgopen_val!=0))
//    {
//        update_thread->thread_alarmshow(QObject::tr("遮光板上升超时，停止曝光!"));
//        qDebug()<<QObject::tr("遮光板上升超时，停止曝光!");
//        zgerror = true;
//    }
    if((false==m_isBgTest))
    {
         if(g_now_frame==0)
        {
            int taim_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
            if(taim_vacuum==0)
            {
                update_thread->thread_alarmshow(QObject::tr("曝光室内真空未达，停止曝光!"));
                iserror = true;
            }
        }
    }
    if((0==update_thread->input.EstopSig)||iserror/*||g_isPause*/)
    {
        qDebug()<<"dwlightrunthread:exit";
        //状态恢复
        g_isBgRunning = false;
        m_isBgTest = false;
        pthread_cancel(runthreadId);
        pthread_exit(NULL);
    }
    CBaoguangFlow lightbgflow;
    //检测扫描灯停止位置
    bool bstopok = lightbgflow.wait_scanstop();
    if(bstopok)
    {
        //曝光模式
        if(g_par.bg_mode==1)//时间
        {
            bool isstateok = true;
            bool buplighton_error= false;//点灯过程中的异常
            g_uplighton_error = false;
            if(g_now_frame==0)//下框在曝光室
            {
                update_thread->m_record_light_time = 0;
            }
//            else if(g_now_frame==1)//上框在曝光室
//            {
//                update_thread->m_record_light_time = 0;
//            }
             update_thread->m_record_light_time = 0;

            if(g_par.is_usescanlight==0)
            {
//                qDebug()<<QObject::tr("开启灯摆动");
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uplight_motor,true);
//                usleep(200000);//0.2等待
//                qDebug()<<QObject::tr("检查摆动状态");
//                int motor_state = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Light_moveok");
//                int cnt=0;
//                if(motor_state==1)
//                {
//                    isstateok = false;
//                }
//                //点灯前0.3s检测摆动异常
//                while((false==isstateok)&&(cnt<30))
//                {
//                    motor_state = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Light_moveok");
//                    if(motor_state==0)
//                    {
//                        isstateok = true;
//                    }
//                    usleep(10000);//10ms
//                    cnt++;
//                }
            }
            else if(g_par.is_usescanlight==1)
            {
                isstateok = 1;
            }
            if(isstateok==1)//正常
            {
                int timecnt=0;
                int light_ok1,power_on_out;
                bool uplightok;
                if(g_now_frame==0)
                {
                  timecnt=g_par.bg_up_time;
                }
//                else if(g_now_frame==1)
//                {
//                    timecnt=g_par.bg_up_time;
//                }
                 timecnt=g_par.bg_up_time;

                if(timecnt>g_par.min_bgtime)
                {
                    qDebug()<<QObject::tr("点灯");
                    if(g_par.is_usescanlight==1)
                    {
                        float upspeed = (g_par.scan_length[0]*1000.0)/timecnt;// 单位：mm/s
                        upspeed = fabs(upspeed);
                        int up_innerlimit,up_outlimit;
                        up_innerlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_innerlimit");
                        up_outlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_outlimit");
                        if((up_innerlimit==1)&&(up_outlimit!=1))
                        {
                            update_thread->threaddriver->Axis_MovePos(AXIS_UPSCAN,upspeed,g_par.scan_length[0]);
                        }
                        else if((up_innerlimit!=1)&&(up_outlimit==1))
                        {
                            update_thread->threaddriver->Axis_MovePos(AXIS_UPSCAN,upspeed,0);
                        }
                    }
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,true);
/*                    if(g_now_frame==0)
                    {
                        while((g_isBgRunning)&&(update_thread->m_record_downframelight_time<timecnt))
                        {
                            if((timecnt>1000)&&(update_thread->m_record_downframelight_time>1000))
                            {
                                light_ok1 = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Light_ison");
                                power_on_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
                                uplightok = true;
                                if(light_ok1!=1 && power_on_out==1)
                                {
                                    uplightok = false;
                                }
                                if(!uplightok)
                                {
                                    buplighton_error = true;
                                }
                                g_uplighton_error = buplighton_error;
                            }
                            update_thread->m_record_downframelight_time++;
                            usleep(1000);//1ms
                        }
                    }
                    else*/ if(g_now_frame==0)
                    {
                        while((g_isBgRunning)&&(update_thread->m_record_light_time<timecnt))
                        {
                            if((timecnt>1000)&&(update_thread->m_record_light_time>1000))
                            {
                                light_ok1 = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Light_ison");
                                power_on_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.power_on,PORT_OUT);
                                uplightok = true;
                                if(light_ok1!=1 && power_on_out==1)
                                {
                                    uplightok = false;
                                }
                                if(!uplightok)
                                {
                                    buplighton_error = true;
                                }
                                g_uplighton_error = buplighton_error;
                            }
                            update_thread->m_record_light_time++;
                            usleep(1000);//1ms
                        }
                    }
                    int up_innerlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_innerlimit");
                    int up_outlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_outlimit");
                    int cnt = 0;
                    while(((up_innerlimit!=1)&&(up_outlimit!=1))&&(cnt<500))
                    {
                        up_innerlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_innerlimit");
                        up_outlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_outlimit");
                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,true);
                        usleep(10000);
                        cnt++;
                    }
                }
                else
                {
                    update_thread->thread_alarmshow(QObject::tr("曝光时间小于下限值，不执行点灯，请检查PCB品质!"));
                }
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,false);
                if(!m_isBgTest)
                {
                    if(g_now_frame==0)
                    {
                        g_par.Frame_finish_num++;
                        g_par.DP_clear_num++;
                        g_par.DP_BG_NUM++;
                    }
//                    else if(g_now_frame==0)
//                    {
//                        g_par.Frame2_finish_num++;
//                        g_par.DP2_clear_num++;
//                        g_par.DP2_BG_NUM++;
//                    }
                }
                usleep(200000);//曝光完延时0.2s后关闭灯摆动
            }
            else
            {
                qDebug()<<QObject::tr("摆动异常");
                buplighton_error = true;
            }
            //记录曝光过程中的灯异常
            if(buplighton_error)
            {
                qDebug()<<QObject::tr("曝光运行点亮异常");
                g_light_errorcnt++;
                if(buplighton_error)
                   buplighton_error =false;
                if(!m_isBgTest)
                      g_isPause = true;//异常退出
                qDebug("g_light_errorcnt = %d\n",g_light_errorcnt);
            }
            if(g_par.is_usescanlight==0)
            {
                qDebug()<<QObject::tr("关闭灯摆动");
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uplight_motor,false);
            }
        }
        else if(g_par.bg_mode==0)//能量
        {
            qDebug()<<QObject::tr("能量曝光");
        }
    }
    else
    {
        qDebug()<<QObject::tr("扫描灯停止位置错误!");
        update_thread->thread_alarmshow(QObject::tr("扫描灯停止位置错误触发急停，请检查PCB品质后复位!"));
        lightbgflow.estop();//急停
    }

    if(0==g_par.isbg_finishairkeep)
    {
        //曝光室框架破真空
        qDebug()<<QObject::tr("曝光室框架破真空");

        if(g_now_frame==0)//上框在曝光室
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
            usleep(g_par.frame_open_flowdelay*1000);
            usleep(g_par.Frame_close_airdelay*1000);
            usleep(g_jtairout_delay);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
          //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
        }
    }
    //判断扫描灯停止位置
    if((g_isSysRunning ||m_isBgTest)&& (false == lightbgflow.wait_scanstop()))
    {
        qDebug()<<QObject::tr("点灯结束时扫描灯停止位置错误!!");
        update_thread->thread_alarmshow(QObject::tr("点灯结束时扫描灯停止位置错误触发急停,请检查PCB品质!"));
        lightbgflow.estop();
    }
    //状态恢复
    g_isBgRunning = false;
    m_isBgTest = false;
    pthread_cancel(runthreadId);
    pthread_exit(NULL);
}
void stop()
{

}

CBaoguangFlow::CBaoguangFlow()
{
    z_step = 0;
    z_movestep=0;
}
CBaoguangFlow::~CBaoguangFlow()
{
    stop();
}

//下菲林对位核对
void CBaoguangFlow::thread_duiweicheck()
{
    return;

//    g_isDownPTDuiWei = true;
//    if((0==update_thread->input.EstopSig)||g_isPause)
//    {
//        qDebug()<<"thread_duiweicheck:g_isPause";
//        g_isDownPTDuiWei = false;
//        return;
//    }
//    //下菲林对位运行线程
//    int ret = pthread_create(&dwcheckthreadId, NULL, dwcheckthread, NULL);
//    if(ret != 0)
//    {
//        qDebug()<<"fail to creat dwcheck thread!";
//        qDebug("cause:%s\n",strerror(ret));
//        g_isPause = true;
//        g_isDownPTDuiWei = false;
//    }
//    else
//    {
//        pthread_detach(dwcheckthreadId);
//    }
}

void CBaoguangFlow::thread_dwlightrun()
{
    if(g_isBGwhileStart) g_isBGwhileStart = false;
    if((0==update_thread->input.EstopSig)/*||g_isPause*/)
    {
        qDebug()<<"thread_dwlightrun:g_isPause";
        g_isBgRunning = false;
        return;
    }
    //对位点灯运行线程
    int ret = pthread_create(&runthreadId, NULL, dwlightrunthread, NULL);//創建线程 ret==0代表线程创建成功
    if(ret != 0)
    {
        qDebug()<<"fail to creat lightrun thread!";
        qDebug("cause:%s\n",strerror(ret));
        g_isPause = true;
        g_isBgRunning = false;
    }
    else
    {
        pthread_detach(runthreadId);//这将该子线程的状态设置为detached,则该线程运行结束后会自动释放所有资源。
    }
}

void CBaoguangFlow::frame_init()
{
//    //CCD抬起
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
//    //遮光板下降
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,true);
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,false);
}

void CBaoguangFlow::dwpt_lmrmove(int ch,CCDLMR_POS value_lmr,int frame)
{
    wait_lmrstop(ch,frame);
    if(ch==0)//上平台
    {
        update_thread->threaddriver->Axis_MovePos(AXIS_L,g_par.pt_speed,value_lmr.value_l);
        update_thread->threaddriver->Axis_MovePos(AXIS_M,g_par.pt_speed,value_lmr.value_m);
        update_thread->threaddriver->Axis_MovePos(AXIS_R,g_par.pt_speed,value_lmr.value_r);
    }
    else if(ch==1)//下平台
    {
        return;
    }
}

void CBaoguangFlow::wait_lmrstop(int ch,int frame)
{
    if(ch==0)
    {
        update_thread->threaddriver->Axis_WaitForStop(AXIS_L);
        update_thread->threaddriver->Axis_WaitForStop(AXIS_M);
        update_thread->threaddriver->Axis_WaitForStop(AXIS_R);
    }
    else if(ch==1)
    {
        return;
    }
}

//平台移动方式 0:回零 1：离板位置 2：对位位置 3：曝光位置
bool CBaoguangFlow::dwpt_zmove(int type,int frame,bool isbreakair)
{
     bool ret = false;
     if(update_thread->m_bCnc_state==0) return ret;
     frame=0;//默认上框
     if(frame ==0)//上框
     {
        switch (type) {
        case PT_HOME:
            target_value =0;
            break;
        case PT_LBPOSITION:
            target_value = g_up_lbposition;            
            break;
        case PT_DWPOSITION:
            target_value = g_up_dwposition;
            break;
        case PT_BGPOSITION:
            target_value = g_up_bgposition;
            break;
        default:
            break;
        }
        lb_wz = g_up_lbposition;
        dw_wz = g_up_dwposition;
        bg_wz = g_up_bgposition;
        pt_check(AXIS_Z);
        ret = pt_move(frame,AXIS_Z,isbreakair);
     }
//     else if(frame == 1)//下框
//     {
//         switch (type) {
//         case PT_HOME:
//             target_value =0;
//             break;
//         case PT_LBPOSITION:
//             target_value = g_down_lbposition;
//             break;
//         case PT_DWPOSITION:
//             target_value = g_down_dwposition;
//             break;
//         case PT_BGPOSITION:
//             target_value = g_down_bgposition;
//         default:
//             break;
//         }
//         lb_wz = g_down_lbposition;
//         dw_wz = g_down_dwposition;
//         bg_wz = g_down_bgposition;
//         pt_check(AXIS_Z);
//         ret = pt_move(frame,AXIS_Z,isbreakair);
//     }
     qDebug("dwpt_zmove:lb_wz=%.3f dw_wz=%.3f bg_wz=%f\n",lb_wz,dw_wz,bg_wz);
     return ret;
}
void CBaoguangFlow::ptz_lrsync(float speed,int axis_l,int axis_r)
{
    return;
    float zl=0.0;
    float zr=0.0;
    float zerror=0.0;
    update_thread->threaddriver->Axis_Enable(axis_l,false);
    update_thread->threaddriver->Axis_Enable(axis_r,false);
    usleep(3000);//延时3ms等待底层状态更新
    zl=update_thread->threaddriver->Axis_CurrentPos(axis_l);
    zr=update_thread->threaddriver->Axis_CurrentPos(axis_r);
    float tmp=zl-zr;
    zerror = fabs(tmp);
    if(tmp>0)
    {
        update_thread->threaddriver->Axis_MovePos(axis_l,speed,zl-zerror);
        update_thread->threaddriver->Axis_MovePos(axis_r,speed,zr);
    }
    else
    {
        update_thread->threaddriver->Axis_MovePos(axis_l,speed,zl);
        update_thread->threaddriver->Axis_MovePos(axis_r,speed,zr-zerror);
    }
}

//判断方向及每段行程  z轴
void CBaoguangFlow::pt_check(int axis)
{
   float now_value = fabs(update_thread->threaddriver->Axis_CurrentPos(axis));//轴运动实际位置
   qDebug("pt_check:now_value=%.3f target_value=%.3f axis=%d\n",now_value,target_value,axis);
   float cmdpos = fabs(target_value);
   if(now_value>=fabs(bg_wz))
   {
       z_step =2;
   }
   else if(now_value>=fabs(dw_wz))
   {
       z_step =1;
   }
   else if(now_value>=fabs(lb_wz))
   {
       z_step =0;
   }
   else if(now_value<fabs(lb_wz))
   {
       z_step =-1;
   }
   //目标行程  
   if(cmdpos>=fabs(bg_wz))
   {
       z_movestep =2;
   }
   else if(cmdpos>=fabs(dw_wz))
   {
       z_movestep =1;
   }
   else if(cmdpos>=fabs(lb_wz))
   {
       z_movestep =0;
   }
   else if(cmdpos<fabs(lb_wz))
   {
       z_movestep =-1;
   }
}

bool CBaoguangFlow::tm_locatecheck()
{
     return true;
//    //左右台面定位检测
//    int lefttaim_locate,righttaim_locate;
//    lefttaim_locate = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"PT_z_leftlocate");
//    righttaim_locate = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"PT_z_rightlocate");
//    bool val =false;
//    //检测左右台面感应信号
//    if((lefttaim_locate==1)&&(righttaim_locate==1))
//    {
//        val = true;
//    }
//    int num =0;
//    while((val==false)&&(num<DELAYCOUNT))
//    {
//        lefttaim_locate = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"PT_z_leftlocate");
//        righttaim_locate = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"PT_z_rightlocate");
//        if((lefttaim_locate==1)&&(righttaim_locate==1))
//        {
//            val = true;
//        }
//        usleep(USLEEPTIME);//10ms
//        num++;
//    }
//    if(val==false)
//    {
//        return false;
//    }
//    return true;
}
//平台移动 上升或者下降
bool CBaoguangFlow::pt_move(int frame,int axis_l,bool isairbreak)
{
    qDebug("z_step:%d z_movestep:%d\n",z_step,z_movestep);
    CCDLMR_POS lmrpos;
    lmrpos.value_l=0;
    lmrpos.value_m=0;
    lmrpos.value_r=0;
    update_thread->threaddriver->Axis_WaitForStop(axis_l);
    if(z_step<=z_movestep)//平台上升
    {
       if((z_step==-1)&&(z_movestep==-1))
       {
             update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
             update_thread->threaddriver->Axis_WaitForStop(axis_l);
             qDebug()<<QObject::tr("下降完成:平台LMR回零");
             dwpt_lmrmove(0,lmrpos,g_now_frame);
             wait_lmrstop(0,g_now_frame);
       }
       else if((z_step==-1)&&(z_movestep==0))
       {
            qDebug()<<QObject::tr("平台上升，LMR回零");
            dwpt_lmrmove(0,lmrpos,g_now_frame);
            wait_lmrstop(0,g_now_frame);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
            //3秒之内必须感应
//            if(tm_locatecheck()==false)
//            {
//                qDebug()<<QObject::tr("台面未上到位!");
//                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                return false;
//            }
           // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==-1)&&(z_movestep==1))
        {
            qDebug()<<QObject::tr("平台上升，LMR回零");
            dwpt_lmrmove(0,lmrpos,g_now_frame);
            wait_lmrstop(0,g_now_frame);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
//            if(tm_locatecheck()==false)
//            {
//                qDebug()<<QObject::tr("台面未上到位!");
//                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                return false;
//            }
          //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==-1)&&(z_movestep==2))
        {
            qDebug()<<QObject::tr("平台上升，LMR回零");
            dwpt_lmrmove(0,lmrpos,g_now_frame);
            wait_lmrstop(0,g_now_frame);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
//            if(tm_locatecheck()==false)
//            {
//                qDebug()<<QObject::tr("台面未上到位!");
//                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                return false;
//            }
         //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,dw_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
      }
      else if((z_step==0)&&(z_movestep==0))
      {
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
          if(target_value==lb_wz)
          {
//              if(tm_locatecheck()==false)
//              {
//                  qDebug()<<QObject::tr("台面未上到位!");
//                  update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                  return false;
//              }
          //    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
          }
      }
      else if((z_step==0)&&(z_movestep==1))
      {
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,lb_wz);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
//          if(tm_locatecheck()==false)
//          {
//              qDebug()<<QObject::tr("台面未上到位!");
//              update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//              return false;
//          }
       //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,target_value);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
      }
      else if((z_step==0)&&(z_movestep==2))
      {
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,lb_wz);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
          if(tm_locatecheck()==false)
          {
              qDebug()<<QObject::tr("台面未上到位!");
              update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
              return false;
          }
       //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,dw_wz);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,target_value);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
      }
      else if((z_step==1)&&(z_movestep==1))
      {
          if(tm_locatecheck()==false)
          {
              qDebug()<<QObject::tr("台面未上到位!");
              update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
              return false;
          }
        //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,target_value);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
      }
      else if((z_step==1)&&(z_movestep==2))
      {
          if(tm_locatecheck()==false)
          {
              qDebug()<<QObject::tr("台面未上到位!");
              update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
              return false;
          }
      //    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,dw_wz);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,target_value);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
      }
      else if((z_step==2)&&(z_movestep==2))
      {
          if(tm_locatecheck()==false)
          {
              qDebug()<<QObject::tr("台面未上到位!");
              update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
              return false;
          }
        //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
          update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,target_value);
          update_thread->threaddriver->Axis_WaitForStop(axis_l);
      }
    }
    else//平台下降
    {
        if((z_step==2)&&(isairbreak==true))
        {
            //台面真空度检测
            int taim_vacuum;
            //台面真空度输出
            int frame_absorb_out;
            frame=0;
//            if(frame==1)
//            {
//                taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");
//                frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
//                if((taim_vacuum==1)||(frame_absorb_out==1))
//                {
//                    qDebug("z move:frame%d air break!",frame);
//                    air_break(frame);//当前在对位位置之上，下降时需先破真空
//                }
//            }
             if(frame==0)
            {
                taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
                frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
                if((taim_vacuum==1)||(frame_absorb_out==1))
                {
                    qDebug("z move:frame%d air break!",frame);
                    air_break(frame);//当前在对位位置之上，下降时需先破真空
                }
            }
        }
        if((z_step==2)&&(z_movestep==2))
        {
            if(tm_locatecheck()==false)
            {
                qDebug()<<QObject::tr("台面未上到位!");
                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
                return false;
            }
       //     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==2)&&(z_movestep==1))
        {
            if(tm_locatecheck()==false)
            {
                qDebug()<<QObject::tr("台面未上到位!");
                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
                return false;
            }
       //     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,dw_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==2)&&(z_movestep==0))
        {
            if(tm_locatecheck()==false)
            {
                qDebug()<<QObject::tr("台面未上到位!");
                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
                return false;
            }
        //    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,dw_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==2)&&(z_movestep==-1))
        {
            if(isairbreak==true)
            {
                update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_minspeed,dw_wz);
                update_thread->threaddriver->Axis_WaitForStop(axis_l);
                update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,lb_wz);
                update_thread->threaddriver->Axis_WaitForStop(axis_l);
//                if(tm_locatecheck()==false)
//                {
//                    qDebug()<<QObject::tr("台面未上到位!");
//                    update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                    return false;
//                }
                frame=0;
                if(frame==0)
                {
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
                }
                else if(frame==1)
                {
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
                }
            }
//            int ct_flg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT);
//            if(ct_flg==1)
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
//                usleep(100000);
//            }
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==1)&&(z_movestep==1))
        {
            if(tm_locatecheck()==false)
            {
                qDebug()<<QObject::tr("台面未上到位!");
                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
                return false;
            }
         //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==1)&&(z_movestep==0))
        {
            if(tm_locatecheck()==false)
            {
                qDebug()<<QObject::tr("台面未上到位!");
                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
                return false;
            }
           //update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==1)&&(z_movestep==-1))
        {
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_midspeed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
//            if(tm_locatecheck()==false)
//            {
//                qDebug()<<QObject::tr("台面未上到位!");
//                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                return;
//            }
            if(isairbreak==true)
            {
                frame=0;
                if(frame==0)
                {
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
                }
                else if(frame==1)
                {
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
                }
            }
//            int ct_flg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT);
//            if(ct_flg==1)
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
//                usleep(100000);
//            }
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==0)&&(z_movestep==0))
        {
            if(tm_locatecheck()==false)
            {
                qDebug()<<QObject::tr("台面未上到位!");
                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
                return false;
            }
         //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
        else if((z_step==0)&&(z_movestep==-1))
        {
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,lb_wz);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
//            if(tm_locatecheck()==false)
//            {
//                qDebug()<<QObject::tr("台面未上到位!");
//                update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
//                return false;
//            }
            if(isairbreak==true)
            {
                frame=0;
                if(frame==0)
                {
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
                }
//                else if(frame==1)
//                {
//                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//                }
            }
           // int ct_flg = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT);
//            if(ct_flg==1)
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
//                usleep(100000);
//            }
            update_thread->threaddriver->Axis_MovePos(axis_l,g_par.z_speed,target_value);
            update_thread->threaddriver->Axis_WaitForStop(axis_l);
        }
    }
    return true;
}
//CCD让位
bool CBaoguangFlow::ccd_rw()
{
    if(update_thread->m_bCnc_state==0) return false;
    if(g_isPause)
    {
        qDebug()<<QObject::tr("CCD让位:g_isPause!");
        return false;
    }
    if((true==update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD3Y))
            && (true==update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD4Y)))
    {
        qDebug()<<QObject::tr("CCD已在让位位置!");
        return true;
    }
    //检测框架关闭到位
//    int up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    int down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    if((0==up_aircheck)||(0==down_aircheck))
//    {
//        //QMessageBox::information(NULL, QString("TOP"), QObject::tr("框架未关闭，请先关闭框架!"));
//        qDebug()<<QObject::tr("框架未关闭，请先关闭框架!");
//        update_thread->thread_alarmshow(QObject::tr("框架未关闭，请先关闭框架!"));
//        return false;
//    }
    //CCD抬起
//    qDebug()<<QObject::tr("ccd up 3");
//  //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
    Mark_pos marktmp;
    marktmp.x1 = 0.00;
    marktmp.x2 = 0.00;
    marktmp.x3 = 0.00;
    marktmp.x4 = 0.00;
    marktmp.y1 = 0.00;
    marktmp.y2 = 0.00;
    marktmp.y3 = 0.00;
    marktmp.y4 = 0.00;
    ccd_moveposition(false,false,marktmp);
//    //CCD定位检测
//    int left = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Left_ccd_locate");
//    int right = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Right_ccd_locate");
//    bool val =false;
//    if((left==1)&&(right==1))
//    {
//        val = true;
//    }
//    int num =0;
//    while((val==false)&&(num<DELAYCOUNT))
//    {
//        //CCD定位检测
//        left = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Left_ccd_locate");
//        right = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Right_ccd_locate");
//        if((left==1)&&(right==1))
//        {
//            val = true;
//        }
//        usleep(USLEEPTIME);//10ms
//        num++;
//    }
//    if(val==false)
//    {
//        qDebug()<<QObject::tr("相机超时未上到位!");
//        update_thread->thread_alarmshow(QObject::tr("相机超时未上到位!"));
//        return false;
//    }
    return true;
}

bool CBaoguangFlow::wait_downccdstop(bool isrw)
{
    return true;
}

bool CBaoguangFlow::wait_ccdstop(bool isrw)
{
//    int cnt = 0;
//    while(m_ccdisstoping &&(cnt<60))
//    {
//        usleep(50000);
//        cnt++;
//    }
    m_ccdisstoping = true;
    for(int anum=AXIS_CCD1X;anum<=AXIS_CCD4Y;anum++)
    {
        update_thread->threaddriver->Axis_WaitForStop(anum);
    }
    //CCD让位停止时检测停止位置原点信号
    if(isrw)
    {
        //CCD停止位置检测
        qDebug()<<QObject::tr("ccdrw:CCD停止位置检测!");
        int isccd1y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD1Y);
        int isccd2y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD2Y);
        int isccd3y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD3Y);
        int isccd4y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD4Y);
        bool issafe =false;
        if((isccd1y==1)&&(isccd2y==1)&&(isccd3y==1)&&(isccd4y==1))
        {
            issafe = true;
        }
        int cnt =0;
        while((issafe==false)&&(cnt<DELAYCOUNT))
        {
            //CCD定位检测
            isccd1y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD1Y);
            isccd2y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD2Y);
            isccd3y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD3Y);
            isccd4y = update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD4Y);
            if((isccd1y==1)&&(isccd2y==1)&&(isccd3y==1)&&(isccd4y==1))
            {
                issafe = true;
            }
            usleep(USLEEPTIME);//10ms
            cnt++;
        }
        if(issafe==false)
        {
            m_ccdisstoping = false;
            update_thread->thread_alarmshow(QObject::tr("CCD让位停止位置错误!"));
            return false;
        }
    }
    m_ccdisstoping = false;
    return true;
}

//检测CCD是否已经让位，处于安全位置
bool CBaoguangFlow::isccdsafe(int ch)
{
    bool issafe = true;
    float ccdpos[4];
    int offset = 0;
    if(ch==1)
    {
        offset = 0;//AXIS_CCD5Y - AXIS_CCD1Y;
    }
    ccdpos[0] = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD1Y+offset);
    ccdpos[1] = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD2Y+offset);
    ccdpos[2] = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD3Y+offset);
    ccdpos[3] = update_thread->threaddriver->Axis_CurrentPos(AXIS_CCD4Y+offset);
    if(fabs(ccdpos[0])>0.5||fabs(ccdpos[1])>0.5||fabs(ccdpos[2])>0.5||fabs(ccdpos[3])>0.5)
    {
        issafe = false;
    }
    return issafe;
}
//踢板
void CBaoguangFlow::tb()
{
   // bool frameopen =false;
    if(g_now_frame==0)
    {
        //检测框架是否开启
//        int up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//        if(up_aircheck==0)
//        {
//            frameopen = true;
//        }
//        int num =0;
//        while((frameopen!=true)&&(num<g_frameopen_delay))
//        {
//            up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//            if(up_aircheck==0)
//            {
//                frameopen = true;
//            }
//            usleep(1000);//1ms
//            num++;
//        }
//        if(frameopen!=true)
//        {
//            qDebug()<<QObject::tr("开框超时，踢板失败!");
//            return;
//        }
        usleep(g_par.TB_delay);
        qDebug()<<QObject::tr("开框踢板");
        //打开顶板气缸  关闭张紧板气缸
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,true);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb,false);
//        usleep(200000);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,false);
    }
//    else if(g_now_frame==1)
//    {
//        //检测框架是否开启
//        int down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//        if(down_aircheck==0)
//        {
//            frameopen = true;
//        }
//        int num =0;
//        while((frameopen!=true)&&(num<g_frameopen_delay))
//        {
//            down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//            if(down_aircheck==0)
//            {
//                frameopen = true;
//            }
//            usleep(1000);//1ms
//            num++;
//        }
//        if(frameopen!=true)
//        {
//            qDebug()<<QObject::tr("开框超时，踢板失败!");
//            return;
//        }
//        usleep(g_par.TB2_delay);
//        qDebug()<<QObject::tr("开框踢板");
//        //打开顶板气缸  关闭张紧板气缸
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,true);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb2,false);
//        usleep(200000);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,false);
//    }
}
//拒曝后的动作
void CBaoguangFlow::reject_done()
{
    CCDLMR_POS lmrpos;
    lmrpos.value_l=0;
    lmrpos.value_m=0;
    lmrpos.value_r=0;
    qDebug()<<QObject::tr("拒曝:CCD让位与破真空");
    bool brw = ccd_rw();
    dwpt_lmrmove(0,lmrpos,g_now_frame);
    wait_lmrstop(0,g_now_frame);
    bool brwstop = wait_ccdstop(true);//让位 和让位停止型号检测
    if((brw==false)||(false==brwstop))
    {
        return;
    }
    int frame_id = 0;
//    int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
   // int down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//    if((up_stop==1)/*&&(down_stop==0)*/)
//    {
//        frame_id = 1;
//    }
//    else if(/*(down_stop==1)&&*/(up_stop==0))
//    {
//        frame_id = 0;
//    }
//    int count = 0;
//    while((g_now_frame!=frame_id)&&(count<100))
//    {
//        usleep(USLEEPTIME);
//        count++;
//        up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//        //down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//        if((up_stop==1)&&(down_stop==0))
//        {
//            frame_id = 1;
//        }
//        else if((down_stop==1)&&(up_stop==0))
//        {
//            frame_id = 0;
//        }
//    }
//    if(g_now_frame!=frame_id)
//    {
//        qDebug()<<QObject::tr("拒曝:框架ID错误");
//        update_thread->thread_alarmshow(QObject::tr("框架ID错误，请检查感应位置!"));
//        return;
//    }
    if(true)
    {
        qDebug()<<QObject::tr("真空保持，破真空 移框结束");
        air_break(frame_id);
        qDebug()<<QObject::tr("板面真空");
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
            qDebug()<<QObject::tr("平台Z回到dw位置！1");
            if(false==dwpt_zmove(PT_DWPOSITION,g_now_frame,false))
            {
                thread_pause(false);
                return;
            }
            qDebug()<<QObject::tr("平台Z回到lb位置！1");
            if(false==dwpt_zmove(PT_LBPOSITION,g_now_frame,false))
            {
                thread_pause(false);
                return;
            }
                usleep(1000000);
         qDebug()<<QObject::tr("平台Z回到原点位置！1");
         if(false==dwpt_zmove(PT_HOME,g_now_frame,false))
         {
             thread_pause(false);
             return;
         }
         update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);

    }

//    if(frame_id==0)
//    {
//        int isjiaotiao = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.jt_airin,PORT_OUT);
//        int isvacuum = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//        if((isjiaotiao==1)||(isvacuum==1))
//        {
//            qDebug()<<QObject::tr("拒曝:破真空");
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
//            usleep(g_par.Frame_close_airdelay*1000);
//            usleep(g_jtairout_delay);
//        }
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
//        usleep(g_par.frame_open_flowdelay*1000);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//        dwpt_zmove(PT_HOME,frame_id,true);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);


}

void CBaoguangFlow::ptz_lbpos()
{
    float lbpos = 0;
    if(g_isPause) return;
//    //检测Z轴是否在原点位置
//    int z_safe = update_thread->threaddriver->Axis_OrgLimit(AXIS_Z);
//    if(z_safe==0)
//    {
//        qDebug()<<QObject::tr("LBPOS:Z轴未到达安全位置!");
//        return;
//    }
    if(g_now_frame ==0)//上框
    {
       lbpos = g_up_lbposition;
    }
//    else if(g_now_frame == 1)//下框
//    {
//       lbpos = g_down_lbposition;
//    }
    update_thread->threaddriver->Axis_MovePos(AXIS_Z,g_par.z_speed,lbpos);
}

bool CBaoguangFlow::wait_ptz_stop()
{
    update_thread->threaddriver->Axis_WaitForStop(AXIS_Z);
    //3秒之内必须感应
    if(tm_locatecheck()==false)
    {
        qDebug()<<QObject::tr("LBPOS:台面未上到位!");
        update_thread->thread_alarmshow(QObject::tr("台面未上到位!"));
        thread_pause(false);        
        return false;
    }
   // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
    return true;
}

//开关框架
void CBaoguangFlow::open_frame()
{
//    bool flg = false;
//    int frame_id = -1;
//    int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//    int down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//    if((up_stop==1)&&(down_stop==0))
//    {
//        frame_id = 1;
//    }
//    else if((down_stop==1)&&(up_stop==0))
//    {
//        frame_id = 0;
//    }
//    int count = 0;
//    while((g_now_frame!=frame_id)&&(count<100))
//    {
//        usleep(USLEEPTIME);
//        count++;
//        up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//        down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//        if((up_stop==1)&&(down_stop==0))
//        {
//            frame_id = 1;
//        }
//        else if((down_stop==1)&&(up_stop==0))
//        {
//            frame_id = 0;
//        }
//    }
//    if(g_now_frame!=frame_id)
//    {
//        update_thread->thread_alarmshow(QObject::tr("框架ID错误，请检查感应位置!"));
//        return;
//    }
//    if(frame_id==1)
//    {
//         flg = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//    }
//    else if(frame_id==0)
//    {
//        //检测框架关闭到位
//         flg = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//    }
//    //开框前CCD让位
//    if(flg)
//    {
//        if(g_ishomed)
//        {
//            if(false==isccdsafe(0))
//            {
//                qDebug()<<QObject::tr("CCD停止位置不安全,CCD让位!");
//                bool brw = ccd_rw();
//                bool brwstop = wait_ccdstop(true);
//                if((brw==false)||(false==brwstop))
//                {
//                    qDebug()<<QObject::tr("CCD让位失败!");
//                    update_thread->thread_alarmshow(QObject::tr("CCD让位失败!"));
//                    return;
//                }
//            }
//        }
//        if((false==update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD3Y))
//                || (false==update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD4Y)))
//        {
//            update_thread->thread_alarmshow(QObject::tr("CCD停止位置不安全!"));
//            return;
//        }
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
//    //        QMessageBox::information(NULL, QString("TOP"), QObject::tr("相机超时未上到位!"));
//            qDebug()<<QObject::tr("相机超时未上到位!");
//            update_thread->thread_alarmshow(QObject::tr("相机超时未上到位!"));
//            return ;
//        }
//        //开框前破真空
//        if(frame_id==1)//下框
//        {
//            int isjiaotiao2 = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.jt2_airin,PORT_OUT);
//            int isvacuum2 = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
//            if((isjiaotiao2==1)||(isvacuum2==1))
//            {
//                qDebug()<<QObject::tr("开框前破真空!");
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,false);
//                usleep(g_par.Frame_close_airdelay2*1000);
//                usleep(g_jtairout_delay);
//            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,true);
//            usleep(g_par.frame2_open_flowdelay*1000);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
//        }
//        else if(frame_id==0)
//        {
//            int isjiaotiao = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.jt_airin,PORT_OUT);
//            int isvacuum = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//            if((isjiaotiao==1)||(isvacuum==1))
//            {
//                qDebug()<<QObject::tr("开框前破真空!");
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
//                usleep(g_par.Frame_close_airdelay*1000);
//                usleep(g_jtairout_delay);
//            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
//            usleep(g_par.frame_open_flowdelay*1000);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
//        }
//    }
//    if(frame_id==1)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,false);
//        usleep(200000);
//        if(flg==true)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,false);
//            tb();
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//        }
//        else
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,true);
//            //开启张紧板气缸 关闭顶板气缸
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb2,true);
//            usleep(10000);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,false);
//        }
////        qDebug()<<"frame2="+QString::number(flg,10);
//    }
//    else if(frame_id==0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
//        usleep(200000);
//        if(flg==true)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);
//            tb();
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
//        }
//        else
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);
//            //开启张紧板气缸 关闭顶板气缸
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb,true);
//            usleep(10000);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,false);
//        }
////        qDebug()<<"frame="+QString::number(flg,10);
//    }
////    qDebug()<<QObject::tr("update_thread->threaddriver->m_output.frame_open!");
//    if(flg==false)
//    {
//        qDebug()<<QObject::tr("手动开关框架:检测框架是否完全关闭");
//        int up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//        int down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//        bool framecheck =false;
//        if((up_aircheck==1)&&(down_aircheck==1))
//        {
//            framecheck = true;
//        }
//        int num =0;
//        while((framecheck!=true)&&(num<g_frameclose_delay))
//        {
//            up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//            down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//            if((up_aircheck==1)&&(down_aircheck==1))
//            {
//                framecheck = true;
//            }
//            usleep(1000);//1ms
//            if((1==update_thread->input.Frame_w_safe)&&(1==g_par.ischecksafe))
//            {
//                num=0;
//            }
//            else
//            {
//                num++;
//            }
//        }
//        if(framecheck!=true)
//        {
//            update_thread->thread_alarmshow(QObject::tr("框架未关闭好!"));
//            return;
//        }
//        usleep(USLEEPTIME*20);
//        if(frame_id==0)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,true);
//        }
//        else if(frame_id==1)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,true);
//        }
//    }
}

void *openframethread(void*arg)
{
//    qDebug()<<"start open frame thread!";
//    CBaoguangFlow bgflow;
//    if(g_ishomed)
//    {
//        float znow_value = update_thread->threaddriver->Axis_CurrentPos(AXIS_Z);
//        float lbpos,bgpos;
//        if(g_now_frame==0)
//        {
//            lbpos = g_up_lbposition;
//            bgpos = g_up_bgposition;
//        }
//        else if(g_now_frame==1)
//        {
//            lbpos = g_down_lbposition;
//            bgpos = g_down_bgposition;
//        }
//        if((znow_value>=lbpos)&&(znow_value<=bgpos))
//        {
//            bgflow.dwpt_zmove(PT_LBPOSITION,g_now_frame,true);
//            bgflow.wait_ptz_stop();
//        }
//        bgflow.wait_ccdstop(false);
//    }
//    bgflow.open_frame();
//    if(g_now_frame==0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//    }
//    else if(g_now_frame==1)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
//    }
//    g_framemannual = false;
//    pthread_cancel(openframethreadId);
//    pthread_exit(NULL);
}
void CBaoguangFlow::thread_openframe()
{
//    g_framemannual = true;
//    //框
//    int ret = pthread_create(&openframethreadId, NULL, openframethread, NULL);
//    if(ret != 0)
//    {
//        qDebug()<<"fail to creat move frame thread!";
//        qDebug("cause:%s\n",strerror(ret));
//        g_framemannual = false;
//        return;
//    }
//    pthread_detach(openframethreadId);
}
//底片架设
void *dploadthread(void*arg)
{
    qDebug()<<"start dp load thread!";
    if(g_par.is_dpvacuum==1)
    {
        CBaoguangFlow dploadflow;
        //关框
//        bool is_upframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//        bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
        g_now_frame=1;//默認上框
//        if((is_upframeclose==false)||(is_downframeclose==false))
//        {
//            if(g_now_frame==1)
//            {
//                int is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//                if(is_downframeclose==0)
//                {
//                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,false);
//                    usleep(10000);
//                }
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,true);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb2,true);
//                usleep(10000);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
//            }
//            else if(g_now_frame==0)
//            {
////                int is_frameclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
////                if(is_frameclose==0)
////                {
////                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
////                    usleep(10000);
////                }
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);
//                //开启张紧板气缸 关闭顶板气缸
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb,true);
//                usleep(10000);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,false);
//            }
//        }
/*        if(g_now_frame==1)//下框
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
        }
        else */if(g_now_frame==0)
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
        }
        usleep(g_jtairout_delay);
//        int up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//        int down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//        bool framecheck =true;//框架检测关闭
//        if((up_aircheck==1)&&(down_aircheck==1))
//        {
//            framecheck = true;
//        }
        int num =0;
        //不需要检测框架关闭
//        while((framecheck!=true)&&(num<g_frameclose_delay))
//        {
//            up_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//            down_aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//            if((up_aircheck==1)&&(down_aircheck==1))
//            {
//                framecheck = true;
//            }
//            usleep(1000);//1ms
//            if((1==update_thread->input.Frame_w_safe)&&(1==g_par.ischecksafe))
//            {
//                num=0;
//            }
//            else
//            {
//                num++;
//            }
//        }
//        if(framecheck!=true)
//        {
//            update_thread->thread_alarmshow(QObject::tr("框架未关闭好!"));
//            pthread_cancel(dploadthreadId);
//            pthread_exit(NULL);
//        }
//        usleep(USLEEPTIME*20);
//        if(g_now_frame==0)
//        {
//          //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,true);
//         //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
//        }
//        else if(g_now_frame==1)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,true);
//        }
        qDebug()<<QObject::tr("dpload:BG position");
        dploadflow.dwpt_zmove(PT_BGPOSITION,g_now_frame,true);
        usleep(100000);//0.1s
        //框架真空与胶条充气
        int taim_vacuum=0,dp_vacuum=0;
        if(g_now_frame==0)
        {
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,true);
           usleep(g_jtairin_delay);
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,true);
           taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
           num =0;
           qDebug()<<QObject::tr("dpload:frame vacuum");
           while((taim_vacuum!=1)&&(num<DELAYCOUNT))
           {
               taim_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
               usleep(USLEEPTIME);//0.01s
               num++;
           }
           if(taim_vacuum!=1)
           {
               update_thread->thread_alarmshow(QObject::tr("上框框架真空到达超时，底片架设失败！"));
               air_break(g_now_frame);
               dploadflow.dwpt_zmove(PT_DWPOSITION,g_now_frame,true);
               pthread_cancel(dploadthreadId);
               pthread_exit(NULL);
           }
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,true);
           dp_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");
           num =0;
           qDebug()<<QObject::tr("dpload:dp vacuum");
           while((dp_vacuum!=1)&&(num<DELAYCOUNT))
           {
               dp_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");
               usleep(USLEEPTIME);//0.01s
               num++;
           }
           if(dp_vacuum!=1)
           {
               update_thread->thread_alarmshow(QObject::tr("上框底片架设失败，请检查底片是否摆放正常！"));
               air_break(g_now_frame);
               dploadflow.dwpt_zmove(PT_DWPOSITION,g_now_frame,true);
               pthread_cancel(dploadthreadId);
               pthread_exit(NULL);
           }
        }
//        else if(g_now_frame==1)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,true);
//            usleep(g_jtairin_delay);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,true);
//            taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");
//            num =0;
//            qDebug()<<QObject::tr("dpload:frame vacuum");
//            while((taim_vacuum!=1)&&(num<DELAYCOUNT))
//            {
//                taim_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");
//                usleep(USLEEPTIME);//0.01s
//                num++;
//            }
//            if(taim_vacuum!=1)
//            {
//                update_thread->thread_alarmshow(QObject::tr("下框框架真空到达超时，底片架设失败！"));
//                air_break(g_now_frame);
//                dploadflow.dwpt_zmove(PT_DWPOSITION,g_now_frame,true);
//                pthread_cancel(dploadthreadId);
//                pthread_exit(NULL);
//            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownDP_Vacumm_out,true);
//            dp_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"DownDP_vacuum_check");
//            num =0;
//            qDebug()<<QObject::tr("dpload:dp vacuum");
//            while((dp_vacuum!=1)&&(num<DELAYCOUNT))
//            {
//                dp_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"DownDP_vacuum_check");
//                usleep(USLEEPTIME);//0.01s
//                num++;
//            }
//            if(dp_vacuum!=1)
//            {
//                update_thread->thread_alarmshow(QObject::tr("下框底片架设失败，请检查底片是否摆放正常！"));
//                air_break(g_now_frame);
//                dploadflow.dwpt_zmove(PT_DWPOSITION,g_now_frame,true);
//                pthread_cancel(dploadthreadId);
//                pthread_exit(NULL);
//            }
//        }
        qDebug()<<QObject::tr("CCD移动到预设靶标位置!");
        dploadflow.ccd_moveposition(true,false,update_thread->m_currentmark);
        air_break(g_now_frame);
        dploadflow.dwpt_zmove(PT_DWPOSITION,g_now_frame,true);
        dploadflow.wait_ccdstop(false);
        qDebug()<<QObject::tr("dpload:success and break");       
    }
    pthread_cancel(dploadthreadId);
    pthread_exit(NULL);
}
void CBaoguangFlow::thread_dpload()
{
    int ret = pthread_create(&dploadthreadId, NULL, dploadthread, NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat dpload thread thread!";
        qDebug("cause:%s\n",strerror(ret));
        return;
    }
    pthread_detach(dploadthreadId);//释放资源
}

//按下急停后的动作
void CBaoguangFlow::estop()
{
    g_isPause = false;
    g_isBGwhileStart = false;
    g_isSysRunning = false;
    g_isPauseRun = false;
    g_isPauseStart = false;
    g_isSysContinueRun = false;
    g_isDownPTDuiWei = false;
    //
    m_downccdisstoping=false;
    m_ccdisstoping=false;
    m_ccdismoving=false;
    m_downccdismoving=false;

    update_thread->threaddriver->m_isWarnStop = true;
    update_thread->ccdmove_type[0] = -1;
    update_thread->ccdmove_type[1] = -1;
    update_thread->m_framemovepause = false;
    //关闭所有轴使能
    for(int num=0;num<=AXIS_UPSCAN;num++)
    {
        update_thread->threaddriver->Axis_Enable(num,false);
    }
    //关闭控制器使能
    update_thread->threaddriver->CNC_Disable();
    //CCD抬起
//    if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.ccd_down,PORT_OUT))
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
//    }
    //关闭灯摆动  和 灯点亮
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,false);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uplight_motor,false);
    //关锁销
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,false);
    //关闭电磁铁
   // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
    //按下急停会触发驱动器断电
    if(0==update_thread->input.EstopSig)
    {
        //关闭伺服电源
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.lmr_power,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uccd_power,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.wz_power,false);

    }
    //台面真空度检测
    int taim_vacuum;
    //台面真空度输出
    int frame_absorb_out;
    //曝光室内状态保持，室外状态恢复
    if(g_isBgRunning)
    {
        if(g_now_frame==1)
        {
            taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");
            frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
            if((taim_vacuum==1)||(frame_absorb_out==1))
            {
                air_break(1);
            }
            //关闭胶条充气抽气 张紧板气缸 框架吹气
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);

        }
        else if(g_now_frame==0)
        {
            taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
            frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
            if((taim_vacuum==1)||(frame_absorb_out==1))
            {
                air_break(0);
            }
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);

        }
    }
    else
    {
        //遮光板下降
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,true);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,false);
        //关闭上灯摇摆
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uplight_motor,false);
        //上下框破真空
        taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
        frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
        if((taim_vacuum==1)||(frame_absorb_out==1))
        {
            air_break(0);//
        }
//        taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");
//        frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT);
//        if((taim_vacuum==1)||(frame_absorb_out==1))
//        {
//            air_break(1);
//        }
        //关闭胶条充气抽气 张紧板气缸 框架吹气 冷风机
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);


//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);


       update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.cool_out,false);
    }
}

bool CBaoguangFlow::framechange2()

{
    //先判断是否在原点位置 如果在就移动 固定行程 / 如果不在原点位置 z轴下降到原点位置 然后移动固定行程回到原点 感应器感应到

    update_thread->m_isframemoving = true;
    if(g_isPause)
    {
        update_thread->m_isframemoving = false;
        return false;
    }
    //检测Z轴是否在原点位置
    qDebug()<<QObject::tr("检测Z轴是否在原点位置");
    int z_safe = update_thread->threaddriver->Axis_OrgLimit(AXIS_Z);
    int zsafenum =0;
    while((z_safe!=true)&&(zsafenum<DELAYCOUNT))
    {
        z_safe = update_thread->threaddriver->Axis_OrgLimit(AXIS_Z);
        usleep(USLEEPTIME);//0.01s
        zsafenum++;
    }
    //z_safe ==true 框架准备移动到行程位置

    if(z_safe==false)
    {
        qDebug()<<QObject::tr("Z轴未到达安全位置!");
        update_thread->thread_alarmshow(QObject::tr("Z轴未到达安全位置!"));
        update_thread->m_isframemoving = false;
        return false;
    }
    update_thread->m_framemovepause = false;
    int w_safe = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");//安全光匝
    if(w_safe)  update_thread->m_framemovepause = true;
    while(update_thread->m_framemovepause &&(1==w_safe))
    {
          w_safe = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");
          usleep(50000);
    }
    update_thread->m_framemovepause = false;
    qDebug()<<QObject::tr("检测Z轴在原点位置 准备框架交换");

    //上框停止位置，即上框在曝光室，
    int org_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");//曝光位置
    int home_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//原点
    if(org_stop==1)
    {
        qDebug()<<QObject::tr("框架交换 回到原点");
        update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);
        update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//面真空1
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//面真空2


    }else if(home_stop==1){
        qDebug()<<QObject::tr("框架交换 到达曝光位置");
        update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit);
        update_thread->threaddriver->Axis_WaitForStop(AXIS_W);

       }else{
        qDebug()<<QObject::tr("框架交换 回到原点");
        update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);
        update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//面真空1
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//面真空2

    }

    update_thread->m_isframemoving = false;
    return true;

}


//框架切换
bool CBaoguangFlow::framechange()

{
    //先判断是否在原点位置 如果在就移动 固定行程 / 如果不在原点位置 z轴下降到原点位置 然后移动固定行程回到原点 感应器感应到

    update_thread->m_isframemoving = true;
    if(g_isPause)
    {
        update_thread->m_isframemoving = false;
        return false;
    }
    //检测Z轴是否在原点位置
    int z_safe = update_thread->threaddriver->Axis_OrgLimit(AXIS_Z);
    int zsafenum =0;
    while((z_safe!=true)&&(zsafenum<DELAYCOUNT))
    {
        z_safe = update_thread->threaddriver->Axis_OrgLimit(AXIS_Z);
        usleep(USLEEPTIME);//0.01s
        zsafenum++;
    }
    //z_safe ==true 框架准备移动到行程位置

    if(z_safe==false)
    {
        qDebug()<<QObject::tr("Z轴未到达安全位置!");
        update_thread->thread_alarmshow(QObject::tr("Z轴未到达安全位置!"));
        update_thread->m_isframemoving = false;
        return false;
    }
    update_thread->m_framemovepause = false;
    int w_safe = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");//安全光匝
    if(w_safe)  update_thread->m_framemovepause = true;
    while(update_thread->m_framemovepause &&(1==w_safe))
    {
          w_safe = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");
          usleep(50000);
    }
    update_thread->m_framemovepause = false;
    //上框停止位置，即上框在曝光室，将其移出
    int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
    int home_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//原点
    if((home_stop==1)&&(up_stop==0))
    {
       update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);
//       if(g_framechange && ((update_thread->ccdmove_type[0]<=3)&&(update_thread->ccdmove_type[0]>=1)))
//       {
//             ccd_moveposition(true,true,g_par.main_markpos[0]);
//       }
//       if(g_framechange && ((update_thread->ccdmove_type[1]<=3)&&(update_thread->ccdmove_type[1]>=1)))
//       {
//             downccd_moveposition(true,g_par.main_downmarkpos[0]);
//       }
    }
    else if((up_stop==0)&&(home_stop==1))
    {
        //下框停止位置，即下框在曝光室，将其移出
       update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit);
//       if(g_framechange && ((update_thread->ccdmove_type[0]<=3)&&(update_thread->ccdmove_type[0]>=1)))
//       {
//             ccd_moveposition(true,true,g_par.main_markpos[0]);
//       }
//       if(g_framechange && ((update_thread->ccdmove_type[1]<=3)&&(update_thread->ccdmove_type[1]>=1)))
//       {
//             downccd_moveposition(true,g_par.main_downmarkpos[1]);
//       }
    }

    update_thread->m_isframemoving = false;
    return true;
////////////////////////////////////////////////////////////////////////////////////////
    if(1==update_thread->threaddriver->Axis_WaitForStop(AXIS_W))
    {
        usleep(200000);//延时0.2秒待框架停稳
        qDebug()<<QObject::tr("框架已停止!");
        //检测停止位置
        up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
        //down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
        int cnt =0;
        while((up_stop==0)/*&&(down_stop==0)*/&&(cnt<DELAYCOUNT))
        {
            up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
           // down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
            usleep(USLEEPTIME);
            cnt++;
        }
        if((up_stop==0)/*&&(down_stop==0)*/)
        {
//            QMessageBox::information(NULL, QString("TOP"), QObject::tr("框架未到达停止位置!"));
            update_thread->thread_alarmshow(("框架未到达停止位置!"));
            update_thread->m_berrorframe = true;
            update_thread->m_isframemoving = false;
            return false;
        }
        update_thread->m_berrorframe = false;
        //判断单框模式
        int up_frame_select=0,down_frame_select=0;
        if(g_par.Frame_select==0)//上框
        {
            up_frame_select = 1;
            down_frame_select = 0;
        }
//        else if(g_par.Frame_select==1)//下框
//        {
//            up_frame_select = 0;
//            down_frame_select = 1;
//        }
//        else if(g_par.Frame_select==2)//上下框
//        {
//            up_frame_select = 1;
//            down_frame_select = 1;
//        }
//        if((down_frame_select==1)&&(down_stop==0)&&(up_stop==1))
//        {
//            //下框在外将CCD降下
//           // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,true);
//            qDebug()<<QObject::tr("下框在外将CCD1-4降下!");
//        }
    }
    else
    {
        update_thread->m_isframemoving = false;
        return false;
    }
    update_thread->m_isframemoving = false;
    return true;

}


void *moveframethread(void*arg)
{
    qDebug()<<"start move frame thread!";
    CBaoguangFlow bgflow;
    //判断真空，若真空到达则破真空直接下降Z轴
 //   int taim_vacuum,frame_absorb_out;
    bool isairbreak = true;
//     if(g_now_frame==0)
//    {
//        taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
//        frame_absorb_out = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT);
//        if((taim_vacuum==1)&&(frame_absorb_out==1))
//        {
//           // isairbreak = false;
//        }
//    }

    //air_break(0);//破真空
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//胶条充气
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);//胶条抽气
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);//框架吹气

        //框架真空开启则关闭框架真空
        if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT)!=0)
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
            usleep(100000);
        }
    qDebug()<<QObject::tr("moveframethread:平台Z回到原点位置！");
    bgflow.dwpt_zmove(PT_HOME,g_now_frame,isairbreak);
    if(false==bgflow.framechange2())
    {
        qDebug()<<"平台Z回到原点位置！<<<<<<<<<<框架交换《《移框失败";
         update_thread->thread_alarmshow(QObject::tr("移框失败!"));
//        QMessageBox::information(NULL, QString("TOP"), QObject::tr("移框失败!"));
    }
    g_framechange = false;
    pthread_cancel(moveframethreadId);
    pthread_exit(NULL);
}
void CBaoguangFlow::thread_framechange()
{
    //移框
    int ret = pthread_create(&moveframethreadId, NULL, moveframethread, NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat move frame thread!";
        qDebug("cause:%s\n",strerror(ret));
        g_framechange = false;
        return;
    }
    pthread_detach(moveframethreadId);
    g_framechange = true;
}

void*bgrunthread(void *arg)
{
    CBaoguangFlow bgflow;
    qDebug()<<"manual="+QString::number(g_par.auto_manual,10);
    //false:自动 true:手动  自动定位下只执行手动运行
    if((g_par.auto_manual==1)||g_autoset)
    {
        bgflow.run();
        if(g_isSysContinueRun==false)
        {
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
        }
    }
    else if(g_autoset==false)
    {
        while(g_par.auto_manual==0)
        {
            if((0==update_thread->input.EstopSig)||g_isPause||update_thread->threaddriver->m_isWarnStop) break;
            bgflow.run();
            if(g_isSysContinueRun==false)
            {
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
            }
            if((0==update_thread->input.EstopSig)||g_isPause||update_thread->threaddriver->m_isWarnStop) break;
            //判断单框模式
            int up_frame_select=0,down_frame_select=0;
            if(g_par.Frame_select==0)//上框
            {
                up_frame_select = 1;
                down_frame_select = 0;
            }
//            else if(g_par.Frame_select==1)//下框
//            {
//                up_frame_select = 0;
//                down_frame_select = 1;
//            }
//            else if(g_par.Frame_select==2)//上下框
//            {
//                up_frame_select = 1;
//                down_frame_select = 1;
//            }
//            if(((g_now_frame==0)&&(up_frame_select==1))||
//               ((g_now_frame==1)&&(down_frame_select==1)))
//            {
//                //开框延时
//                usleep(200000);//0.2s
//                int frameopen = 1;
//                if(g_now_frame==1)
//                {
//                     frameopen = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//                }
//                else if(g_now_frame==0)
//                {
//                     frameopen = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//                }
//                if(frameopen==0)
//                {
//                    usleep(800000);//0.8s
//                }
//                //框架开启完成则停留，取放板时间
//                usleep(g_par.bg_switchboardtime*1000);
//            }
        }
    }
    //等待破真空完成
    int cnt = 0;
    while(g_isPauseStart &&(cnt<60))
    {
        usleep(50000);
        cnt++;
    }
    update_thread->threaddriver->m_isWarnStop = false;
    g_isPause = false;
    g_isSysRunning = false;//运行退出
    g_isDownPTDuiWei = false;
    pthread_cancel(bgrunthreadId);
    pthread_exit(NULL);
}
void CBaoguangFlow::thread_bgrun()
{
    int ret = pthread_create(&bgrunthreadId, NULL, bgrunthread, NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat bgrun thread!";
        qDebug("cause:%s\n",strerror(ret));
        g_isSysRunning = false;
        return;
    }
    pthread_detach(bgrunthreadId);
    g_isSysRunning = true;//进入启动运行
    g_framechange = false;
    g_isDownPTDuiWei = false;
}

void*bgtesthread(void *arg)
{
    CBaoguangFlow bgflowtest;
    bgflowtest.debug_run();
    g_isSysRunning = false;
    pthread_cancel(bgtestthreadId);
    pthread_exit(NULL);
}

void CBaoguangFlow::thread_bgtest()
{
    int ret = pthread_create(&bgtestthreadId, NULL, bgtesthread, NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat bgtest thread!";
        qDebug("cause:%s\n",strerror(ret));
        return;
    }
    pthread_detach(bgtestthreadId);
    g_isSysRunning = true;
}

void*bgcontinuethread(void *arg)
{
    CBaoguangFlow bgflow;
    //false:自动 true:手动
    if((g_par.auto_manual==1)&&(g_isBgRunning))
    {
        qDebug()<<"start bgcontinuerun thread!";
        g_isSysContinueRun = true;
        bgflow.run();
    }
    //等待破真空完成
    int cnt = 0;
    while(g_isPauseStart &&(cnt<60))
    {
        usleep(50000);
        cnt++;
    }
    update_thread->threaddriver->m_isWarnStop = false;
    g_isPause = false;
    g_isDownPTDuiWei = false;
    g_isSysContinueRun = false;
    g_isBGwhileStart = false;
    pthread_cancel(bgcontinuethreadId);
    pthread_exit(NULL);
}
void CBaoguangFlow::thread_bgcontinuerun()
{
    int ret = pthread_create(&bgcontinuethreadId, NULL, bgcontinuethread, NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat bgcontinuerun thread!";
        qDebug("cause:%s\n",strerror(ret));
        return;
    }
    pthread_detach(bgcontinuethreadId);
}

void*ptzmovethread(void*arg)
{
    int type= update_thread->zmove_type;
    QTime zmovetime;
    zmovetime.start();
    qDebug()<<"ptzmovethread="+QString::number(type,10);
    CBaoguangFlow bgflow;

        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);//框架吹气
        usleep(200000);//延时200ms

    switch (type)
    {
    case PT_HOME:
        bgflow.dwpt_zmove(PT_HOME,g_now_frame,true);
        break;
    case PT_LBPOSITION:
        bgflow.dwpt_zmove(PT_LBPOSITION,g_now_frame,true);
        break;
    case PT_DWPOSITION:
        bgflow.dwpt_zmove(PT_DWPOSITION,g_now_frame,true);
        break;
    case PT_BGPOSITION:
        bgflow.dwpt_zmove(PT_BGPOSITION,g_now_frame,true);
        break;
    default:
        break;
    }

        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);

    int zelapse = zmovetime.elapsed();
    qDebug()<<"z move time="+QString::number(zelapse,10);
    pthread_cancel(ptzmovethreadId);
    pthread_exit(NULL);
}

void CBaoguangFlow::thread_ptzmove(int type)
{
    qDebug()<<"ptzmovetype="+QString::number(type,10);
    update_thread->zmove_type=type;
    int ret = pthread_create(&ptzmovethreadId, NULL, ptzmovethread, NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat ptzmove thread!";
        qDebug("cause:%s\n",strerror(ret));
        return;
    }
    pthread_detach(ptzmovethreadId);
}
void*ptlmrmovethread(void* arg)
{
//    CCDLMR_POS pos=*(CCDLMR_POS*)arg;//类型转换
    qDebug()<<"ptlmrmovethread";
    CBaoguangFlow bgflow;
    bgflow.dwpt_lmrmove(0,update_thread->lmrvalue,0);
    bgflow.wait_lmrstop(0,0);
    pthread_cancel(ptlmrmovethreadId);
    pthread_exit(NULL);
}

void*downptlmrmovethread(void* arg)
{
//    CCDLMR_POS pos=*(CCDLMR_POS*)arg;//类型转换
    qDebug()<<"downptlmrmovethread";
    CBaoguangFlow bgflow;
    bgflow.dwpt_lmrmove(1,update_thread->downlmrvalue,0);
    bgflow.wait_lmrstop(1,g_now_frame);
    pthread_cancel(downptlmrmovethreadId);
    pthread_exit(NULL);
}

void CBaoguangFlow::thread_ptlmrmove(int ch)
{
    if(ch==0)
    {
        int ret = pthread_create(&ptlmrmovethreadId, NULL, ptlmrmovethread, NULL);
        if(ret != 0)
        {
            qDebug()<<"fail to creat ptzmove thread!";
            qDebug("cause:%s\n",strerror(ret));
            return;
        }
        pthread_detach(ptlmrmovethreadId);
    }
    else if(ch==1)
    {
        return;
    }
}

void CBaoguangFlow::scanlight_move(bool bwait,float speed)
{
    if(g_par.is_usescanlight!=1)
    {
       return;
    }
    int up_innerlimit,up_outlimit;
    up_innerlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_innerlimit");
    up_outlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_outlimit");
    if(((up_innerlimit!=1)&&(up_outlimit!=1)))
    {
        update_thread->threaddriver->Axis_MovePos(AXIS_UPSCAN,speed,0);
    }
    if(bwait)
    {
        wait_scanstop();
    }
}

bool CBaoguangFlow::wait_scanstop()
{
    if(g_par.is_usescanlight!=1)
    {
       return true;
    }
    int up_innerlimit,up_outlimit;
    update_thread->threaddriver->Axis_WaitForStop(AXIS_UPSCAN);
    usleep(100000);
    up_innerlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_innerlimit");
    up_outlimit = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpLight_outlimit");
    if((up_innerlimit!=1)&&(up_outlimit!=1))
    {
        return false;
    }
    return true;
}
void CBaoguangFlow::downccd_moveposition(bool bwait,Mark_pos mark)
{
    return;
}

bool CBaoguangFlow::IsSameMark(Mark_pos mark1,Mark_pos mark2)
{
     bool ret = false;
     if((mark1.x1==mark2.x1)&&(mark1.x2==mark2.x2)&&
        (mark1.x3==mark2.x3)&&(mark1.x4==mark2.x4)&&
        (mark1.y1==mark2.y1)&&(mark1.y2==mark2.y2)&&
        (mark1.y3==mark2.y3)&&(mark1.y4==mark2.y4))
     {
         ret = true;
     }
     return ret;
}

//CCD移动到主靶点
void CBaoguangFlow::ccd_moveposition(bool isCCD34first,bool bwait,Mark_pos mark)
{
    if(g_now_frame<0) return;
    if(g_isPause) return;
    //等待CCD上一次动作完成
//    int cnt = 0;
//    while(m_ccdismoving &&(cnt<60))
//    {
//        usleep(50000);
//        cnt++;
//    }
    m_ccdismoving = true;
    if(false==wait_ccdstop(false)) return;
    int firstmove[2],secondmove[2];
    float firstpos[2],secondpos[2];
    //让位时CCD1Y 2Y先移动：0  非让位则CCD3Y 4Y先移动
    if(isCCD34first)
    {
        firstmove[0] = AXIS_CCD3Y;
        firstmove[1] = AXIS_CCD4Y;
        secondmove[0] = AXIS_CCD1Y;
        secondmove[1] = AXIS_CCD2Y;
        firstpos[0] = mark.y3;
        firstpos[1] = mark.y4;
        secondpos[0] = mark.y1;
        secondpos[1] = mark.y2;
    }
    else
    {
        firstmove[0] = AXIS_CCD1Y;
        firstmove[1] = AXIS_CCD2Y;
        secondmove[0] = AXIS_CCD3Y;
        secondmove[1] = AXIS_CCD4Y;
        firstpos[0] = mark.y1;
        firstpos[1] = mark.y2;
        secondpos[0] = mark.y3;
        secondpos[1] = mark.y4;
    }
    update_thread->threaddriver->Axis_MovePos(firstmove[0],g_par.y_speed,firstpos[0]);
    update_thread->threaddriver->Axis_MovePos(firstmove[1],g_par.y_speed,firstpos[1]);
    int isCCD13YSafe = update_thread->input.Ccd13_protect;
    int isCCD24YSafe = update_thread->input.Ccd24_protect;
    bool bsafe = false;
    if((0==isCCD13YSafe)&&(0==isCCD24YSafe))
    {
        bsafe = true;
    }
    if(isCCD34first==false)
    {
        if((true==update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD1Y))
                &&(true==update_thread->threaddriver->Axis_OrgLimit(AXIS_CCD2Y)))
        {
            bsafe = true;
        }
    }
    int cnt = 0;
    while((false==bsafe)&&(cnt<300))//等待CCD13 24安全后开始移动内测CCD，最多延时5s,速度慢的时候时间必须保证
    {
        isCCD13YSafe = update_thread->input.Ccd13_protect;
        isCCD24YSafe = update_thread->input.Ccd24_protect;
        if((0==isCCD13YSafe)&&(0==isCCD24YSafe))
        {
            bsafe = true;
        }
        cnt++;
        usleep(10000);//0.01s
    }
    if(false==bsafe) qDebug()<<"ccd move:ccd protect is error!";
    update_thread->threaddriver->Axis_MovePos(secondmove[0],g_par.y_speed,secondpos[0]);
    update_thread->threaddriver->Axis_MovePos(secondmove[1],g_par.y_speed,secondpos[1]);
    update_thread->threaddriver->Axis_MovePos(AXIS_CCD1X,g_par.x_speed,mark.x1);
    update_thread->threaddriver->Axis_MovePos(AXIS_CCD2X,g_par.x_speed,mark.x2);
    update_thread->threaddriver->Axis_MovePos(AXIS_CCD3X,g_par.x_speed,mark.x3);
    update_thread->threaddriver->Axis_MovePos(AXIS_CCD4X,g_par.x_speed,mark.x4);
    //让位结束，初始化CCD移动类型
    if(!isCCD34first)
    {
        update_thread->ccdmove_type[0] = -1;
    }
    if(bwait)
    {
       wait_ccdstop(false);
    }
    m_ccdismoving = false;
}

//对位与核对
bool CBaoguangFlow::DuiweiCheck()

{
    qDebug()<<QObject::tr("对位核对开始 T退出 继续对位 强制曝光2");
    CCDLMR_POS lmrpos;
    lmrpos.value_l=0;
    lmrpos.value_m=0;
    lmrpos.value_r=0;
    if((0==update_thread->input.EstopSig)||g_isPause) return false;
    qDebug()<<QObject::tr("清空加工结果");
    update_thread->threaddriver->CCDSend_DWSart(0);//置对位开始
    update_thread->threaddriver->CCDSend_DWSart(1);
    qDebug()<<QObject::tr("开环形灯");
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upring_light,true);
    qDebug()<<QObject::tr("关闭对位标志位");
    //上平台对位 核对
    g_dw_num = 0;
    g_dw_continuenum  = 0;
    g_dw_start = false;
    g_dw_answer  = 0;
    g_ch_start = false;
    g_ch_answer = 0;   
    update_thread->tj_lmrvalue[0] = lmrpos;
    update_thread->tj_lmrvalue[1] = lmrpos;   
    if(g_now_frame==0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
        qDebug()<<QObject::tr("上框胶条抽气");
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);

        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
        qDebug()<<QObject::tr("上框关框");
    }
    qDebug()<<QObject::tr("平台LMR回零");
    dwpt_lmrmove(0,lmrpos,g_now_frame);
    //LMR回零等待
    wait_lmrstop(0,g_now_frame);
    usleep(50000);
    qDebug()<<QObject::tr("平台Z移到离板位置");
    ptz_lbpos();
    update_thread->threaddriver->Axis_WaitForStop(AXIS_Z);
    int cnttime = timecnt.elapsed();
    qDebug()<<QObject::tr("对位核对,关框时间:")+QString::number(cnttime,10);
    qDebug()<<QObject::tr("CCD移动到预设靶标位置!");
    ccd_moveposition(true,false,update_thread->m_currentmark);
    //记录对位开始的MARK点
    Mark_pos last_mark = update_thread->m_currentmark;
    qDebug()<<QObject::tr("平台Z移到对位位置");
    if(false==dwpt_zmove(PT_DWPOSITION,g_now_frame,false))
    {
        thread_pause(false);
        return false;
    }
    //LMR移动到上一次对位OK的位置
    qDebug()<<QObject::tr("平台LMR对位OK的位置");
    dwpt_lmrmove(0,update_thread->m_uppcbok[g_now_frame],g_now_frame);
    wait_lmrstop(0,g_now_frame);
    int bgccd_234=0;
    if(g_now_frame==0)
    {
        bgccd_234 = g_par.ccd_234[0];
        bgcheck8 = g_par.check8[0];
    }
    g_dw_continuenum=0;
    g_dw_num=0;
    dw_answer=0;
    g_bChange_mark =0;
    ch_answer =0;
    int max_dwnum =0;
    if(g_now_frame==0)
    {
        max_dwnum = g_par.DW_max_num ;
    }
    if(bgccd_234!=3)
    {
        //等待核对结果ch_answer=1 核对成功，超过最大对位次数退出对位
        while((ch_answer==0)&&(g_dw_num<max_dwnum)&&(g_isPause==false)&&((g_isSysRunning==true)||(g_isSysContinueRun==true)))
        {
            if(g_now_frame==0)
            {
                qDebug()<<QObject::tr("run释放框架真空");
                if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT)!=0)
                {
                  //  qDebug()<<QObject::tr("重复对位破真空前开磁铁!");
                 //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
                    usleep(50000);
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
                    usleep(g_par.Frame_close_airdelay*1000);
                    if(false==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.jt_airout,PORT_OUT))
                    {
                        qDebug()<<QObject::tr("dw胶条抽气");
                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
                        usleep(g_jtairout_delay);
                    }
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
                    qDebug()<<QObject::tr("到达离板位置，离开菲林!");
                    if(false==dwpt_zmove(PT_LBPOSITION,g_now_frame,false))
                    {
                        thread_pause(false);
                        return false;
                    }
//                    //重复对位
//                    if(g_dw_num>0)
//                    {
//                        dwpt_lmrmove(0,update_thread->tj_lmrvalue[0],g_now_frame);
//                        wait_lmrstop(0,g_now_frame);
//                    }
                }
            }
            qDebug()<<QObject::tr("去对位位置");
            if(false==dwpt_zmove(PT_DWPOSITION,g_now_frame,false))
            {
                thread_pause(false);
                return false;
            }
            if(false==wait_ccdstop(false))
            {
                thread_pause(false);
                return false;
            }
            //上菲林对位
            qDebug()<<QObject::tr("开始对位，并等待对位结果");
            g_dw_continuenum=0;
            dw_answer=0;
            g_bChange_mark =0;
            int num =0;
            //等待对位结果 0：继续对位 1：成功 2：失败 3：软件限位失败
            while((dw_answer==0)&&(g_isPause==false)&&((g_isSysRunning==true)||(g_isSysContinueRun==true)))
            {
                /*****************
                ******对位处理*****
                ******************/
                ccd_dwtype(0,1);//发送对位命令 g_dw_start = 1;
                num =0;
                while(g_dw_start &&(num<20))//等待CCD反馈对位结果 超时1s
                {
                    usleep(50000);//50ms
                    num++;
                }
                if(num<20)
                {
                    dw_answer = g_dw_answer;
                }
                else
                {
                    dw_answer = 2;
                }
                if(g_bDebug_run)
                {
                    dw_answer = g_bDWTest;
                    if(dw_answer==0) dw_answer=g_dw_answer;
                }
                if((g_no_dw[0]==1)||(g_runtest[g_now_frame]==true))
                {
                    dw_answer =1;
                    ch_answer = 1;
                }
                if(dw_answer==0)//根据视觉反馈坐标偏移后继续对位
                {
                   dwpt_lmrmove(0,update_thread->lmrvalue,g_now_frame);
                   wait_lmrstop(0,g_now_frame);
                   usleep(g_par.move_delay*1000000);//等待轴完全停止
                }
                g_dw_continuenum++;//继续对位次数
                if((g_dw_continuenum>ONCEDW_MAX)||(num>=20))//超过单次连续对位次数
                {
                    dw_answer = 2;
                    qDebug("g_dw_continuenum=%d num=%d\n",g_dw_continuenum,num);
                    num =0;
                    qDebug()<<QObject::tr("对位超时");
                   // update_thread->thread_alarmshow(QObject::tr("对位超时!"));
                }
                //打开次靶标切换 20200701
                if((dw_answer==3)&&(g_bChange_mark==0))
                {
                    qDebug()<<QObject::tr("对位失败，切换靶点");
                    g_bChange_mark =1;
                    dwpt_lmrmove(0,lmrpos,g_now_frame);
                    wait_lmrstop(0,g_now_frame);
                    if(false==IsSameMark(last_mark,update_thread->m_currentmark))
                    {
                        qDebug()<<QObject::tr("靶标切换");
                        ccd_moveposition(true,false,update_thread->m_currentmark);
                        last_mark = update_thread->m_currentmark;
                        wait_ccdstop(false);
                        dw_answer=0;
                    }
                    else
                    {
                        dw_answer=2;
                    }
                }
            }
            //对位完成，蜂鸣器提示
            g_beeptype =1;
            //限位报警 20200701
            if((dw_answer==2)||(dw_answer==3))
            {
                qDebug()<<QObject::tr("对位失败，即将退出");
                if(g_now_frame==0)
                {
                    if(dw_answer==2)
                    {

                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
                            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2
                        update_thread->thread_alarmshow(QObject::tr("上框对位拒曝！"));
                        g_par.bg_pos--;//第i次
//                        update_thread->thread_MoveDuiwei();
                    }
                    else
                    {
                        update_thread->thread_alarmshow(QObject::tr("上框对位超限位，请检查底片与PCB的误差!"));
                    }
                    g_par.JB_finish_num++;
                }
                //保存参数
                CIniRW filesave(TOPPARAMETERPATH);
                filesave.WriteIni("TOP","JB_FINISH_NUM",QString::number(g_par.JB_finish_num,10));
                filesave.WriteIni("TOP","JB2_FINISH_NUM",QString::number(g_par.JB2_finish_num,10));
                if(g_autoset==1)
                {
                    dwpt_lmrmove(0,lmrpos,g_now_frame);
                    if(false==dwpt_zmove(PT_HOME,g_now_frame,true)) return false;
                    wait_lmrstop(0,g_now_frame);
                    qDebug()<<QObject::tr("jb关闭胶条抽气");
                    if(g_now_frame==0)
                    {
                       // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
                        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
                    }
                    return false;
                }
                thread_pause(true);
                return false;
            }
            //记录上一次对位OK的位置
            update_thread->m_uppcbok[g_now_frame] = update_thread->lmrvalue;
            if(g_autoset==1)
            {
                qDebug()<<QObject::tr("自动对位完毕");
                return true;
            }
            cnttime = timecnt.elapsed();
            qDebug()<<QObject::tr("对位核对,对位完成的时间:")+QString::number(cnttime,10);
            qDebug()<<QObject::tr("去曝光位置");
            if(false==dwpt_zmove(PT_BGPOSITION,g_now_frame,false))
            {
                thread_pause(false);
                return false;
            }
           // qDebug()<<QObject::tr("曝光位置释放磁铁");
         //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
            cnttime = timecnt.elapsed();
            qDebug()<<QObject::tr("对位核对,去曝光位置的时间:")+QString::number(cnttime,10);
            if(g_isPause) return false;
            qDebug()<<QObject::tr("关闭胶条抽气打开胶条充气与框架真空,吸真空");
            int taim_vacuum=0;
            num =0;
            if(g_now_frame==0)
            {
               if(g_isPauseStart)  return false;
               qDebug()<<QObject::tr("破板面真空");
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1
                   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2
               qDebug()<<QObject::tr("up胶条充气");
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,true);
               usleep(g_jtairin_delay);
               if(g_isPauseStart)  return false;
               qDebug()<<QObject::tr("up框架真空");
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,true);
               taim_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
               num =0;
               while((g_isPauseStart==false)&&(taim_vacuum!=1)&&(num<DELAYCOUNT))
               {
                   taim_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
                   usleep(USLEEPTIME*2);//0.01s
                   num++;
               }
               if((g_isPauseStart==false)&&(taim_vacuum!=1))
               {
                   air_break(g_now_frame);
                   g_par.bg_pos--;//第i次
                   update_thread->thread_alarmshow(QObject::tr("上框框架真空到达超时，破真空！"));
                   thread_pause(false);
                   return false;
               }
            }
            if(g_isPauseStart)  return false;
            if(0==ch_answer)
            {
                /*********核对*************/
                 qDebug()<<QObject::tr("核对");
                 usleep(g_checkdelay*1000);
                 g_ch_main =1;
                 ccd_dwtype(0,3);//发送核对命令 g_ch_start = 1;
                 qDebug()<<QObject::tr("等待CCD核对结果");
                 num =0;
                 while(g_ch_start &&(num<20))//等待CCD反馈核对结果 超时1s
                 {
                     usleep(50000);//50ms
                     num++;
                 }
                 if(num<20)
                 {
                     ch_answer = g_ch_answer;
                 }
                 else
                 {
                     ch_answer = 0;
                 }
                 if(g_bDebug_run)
                 {
                     ch_answer = g_bCHTest;
                 }
                 //视觉核对过程中会反馈核对结果
                 if(g_no_ch[0])
                 {
                     ch_answer =1;
                 }
                 //核对超时
                 if(num>=20)
                 {
                     qDebug()<<QObject::tr("核对超时");
                     update_thread->thread_alarmshow(QObject::tr("核对超时!"));
                     ch_answer = 0;
                     num =0;
                 }
                 //
                 ccd_dwtype(0,5);//获取平均位置;
                 qDebug()<<QObject::tr("等待LMR均值");
                 num =0;
                 while(g_dw_tj &&(num<10))//等待CCD反馈结果 超时0.1s
                 {
                     usleep(10000);//10ms
                     num++;
                 }
                 if(num>=10)
                 {
                     update_thread->tj_lmrvalue[0] = lmrpos;
                 }
                 if((bgcheck8==1)&&(ch_answer==1)&&(g_no_ch[0]==0))
                 {
                     qDebug()<<QObject::tr("8点核对:切换到次标靶核对");
                     ccd_moveposition(true,true,g_par.other_markpos[g_now_frame]);
                     usleep(g_checkdelay*1000);
                     ccd_dwtype(0,3);//发送核对命令 g_ch_start = 1;
                     qDebug()<<QObject::tr("等待核对完成");
                     num =0;
                     while(g_ch_start &&(num<20))//等待CCD反馈核对结果
                     {
                         usleep(50000);//50ms
                         num++;
                     }
                     if(num<20)
                     {
                         ch_answer = g_ch_answer;
                     }
                     else
                     {
                         ch_answer = 0;
                     }
                     if(g_bDebug_run)
                     {
                         ch_answer = g_bCHTest;
                     }
                     //视觉核对过程中会反馈核对结果
                     if(g_no_ch[0])
                     {
                         ch_answer =1;
                     }
                     //8点核对超时
                     if(num>=20)
                     {
                         qDebug()<<QObject::tr("8点核对超时");
                         update_thread->thread_alarmshow(QObject::tr("8点核对超时!"));
                         ch_answer = 0;
                         num =0;
                     }
                     int new_num= g_dw_num + 1;
                     if((new_num<max_dwnum)&&(ch_answer!=1))
                     {
                         qDebug()<<QObject::tr("8点核对失败，切换到主靶标");
                         ccd_moveposition(true,true,g_par.main_markpos[g_now_frame]);
                     }
                 }
                 qDebug("核对结果为 :%d\n",ch_answer);
                 g_dw_num++;
                 qDebug()<<QObject::tr("核对完成锁定图像");
                 ccd_dwtype(0,4);//锁定图像
            }
        }
        cnttime = timecnt.elapsed();
        qDebug()<<QObject::tr("对位核对,核对完成的时间:")+QString::number(cnttime,10);
        if(ch_answer==0)
        {
            if(g_now_frame==0)
            {
                update_thread->thread_alarmshow(QObject::tr("上框核对对位拒曝！"));
                g_par.bg_pos--;//第i次
//                update_thread->thread_MoveDuiwei();//弹出选择框

                g_par.JB_finish_num++;
            }
            thread_pause(true);
            bool brw = ccd_rw();
            bool brwstop = wait_ccdstop(true);
            if((brw==false)||(false==brwstop))
            {
                thread_pause(false);
                return false;
            }
            return false;
        }
        if(g_bDebug_run)
        {
            bool brw = ccd_rw();
            bool brwstop = wait_ccdstop(true);
            if((brw==false)||(false==brwstop))
            {
                return false;
            }
            qDebug()<<QObject::tr("开始等待外部运行触发信号");
        }
        g_bDebug_run =0;
    }
    else
    {
        if(g_autoset==1)
        {
            qDebug()<<QObject::tr("自动对位完毕");
            return true;
        }
        //测试延时
        usleep(100000);       
        if(false==dwpt_zmove(PT_BGPOSITION,g_now_frame,false))
        {
            thread_pause(false);
            return false;
        }
//        qDebug()<<QObject::tr("曝光位置释放磁铁");
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
        int aircheck=0;
        if(g_now_frame==0)
        {
            if(g_isPauseStart)  return false;
            qDebug()<<QObject::tr("up胶条充气2");
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,true);
            usleep(g_jtairin_delay);
            if(g_isPauseStart)  return false;
            qDebug()<<QObject::tr("up框架真空2");
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,true);
            aircheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
          int  num =0;
            while((g_isPauseStart==false)&&(aircheck!=1)&&(num<DELAYCOUNT))
            {
                aircheck =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check");
                usleep(USLEEPTIME);//0.01s
                num++;
            }
            if((g_isPauseStart==false)&&(aircheck!=1))
            {
                qDebug()<<QObject::tr("真空到达超时，破真空");
               update_thread->thread_alarmshow(QObject::tr("真空到达超时，破真空!"));
                   air_break(0);
                   qDebug()<<QObject::tr("板面真空");
                   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
                       update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2

                thread_pause(false);
                   g_par.bg_pos--;//第i次
//                   update_thread->thread_MoveDuiwei();//弹出选择框
                return false;
            }
        }
    }
    if(g_isPauseStart)  return false;
//    qDebug()<<QObject::tr("台面上升");
//     if(g_now_frame==0)
//    {
//     //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,true);
//    }
    qDebug()<<QObject::tr("关闭环形灯");
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.upring_light,false);
    qDebug()<<QObject::tr("CCD归位");
  //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
    qDebug()<<QObject::tr("执行CCD让位");
    if(false==ccd_rw())
    {
        update_thread->thread_alarmshow(QObject::tr("CCD让位失败,运行退出!"));
        thread_pause(false);
        return false;
    }
//    qDebug()<<QObject::tr("DuiweiCheck:平台Z回到原点位置！");
//    if(false==dwpt_zmove(PT_HOME,g_now_frame,false))
//    {
//        thread_pause(false);
//        return false;
//    }

    //////////////////////// 不移动框架
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_light,false);//关闭 曝光室照明电源
    qDebug()<<QObject::tr("进入曝光流程");
    if(g_isBgRunning)
        qDebug()<<QObject::tr("等待另一个框曝光完成2");
    while(g_isBgRunning)
    {
        usleep(50000);//50ms
    }
    if(g_isBGwhileStart==true)
    {
        while(g_isSysRunning && (g_isPause==false))
        {
            usleep(50000);//50ms
        }
    }
    //等待曝光完成判断报警和停止命令
    if((0==update_thread->input.EstopSig)||g_isPause) return false;
    if(g_isPauseRun && (g_isPause==false))
           qDebug()<<QObject::tr("进入暂停状态");
    while(g_isPauseRun && (g_isPause==false))
    {
        usleep(50000);//50ms
    }
//    qDebug()<<QObject::tr("准备移框2");
//    if(false==framechange())//移框
//    {
//        qDebug()<<QObject::tr("移框失败2!");
//        thread_pause(false);
//        return false;
//    }
    usleep(50000);//延时等待更新
    qDebug()<<QObject::tr("等待CCD停止");
    if((false==wait_ccdstop(true)))
    {
        update_thread->thread_alarmshow(QObject::tr("CCD停止位置错误!"));
        thread_pause(false);
        return false;
    }
//    qDebug()<<QObject::tr("下CCD运动到靶标");
//    downccd_moveposition(false,update_thread->m_downcurrentmark);
   // cnttime = timecnt.elapsed();
  //  qDebug()<<QObject::tr("对位核对,核对完成后框架移出的时间:")+QString::number(cnttime,10);
 //   cnttime = timecnt.elapsed();
   // qDebug()<<QObject::tr("移框后发送曝光命令");
    g_isBgRunning =true;//使能曝光点灯
    thread_dwlightrun();//开启对位点灯运行曝光线程
    return true;
}

void CBaoguangFlow::run_quit(){
    CCDLMR_POS stoplmrpos;
    stoplmrpos.value_l=0;
    stoplmrpos.value_m=0;
    stoplmrpos.value_r=0;
    int frame_id=0;

    if(true)
    {
        qDebug()<<QObject::tr("破真空");
        air_break(frame_id);
         qDebug()<<QObject::tr("平台Z回到原点位置！");
         if(false==dwpt_zmove(PT_HOME,g_now_frame,false))
         {
             thread_pause(false);
             return;
         }
    }

    if(frame_id==0)
    {
        bool brw = ccd_rw();
        bool brwstop = wait_ccdstop(true);
        if((brw==false)||(false==brwstop))
        {
            thread_pause(false);
            return;
        }
        usleep(20000);//延时等待靶标更新
        qDebug()<<QObject::tr("LMR回零");
        dwpt_lmrmove(0,stoplmrpos,frame_id);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
        usleep(g_par.frame_open_flowdelay*1000);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
        wait_downccdstop(false);
        wait_lmrstop(0,0);

    }
    if((0==update_thread->input.EstopSig)||g_isPause) return;

}
//条件 重复对位 跳过 退出
void CBaoguangFlow::run_move(int ch)
{
    CIniRW inifile(TOPPARAMETERPATH);
    List_parmeter.clear();
    List_parmeterJFx.clear();
    List_parmeterJFy.clear();
    List_parmeterWCx.clear();
    List_parmeterWCy.clear();
    List_parmeterDWJD.clear();
    g_par.bg_pos=0;
    int check = inifile.ReadIni("MOVEPATH","BM_KG").toInt();
    QString  QLIST_PA = inifile.ReadIni("MOVEPATH","LIST_PA").toString();
  if(QLIST_PA.size()>0){
      QStringList group_list =  QLIST_PA.split(",");
      for(int i=1;i<=group_list.size();i++){
          QStringList list =  group_list.at(i-1).split("/");
          List_parmeterJFx.append(list.at(1));
          List_parmeterJFy.append(list.at(2));
          List_parmeterWCx.append(list.at(4));
          List_parmeterWCy.append(list.at(5));
          List_parmeterDWJD.append(list.at(0));
          if(i==1){
              List_parmeter.append(list.at(3));//移动距离
          }else{
              List_parmeter.append(list.at(3));//移动距离
          }
      }
  }
    //ch: 0 跳过 1重复对位  2 退出
    if(ch==0){//跳过
        qDebug()<<"跳过";
            // 直接退出 判断剩余次数<=1
           if (g_par.bg_down_time==1||g_par.bg_down_time-g_par.bg_pos==1){
               run_quit();//po
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//吹气
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,false);//板面吹气 O15.2
               update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);//平台回零
               update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2

                g_par.bg_pos=0;//归零
                qDebug()<<QObject::tr("the run is end");
                 return;
           }

            g_par.bg_pos++;
           int hh = List_parmeter.at(g_par.bg_pos-1).toInt()+List_parmeter.at(g_par.bg_pos).toInt();//2个距离相加 跳一个板子的距离
           List_parmeter.replace(g_par.bg_pos,QString::number(hh));
           qDebug()<<"跳过 for循环开始";
        for(int i=0;i<g_par.bg_down_time;i++){
            if(List_parmeter.size()<g_par.bg_pos){

                    break;//跳出循环 退出
            }
            run_quit();//破真空 平台下降
            update_thread->threaddriver->Axis_MovePos(AXIS_W, g_par.w_speed, g_par.w_movelimit-List_parmeter.at(g_par.bg_pos).toFloat());//g_par.w_movelimit-菲林大小
            update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
            //移动参数更新
            g_par.dw_up_jingdu =List_parmeterDWJD.at(g_par.bg_pos).toFloat();
             g_par.DP_up_error_x =List_parmeterWCx.at(g_par.bg_pos).toFloat();
             g_par.DP_up_error_y =List_parmeterWCy.at(g_par.bg_pos).toFloat();
             g_par.DP_up_junfen_x = List_parmeterJFx.at(g_par.bg_pos).toFloat();
             g_par.DP_up_junfen_y = List_parmeterJFx.at(g_par.bg_pos).toFloat();

                if(g_par.bg_pos<g_par.bg_down_time){
                    g_par.bg_pos++;//第i次
                    qDebug()<<"g_par.bg_pos=="<<g_par.bg_pos;
                }else{
                    break;
                }
                qDebug()<<QObject::tr("对位核对！DuiweiCheck");

                if(false==DuiweiCheck())
                {
                   qDebug()<<QObject::tr("对位核对失败！");
                   //T退出 继续对位 强制曝光
                   return;
                }

                qDebug()<<QObject::tr("等待曝光完成");
                //等待曝光完成
                while(g_isBgRunning)
                {
                    usleep(50000);//50ms
                }
                run_quit();//破真空 平台下降
            }
          update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);//平台回零
          update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
          int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
          int num=0;
        while((up_stop!=1)&&(num<DELAYCOUNT))//3s
        {
             up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
            usleep(USLEEPTIME);//0.01s
            num++;
        }
        if(up_stop==0){
            update_thread->thread_alarmshow("平台未回到取板位置");
            thread_pause(false);
           return;
        }          update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//吹气
          update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,false);//板面吹气 O15.2

          update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1
              update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2

           g_par.bg_pos=0;//归零
           qDebug()<<QObject::tr("the run is end");
           return;
    }else if(ch==1){
        qDebug()<<" 1重复对位";
        qDebug()<<"g_par.bg_pos=="<<g_par.bg_pos;

       for(int i=0;i<g_par.bg_down_time;i++){
           qDebug()<<QObject::tr("板面真空");
           update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2

           if(g_par.bg_pos==0){
               update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit-List_parmeter.at(g_par.bg_pos).toFloat());//g_par.w_movelimit-菲林大小
               qDebug()<<"movelimit 重复对位阿=="<<List_parmeter.at(g_par.bg_pos).toFloat();

           }else{
               //移动参数更新
               g_par.dw_up_jingdu =List_parmeterDWJD.at(g_par.bg_pos).toFloat();
                g_par.DP_up_error_x =List_parmeterWCx.at(g_par.bg_pos).toFloat();
                g_par.DP_up_error_y =List_parmeterWCy.at(g_par.bg_pos).toFloat();
                g_par.DP_up_junfen_x = List_parmeterJFx.at(g_par.bg_pos).toFloat();
                g_par.DP_up_junfen_y = List_parmeterJFx.at(g_par.bg_pos).toFloat();
                g_reloadpara =true;//参数修改后重新初始化
                qDebug()<<"参数修改后重新初始化";
               float movelimit=0;
               for(int i=0;i<=g_par.bg_pos;i++){
                 movelimit += List_parmeter.at(i).toFloat();
               }
               qDebug()<<"movelimit 重复对位阿=="<<movelimit;
               update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit-movelimit);//g_par.w_movelimit-菲林大小
           }

           update_thread->threaddriver->Axis_WaitForStop(AXIS_W);

               if(g_par.bg_pos<g_par.bg_down_time){
                   g_par.bg_pos++;//第i次
                   qDebug()<<"第i次g_par.bg_pos=="<<g_par.bg_pos;

               }else{
                 //  g_par.bg_pos--;//第i次
                   return;
               }
               qDebug()<<"false==DuiweiCheck()";
               if(false==DuiweiCheck())
               {
                  qDebug()<<QObject::tr("对位核对失败！");
                  //T退出 继续对位 强制曝光
                  return;
               }

               qDebug()<<QObject::tr("等待曝光完成");
               //等待曝光完成
               while(g_isBgRunning)
               {
                   usleep(50000);//50ms
               }
               run_quit();//破真空 平台下降
             }
                run_quit();//破真空 平台下降
                 update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);//平台回零
                 update_thread->threaddriver->Axis_WaitForStop(AXIS_W);

                 int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
                update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
                 int num=0;
               while((up_stop!=1)&&(num<DELAYCOUNT))//3s
               {
                    up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
                   usleep(USLEEPTIME);//0.01s
                   num++;
               }
               if(up_stop==0){
                   update_thread->thread_alarmshow("平台未回到取板位置");
                   thread_pause(false);
                  return;
               }
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//吹气
                 update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,false);//板面吹气 O15.2

                 update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1
                     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2


                  g_par.bg_pos=0;//归零
                  qDebug()<<QObject::tr("the run is end");
                  return;
    }else if(ch==2){
                run_quit();
                update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);//平台回零

                int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
                int num=0;
                update_thread->threaddriver->Axis_WaitForStop(AXIS_W);

              while((up_stop!=1)&&(num<DELAYCOUNT))//3s
              {
                   up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
                  usleep(USLEEPTIME);//0.01s
                  num++;
              }
              if(up_stop==0){
                  update_thread->thread_alarmshow("平台未回到取板位置");
                  thread_pause(false);
                 return;
              }
              update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//吹气
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,false);//板面吹气 O15.2

                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2


                 g_par.bg_pos=0;//归零
                 qDebug()<<QObject::tr("the run is end");
                 return;
    }




}

//单键启动与自动定位运行
void CBaoguangFlow::run()

{
   // int bg_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");//先判断曝光位子
    int uhome=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//先判断原点位子
    if(!uhome){
        qDebug()<<"框架未在原点位置 --移框";
        if(false==framechange2())//移框
        {
            qDebug()<<QObject::tr("移框失败1!");
            thread_pause(false);
            return;
        }
    }
    CIniRW inifile(TOPPARAMETERPATH);
    List_parmeter.clear();
    List_parmeterJFx.clear();
    List_parmeterJFy.clear();
    List_parmeterWCx.clear();
    List_parmeterWCy.clear();
    List_parmeterDWJD.clear();
    //是否开启大板面开关
    int check = inifile.ReadIni("MOVEPATH","BM_KG").toInt();
    int check_cf = inifile.ReadIni("MOVEPATH","BM_CF").toInt();

    if(g_par.bg_down_time>=2&&g_par.bg_pos<g_par.bg_down_time&&g_par.bg_pos>0&&check_cf==1){
        //不做处理 不归零

    }else{
        g_par.bg_pos=0;//归零

    }


    QString  QLIST_PA = inifile.ReadIni("MOVEPATH","LIST_PA").toString();
  if(QLIST_PA.size()>0){
      QStringList group_list =  QLIST_PA.split(",");
      for(int i=1;i<=group_list.size();i++){
          QStringList list =  group_list.at(i-1).split("/");
          List_parmeterJFx.append(list.at(1));
          List_parmeterJFy.append(list.at(2));
          List_parmeterWCx.append(list.at(4));
          List_parmeterWCy.append(list.at(5));
          List_parmeterDWJD.append(list.at(0));
          if(i==1){
              List_parmeter.append(list.at(3));//移动距离
          }else{
              List_parmeter.append(list.at(3));//移动距离
          }
      }
  }
    CCDLMR_POS stoplmrpos;
    stoplmrpos.value_l=0;
    stoplmrpos.value_m=0;
    stoplmrpos.value_r=0;
    timecnt.start();
    int elapsetime =0;
    if((0==update_thread->input.EstopSig)||g_isPause) return;
   // int up_aircheck = -1,down_aircheck = -1;
    int zg_val = -1,num = 0;
 //   bool framecheck =false;
    if(g_isBgRunning==false)
    {
        scanlight_move(false,g_par.light_speed);
    }

    //先判断底片真空
   int dp_vacuum=0;
    dp_vacuum = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"UpDP_vacuum_check");//底片真空达到
    if(dp_vacuum!=1)
    {
        update_thread->thread_alarmshow(QObject::tr("底片真空未达到，请检查底片是否摆放正常！"));
        thread_pause(false);
        air_break(0);
    }
    bool is_bg=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_manual");//框架曝光位置开关感应器
    if(is_bg==0){
        update_thread->thread_alarmshow("框架底片菲林未到达曝光位置");
        thread_pause(false);
        return;
    }


//版面真空是否到到达 检测里到位锁是否锁住 判断板子的大小来开启几个版面真空 BM2_absorb2_out
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//开启版面真空1
     update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"BM_absorb_out");
     if(g_par.up_ylength>255||check==1){//
         update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//开启版面真空2
         update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"BM2_absorb_out");
     }
    int  taim_vacuum=0;num=0;
  while((taim_vacuum!=1)&&(num<DELAYCOUNT))//检测板面真空是否到达
  {
      taim_vacuum =update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Air_check2");//板面的真空信号
      usleep(USLEEPTIME);//0.01s
      num++;
  }

  if(taim_vacuum!=1){
      update_thread->thread_alarmshow("板面真空未达");
      thread_pause(false);
       return ;
  }
    qDebug()<<QObject::tr("准备移框 单键");

    if(false==framechange2())//移框
    {
        qDebug()<<QObject::tr("移框失败1!");
        thread_pause(false);
        return;
    }


  //   g_par.bg_down_time 分几次 g_par.bg_pos//改为单前曝光位置 第几次
    //CIniRW inifile(TOPPARAMETERPATH);
    int gg = g_par.bg_pos;
    for(int i=gg;i<g_par.bg_down_time;i++){
        qDebug()<<QObject::tr("对位循环开始！")<<QString::number(i)<<"次曝光";
        g_par.dw_up_jingdu =List_parmeterDWJD.at(g_par.bg_pos).toFloat();
         g_par.DP_up_error_x =List_parmeterWCx.at(g_par.bg_pos).toFloat();
         g_par.DP_up_error_y =List_parmeterWCy.at(g_par.bg_pos).toFloat();
         g_par.DP_up_junfen_x = List_parmeterJFx.at(g_par.bg_pos).toFloat();
         g_par.DP_up_junfen_y = List_parmeterJFx.at(g_par.bg_pos).toFloat();
         g_reloadpara =true;//参数修改后重新初始化
         qDebug()<<" g_par.DP_up_junfen_y=="<< g_par.DP_up_junfen_y<<"g_par.DP_up_junfen_x"<<g_par.DP_up_junfen_x;
         qDebug()<<QObject::tr("板面真空");
         update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
             update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2

         if(g_par.bg_pos==0){
             update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit + List_parmeter.at(g_par.bg_pos).toFloat());//g_par.w_movelimit-菲林大小
         }else{
             float movelimit=0;
             for(int i=0;i<=g_par.bg_pos;i++){
               movelimit += List_parmeter.at(i).toFloat();
             }
             qDebug()<<"movelimit=="<<movelimit;
             if(1395<(g_par.w_movelimit + movelimit)){
                 qDebug()<<QObject::tr("移框距离过大");
                 update_thread->thread_alarmshow("移框距离过大");
                 thread_pause(false);
                 return;
             }
             update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit + movelimit);//g_par.w_movelimit-菲林大小
         }
        update_thread->threaddriver->Axis_WaitForStop(AXIS_W);//等待w轴停止
        qDebug()<<QObject::tr("对位循环开始！")<<QString::number(i)<<"w移动";

//        if(((update_thread->ccdmove_type[0]<=3)&&(update_thread->ccdmove_type[0]>=1)))
//        {
//              ccd_moveposition(true,true,g_par.main_markpos[0]);
//        }
        {
            if(g_par.bg_pos<g_par.bg_down_time){
                g_par.bg_pos++;//第i次
            }else{
                return;
            }
        if(false==DuiweiCheck())
            {
               qDebug()<<QObject::tr("对位核对失败！T退出 555555");
               //T退出 继续对位
               qDebug()<<QObject::tr("板面真空");
               update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
                   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2
                break;
              // return;
            }

        if(g_autoset){
            qDebug()<<QObject::tr("单键运行结束时间:自动定位end");
            g_par.bg_pos=0;//归零
            return;
        }

            qDebug()<<QObject::tr("等待曝光完成");
            //等待曝光完成
            while(g_isBgRunning)
            {
                usleep(50000);//50ms
            }
            int frame_id = 0;

            if(true)
            {
                qDebug()<<QObject::tr("板面真空");
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,true);//板面真空1
                    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,true);//板面真空2
                qDebug()<<QObject::tr("真空保持，破真空 ");
                air_break(frame_id);
                dwpt_lmrmove(0,stoplmrpos,frame_id);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
//                usleep(g_par.frame_open_flowdelay*1000);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);


                 qDebug()<<QObject::tr("平台Z回到原点位置！1");
                 if(false==dwpt_zmove(PT_HOME,g_now_frame,false))
                 {
                     thread_pause(false);
                     return;
                 }
            }

            if(frame_id==0)
            {
                bool brw = ccd_rw();
                bool brwstop = wait_ccdstop(true);
                if((brw==false)||(false==brwstop))
                {
                    thread_pause(false);
                    return;
                }
                usleep(20000);//延时等待靶标更新
                qDebug()<<QObject::tr("LMR回零");

                wait_downccdstop(false);
                wait_lmrstop(0,0);

            }
            if((0==update_thread->input.EstopSig)||g_isPause) return;
        }
        qDebug()<<QObject::tr("//////////////////////////////////////////////");

    }
    qDebug()<<QObject::tr("平台Z回到原点位置！22");
    //air_break(0);
    if(false==dwpt_zmove(PT_HOME,g_now_frame,false))
    {
        thread_pause(false);
        return;
    }
    qDebug()<<QObject::tr("平台w回到原点位置！22");
    update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,0);//平台回零
    update_thread->threaddriver->Axis_WaitForStop(AXIS_W);//等待w轴停止

    int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
  while((up_stop!=1)&&(num<DELAYCOUNT))//3s
  {
       up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");//判断是否在原点位子
      usleep(USLEEPTIME);//0.01s
      num++;
  }
  if(up_stop==0){
      update_thread->thread_alarmshow("平台未回到取板位置");
      thread_pause(false);
     return;
  }
  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);//吹气
  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,true);//板面吹气 O15.2

    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb_out,false);//破板面真空1

        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM2_absorb_out,false);//破板面真空2
        usleep(USLEEPTIME*100);//0.01s
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.BM_absorb2_out,false);//板面吹气 O15.2

     //g_par.bg_pos=0;//归零
     qDebug()<<QObject::tr("the run is end");
     qDebug()<<QObject::tr("单键运行结束时间:")+QString::number(elapsetime,10);
     return;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// int frame_id=0;
//    if(false)
//    {
//        qDebug()<<QObject::tr("忽略框处理2");
//        if(true)
//        {
//            qDebug()<<QObject::tr("真空保持，移框结束破真空2");
//            air_break(frame_id);
//            if(frame_id==1)
//            {
//             //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
//            }
//            else if(frame_id==0)
//            {
//             //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
//            }
//        }
//        if(frame_id==0)
//        {
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
////            usleep(10000);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);
////            bool brw = ccd_rw();
////            bool brwstop = wait_ccdstop(true);
////            if((brw==false)||(false==brwstop))
////            {
////                thread_pause(false);
////                return;
////            }
//            usleep(20000);//延时等待靶标更新
//          //  qDebug()<<QObject::tr("下CCD运动到靶标");
//          //  downccd_moveposition(false,update_thread->m_downcurrentmark);
//            ptz_lbpos();
//            qDebug()<<QObject::tr("LMR回零");
//            dwpt_lmrmove(0,stoplmrpos,frame_id);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
//            usleep(g_par.frame_open_flowdelay*1000);
//            //update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);
//           // tb();
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//            if(false==wait_ptz_stop()) return;
//            wait_lmrstop(0,frame_id);
//        }
////        else if(frame_id==1)
////        {
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,false);
////            usleep(10000);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,false);
//////            bool brw = ccd_rw();
//////            bool brwstop = wait_ccdstop(true);
//////            if((brw==false)||(false==brwstop))
//////            {
//////                thread_pause(false);
//////                return;
//////            }
////            usleep(20000);//延时等待靶标更新
////            qDebug()<<QObject::tr("下CCD运动到靶标");
////            downccd_moveposition(false,update_thread->m_downcurrentmark);
////            ptz_lbpos();
////            qDebug()<<QObject::tr("LMR回零");
////            dwpt_lmrmove(0,stoplmrpos,frame_id);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,true);
////            usleep(g_par.frame2_open_flowdelay*1000);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,true);
////            tb();
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
////            if(false==wait_ptz_stop()) return;
////            wait_downccdstop(false);
////            wait_lmrstop(0,frame_id);
////        }

//        //切换靶标
//        g_bChange_mark =true;
//        qDebug()<<QObject::tr("等待曝光完成");
//        //等待曝光完成
//        while(g_isBgRunning)
//        {
//            usleep(50000);//50ms
//        }
//        if((0==update_thread->input.EstopSig)||g_isPause) return;
//    }

//    else
//    {
//        if(g_now_frame==0)
//        {
//            if(1==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT))
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
////                usleep(500000);
//            }
////            qDebug()<<QObject::tr("ccd up 1");
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);//ccd抬起
////            int is_frameclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
////            if(is_frameclose==0)
////            {
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
////                usleep(10000);
////            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,true);
//            //开启张紧板气缸 关闭顶板气缸
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb,true);
////            usleep(10000);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb,false);
//        }
////        else if(g_now_frame==1)
//        {
//            if(1==update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT))
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,false);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
////                usleep(500000);
//            }
////            qDebug()<<QObject::tr("ccd up 2");
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);//ccd抬起
//            int is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//            if(is_downframeclose==0)
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,false);
//                usleep(10000);
//            }
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,true);
//            //开启张紧板气缸 关闭顶板气缸
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zjb2,true);
//            usleep(10000);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.tb2,false);
//        }
//        dwpt_zmove(PT_HOME,g_now_frame);
//        qDebug()<<QObject::tr("检测框架是否完全关闭");
//        if(g_now_frame==0)
//        {
//            int upcheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//            num =0;
//            while((upcheck!=true)&&(num<g_frameclose_delay))
//            {
//                upcheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check");
//                usleep(1000);//1ms
//                num++;
//            }
//            if(upcheck!=true)
//            {
////                QMessageBox::information(NULL, QString("TOP"), QObject::tr("框架未关闭好!"));
//                update_thread->thread_alarmshow(QObject::tr("框架未关闭好!"));
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,false);
//                thread_pause(false);
//                return;
//            }
//        }
//        else if(g_now_frame==1)
//        {
//            int downcheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//            num =0;
//            while((downcheck!=true)&&(num<g_frameclose_delay))
//            {
//                downcheck = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//                usleep(1000);//1ms
//                num++;
//            }
//            if(downcheck!=true)
//            {
////                QMessageBox::information(NULL, QString("TOP"), QObject::tr("框架未关闭好!"));
//                update_thread->thread_alarmshow(QObject::tr("框架未关闭好!"));
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_close,false);
////                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_open,false);
//                thread_pause(false);
//                return;
//            }
//        }
//        usleep(USLEEPTIME*20);
//        if(g_now_frame==0)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,true);
//        }
//        else if(g_now_frame==1)
//        {
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,true);
//        }
//        qDebug()<<QObject::tr("等待另一个框曝光完成end");
//        if(g_isBgRunning && (g_par.auto_manual==0)&&(g_par.Frame_select==2))//非单框模式自动运行
//        {
//            qDebug()<<QObject::tr("曝光室外框架对位核对");
//            if(false==DuiweiCheck())
//            {
//               qDebug()<<QObject::tr("对位核对失败！");
//               return;
//            }
//        }
//        else
//        {
//            qDebug()<<QObject::tr("two:平台Z回到原点位置！");
//            if(false==dwpt_zmove(PT_HOME,g_now_frame,false))
//            {
//                thread_pause(false);
//                return;
//            }
//            while(g_isBgRunning)
//            {
//                usleep(50000);//50ms
//            }
//            qDebug()<<QObject::tr("准备移框end");
//            if(false==framechange2())//移框
//            {
//                qDebug()<<QObject::tr("移框失败end!");
//                thread_pause(false);
//                return;
//            }
//            usleep(50000);
//            elapsetime = timecnt.elapsed();
//            qDebug()<<QObject::tr("点灯完成移框end时间:")+QString::number(elapsetime,10);
//        }
//        qDebug()<<QObject::tr("dw and ch:ok");
 //       frame_id = -1;
//        int up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//        int down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//        if((up_stop==1)&&(down_stop==0))
//        {
//            frame_id = 1;
//        }
//        else if((down_stop==1)&&(up_stop==0))
//        {
//            frame_id = 0;
//        }
//        int count = 0;
//        while((g_now_frame!=frame_id)&&(count<100))
//        {
//            usleep(USLEEPTIME);
//            count++;
////            up_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
////            down_stop=update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Aux_home_w");
//            if((up_stop==1)&&(down_stop==0))
//            {
//                frame_id = 1;
//            }
//            else if((down_stop==1)&&(up_stop==0))
//            {
//                frame_id = 0;
//            }
//        }
//        if(g_now_frame!=frame_id)
//        {
//            update_thread->thread_alarmshow(QObject::tr("框架ID错误，请检查感应位置!"));
//            return;
//        }
//        if(true)
//        {
//            qDebug()<<QObject::tr("真空保持，移框结束破真空");
//            air_break(frame_id);
//            if(frame_id==1)
//            {
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
//            }
//             if(frame_id==0)
//            {
//              //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
//            }
//        }
//        if(frame_id==0)
//        {
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,false);
////            usleep(10000);
////            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_close,false);
//            bool brw = ccd_rw();
//            bool brwstop = wait_ccdstop(true);
//            if((brw==false)||(false==brwstop))
//            {
//                thread_pause(false);
//                return;
//            }
//            usleep(20000);//延时等待靶标更新
//            qDebug()<<QObject::tr("下CCD运动到靶标");
//            downccd_moveposition(false,update_thread->m_downcurrentmark);
//            ptz_lbpos();
//            qDebug()<<QObject::tr("LMR回零");
//            dwpt_lmrmove(0,stoplmrpos,frame_id);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
//            usleep(g_par.frame_open_flowdelay*1000);
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_open,true);
//            tb();
//            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//            if(false==wait_ptz_stop()) return;
//            wait_downccdstop(false);
//            wait_lmrstop(0,frame_id);
//        }
//        elapsetime = timecnt.elapsed();
//        qDebug()<<QObject::tr("移框end后CCD让位与Z到离板位置的时间:")+QString::number(elapsetime,10);

//    }


//    qDebug()<<QObject::tr("the run is end");
//    qDebug()<<QObject::tr("单键运行结束时间:")+QString::number(elapsetime,10);
//    return;
}


//曝光测试
void CBaoguangFlow::debug_run()
{
     scanlight_move(false,g_par.light_speed);
     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.power_on,false);//先关灯
     g_isBgRunning = true;
     m_isBgTest = true;
     thread_dwlightrun();//开启点灯运行曝光线程
     while(g_isBgRunning)
     {
         usleep(50000);//50ms
     }
     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
//     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,false);
//     update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
}

//对位核对线程
void *dwthread(void *arg)
{
    qDebug()<<"start duiwei thread!";
    CBaoguangFlow flow;
    //CCD对位
    if(g_dw_start)
    {
//        if((g_dw_answer==0)&&(g_dw_num<5))
//        {
//            flow.dwpt_lmrmove(0,update_thread->lmrvalue);
//            g_dw_num++;
//            flow.ccd_dwtype(0,1);//继续对位
//        }
//        else
//        {
//            g_dw_num = 0;
//        }
    }
    //CCD学习
    if(g_study_start)
    {
       if((g_study_answer==0)&&(g_study_num<5))
       {
           flow.dwpt_lmrmove(0,update_thread->lmrvalue,g_now_frame);
           flow.wait_lmrstop(0,g_now_frame);
           g_study_num++;
           flow.ccd_dwtype(0,2);//继续对位
       }
       else
       {
           g_study_num = 0;
       }
    }
    pthread_cancel(dwthreadId);
    pthread_exit(NULL);
}

//ccd操作类型选择
void CBaoguangFlow::ccd_dwtype(int ch,int type)
{
    qDebug("ch=%d,ccd_dwtype=%d",ch,type);
    if(ch==0)
    {
        switch(type)
        {
        case 1:
            g_dw_start = true;
            g_study_start = false;
            g_ch_start = false;
            update_thread->is_ccdsuoding = false;
            g_dw_tj = false;
            break;
        case 2:
            g_dw_start = false;
            g_study_start = true;
            g_ch_start = false;
            update_thread->is_ccdsuoding = false;
            g_dw_tj = false;
            break;
        case 3:
            g_dw_start = false;
            g_study_start = false;
            g_ch_start = true;
            update_thread->is_ccdsuoding = false;
            g_dw_tj = false;
            break;
        case 4:
            g_dw_start = false;
            g_study_start = false;
            g_ch_start = false;
            update_thread->is_ccdsuoding = true;
            g_dw_tj = false;
            break;
        case 5:
            g_dw_start = false;
            g_study_start = false;
            g_ch_start = false;
            update_thread->is_ccdsuoding = false;
            g_dw_tj = true;
            break;
        }
        //由定时器发送CCD命令
        //对位
        if(g_dw_start)
        {
            update_thread->is_sendstart = true;
        }
        //CCD学习
        if(g_study_start)
        {
            update_thread->is_sendstudy = true;
        }
        //核对
        if(g_ch_start)
        {
            update_thread->is_sendcheck = true;
        }
        //
        if(g_dw_tj)
        {
            update_thread->is_tjlmr = true;
        }
    }
    else if(ch==1)
    {
        return;
    }
}

void CBaoguangFlow::thread_duiwei()
{
    //对位核对运行线程
    int ret = pthread_create(&dwthreadId, NULL, dwthread,NULL);
    if(ret != 0)
    {
        qDebug()<<"fail to creat dw thread!";
        qDebug("cause:%s\n",strerror(ret));
    }
    else
    {
        pthread_detach(dwthreadId);
    }
}

//CCD移动线程
void *ccdmovethread(void *arg)
{
    qDebug()<<"start ccd move thread!";
    CBaoguangFlow ccdmove;
    Mark_pos marktmp;
    if(g_now_frame==0)
    {
        //CCD上升
       // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,false);
        //CCD定位检测
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
//            qDebug()<<"相机超时未上到位!";
//            pthread_cancel(ccdmovethreadId);
//            pthread_exit(NULL);
//        }
    }
    else if(g_now_frame==1)
    {
        //下框将CCD降下来
       // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.ccd_down,true);
    }
    qDebug()<<"wait for ccd stop";
    ccdmove.wait_ccdstop(false);//CCD停止后再执行下列动作
    if(update_thread->ccdmove_type[0]==0)
    {
        qDebug()<<"ccd move thread:zero!";
        marktmp.x1 = 0.00;
        marktmp.x2 = 0.00;
        marktmp.x3 = 0.00;
        marktmp.x4 = 0.00;
        marktmp.y1 = 0.00;
        marktmp.y2 = 0.00;
        marktmp.y3 = 0.00;
        marktmp.y4 = 0.00;
        ccdmove.ccd_moveposition(false,true,marktmp);
        ccdmove.wait_ccdstop(true);
    }
    else if(update_thread->ccdmove_type[0]==1)
    {
        qDebug()<<"ccd move thread:db!";
        memcpy(&marktmp,&update_thread->ccdvalue,sizeof(Mark_pos));
        ccdmove.ccd_moveposition(true,true,marktmp);
    }
    else if(update_thread->ccdmove_type[0]==2)
    {
        qDebug()<<"ccd move thread:to dw pos!";
//        ccdmove.dwpt_zmove(PT_DWPOSITION,g_now_frame,false);
        ccdmove.ccd_moveposition(true,true,update_thread->m_currentmark);
    }
    pthread_cancel(ccdmovethreadId);
    g_isCCDMoveThread[0] = false;
    pthread_exit(NULL);
}

//下方CCD移动线程
void *downccdmovethread(void *arg)
{
    qDebug()<<"start downccd move thread!";
    CBaoguangFlow ccdmove;
    Mark_pos marktmp;
    ccdmove.wait_downccdstop(false);//CCD停止后再执行下列动作
    if(update_thread->ccdmove_type[1]==0)
    {
        qDebug()<<"downccd move thread:zero!";
        marktmp.x1 = 0.00;
        marktmp.x2 = 0.00;
        marktmp.x3 = 0.00;
        marktmp.x4 = 0.00;
        marktmp.y1 = 0.00;
        marktmp.y2 = 0.00;
        marktmp.y3 = 0.00;
        marktmp.y4 = 0.00;
        ccdmove.downccd_moveposition(true,marktmp);
//        ccdmove.wait_downccdstop(true);
    }
    else if(update_thread->ccdmove_type[1]==1)
    {
        qDebug()<<"downccd move thread:db!";
        memcpy(&marktmp,&update_thread->downccdvalue,sizeof(Mark_pos));
        ccdmove.downccd_moveposition(true,marktmp);
    }
    else if(update_thread->ccdmove_type[1]==2)
    {
        qDebug()<<"downccd move thread:to dw pos!";
        ccdmove.downccd_moveposition(true,update_thread->m_downcurrentmark);
    }
    else if(update_thread->ccdmove_type[1]==3)
    {
        qDebug()<<"downccd move thread:basic!";
        memcpy(&marktmp,&update_thread->m_basicpos,sizeof(Mark_pos));
        ccdmove.downccd_moveposition(true,marktmp);
    }
    pthread_cancel(downccdmovethreadId);
    g_isCCDMoveThread[1] = false;
    pthread_exit(NULL);
}

void CBaoguangFlow::thread_ccdmove(int ch,int type)
{
    if(ch==0)
    {
        update_thread->ccdmove_type[0] = type;
        //CCD移动
        int ret = pthread_create(&ccdmovethreadId, NULL, ccdmovethread, NULL);
        if(ret != 0)
        {
            qDebug()<<"fail to creat ccdmove thread!";
            qDebug("cause:%s\n",strerror(ret));
            g_isCCDMoveThread[0] = false;
        }
        else
        {
            pthread_detach(ccdmovethreadId);
            g_isCCDMoveThread[0] = true;
        }
    }
    else if(ch==1)
    {
        return;
    }
}

//暂停后的动作线程
void *pausethread(void *arg)
{
    qDebug()<<"start pause thread!";
    if(false==g_isPauseStart) g_isPauseStart = true;
    int taim_up = -1;
/*    if(g_now_frame==1)
    {
        taim_up = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.Downlb_air_out,PORT_OUT);
        if(taim_up==1)
        {
            usleep(100000);
            update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
        }
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,false);
    }
    else */if(g_now_frame==0)
    {
       // taim_up = update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.Uplb_air_out,PORT_OUT);
        if(taim_up==1)
        {
            usleep(100000);
          //  update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
        }
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);
    }
    qDebug("Pause:g_isSysRunning=%d,g_isSysContinueRun=%d,g_isPause=%d",g_isSysRunning,g_isSysContinueRun,g_isPause);
    qDebug()<<"exit pause thread!";
    g_isPauseStart = false;
    pthread_cancel(pausethreadId);
    pthread_exit(NULL);
}

void CBaoguangFlow::thread_pause(bool isjb)
{    
    int cnt = 0;
    while(g_isPauseStart &&(cnt<60))
    {
        usleep(50000);
        cnt++;
    }
    if(isjb)
    {
        qDebug()<<"pause:jubao";
        g_isPauseStart = true;
        reject_done();
        g_isPause = true;//暂停
        g_isPauseRun = false;
        g_isPauseStart = false;
    }
    else
    {
        qDebug()<<"pause:stop";
        if(!g_isScreenStart)
        {
/*            if(g_now_frame==1)//下框
            {
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airin,false);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt2_airout,true);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,false);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_air,true);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
            }
            else */if(g_now_frame==0)
            {
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,true);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,true);
//                update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
            }
        }
        g_isPause = true;//暂停
        g_isPauseRun = false;
        g_isPauseStart = true;
        //暂停后的动作
        int ret = pthread_create(&pausethreadId, NULL, pausethread, NULL);
        if(ret != 0)
        {
            qDebug()<<"fail to creat pause thread!";
            qDebug("cause:%s\n",strerror(ret));
            g_isPauseStart = false;
        }
        else
        {
            pthread_detach(pausethreadId);
        }
    }
}
