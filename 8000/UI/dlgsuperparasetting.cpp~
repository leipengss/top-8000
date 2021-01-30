#include "dlgsuperparasetting.h"
#include "ui_dlgsuperparasetting.h"
#include"filemanage/cinirw.h"
#include "driver/global.h"
#include<math.h>
#include<QMessageBox>
#include<QDebug>

DlgSuperParaSetting::DlgSuperParaSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSuperParaSetting)
{
    ui->setupUi(this);
    //
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    //
    input = new DlgInputkey(this);
    input->setParent(ui->frame_key);
    // 把子窗口的信号连接到父窗口来。
    connect(input, SIGNAL(okEvent()),this, SLOT(on_superparainput_okEvent()));
    connect(input, SIGNAL(cancelEvent()),this, SLOT(on_superparainput_cancelEvent()));
    //
    ui->radioButton_upframe->setChecked(true);
    ui->stackedWidget_superpara->setCurrentIndex(0);
    ui->btn_light_clear->hide();
    ui->checkBox_lightcnt->hide();
    ui->checkBox_lighttime->hide();
    ui->frame_btn2->hide();
    //init member
    m_bCancelTwo = false;
    m_bCancelTwoback = false;
    m_bOpenSuperman = false;
    m_bOpenSupermanback = false;
    m_bCancelThree = false;
    m_bCancelThreeback = false;
    m_iLightCnt = 0;
    m_iLightTime = 0;
    m_iLightCntback = 0;
    m_iLightTimeback= 0;
    //
    if(g_language>1)
    {
        ui->label_up_opentimelimit->setStyleSheet("font-size:10px;");
        ui->label_up_vacairtime->setStyleSheet("font-size:10px;");
        ui->label_up_jtairouttime->setStyleSheet("font-size:10px;");
        ui->label_up_tbmaxtime->setStyleSheet("font-size:10px;");
        ui->label_up_closetimelimit->setStyleSheet("font-size:10px;");
        ui->label_up_openairtime->setStyleSheet("font-size:10px;");
        ui->label_up_jtairindelaytime->setStyleSheet("font-size:10px;");
        ui->label_up_tbdelaytime->setStyleSheet("font-size:10px;");
        //
        ui->label_down_opentimelimit->setStyleSheet("font-size:10px;");
        ui->label_down_vacairtime->setStyleSheet("font-size:10px;");
        ui->label_down_jtairouttime->setStyleSheet("font-size:10px;");
        ui->label_down_tbmaxtime->setStyleSheet("font-size:10px;");
        ui->label_down_closetimelimit->setStyleSheet("font-size:10px;");
        ui->label_down_openairtime->setStyleSheet("font-size:10px;");
        ui->label_down_jtairindelaytime->setStyleSheet("font-size:10px;");
        ui->label_down_tbdelaytime->setStyleSheet("font-size:10px;");
        //
        ui->label_light_promisetime->setStyleSheet("font-size:10px;");
        ui->label_light_cooltime->setStyleSheet("font-size:10px;");
        ui->label_light_energystabletime->setStyleSheet("font-size:10px;");
        ui->label_light_windoutdelay->setStyleSheet("font-size:10px;");
        ui->label_light_usetimelimit->setStyleSheet("font-size:10px;");
        ui->label_light_fulllimittime->setStyleSheet("font-size:10px;");
        //
        ui->label_light_triglelimittime->setStyleSheet("font-size:10px;");
        ui->label_light_windindelay->setStyleSheet("font-size:10px;");
        ui->label_dwlock_ptx->setStyleSheet("font-size:10px;");
        ui->label_dwlock_pty->setStyleSheet("font-size:10px;");
        ui->label_dwlock_pta->setStyleSheet("font-size:10px;");
        ui->btn_dwlock_cw->setStyleSheet("font-size:10px;");
        ui->btn_dwlock_ccw->setStyleSheet("font-size:10px;");
        ui->label_up_lbposition->setStyleSheet("font-size:10px;");
        ui->label_down_lbposition->setStyleSheet("font-size:10px;");
        ui->label_up_zmax->setStyleSheet("font-size:10px;");
        ui->label_down_zmax->setStyleSheet("font-size:10px;");
        ui->label_ymax->setStyleSheet("font-size:10px;");
        ui->label_ymin->setStyleSheet("font-size:10px;");
        ui->label_wlength->setStyleSheet("font-size:10px;");
    }
    //init paramerter and show them
    SetTextInputRange();
    InitParameter();
}

