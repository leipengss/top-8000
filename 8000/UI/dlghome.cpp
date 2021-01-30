#include "dlghome.h"
#include "ui_home.h"
#include<stdio.h>
//#include<QMessageBox>
#include<QDebug>
#include<QTime>

static bool is_homeall = false;
static bool is_allhomed = false;
static bool is_pthomed =false;

DlgHome::DlgHome(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgHome)
{
    ui->setupUi(this);
    //屏蔽最大化 关闭
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags()& ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
    ui->label_hometip->setStyleSheet("color:black");
//    ui->label_hometip->setText(QObject::tr("提示："));
    moorrestardelay = 0;
    //回零线程
    homeall_thread = new HomeThread;
    pthome_thread  = new HomeThread;
    connect(homeall_thread,SIGNAL(send_message(int,QString)),this,SLOT(showtips(int,QString)));
    connect(pthome_thread,SIGNAL(send_message(int,QString)),this,SLOT(showtips(int,QString)));
    //初始化报警停止
    update_thread->threaddriver->m_isWarnStop = false;
}

DlgHome::~DlgHome()
{
    if(homeall_thread->isFinished()==false)
    {
        homeall_thread->exit();
    }
    if(pthome_thread->isFinished()==false)
    {
        pthome_thread->exit();
    }
    delete homeall_thread;
    delete pthome_thread;
    delete ui;
}
void DlgHome::btn_enable(bool flg)
{
    ui->btn_homeall->setEnabled(flg);
    ui->btn_homept->setEnabled(flg);
    ui->btn_home_exit->setEnabled(flg);
}
void DlgHome::ms_sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while((true==update_thread->input.EstopSig)&&(QTime::currentTime() <dieTime))
    {
       QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
//       qDebug()<<"time";
    }
}
void DlgHome::on_btn_homeall_clicked()
{
    if(!update_thread->input.EstopSig)
    {
        showtips(ERROR_TIP,QObject::tr("未释放急停开关!"));
        return;
    }
    is_allhomed = false;
    is_pthomed = false;
    btn_enable(false);
    showtips(NORMAL_TIP,QObject::tr("正在重启伺服，请稍候!"));
    update_thread->threaddriver->CNC_Disable();
    ms_sleep(3);
    qDebug()<<QObject::tr("伺服重启");
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.lmr_power,false);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.wz_power,false);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);//z伺服
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uccd_power,false);
    ms_sleep(4000);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.lmr_power,true);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.wz_power,true);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uccd_power,true);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownVacumm_switch_out,true);

    ms_sleep(4000);
    update_thread->threaddriver->CNC_Enable();
    ms_sleep(1000);
    if(update_thread->m_bCnc_state==1)
    {
        showtips(NORMAL_TIP,QObject::tr("伺服重启成功!"));
        ms_sleep(2000);
        is_homeall = true;
        if(homeall_thread->isFinished()==false)
        {
            homeall_thread->exit();
        }
        homeall_thread->start(QThread::NormalPriority);
        showtips(NORMAL_TIP,QObject::tr("正在回零，请稍后!"));
    }
    else if(update_thread->m_bCnc_state==0)
    {
        showtips(ERROR_TIP,QObject::tr("伺服重启失败或密码到期!"));
    }
}
void HomeThread::homeall()
{
    g_ishoming=false;
    g_ishomed=false;
    update_thread->ccdmove_type[0] = -1;
    update_thread->ccdmove_type[1] = -1;
    update_thread->m_framemovepause = false;
    g_isHomeAllThread = true;
    //
    CCDLMR_POS lmrclear;
    lmrclear.value_l = 0;
    lmrclear.value_m = 0;
    lmrclear.value_r = 0;
    for(int k=0;k<2;k++)
    {
        update_thread->m_uppcbok[k] = lmrclear;
        update_thread->m_downpcbok[k] = lmrclear;
        update_thread->tj_lmrvalue[k] = lmrclear;
    }
    //置回零操作条件
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airin,false);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.jt_airout,false);
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_air,false);


    //框架真空开启则关闭框架真空
    if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT)!=0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
        usleep(100000);
    }
   // update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
 //   update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
    usleep(200000);
    qDebug()<<QObject::tr("关闭离板汽缸");
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);



    //平台LMRZ回零
    //Z回零
    update_thread->threaddriver->Axis_home(AXIS_Z);
    update_thread->threaddriver->Axis_home(AXIS_M);
    update_thread->threaddriver->Axis_home(AXIS_L);
    update_thread->threaddriver->Axis_home(AXIS_R);

    if(false==wait_aux(AXIS_Z))
    {
        qDebug()<<QObject::tr("AXIS_Z回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_Z回零失败!"));
        return;
    }
    qDebug()<<QObject::tr("CCD定位检测");
    if(g_par.is_usescanlight==1)
    {
        //扫描灯回零
        update_thread->threaddriver->Axis_home(AXIS_UPSCAN);
    }
    //相机Y1,Y2先回零,Y7/Y8先回零
    update_thread->threaddriver->Axis_home(AXIS_CCD1Y);
    update_thread->threaddriver->Axis_home(AXIS_CCD2Y);
    if(false==wait_aux(AXIS_CCD1Y))
    {
        qDebug()<<QObject::tr("AXIS_CCD1Y回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD1Y回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD2Y))
    {
        qDebug()<<QObject::tr("AXIS_CCD2Y回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD2Y回零失败!"));
        return;
    }
    update_thread->threaddriver->Axis_home(AXIS_CCD1X);
    update_thread->threaddriver->Axis_home(AXIS_CCD2X);
    update_thread->threaddriver->Axis_home(AXIS_CCD3X);
    update_thread->threaddriver->Axis_home(AXIS_CCD4X);
    update_thread->threaddriver->Axis_home(AXIS_CCD3Y);
    update_thread->threaddriver->Axis_home(AXIS_CCD4Y);


    if(false==wait_aux(AXIS_L))
    {
        qDebug()<<QObject::tr("AXIS_L回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_L回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_M))
    {
        qDebug()<<QObject::tr("AXIS_M回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_M回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_R))
    {
        qDebug()<<QObject::tr("AXIS_R回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_R回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD1X))
    {
        qDebug()<<QObject::tr("AXIS_CCD1X回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD1X回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD2X))
    {
        qDebug()<<QObject::tr("AXIS_CCD2X回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD2X回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD3X))
    {
        qDebug()<<QObject::tr("AXIS_CCD3X回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD3X回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD4X))
    {
        qDebug()<<QObject::tr("AXIS_CCD4X回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD4X回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD3Y))
    {
        qDebug()<<QObject::tr("AXIS_CCD3Y回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD3Y回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_CCD4Y))
    {
        qDebug()<<QObject::tr("AXIS_CCD4Y回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_CCD4Y回零失败!"));
        return;
    }
    //W回零
    update_thread->threaddriver->Axis_home(AXIS_W);
    //检测框架轴回零
    if(false==wait_aux(AXIS_W))
    {
        qDebug()<<QObject::tr("框架回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("框架回零失败!"));
        return;
    }
    qDebug()<<QObject::tr("框架W回零完成");
    g_isHomeAllThread = false;
    update_thread->m_berrorframe = false;
//    int w_safe = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");
//    if(w_safe)  update_thread->m_framemovepause = true;
//    while(update_thread->m_framemovepause &&(1==w_safe))
//    {
//          w_safe = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_w_safe");
//          usleep(50000);
//    }
//    update_thread->m_framemovepause = false;
//    update_thread->threaddriver->Axis_MovePos(AXIS_W,g_par.w_speed,g_par.w_movelimit);
//    int is_wmove=update_thread->threaddriver->Axis_WaitForStop(AXIS_W);
//    usleep(200000);//延时0.2秒待框架停稳
//    if(is_wmove<=0)
//    {
//        if(is_wmove==0)
//        {
//           qDebug()<<QObject::tr("移框超时未停止！");
//           emit send_message(ERROR_TIP,QObject::tr("移框超时未停止！"));
//           return;
//        }
//        if(is_wmove==-1)
//        {
//           qDebug()<<QObject::tr("急停，移框结束！");
//           emit send_message(ERROR_TIP,QObject::tr("急停，回零结束！"));
//           return;
//        }
//    }
//    qDebug()<<QObject::tr("回零后移框完成");
//    if(g_par.is_usescanlight==1)
//    {
//        //检测扫描灯轴回零
//        if(false==wait_aux(AXIS_UPSCAN))
//        {
//            qDebug()<<QObject::tr("灯扫描轴回零失败!");
//            emit send_message(ERROR_TIP,QObject::tr("灯扫描轴回零失败!"));
//            return;
//        }
//    }
//    int upframestop = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//  int  num =0;
//    while((upframestop!=1)&&(num<DELAYCOUNT))
//    {
//        upframestop = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Up_frame_stop");
//        usleep(USLEEPTIME);//0.01s
//        num++;
//    }
//    if(upframestop!=1)
//    {
//        qDebug()<<QObject::tr("框架未正确到达停止位置!");
//        update_thread->m_berrorframe = true;
//        emit send_message(ERROR_TIP,QObject::tr("框架未正确到达停止位置,请检查框架行程!"));
//        return;
//    }
    update_thread->m_berrorframe = false;
//    if(upframestop==1)
//    {
//        //遮光板上升
////        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_open,false);
////        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.zg_close,true);
//        //回零结束
//    }
    g_beeptype =1;
    is_allhomed = true;
    emit send_message(NORMAL_TIP,QObject::tr("复位完成!"));
}

bool HomeThread::wait_aux(int axis)
{
    bool ret=true;
    int val= update_thread->threaddriver->Axis_WaitForHome(axis);
    QString axisname = update_thread->axisname_list.at(axis);
    if(val==0)
    {
        emit send_message(ERROR_TIP,axisname+QObject::tr("轴回零超时结束！"));
        qDebug()<<QObject::tr("回零超时结束！");
        ret = false;
    }
    else if(val==-1)
    {
        emit send_message(ERROR_TIP,QObject::tr("急停，回零结束！"));
        qDebug()<<QObject::tr("急停，回零结束！");
        ret = false;
    }
    else if(val==-2)
    {
        int Ccdy_isok = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Ccd_isok");
        QString errstr;
        errstr.clear();
        if(Ccdy_isok==1)
        {
            errstr = "CCD";
        }
        emit send_message(ERROR_TIP,errstr+QObject::tr("驱动器异常，回零结束！"));
        qDebug()<<QObject::tr("驱动器异常，回零结束！");
        ret = false;
    }
//    else if(val==-3)
//    {
//        emit send_message(ERROR_TIP,QObject::tr("安全光闸报警急停，回零结束！"));
//        qDebug()<<QObject::tr("安全光闸报警急停，回零结束！");
//        ret = false;
//    }
    return ret;
}

void HomeThread::homept()
{
    update_thread->ccdmove_type[0] = -1;
    update_thread->ccdmove_type[1] = -1;
    //置回零操作条件
    //框架真空开启则关闭框架真空
    if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_absorb,PORT_OUT)!=0)
    {
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_absorb,false);
        usleep(100000);
    }
//    if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame2_absorb,PORT_OUT)!=0)
//    {
//        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_absorb,false);
//        usleep(100000);
//    }
    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Uplb_air_out,false);
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.Downlb_air_out,false);
    usleep(200000);
    qDebug()<<QObject::tr("打开离板汽缸");
    // kai锁销
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_lock,true);
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame2_lock,true);
//    qDebug()<<QObject::tr("关闭锁销");
//    update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);
//    qDebug()<<QObject::tr("关闭电磁铁");
    usleep(100000);
    //Z回零
    update_thread->threaddriver->Axis_home(AXIS_Z);
    if(false==wait_aux(AXIS_Z))
    {
        qDebug()<<QObject::tr("AXIS_Z回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_Z回零失败!"));
        return;
    }
    update_thread->threaddriver->Axis_home(AXIS_M);
    update_thread->threaddriver->Axis_home(AXIS_L);
    update_thread->threaddriver->Axis_home(AXIS_R);
    if(false==wait_aux(AXIS_M))
    {
        qDebug()<<QObject::tr("AXIS_M回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_M回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_R))
    {
        qDebug()<<QObject::tr("AXIS_R回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_R回零失败!"));
        return;
    }
    if(false==wait_aux(AXIS_L))
    {
        qDebug()<<QObject::tr("AXIS_L回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("AXIS_L回零失败!"));
        return;
    }
    //W回零
    update_thread->threaddriver->Axis_home(AXIS_W);
    //检测框架轴回零
    if(false==wait_aux(AXIS_W))
    {
        qDebug()<<QObject::tr("框架回零失败!");
//        emit send_message(ERROR_TIP,QObject::tr("框架回零失败!"));
        return;
    }
    qDebug()<<QObject::tr("框架W回零完成");
    is_pthomed =true;
    emit send_message(NORMAL_TIP,QObject::tr("平台复位完成!"));
}
void HomeThread::stop()
{
    m_bStopped = false;
}
void HomeThread::run()
{
    g_isSysRunning = false;
    g_isPauseRun = false;
    g_isPauseStart = false;
    g_isBGwhileStart = false;
    g_isBgRunning = false;
    g_isDownPTDuiWei = false;
    g_isSysContinueRun = false;
    g_isCCDMoveThread[0] = false;
    g_isCCDMoveThread[1] = false;
    g_framechange = false;
    g_framemannual = false;
    if(is_homeall)
    {
       homeall();
    }
    else
    {
       homept();
    }
    exit(0);
}
void DlgHome::on_btn_homept_clicked()
{
    if(!update_thread->input.EstopSig)
    {
        showtips(ERROR_TIP,QObject::tr("未释放急停开关!"));
        return;
    }
    is_allhomed = false;
    is_pthomed = false;
    btn_enable(false);
    if(false==g_ishomed)
    {
        showtips(NORMAL_TIP,QObject::tr("正在重启伺服，请稍候!"));
        update_thread->threaddriver->CNC_Disable();
        ms_sleep(3);
        qDebug()<<QObject::tr("伺服重启");
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.lmr_power,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.wz_power,false);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,false);//z
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uccd_power,false);
        ms_sleep(4000);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.lmr_power,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.wz_power,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.frame_ct_out,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.uccd_power,true);
        update_thread->threaddriver->Ctr_Output(update_thread->threaddriver->m_output.DownVacumm_switch_out,true);

        ms_sleep(4000);
        if(update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.lmr_power,PORT_OUT)==false
        ||update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.wz_power,PORT_OUT)==false
        ||update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.frame_ct_out,PORT_OUT)==false
        ||update_thread->threaddriver->Get_PortVal(update_thread->threaddriver->m_output.uccd_power,PORT_OUT)==false)
        {
    //        QMessageBox::information(NULL, QString("TOP"), QObject::tr("请检查CCD及框架驱动器供电，操作失败!"));
            showtips(ERROR_TIP,QObject::tr("请检查CCD及框架驱动器供电，操作失败!"));
            return;
        }
    }
    update_thread->threaddriver->CNC_Enable();
    ms_sleep(1000);
    if(update_thread->m_bCnc_state==1)
    {
       showtips(NORMAL_TIP,QObject::tr("平台伺服重启成功!"));
       ms_sleep(2000);
       is_homeall = false;
       if(pthome_thread->isFinished()==false)
       {
           pthome_thread->exit();
       }
       pthome_thread->start(QThread::NormalPriority);
       showtips(NORMAL_TIP,QObject::tr("正在回零，请稍后!"));
    }
    else if(update_thread->m_bCnc_state==0)
    {
        showtips(ERROR_TIP,QObject::tr("平台伺服重启失败或密码到期!"));
    }
}

void DlgHome::on_btn_home_exit_clicked()
{
    update_thread->threaddriver->m_isWarnStop = false;
    if(homeall_thread->isRunning())
    {
        homeall_thread->stop();
        homeall_thread->quit();
        homeall_thread->wait();
    }
    if(pthome_thread->isRunning())
    {
        pthome_thread->stop();
        pthome_thread->quit();
        pthome_thread->wait();
    }
    usleep(5000);
    this->close();
}
void DlgHome::showtips(int tipstype, QString str_data)
{
    QString str_tip = QObject::tr("提示：");
    switch(tipstype){
    case NULL_TIP://null
        str_data = "";
        ui->label_hometip->setStyleSheet("color:black");
        ui->label_hometip->setText(str_data);
        break;
    case NORMAL_TIP://normal
        ui->label_hometip->setStyleSheet("color:black");
        ui->label_hometip->setText(str_tip+str_data);
        update_thread->logwrite(tipstype,str_data);
        break;
    case WARN_TIP://warn yellow
        ui->label_hometip->setStyleSheet("color:yellow");
        ui->label_hometip->setText(str_tip+str_data);
        update_thread->logwrite(tipstype,str_data);
        break;
    case ERROR_TIP://error red
        ui->label_hometip->setStyleSheet("color:red");
        ui->label_hometip->setText(str_tip+str_data);
        update_thread->logwrite(tipstype,str_data);
        btn_enable(true);//报错退出时按钮有效
        g_ishoming=false;
        g_ishomed=false;
        break;
    }

    //执行完按钮有效
    if(is_pthomed||is_allhomed)
    {
        btn_enable(true);
        //初始化错误信息
        g_light_errorcnt = 0;
        //执行完后正常退出线程
        this->disconnect();
        if(is_allhomed)
        {
            if(homeall_thread->isRunning())
            {
                homeall_thread->stop();
                homeall_thread->quit();
                homeall_thread->wait();
            }
        }
        if(is_pthomed)
        {
            if(pthome_thread->isRunning())
            {
                pthome_thread->stop();
                pthome_thread->quit();
                pthome_thread->wait();
            }
        }
        is_allhomed = false;
        is_pthomed = false;
        update_thread->threaddriver->m_isWarnStop = false;
        ms_sleep(2000);//延时2s
        this->close();
    }
}