DlgSuperParaSetting::~DlgSuperParaSetting()
{
    delete input;
    para.clear();
    paraback.clear();
    delete ui;
}

void DlgSuperParaSetting::SetTextInputRange()
{
    //上框框架设置
    ui->lineEdit_up_opentimelimit->setValidator(new QIntValidator(0,5000,this));//上框开启限时
    //ui->lineEdit_up_opentimelimit->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_up_closetimelimit->setValidator(new QIntValidator(0,5000,this));//上框关闭限时
    //ui->lineEdit_up_closetimelimit->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_up_vacairtime->setValidator(new QIntValidator(0,5000,this));//上框真空后吹气时长
    //ui->lineEdit_up_vacairtime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_up_openairtime->setValidator(new QIntValidator(0,5000,this));//上框开框前吹气时长
    //ui->lineEdit_up_openairtime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_up_jtairouttime->setValidator(new QIntValidator(0,5000,this));//上框胶条排气时长
    //ui->lineEdit_up_jtairouttime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_up_jtairindelaytime->setValidator(new QIntValidator(0,5000,this));//上框胶条充气延迟时间
    //ui->lineEdit_up_jtairindelaytime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_up_tbmaxtime->setValidator(new QIntValidator(0,10000,this));//上框踢板最大时间
    //ui->lineEdit_up_tbmaxtime->setToolTip("建议范围:[0,10000]");
    ui->lineEdit_up_tbdelaytime->setValidator(new QIntValidator(500,5000,this));//上框延时踢板时间
    //ui->lineEdit_up_tbdelaytime->setToolTip("建议范围:[500,5000]");
    //下框框架设置
    ui->lineEdit_down_opentimelimit->setValidator(new QIntValidator(0,5000,this));//下框开启限时
    //ui->lineEdit_down_opentimelimit->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_down_closetimelimit->setValidator(new QIntValidator(0,5000,this));//下框关闭限时
    //ui->lineEdit_down_closetimelimit->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_down_vacairtime->setValidator(new QIntValidator(0,5000,this));//下框真空后吹气时长
    //ui->lineEdit_down_vacairtime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_down_openairtime->setValidator(new QIntValidator(0,5000,this));//下框开框前吹气时长
    //ui->lineEdit_down_openairtime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_down_jtairouttime->setValidator(new QIntValidator(0,5000,this));//下框胶条排气时长
    //ui->lineEdit_down_jtairouttime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_down_jtairindelaytime->setValidator(new QIntValidator(0,5000,this));//下框胶条充气延迟时间
    //ui->lineEdit_down_jtairindelaytime->setToolTip("建议范围:[0,5000]");
    ui->lineEdit_down_tbmaxtime->setValidator(new QIntValidator(0,10000,this));//下框踢板最大时间
    //ui->lineEdit_down_tbmaxtime->setToolTip("建议范围:[0,10000]");
    ui->lineEdit_down_tbdelaytime->setValidator(new QIntValidator(500,5000,this));//下框延时踢板时间
    //ui->lineEdit_down_tbdelaytime->setToolTip("建议范围:[500,5000]");
    //灯源设置
    ui->lineEdit_light_promisetime->setValidator(new QIntValidator(0,9999,this));//灯光保证时长
    //ui->lineEdit_light_promisetime->setToolTip("建议范围:[0,9999]");
    ui->lineEdit_light_usetimelimit->setValidator(new QIntValidator(0,9999,this));//灯光使用时间上限
    //ui->lineEdit_light_usetimelimit->setToolTip("建议范围:[0,9999]");
    ui->lineEdit_light_cooltime->setValidator(new QIntValidator(0,600000,this));//灯管冷却时长
    //ui->lineEdit_light_cooltime->setToolTip("建议范围:[0,600000]");
    ui->lineEdit_light_fulllimittime->setValidator(new QIntValidator(0,180000,this));//全功限时
    //ui->lineEdit_light_fulllimittime->setToolTip("建议范围:[0,180000]");
    ui->lineEdit_light_energystabletime->setValidator(new QIntValidator(0,600000,this));//能量稳定时间
    //ui->lineEdit_light_energystabletime->setToolTip("建议范围:[0,600000]");
    ui->lineEdit_light_triglelimittime->setValidator(new QIntValidator(0,5000,this));//点灯触发时间
    //ui->lineEdit_light_triglelimittime->setToolTip("建议范围:[0,3000]");
    ui->lineEdit_light_windoutdelay->setValidator(new QIntValidator(0,600000,this));//灯光排风延时启动
    //ui->lineEdit_light_windoutdelay->setToolTip("建议范围:[0,600000]");
    ui->lineEdit_light_windindelay->setValidator(new QIntValidator(0,600000,this));//灯光送风延时启动
    //ui->lineEdit_light_windindelay->setToolTip("建议范围:[0,600000]");
    //对位销校正
    ui->lineEdit_dwlock_ptx->setValidator(new QIntValidator(0,10000,this));//平台L偏移
    //ui->lineEdit_dwlock_ptx->setToolTip("建议范围:[0,10000]");
    ui->lineEdit_dwlock_pty->setValidator(new QIntValidator(0,10000,this));//平台M偏移
    //ui->lineEdit_dwlock_pty->setToolTip("建议范围:[0,10000]");
    ui->lineEdit_dwlock_pta->setValidator(new QIntValidator(0,10000,this));//平台R偏移
    //ui->lineEdit_dwlock_pta->setToolTip("建议范围:[0,10000]");
    //其他参数
    ui->lineEdit_up_lbposition->setValidator(new QIntValidator(0,200000,this));//上框离板位置
    //ui->lineEdit_up_lbposition->setToolTip("建议范围:[0,200000]");
    ui->lineEdit_down_lbposition->setValidator(new QIntValidator(0,200000,this));//下框离板位置
    //ui->lineEdit_down_lbposition->setToolTip("建议范围:[0,200000]");
    ui->lineEdit_up_zmax->setValidator(new QIntValidator(0,200000,this));//上框Z轴最高行程
    //ui->lineEdit_up_zmax->setToolTip("建议范围:[0,200000]");
    ui->lineEdit_down_zmax->setValidator(new QIntValidator(0,200000,this));//下框Z轴最高行程
    //ui->lineEdit_down_zmax->setToolTip("建议范围:[0,200000]");
    ui->lineEdit_ymax->setValidator(new QIntValidator(0,500000,this));//CCD<Y>最大间距
    //ui->lineEdit_ymax->setToolTip("建议范围:[0,500000]");
    ui->lineEdit_ymin->setValidator(new QIntValidator(0,500000,this));//CCD<Y>最小间距
    //ui->lineEdit_ymin->setToolTip("建议范围:[0,500000]");
    ui->lineEdit_wlength->setValidator(new QIntValidator(0,1000000000,this));//框架行程
    //ui->lineEdit_wlength->setToolTip("建议范围:[0,1000000000]");
}

void DlgSuperParaSetting::InitParameter()
{
    QString strlight;
    QString tmp;
    float val = 0.000;
    CIniRW inifile(TOPPARAMETERPATH);
    m_bCancelTwo = inifile.ReadIni("TOP","CANCEL_TWO").toInt();
    m_bCancelTwoback = m_bCancelTwo;
    ui->checkBox_canceltwo->setChecked(m_bCancelTwo);
    m_bCancelThree = inifile.ReadIni("TOP","CANCEL_THREE").toInt();
    m_bCancelThreeback = m_bCancelThree;
    ui->checkBox_cancelthree->setChecked(m_bCancelThree);
    m_bOpenSuperman = inifile.ReadIni("TOP","OpenSuperman").toInt();
    m_bOpenSupermanback = m_bOpenSuperman;
    ui->checkBox_issuperman->setChecked(m_bOpenSuperman);
    m_iLightCnt = inifile.ReadIni("TOP","LIGHT_NUM").toInt();
    m_iLightCntback = m_iLightCnt;
    strlight.sprintf("%d",m_iLightCnt);
    ui->label_lightnum->setText(strlight);
    m_iLightTime = inifile.ReadIni("TOP","LIGHT_TIME").toFloat();
    m_iLightTimeback = m_iLightTime;
    strlight.sprintf("%d:%d",(int)m_iLightTime/60,(int)m_iLightTime%60);
    ui->label_lighttime->setText(strlight);
    //up frame
    tmp = inifile.ReadIni("TRAJ","OPEN_DELAY").toString();
    ui->lineEdit_up_opentimelimit->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","CLOSE_TIME").toString();
    ui->lineEdit_up_closetimelimit->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","AIR_FLOW_TIME").toString();
    ui->lineEdit_up_vacairtime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","OPEN_FRAME_FLOW_TIME").toString();
    ui->lineEdit_up_openairtime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","JIAOTIAO_PAIQI_TIME").toString();
    ui->lineEdit_up_jtairouttime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","JIAOTIAO_CHONGQI_DELAY_TIME").toString();
    ui->lineEdit_up_jtairindelaytime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","TB_MAX_TIME").toString();
    ui->lineEdit_up_tbmaxtime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","TB_DELAY").toString();
    ui->lineEdit_up_tbdelaytime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    //down frame
    tmp = inifile.ReadIni("TRAJ","OPEN_DELAY2").toString();
    ui->lineEdit_down_opentimelimit->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","CLOSE_TIME2").toString();
    ui->lineEdit_down_closetimelimit->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","AIR_FLOW_TIME2").toString();
    ui->lineEdit_down_vacairtime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","OPEN_FRAME_FLOW_TIME2").toString();
    ui->lineEdit_down_openairtime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","JIAOTIAO_PAIQI_TIME2").toString();
    ui->lineEdit_down_jtairouttime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","JIAOTIAO_CHONGQI_DELAY_TIME2").toString();
    ui->lineEdit_down_jtairindelaytime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","TB_MAX_TIME2").toString();
    ui->lineEdit_down_tbmaxtime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","TB_DELAY2").toString();
    ui->lineEdit_down_tbdelaytime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    //light setting
    tmp = inifile.ReadIni("TRAJ","LIGHT_PROMISE_TIME").toString();
    ui->lineEdit_light_promisetime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","LIGHT_USAGE_TIME_MAX").toString();
    ui->lineEdit_light_usetimelimit->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","LIGHT_COLD_TIME").toString();
    ui->lineEdit_light_cooltime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","ENERGY_TIME_LIMIT").toString();
    ui->lineEdit_light_fulllimittime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","ENERGY_STEADY_TIME").toString();
    ui->lineEdit_light_energystabletime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","LIGHT_TRIGGER_DELAY").toString();
    ui->lineEdit_light_triglelimittime->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","LIGHT_PAIFENG_DELAY_WORK").toString();
    ui->lineEdit_light_windoutdelay->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","LIGHT_SONGFENG_DELAY_WORK").toString();
    ui->lineEdit_light_windindelay->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    //dw lock
    tmp = inifile.ReadIni("TRAJ","PLATFORM_X_PIANYI").toString();
    ui->lineEdit_dwlock_ptx->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","PLATFORM_Y_PIANYI").toString();
    ui->lineEdit_dwlock_pty->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","PLATFORM_A_PIANYI").toString();
    ui->lineEdit_dwlock_pta->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    //other parameter
    tmp = inifile.ReadIni("TRAJ","LB_DISTANCE").toString();
    if(tmp.toFloat()<0.000)
    {
        val = fabs(tmp.toFloat());
        tmp.sprintf("%.1f",val*1000.0);
    }
    else
    {
        val = tmp.toFloat();
        tmp.sprintf("%.1f",val*1000.0);
    }
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_up_lbposition->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","LB_DISTANCE2").toString();
    if(tmp.toFloat()<0.000)
    {
        val = fabs(tmp.toFloat());
        tmp.sprintf("%.1f",val*1000.0);
    }
    else
    {
        val = tmp.toFloat();
        tmp.sprintf("%.1f",val*1000.0);
    }
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_down_lbposition->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","ZMIN_DISTANCE").toString();
    if(tmp.toFloat()<0.000)
    {
        val = fabs(tmp.toFloat());
        tmp.sprintf("%.1f",val*1000.0);
    }
    else
    {
        val = tmp.toFloat();
        tmp.sprintf("%.1f",val*1000.0);
    }
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_up_zmax->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TRAJ","ZMIN_DISTANCE2").toString();
    if(tmp.toFloat()<0.000)
    {
        val = fabs(tmp.toFloat());
        tmp.sprintf("%.1f",val*1000.0);
    }
    else
    {
        val = tmp.toFloat();
        tmp.sprintf("%.1f",val*1000.0);
    }
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_down_zmax->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","CCD_Y_MAX").toString();
    val = tmp.toFloat();
    tmp.sprintf("%.1f",val*1000.0);
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_ymax->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","CCD_Y_MIN").toString();
    val = tmp.toFloat();
    tmp.sprintf("%.1f",val*1000.0);
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_ymin->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    tmp = inifile.ReadIni("TOP","W_LENGTH").toString();
    if(tmp.toFloat()<0.000)
    {
        val = fabs(tmp.toFloat());
        tmp.sprintf("%.1f",val*1000.0);
    }
    else
    {
        val = tmp.toFloat();
        tmp.sprintf("%.1f",val*1000.0);
    }
    tmp = tmp.mid(0,tmp.length()-2);
    ui->lineEdit_wlength->setText(tmp);
    para.append(tmp);
    //paraback.append(tmp);
    //将para赋值给paraback,更新备份参数
    QVector<QString> strtmp(para);
    paraback.swap(strtmp);
    //
    if(1==g_par.is_dpvacuum)
    {
        ui->checkBox_isdpload->setChecked(true);
    }
    else if(0==g_par.is_dpvacuum)
    {
        ui->checkBox_isdpload->setChecked(false);
    }
}

bool DlgSuperParaSetting::IsParaChanged()
{
   bool ischange=false;
   int num = para.size();
//   qDebug()<<QString::number(num,10);
   if(num ==34)
   {
     para[0] = ui->lineEdit_up_opentimelimit->text();
     para[1] = ui->lineEdit_up_closetimelimit->text();
     para[2] = ui->lineEdit_up_vacairtime->text();
     para[3] = ui->lineEdit_up_openairtime->text();
     para[4] = ui->lineEdit_up_jtairouttime->text();
     para[5] = ui->lineEdit_up_jtairindelaytime->text();
     para[6] = ui->lineEdit_up_tbmaxtime->text();
     para[7] = ui->lineEdit_up_tbdelaytime->text();
     //
     para[8] = ui->lineEdit_down_opentimelimit->text();
     para[9] = ui->lineEdit_down_closetimelimit->text();
     para[10] = ui->lineEdit_down_vacairtime->text();
     para[11] = ui->lineEdit_down_openairtime->text();
     para[12] = ui->lineEdit_down_jtairouttime->text();
     para[13] = ui->lineEdit_down_jtairindelaytime->text();
     para[14] = ui->lineEdit_down_tbmaxtime->text();
     para[15] = ui->lineEdit_down_tbdelaytime->text();
     //light setting
     para[16] = ui->lineEdit_light_promisetime->text();
     para[17] = ui->lineEdit_light_usetimelimit->text();
     para[18] = ui->lineEdit_light_cooltime->text();
     para[19] = ui->lineEdit_light_fulllimittime->text();
     para[20] = ui->lineEdit_light_energystabletime->text();
     para[21] = ui->lineEdit_light_triglelimittime->text();
     para[22] = ui->lineEdit_light_windoutdelay->text();
     para[23] = ui->lineEdit_light_windindelay->text();
     //dw lock
     para[24] = ui->lineEdit_dwlock_ptx->text();
     para[25] = ui->lineEdit_dwlock_pty->text();
     para[26] = ui->lineEdit_dwlock_pta->text();
     //other parameter
     para[27] = ui->lineEdit_up_lbposition->text();
     para[28] = ui->lineEdit_down_lbposition->text();
     para[29] = ui->lineEdit_up_zmax->text();
     para[30] = ui->lineEdit_down_zmax->text();
     para[31] = ui->lineEdit_ymax->text();
     para[32] = ui->lineEdit_ymin->text();
     para[33] = ui->lineEdit_wlength->text();
    }
    for(int i=0;i<num;i++)
    {
        if(para.at(i).compare(paraback.at(i)))
        {
            ischange = true;
            qDebug()<<QString::number(i,10);
            qDebug()<<para[i];
            qDebug()<<paraback[i];
        }
    }
    if((m_bCancelTwo!=m_bCancelTwoback)||(m_bCancelThree!=m_bCancelThreeback)
            ||(m_iLightCnt!=m_iLightCntback)||(m_iLightTime!=m_iLightTimeback)||(m_bOpenSuperman!=m_bOpenSupermanback))
    {
        ischange = true;
        qDebug()<<"box";
    }
    return ischange;
}

void DlgSuperParaSetting::SavePara()
{
    CIniRW inifile(TOPPARAMETERPATH);
    QString tr_val;
    tr_val =QString::number(m_bCancelTwo,10);
    inifile.WriteIni("TOP","CANCEL_TWO",tr_val);

    tr_val =QString::number(m_bCancelThree,10);
    inifile.WriteIni("TOP","CANCEL_THREE",tr_val);

    tr_val =QString::number(m_bOpenSuperman,10);
    inifile.WriteIni("TOP","OpenSuperman",tr_val);
    g_par.isopensuperpar = m_bOpenSuperman;

    g_par.light_num = m_iLightCnt;
    tr_val =QString::number(m_iLightCnt,10);
    inifile.WriteIni("TOP","LIGHT_NUM",tr_val);

    g_par.light_time = m_iLightTime;
    tr_val.sprintf("%f",g_par.light_time);
    inifile.WriteIni("TOP","LIGHT_TIME",tr_val);

    inifile.WriteIni("TRAJ","OPEN_DELAY",para[0]);
    //ui->lineEdit_up_opentimelimit->setText(tmp);
    g_par.frame_open_delay = para[0].toInt();

    inifile.WriteIni("TRAJ","CLOSE_TIME",para[1]);
    //ui->lineEdit_up_closetimelimit->setText(tmp);
    g_par.frame_close_delay = para[1].toInt();

    inifile.WriteIni("TRAJ","AIR_FLOW_TIME",para[2]);
    //ui->lineEdit_up_vacairtime->setText(tmp);
    g_par.Frame_close_airdelay = para[2].toInt();

    inifile.WriteIni("TRAJ","OPEN_FRAME_FLOW_TIME",para[3]);
    //ui->lineEdit_up_openairtime->setText(tmp);
    g_par.frame_open_flowdelay = para[3].toInt();

    inifile.WriteIni("TRAJ","JIAOTIAO_PAIQI_TIME",para[4]);
    g_par.JT_airout_delay = para[4].toInt()*1000;
    //ui->lineEdit_up_jtairouttime->setText(tmp);

    inifile.WriteIni("TRAJ","JIAOTIAO_CHONGQI_DELAY_TIME",para[5]);
    g_par.JT_airin_delay = para[5].toInt()*1000;
    //ui->lineEdit_up_jtairindelaytime->setText(tmp);

    inifile.WriteIni("TOP","TB_MAX_TIME",para[6]);
    //ui->lineEdit_up_tbmaxtime->setText(tmp);
    g_par.TB_delay = para[7].toInt()*1000;
    inifile.WriteIni("TOP","TB_DELAY",para[7]);
    //ui->lineEdit_up_tbdelaytime->setText(tmp);

    //down frame
    inifile.WriteIni("TRAJ","OPEN_DELAY2",para[8]);
    //ui->lineEdit_down_opentimelimit->setText(tmp);
    g_par.frame2_open_delay = para[8].toInt();

    inifile.WriteIni("TRAJ","CLOSE_TIME2",para[9]);
    //ui->lineEdit_down_closetimelimit->setText(tmp);
    g_par.frame2_close_delay = para[9].toInt();

    inifile.WriteIni("TRAJ","AIR_FLOW_TIME2",para[10]);
    //ui->lineEdit_down_vacairtime->setText(tmp);
    g_par.Frame_close_airdelay2 = para[10].toInt();

    inifile.WriteIni("TRAJ","OPEN_FRAME_FLOW_TIME2",para[11]);
    //ui->lineEdit_down_openairtime->setText(tmp);
    g_par.frame2_open_flowdelay = para[11].toInt();

    inifile.WriteIni("TRAJ","JIAOTIAO_PAIQI_TIME2",para[12]);
    g_par.JT2_airout_delay = para[12].toInt()*1000;
    //ui->lineEdit_down_jtairouttime->setText(tmp);

    inifile.WriteIni("TRAJ","JIAOTIAO_CHONGQI_DELAY_TIME2",para[13]);
    g_par.JT2_airin_delay = para[13].toInt()*1000;
    //ui->lineEdit_down_jtairindelaytime->setText(tmp);

    inifile.WriteIni("TOP","TB_MAX_TIME2",para[14]);
    //ui->lineEdit_down_tbmaxtime->setText(tmp);

    g_par.TB2_delay = para[15].toInt()*1000;
    inifile.WriteIni("TOP","TB_DELAY2",para[15]);
    //ui->lineEdit_down_tbdelaytime->setText(tmp);

    //light setting
    inifile.WriteIni("TRAJ","LIGHT_PROMISE_TIME",para[16]);
    //ui->lineEdit_light_promisetime->setText(tmp);

    inifile.WriteIni("TRAJ","LIGHT_USAGE_TIME_MAX",para[17]);
    //ui->lineEdit_light_usetimelimit->setText(tmp);

    inifile.WriteIni("TRAJ","LIGHT_COLD_TIME",para[18]);
    //ui->lineEdit_light_cooltime->setText(tmp);

    inifile.WriteIni("TRAJ","ENERGY_TIME_LIMIT",para[19]);
    //ui->lineEdit_light_fulllimittime->setText(tmp);
    g_par.quick_door_time = para[19].toInt();

    inifile.WriteIni("TRAJ","ENERGY_STEADY_TIME",para[20]);
    //ui->lineEdit_light_energystabletime->setText(tmp);

    inifile.WriteIni("TRAJ","LIGHT_TRIGGER_DELAY",para[21]);
    //ui->lineEdit_light_triglelimittime->setText(tmp);
    g_par.min_bgtime = para[21].toInt();

    inifile.WriteIni("TRAJ","LIGHT_PAIFENG_DELAY_WORK",para[22]);
    //ui->lineEdit_light_windoutdelay->setText(tmp);

    inifile.WriteIni("TRAJ","LIGHT_SONGFENG_DELAY_WORK",para[23]);
    //ui->lineEdit_light_windindelay->setText(tmp);

    //dw lock
    inifile.WriteIni("TRAJ","PLATFORM_X_PIANYI",para[24]);
    //ui->lineEdit_dwlock_ptx->setText(tmp);

    inifile.WriteIni("TRAJ","PLATFORM_Y_PIANYI",para[25]);
    //ui->lineEdit_dwlock_pty->setText(tmp);

    inifile.WriteIni("TRAJ","PLATFORM_A_PIANYI",para[26]);
    //ui->lineEdit_dwlock_pta->setText(tmp);

    //other parameter
    float writeval=0.000;
    QString str;
    writeval = para[27].toFloat()/1000.0;
    str.sprintf("%.3f",writeval);
    inifile.WriteIni("TRAJ","LB_DISTANCE",str);
    g_par.up_lb_height =(float)writeval;
    //ui->lineEdit_up_lbposition->setText(tmp);

    writeval = para[28].toFloat()/1000.0;
    str.sprintf("%.3f",writeval);
    inifile.WriteIni("TRAJ","LB_DISTANCE2",str);
    g_par.down_lb_height =(float)writeval;
    //ui->lineEdit_down_lbposition->setText(tmp);

    writeval = para[29].toFloat()/1000.0;
    str.sprintf("%.3f",writeval);
    inifile.WriteIni("TRAJ","ZMIN_DISTANCE",str);
    g_par.up_max_value =(float)writeval;
    //ui->lineEdit_up_zmax->setText(tmp);

    writeval = para[30].toFloat()/1000.0;
    str.sprintf("%.3f",writeval);
    inifile.WriteIni("TRAJ","ZMIN_DISTANCE2",str);
    g_par.down_max_value =(float)writeval;
    //ui->lineEdit_down_zmax->setText(tmp);

    writeval = para[31].toFloat()/1000.0;
    str.sprintf("%.3f",writeval);
    inifile.WriteIni("TOP","CCD_Y_MAX",str);
    g_par.ccdy_maxdis = (float)writeval;
    //ui->lineEdit_ymax->setText(tmp);

    writeval = para[32].toFloat()/1000.0;
    str.sprintf("%.3f",writeval);
    inifile.WriteIni("TOP","CCD_Y_MIN",str);
    g_par.ccdy_mindis = (float)writeval;
    //ui->lineEdit_ymin->setText(tmp);

    writeval = para[33].toFloat()/1000.0;
    qDebug()<<"33333..."<<writeval;
    str.sprintf("%.3f",writeval);
    qDebug()<<"str33"<<str;
    inifile.WriteIni("TOP","W_LENGTH",str);
    g_par.w_movelimit =(float)writeval;
    //ui->lineEdit_wlength->setText(tmp);

    //将para赋值给paraback,更新备份参数
    QVector<QString> strtmp(para);
    paraback.swap(strtmp);

    m_bCancelTwoback = m_bCancelTwo;
    m_bCancelThreeback = m_bCancelThree;
    m_bOpenSupermanback = m_bOpenSuperman;
    m_iLightCntback = m_iLightCnt;
    m_iLightTimeback = m_iLightTime;
}

void DlgSuperParaSetting::on_superparainput_okEvent()
{
     if(IsParaChanged())
     {
        SavePara();
        g_reloadpara = true;//update para
     }
}

void DlgSuperParaSetting::on_superparainput_cancelEvent()
{
    if(IsParaChanged())
    {
        QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("参数有更改，是否保存?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            SavePara();
            g_reloadpara = true;//update para
        }
        else
        {
            this->close();
        }
    }
    else
    {
        this->close();
    }
}

void DlgSuperParaSetting::on_radioButton_upframe_clicked()
{
    ui->stackedWidget_superpara->setCurrentIndex(0);
    ui->btn_light_clear->hide();
    ui->checkBox_lightcnt->hide();
    ui->checkBox_lighttime->hide();
    ui->frame_btn2->hide();
}

void DlgSuperParaSetting::on_radioButton_downframe_clicked()
{
    ui->stackedWidget_superpara->setCurrentIndex(1);
    ui->btn_light_clear->hide();
    ui->checkBox_lightcnt->hide();
    ui->checkBox_lighttime->hide();
    ui->frame_btn2->hide();
}

void DlgSuperParaSetting::on_radioButton_light_clicked()
{
    ui->stackedWidget_superpara->setCurrentIndex(2);
    ui->btn_light_clear->show();
    ui->checkBox_lightcnt->show();
    ui->checkBox_lighttime->show();
    ui->frame_btn2->show();
}

void DlgSuperParaSetting::on_radioButton_dwlock_clicked()
{
    ui->stackedWidget_superpara->setCurrentIndex(3);
    ui->btn_light_clear->hide();
    ui->checkBox_lightcnt->hide();
    ui->checkBox_lighttime->hide();
    ui->frame_btn2->hide();
}

void DlgSuperParaSetting::on_radioButton_supersetting_clicked()
{
    ui->stackedWidget_superpara->setCurrentIndex(4);
    ui->btn_light_clear->hide();
    ui->checkBox_lightcnt->hide();
    ui->checkBox_lighttime->hide();
    ui->frame_btn2->hide();
}

void DlgSuperParaSetting::on_radioButton_otherpara_clicked()
{
    ui->stackedWidget_superpara->setCurrentIndex(5);
    ui->btn_light_clear->hide();
    ui->checkBox_lightcnt->hide();
    ui->checkBox_lighttime->hide();
    ui->frame_btn2->hide();
}

void DlgSuperParaSetting::on_btn_light_clear_clicked()
{
     QString strlight;
     if(ui->checkBox_lightcnt->isChecked())
     {
         m_iLightCnt = 0;
         strlight.sprintf("%d",m_iLightCnt);
         ui->label_lightnum->setText(strlight);
     }
     if(ui->checkBox_lighttime->isChecked())
     {
         m_iLightTime = 0;
         strlight.sprintf("%d:%d",(int)m_iLightTime/60,(int)m_iLightTime%60);
         ui->label_lighttime->setText(strlight);
     }
}

void DlgSuperParaSetting::on_btn_dwlock_cw_clicked()
{

}

void DlgSuperParaSetting::on_btn_dwlock_ccw_clicked()
{

}

void DlgSuperParaSetting::on_btn_dwlock_yplus_clicked()
{

}

void DlgSuperParaSetting::on_btn_dwlock_xplus_clicked()
{

}

void DlgSuperParaSetting::on_btn_dwlock_yminus_clicked()
{

}

void DlgSuperParaSetting::on_btn_dwlock_xminus_clicked()
{

}

void DlgSuperParaSetting::on_checkBox_canceltwo_clicked()
{
     m_bCancelTwo = !m_bCancelTwo;
}

void DlgSuperParaSetting::on_checkBox_cancelthree_clicked()
{
     m_bCancelThree = !m_bCancelThree;
}

void DlgSuperParaSetting::on_checkBox_issuperman_clicked()
{
    if(ui->checkBox_issuperman->isChecked())
    {
        m_bOpenSuperman = true;
    }
    else
    {
        m_bOpenSuperman = false;
    }
}

void DlgSuperParaSetting::on_checkBox_isdpload_clicked()
{
    if(ui->checkBox_isdpload->isChecked())
    {
        g_par.is_dpvacuum = 1;
    }
    else
    {
        g_par.is_dpvacuum = 0;
    }
}
