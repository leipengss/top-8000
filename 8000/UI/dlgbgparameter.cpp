#include "dlgbgparameter.h"
#include "ui_bgparameter.h"
#include"filemanage/cinirw.h"
//#include "driver/global.h"
#include<QMessageBox>
#include<QDebug>
#include<stdio.h>
#include"driver/cupdatedatathread.h"
const static int miltoum = 25;

dlgBGParameter::dlgBGParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgBGParameter)
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
    connect(input, SIGNAL(okEvent()),this, SLOT(on_bginput_okEvent()));
    connect(input, SIGNAL(cancelEvent()),this, SLOT(on_bginput_cancelEvent()));
    //
    ui->radioButton_dppara->setEnabled(false);
    //
    ui->radioButton_bgpara->setChecked(true);
    ui->stackedWidget_bgpara->setCurrentIndex(0);
    ui->btn_ccd2->hide();
    ui->btn_ccd3->hide();
    ui->btn_ccd4->hide();
    if(g_par.isopensuperpar==0)
    {
        ui->btn_auto->setEnabled(false);
        ui->btn_manual->setEnabled(false);
        ui->btn_energy->setEnabled(false);
        ui->btn_time->setEnabled(false);
        ui->btn_halfpower->setEnabled(false);
        ui->btn_fullpower->setEnabled(false);
        //
        ui->lineEdit_up_dperrorx->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-X
        ui->lineEdit_up_dperrory->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-Y
        ui->lineEdit_up_dpdivx->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dpdivy->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dwjingdu->setEnabled(false);//(new QIntValidator(3,30, this));//上框对位完成精度

        ui->lineEdit_up_dperrorx_2->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-X
        ui->lineEdit_up_dperrory_2->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-Y
        ui->lineEdit_up_dpdivx_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dpdivy_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dwjingdu_2->setEnabled(false);//(new QIntValidator(3,30, this));//上框对位完成精度

        ui->lineEdit_down_dperrorx->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-X
        ui->lineEdit_down_dperrory->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-Y
        ui->lineEdit_down_dpdivx->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dpdivy->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dwjingdu->setEnabled(false);//(new QIntValidator(3,30, this));//下框对位完成精度

        ui->lineEdit_down_dperrorx_2->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-X
        ui->lineEdit_down_dperrory_2->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-Y
        ui->lineEdit_down_dpdivx_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dpdivy_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dwjingdu_2->setEnabled(false);//(new QIntValidator(3,30, this));//下框对位完成精度
    }
    //
    if(g_language>1)
    {
        ui->label_upbg_delay->setStyleSheet("font-size:10px;");
        ui->label_downbg_delay->setStyleSheet("font-size:10px;");
        ui->label_upbg_time->setStyleSheet("font-size:10px;");
        ui->label_downbg_time->setStyleSheet("font-size:10px;");
        ui->label_upbg_energy->setStyleSheet("font-size:10px;");
        ui->label_downbg_energy->setStyleSheet("font-size:10px;");
        ui->label_updpclear_count->setStyleSheet("font-size:10px;");
        ui->label_downdpclear_count->setStyleSheet("font-size:10px;");
        ui->label_updpbg_count->setStyleSheet("font-size:10px;");
        ui->label_downdpbg_count->setStyleSheet("font-size:10px;");
        ui->label_zhenkongbaochi->setStyleSheet("font-size:10px;");
        ui->label_switchboard_time->setStyleSheet("font-size:10px;");
        //
        ui->label_up_checkdelay->setStyleSheet("font-size:10px;");
        ui->label_up_maxjb->setStyleSheet("font-size:10px;");
        ui->label_up_dwjingdu->setStyleSheet("font-size:10px;");
        ui->label_up_dperrorx->setStyleSheet("font-size:10px;");
        ui->label_up_dpdivx->setStyleSheet("font-size:10px;");
        ui->btn_up_checkallmarks->setStyleSheet("font-size:10px;");
        //
        ui->label_down_checkdelay->setStyleSheet("font-size:10px;");
        ui->label_down_maxjb->setStyleSheet("font-size:10px;");
        ui->label_down_dwjingdu->setStyleSheet("font-size:10px;");
        ui->label_down_dperrorx->setStyleSheet("font-size:10px;");
        ui->label_down_dpdivx->setStyleSheet("font-size:10px;");
        ui->btn_down_checkallmarks->setStyleSheet("font-size:10px;");
        //
        ui->label_up_piecehightmil->setStyleSheet("font-size:10px;");
        ui->label_down_piecehightmil->setStyleSheet("font-size:10px;");
        ui->label_up_piecehight->setStyleSheet("font-size:10px;");
        ui->label_down_piecehight->setStyleSheet("font-size:10px;");
        ui->label_up_dwdistance->setStyleSheet("font-size:10px;");
        ui->label_down_dwdistance->setStyleSheet("font-size:10px;");
        ui->label_up_xlength->setStyleSheet("font-size:10px;");
        ui->label_down_xlength->setStyleSheet("font-size:10px;");
        ui->checkBox_up_checkdistance->setStyleSheet("font-size:10px;");
        ui->checkBox_down_checkdistance->setStyleSheet("font-size:10px;");
        ui->checkBox_up_cycle->setStyleSheet("font-size:10px;");
        ui->checkBox_down_cycle->setStyleSheet("font-size:10px;");
        ui->checkBox_binarization->setStyleSheet("font-size:10px;");
    }
    //
    SetTextInputRange();
    InitParameter();
}

dlgBGParameter::~dlgBGParameter()
{
    delete input;
    delete ui;
}

void dlgBGParameter::SetTextInputRange()
{
    if(g_settings_kind<11)
    {
        ui->lineEdit_upbg_delay->setEnabled(false);//(new QIntValidator(0, 30000, this));
        ui->lineEdit_upbg_time->setEnabled(false);//(new QIntValidator(0, 90000, this));
     //   ui->lineEdit_upbg_time2->setEnabled(false);//(new QIntValidator(0, 90000, this));
        ui->lineEdit_upbg_energy->setEnabled(false);//(new QIntValidator(0, 5000, this));
        ui->lineEdit_updpclear_count->setEnabled(false);//(new QIntValidator(0,10000, this));
        ui->lineEdit_updpbg_count->setEnabled(false);//(new QIntValidator(0,10000, this));

        ui->lineEdit_downbg_delay->setEnabled(false);//(new QIntValidator(0, 30000, this));
        ui->lineEdit_downbg_time->setEnabled(false);//(new QIntValidator(0, 90000, this));
    //    ui->lineEdit_downbg_time2->setEnabled(false);//(new QIntValidator(0, 90000, this));
        ui->lineEdit_downbg_energy->setEnabled(false);//(new QIntValidator(0, 5000, this));
        ui->lineEdit_downdpclear_count->setEnabled(false);//(new QIntValidator(0,10000, this));
        ui->lineEdit_downdpbg_count->setEnabled(false);//(new QIntValidator(0,10000, this));
        ui->lineEdit_switchboard_time->setEnabled(false);//(new QIntValidator(1000,10000, this));
        //上框影像检测装置
        ui->lineEdit_up_checkdelay->setEnabled(false);//(new QIntValidator(500,10000, this));//上框检测延时时间
        ui->lineEdit_up_dperrorx->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-X
        ui->lineEdit_up_dperrory->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-Y
        ui->lineEdit_up_dpdivx->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dpdivy->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_filmdia->setEnabled(false);//(new QIntValidator(500,5000, this));//上菲林直径
        ui->lineEdit_up_maxjb->setEnabled(false);//(new QIntValidator(1,10, this));//上框最大拒曝次数
        ui->lineEdit_up_boarddia->setEnabled(false);//(new QIntValidator(500,5000, this));//上底板直径
        ui->lineEdit_up_dwjingdu->setEnabled(false);//(new QIntValidator(3,30, this));//上框对位完成精度

        ui->lineEdit_up_checkdelay_2->setEnabled(false);//(new QIntValidator(500,10000, this));//上框检测延时时间
        ui->lineEdit_up_dperrorx_2->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-X
        ui->lineEdit_up_dperrory_2->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-Y
        ui->lineEdit_up_dpdivx_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dpdivy_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_filmdia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//上菲林直径
        ui->lineEdit_up_maxjb_2->setEnabled(false);//(new QIntValidator(1,10, this));//上框最大拒曝次数
        ui->lineEdit_up_boarddia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//上底板直径
        ui->lineEdit_up_dwjingdu_2->setEnabled(false);//(new QIntValidator(3,30, this));//上框对位完成精度

        ui->lineEdit_down_checkdelay->setEnabled(false);//(new QIntValidator(500,10000, this));//下框检测延时时间
        ui->lineEdit_down_dperrorx->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-X
        ui->lineEdit_down_dperrory->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-Y
        ui->lineEdit_down_dpdivx->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dpdivy->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_filmdia->setEnabled(false);//(new QIntValidator(500,5000, this));//下菲林直径
        ui->lineEdit_down_maxjb->setEnabled(false);//(new QIntValidator(1,10, this));//下框最大拒曝次数
        ui->lineEdit_down_boarddia->setEnabled(false);//(new QIntValidator(500,5000, this));//下底板直径
        ui->lineEdit_down_dwjingdu->setEnabled(false);//(new QIntValidator(3,30, this));//下框对位完成精度

        ui->lineEdit_down_checkdelay_2->setEnabled(false);//(new QIntValidator(500,10000, this));//下框检测延时时间
        ui->lineEdit_down_dperrorx_2->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-X
        ui->lineEdit_down_dperrory_2->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-Y
        ui->lineEdit_down_dpdivx_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dpdivy_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_filmdia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//下菲林直径
        ui->lineEdit_down_maxjb_2->setEnabled(false);//(new QIntValidator(1,10, this));//下框最大拒曝次数
        ui->lineEdit_down_boarddia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//下底板直径
        ui->lineEdit_down_dwjingdu_2->setEnabled(false);//(new QIntValidator(3,30, this));//下框对位完成精度
        //平台设置
        ui->lineEdit_up_piecehightmil->setEnabled(false);//(new QIntValidator(0,119, this));//上框工件板厚mil
        ui->lineEdit_up_piecehight->setEnabled(false);//(new QIntValidator(100,3000, this));//上框工件板厚um
        ui->lineEdit_up_dwdistance->setEnabled(false);//(new QIntValidator(100,10000, this));//上框对位间距
        ui->lineEdit_down_piecehightmil->setEnabled(false);//(new QIntValidator(0,119, this));//上框工件板厚mil
        ui->lineEdit_down_piecehight->setEnabled(false);//(new QIntValidator(100,3000, this));//上框工件板厚um
        ui->lineEdit_down_dwdistance->setEnabled(false);//(new QIntValidator(100,10000, this));//上框对位间距

        ui->lineEdit_up_xlength->setEnabled(false);//(new QIntValidator(0,1999, this));
        ui->lineEdit_up_ylength->setEnabled(false);//(new QIntValidator(0,1999, this));
        ui->lineEdit_down_xlength->setEnabled(false);//(new QIntValidator(0,1999, this));
        ui->lineEdit_down_ylength->setEnabled(false);//(new QIntValidator(0,1999, this));
        //
        ui->btn_fullpower->setEnabled(false);
        ui->btn_halfpower->setEnabled(false);
        ui->btn_auto->setEnabled(false);
        ui->btn_manual->setEnabled(false);
        ui->btn_time->setEnabled(false);
        ui->btn_energy->setEnabled(false);
        ui->btn_upframe->setEnabled(false);
        ui->btn_downframe->setEnabled(false);
        ui->btn_ccd2->setEnabled(false);
        ui->btn_ccd3->setEnabled(false);
        ui->btn_ccd4->setEnabled(false);
        ui->checkBox_binarization->setEnabled(false);
        ui->checkBox_down_checkdistance->setEnabled(false);
        ui->checkBox_down_cycle->setEnabled(false);
        ui->checkBox_up_checkdistance->setEnabled(false);
        ui->checkBox_up_cycle->setEnabled(false);
        ui->btn_down_ccd1234->setEnabled(false);
        ui->btn_down_checkallmarks->setEnabled(false);
        ui->btn_down_ccd1234_2->setEnabled(false);
        ui->btn_down_checkallmarks_2->setEnabled(false);
        ui->btn_up_ccd1234->setEnabled(false);
        ui->btn_up_checkallmarks->setEnabled(false);
        ui->btn_up_ccd1234_2->setEnabled(false);
        ui->btn_up_checkallmarks_2->setEnabled(false);
        ui->spinBox_zhenkongbaochi->setEnabled(false);
    }
    ui->lineEdit_upbg_delay->setValidator(new QIntValidator(0, 30000, this));
    ui->lineEdit_upbg_time->setValidator(new QIntValidator(0, 90000, this));
    ui->lineEdit_upbg_energy->setValidator(new QIntValidator(0, 5000, this));
    ui->lineEdit_updpclear_count->setValidator(new QIntValidator(0,10000, this));
    ui->lineEdit_updpbg_count->setValidator(new QIntValidator(0,10000, this));

    ui->lineEdit_downbg_delay->setValidator(new QIntValidator(0, 30000, this));
    ui->lineEdit_downbg_time->setValidator(new QIntValidator(0, 50, this));
    ui->lineEdit_downbg_energy->setValidator(new QIntValidator(0, 5000, this));
    ui->lineEdit_downdpclear_count->setValidator(new QIntValidator(0,10000, this));
    ui->lineEdit_downdpbg_count->setValidator(new QIntValidator(0,10000, this));
    ui->lineEdit_switchboard_time->setValidator(new QIntValidator(1000,10000, this));
    //上框影像检测装置
    ui->lineEdit_up_checkdelay->setValidator(new QIntValidator(500,10000, this));//上框检测延时时间
    ui->lineEdit_up_dperrorx->setValidator(new QIntValidator(5,999, this));//上框底片误差值+-X
    ui->lineEdit_up_dperrory->setValidator(new QIntValidator(5,999, this));//上框底片误差值+-Y
    ui->lineEdit_up_dpdivx->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_up_dpdivy->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_up_filmdia->setValidator(new QIntValidator(500,5000, this));//上菲林直径
    ui->lineEdit_up_maxjb->setValidator(new QIntValidator(1,10, this));//上框最大拒曝次数
    ui->lineEdit_up_boarddia->setValidator(new QIntValidator(500,5000, this));//上底板直径
    ui->lineEdit_up_dwjingdu->setValidator(new QIntValidator(3,30, this));//上框对位完成精度

    ui->lineEdit_up_checkdelay_2->setValidator(new QIntValidator(500,10000, this));//上框检测延时时间
    ui->lineEdit_up_dperrorx_2->setValidator(new QIntValidator(5,999, this));//上框底片误差值+-X
    ui->lineEdit_up_dperrory_2->setValidator(new QIntValidator(5,999, this));//上框底片误差值+-Y
    ui->lineEdit_up_dpdivx_2->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_up_dpdivy_2->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_up_filmdia_2->setValidator(new QIntValidator(500,5000, this));//上菲林直径
    ui->lineEdit_up_maxjb_2->setValidator(new QIntValidator(1,10, this));//上框最大拒曝次数
    ui->lineEdit_up_boarddia_2->setValidator(new QIntValidator(500,5000, this));//上底板直径
    ui->lineEdit_up_dwjingdu_2->setValidator(new QIntValidator(3,30, this));//上框对位完成精度

    ui->lineEdit_down_checkdelay->setValidator(new QIntValidator(500,10000, this));//下框检测延时时间
    ui->lineEdit_down_dperrorx->setValidator(new QIntValidator(5,999, this));//下框底片误差值+-X
    ui->lineEdit_down_dperrory->setValidator(new QIntValidator(5,999, this));//下框底片误差值+-Y
    ui->lineEdit_down_dpdivx->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_down_dpdivy->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_down_filmdia->setValidator(new QIntValidator(500,5000, this));//下菲林直径
    ui->lineEdit_down_maxjb->setValidator(new QIntValidator(1,10, this));//下框最大拒曝次数
    ui->lineEdit_down_boarddia->setValidator(new QIntValidator(500,5000, this));//下底板直径
    ui->lineEdit_down_dwjingdu->setValidator(new QIntValidator(3,30, this));//下框对位完成精度

    ui->lineEdit_down_checkdelay_2->setValidator(new QIntValidator(500,10000, this));//下框检测延时时间
    ui->lineEdit_down_dperrorx_2->setValidator(new QIntValidator(5,999, this));//下框底片误差值+-X
    ui->lineEdit_down_dperrory_2->setValidator(new QIntValidator(5,999, this));//下框底片误差值+-Y
    ui->lineEdit_down_dpdivx_2->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_down_dpdivy_2->setValidator(new QIntValidator(3,999, this));
    ui->lineEdit_down_filmdia_2->setValidator(new QIntValidator(500,5000, this));//下菲林直径
    ui->lineEdit_down_maxjb_2->setValidator(new QIntValidator(1,10, this));//下框最大拒曝次数
    ui->lineEdit_down_boarddia_2->setValidator(new QIntValidator(500,5000, this));//下底板直径
    ui->lineEdit_down_dwjingdu_2->setValidator(new QIntValidator(3,30, this));//下框对位完成精度
    //平台设置
    ui->lineEdit_up_piecehightmil->setValidator(new QIntValidator(0,119, this));//上框工件板厚mil
    ui->lineEdit_up_piecehight->setValidator(new QIntValidator(100,3000, this));//上框工件板厚um
    ui->lineEdit_up_dwdistance->setValidator(new QIntValidator(100,10000, this));//上框对位间距
    ui->lineEdit_down_piecehightmil->setValidator(new QIntValidator(0,119, this));//上框工件板厚mil
    ui->lineEdit_down_piecehight->setValidator(new QIntValidator(100,3000, this));//上框工件板厚um
    ui->lineEdit_down_dwdistance->setValidator(new QIntValidator(100,10000, this));//上框对位间距

    ui->lineEdit_up_xlength->setValidator(new QIntValidator(0,1999, this));
    ui->lineEdit_up_ylength->setValidator(new QIntValidator(0,1999, this));
    ui->lineEdit_down_xlength->setValidator(new QIntValidator(0,1999, this));
    ui->lineEdit_down_ylength->setValidator(new QIntValidator(0,1999, this));
}
void dlgBGParameter::InitParameter()
{
    CIniRW inifile(TOPPARAMETERPATH);
    /**曝光参数设置**/
    //上框
    m_sUpBG_timedelay = inifile.ReadIni("TRAJ","BAO_GUANG_DELAY").toString();
    ui->lineEdit_upbg_delay->setText(m_sUpBG_timedelay);
    m_sUpBG_time = inifile.ReadIni("TRAJ","SHANG_DI_PIAN").toString();
    ui->lineEdit_upbg_time->setText(m_sUpBG_time);
    m_sUpBG_energy = inifile.ReadIni("TRAJ","BAO_GUANG_ENERGY").toString();
    ui->lineEdit_upbg_energy->setText(m_sUpBG_energy);
    m_sUpDP_clearcnt = inifile.ReadIni("TRAJ","CLEAN_DIPIAN_COUNT").toString();
    ui->lineEdit_updpclear_count->setText(m_sUpDP_clearcnt);
    m_sUpDP_bgcnt = inifile.ReadIni("TRAJ","DIPIAN_COUNT").toString();
    ui->lineEdit_updpbg_count->setText(m_sUpDP_bgcnt);
    //下框
    m_sDownBG_timedelay = inifile.ReadIni("TRAJ","BAO_GUANG_DELAY2").toString();
    ui->lineEdit_downbg_delay->setText(m_sDownBG_timedelay);
    m_sDownBG_time = inifile.ReadIni("TRAJ","XIA_DI_PIAN").toString();
    ui->lineEdit_downbg_time->setText(m_sDownBG_time);
    m_sDownBG_energy = inifile.ReadIni("TRAJ","BAO_GUANG_ENERGY2").toString();
    ui->lineEdit_downbg_energy->setText(m_sDownBG_energy);
    m_sDownDP_clearcnt = inifile.ReadIni("TRAJ","CLEAN_DIPIAN_COUNT2").toString();
    ui->lineEdit_downdpclear_count->setText(m_sDownDP_clearcnt);
    m_sDownDP_bgcnt = inifile.ReadIni("TRAJ","DIPIAN_COUNT2").toString();
    ui->lineEdit_downdpbg_count->setText(m_sDownDP_bgcnt);
    m_sBG_finishair = inifile.ReadIni("TRAJ","AIR_KEEP").toString();
    ui->spinBox_zhenkongbaochi->setValue(m_sBG_finishair.toInt());
    m_sBG_switchboardtime = inifile.ReadIni("TRAJ","QU_FANG_TIME").toString();
    ui->lineEdit_switchboard_time->setText(m_sBG_switchboardtime);
    /**上框影像检测装置**/
    m_sUp_checkdelay = inifile.ReadIni("TRAJ","CHECK_DELAY").toString();
    ui->lineEdit_up_checkdelay->setText(m_sUp_checkdelay);
    m_sUp_maxjb = inifile.ReadIni("TRAJ","DW_MAX_NUM").toString();
    ui->lineEdit_up_maxjb->setText(m_sUp_maxjb);
    m_sUp_dwjingdu = inifile.ReadIni("TRAJ","DUIWEI_FINISH_PRECISION").toString();
    ui->lineEdit_up_dwjingdu->setText(m_sUp_dwjingdu);
    m_sUp_dperror_x = inifile.ReadIni("TRAJ","DIPIAN_WUCHA_X").toString();
    ui->lineEdit_up_dperrorx->setText(m_sUp_dperror_x);
    m_sUp_dperror_y = inifile.ReadIni("TRAJ","DIPIAN_WUCHA_Y").toString();
    ui->lineEdit_up_dperrory->setText(m_sUp_dperror_y);
    m_sUp_dpdiv_x = inifile.ReadIni("TRAJ","JUNFEN_WUCHA_X").toString();
    ui->lineEdit_up_dpdivx->setText(m_sUp_dpdiv_x);
    m_sUp_dpdiv_y = inifile.ReadIni("TRAJ","JUNFEN_WUCHA_Y").toString();
    ui->lineEdit_up_dpdivy->setText(m_sUp_dpdiv_y);
    m_sUp_filmDia = inifile.ReadIni("TRAJ","UP_FILM_D").toString();
    ui->lineEdit_up_filmdia->setText(m_sUp_filmDia);
    m_sUp_boardDia = inifile.ReadIni("TRAJ","BOARD_D").toString();
    ui->lineEdit_up_boarddia->setText(m_sUp_boardDia);

    /**下框影像检测装置**/   
    m_sDown_checkdelay = inifile.ReadIni("TRAJ","CHECK_DELAY2").toString();
    ui->lineEdit_down_checkdelay->setText(m_sDown_checkdelay);
    m_sDown_maxjb = inifile.ReadIni("TRAJ","DW_MAX_NUM2").toString();
    ui->lineEdit_down_maxjb->setText(m_sDown_maxjb);
    m_sDown_dwjingdu = inifile.ReadIni("TRAJ","DUIWEI_FINISH_PRECISION2").toString();
    ui->lineEdit_down_dwjingdu->setText(m_sDown_dwjingdu);
    m_sDown_dperror_x = inifile.ReadIni("TRAJ","DIPIAN_WUCHA_X2").toString();
    ui->lineEdit_down_dperrorx->setText(m_sDown_dperror_x);
    m_sDown_dperror_y = inifile.ReadIni("TRAJ","DIPIAN_WUCHA_Y2").toString();
    ui->lineEdit_down_dperrory->setText(m_sDown_dperror_y);
    m_sDown_dpdiv_x = inifile.ReadIni("TRAJ","JUNFEN_WUCHA_X2").toString();
    ui->lineEdit_down_dpdivx->setText(m_sDown_dpdiv_x);
    m_sDown_dpdiv_y = inifile.ReadIni("TRAJ","JUNFEN_WUCHA_Y2").toString();
    ui->lineEdit_down_dpdivy->setText(m_sDown_dpdiv_y);
    m_sDown_filmDia = inifile.ReadIni("TRAJ","DOWN_FILM_D").toString();
    ui->lineEdit_down_filmdia->setText(m_sDown_filmDia);
    m_sDown_boardDia = inifile.ReadIni("TRAJ","BOARD_D2").toString();
    ui->lineEdit_down_boarddia->setText(m_sDown_boardDia);

    /**平台设置**/
    m_sUp_boardHeightMil = inifile.ReadIni("TRAJ","PRODUCT_THICKNESS_MIL").toString();
    ui->lineEdit_up_piecehightmil->setText(m_sUp_boardHeightMil);
    m_sDown_boardHeightMil = inifile.ReadIni("TRAJ","PRODUCT_THICKNESS_MIL2").toString();
    ui->lineEdit_down_piecehightmil->setText(m_sDown_boardHeightMil);
    m_sUp_boardHeight = inifile.ReadIni("TRAJ","PRODUCT_THICKNESS_UM").toString();
    ui->lineEdit_up_piecehight->setText(m_sUp_boardHeight);
    m_sDown_boardHeight = inifile.ReadIni("TRAJ","PRODUCT_THICKNESS_UM2").toString();
    ui->lineEdit_down_piecehight->setText(m_sDown_boardHeight);
    m_sUp_dwDistance = inifile.ReadIni("TRAJ","DUIWEI_DISTANCE").toString();
    ui->lineEdit_up_dwdistance->setText(m_sUp_dwDistance);
    m_sDown_dwDistance = inifile.ReadIni("TRAJ","DUIWEI_DISTANCE2").toString();
    ui->lineEdit_down_dwdistance->setText(m_sDown_dwDistance);
    m_sUp_xlength = inifile.ReadIni("TRAJ","WIDTH").toString();
    ui->lineEdit_up_xlength->setText(m_sUp_xlength);
    m_sDown_xlength = inifile.ReadIni("TRAJ","WIDTH2").toString();
    ui->lineEdit_down_xlength->setText(m_sDown_xlength);
    m_sUp_ylength = inifile.ReadIni("TRAJ","HEIGHT").toString();
    ui->lineEdit_up_ylength->setText(m_sUp_ylength);
    m_sDown_ylength = inifile.ReadIni("TRAJ","HEIGHT2").toString();
    ui->lineEdit_down_ylength->setText(m_sDown_ylength);
    //
    m_iFrameSelect = inifile.ReadIni("TOP","FRAME_SELECT").toInt();
    m_bfull = inifile.ReadIni("TRAJ","QUANGONG_OR_BANGONG").toInt();
    m_bmannual = inifile.ReadIni("TRAJ","AUTO_OR_MANUAL").toInt();
    m_benergyortime = inifile.ReadIni("TRAJ","ENERGY_OR_TIME").toInt();
    m_iUpCCDSelect[0] = inifile.ReadIni("TRAJ","CCD_234").toInt();//上框上底片
    m_iUpCCDSelect[1] = inifile.ReadIni("TRAJ","CCD_234_2").toInt();//下框上底片
    m_iDownCCDSelect[0] = inifile.ReadIni("TRAJ","DOWNCCD_234").toInt();//下框上底片
    m_iDownCCDSelect[1] = inifile.ReadIni("TRAJ","DOWNCCD_234_2").toInt();//下框下底片
    //
    m_btn_upccd[0] = inifile.ReadIni("TRAJ","CCD_SWITCH").toInt();
    m_btn_downccd[0] = inifile.ReadIni("TRAJ","CCD_SWITCH2").toInt();
    m_btn_upcheckmark[0] = inifile.ReadIni("TOP","CHECK8").toInt();
    m_btn_downcheckmark[0] = inifile.ReadIni("TOP","CHECK8_2").toInt();

    m_btn_upccd[1] = inifile.ReadIni("TRAJ","DOWNCCD_SWITCH").toInt();
    m_btn_downccd[1] = inifile.ReadIni("TRAJ","DOWNCCD_SWITCH2").toInt();
    m_btn_upcheckmark[1] = inifile.ReadIni("TOP","DOWNCHECK8").toInt();
    m_btn_downcheckmark[1] = inifile.ReadIni("TOP","DOWNCHECK8_2").toInt();
    //check box
    m_bCheckbox_Upcycle = inifile.ReadIni("TRAJ","DW_KXY_BASE").toInt();
    m_bCheckbox_Downcycle = inifile.ReadIni("TRAJ","DW_KXY_BASE2").toInt();
    m_bCheckbox_UpCheckDis = inifile.ReadIni("TRAJ","GC1").toInt();
    m_bCheckbox_DownCheckDis = inifile.ReadIni("TRAJ","GC2").toInt();
    m_bCheckbox_Binary = inifile.ReadIni("TOP","EZH").toInt();

    m_bCancelTow = inifile.ReadIni("TOP","CANCEL_TWO").toInt();
    m_bCancelThree = inifile.ReadIni("TOP","CANCEL_THREE").toInt();
    /****备份参数******/
    //上框
    m_sUpBG_timedelay_back = m_sUpBG_timedelay;
    m_sUpBG_time_back = m_sUpBG_time  ;
    m_sUpBG_time2_back = m_sUpBG_time2  ;
    m_sUpBG_energy_back = m_sUpBG_energy  ;
    m_sUpDP_clearcnt_back = m_sUpDP_clearcnt  ;
    m_sUpDP_bgcnt_back = m_sUpDP_bgcnt  ;
    //下框
    m_sDownBG_timedelay_back = m_sDownBG_timedelay  ;
    m_sDownBG_time_back = m_sDownBG_time  ;
    m_sDownBG_time2_back = m_sDownBG_time2  ;
    m_sDownBG_energy_back = m_sDownBG_energy  ;
    m_sDownDP_clearcnt_back = m_sDownDP_clearcnt  ;
    m_sDownDP_bgcnt_back = m_sDownDP_bgcnt  ;
    m_sBG_finishair_back = m_sBG_finishair  ;
    m_sBG_switchboardtime_back = m_sBG_switchboardtime  ;
    /**上框影像检测装置**/
    m_sUp_checkdelay_back = m_sUp_checkdelay  ;
    m_sUp_maxjb_back = m_sUp_maxjb  ;
    m_sUp_dwjingdu_back = m_sUp_dwjingdu  ;
    m_sUp_dperror_x_back = m_sUp_dperror_x  ;
    m_sUp_dperror_y_back = m_sUp_dperror_y  ;
    m_sUp_dpdiv_x_back = m_sUp_dpdiv_x  ;
    m_sUp_dpdiv_y_back = m_sUp_dpdiv_y  ;
    m_sUp_filmDia_back = m_sUp_filmDia  ;
    m_sUp_boardDia_back = m_sUp_boardDia  ;

    /**下框影像检测装置**/
    m_sDown_checkdelay_back = m_sDown_checkdelay  ;
    m_sDown_maxjb_back = m_sDown_maxjb  ;
    m_sDown_dwjingdu_back = m_sDown_dwjingdu  ;
    m_sDown_dperror_x_back = m_sDown_dperror_x  ;
    m_sDown_dperror_y_back = m_sDown_dperror_y  ;
    m_sDown_dpdiv_x_back = m_sDown_dpdiv_x  ;
    m_sDown_dpdiv_y_back = m_sDown_dpdiv_y  ;
    m_sDown_filmDia_back = m_sDown_filmDia  ;
    m_sDown_boardDia_back = m_sDown_boardDia  ;

    /**平台设置**/
    m_sUp_boardHeightMil_back = m_sUp_boardHeightMil  ;
    m_sDown_boardHeightMil_back = m_sDown_boardHeightMil  ;
    m_sUp_boardHeight_back = m_sUp_boardHeight  ;
    m_sDown_boardHeight_back = m_sDown_boardHeight  ;
    m_sUp_dwDistance_back = m_sUp_dwDistance  ;
    m_sDown_dwDistance_back = m_sDown_dwDistance  ;
    m_sUp_xlength_back = m_sUp_xlength  ;
    m_sDown_xlength_back = m_sDown_xlength  ;
    m_sUp_ylength_back = m_sUp_ylength  ;
    m_sDown_ylength_back = m_sDown_ylength  ;
    //right btn status
    m_bfull_back = m_bfull  ;
    m_bmannual_back = m_bmannual  ;
    m_benergyortime_back = m_benergyortime  ;
    m_iFrameSelect_back = m_iFrameSelect  ;//
    m_iUpCCDSelect_back[0] = m_iUpCCDSelect[0]  ;
    m_iDownCCDSelect_back[0] = m_iDownCCDSelect[0]  ;
    m_iUpCCDSelect_back[1] = m_iUpCCDSelect[1]  ;
    m_iDownCCDSelect_back[1] = m_iDownCCDSelect[1]  ;
    //page btn
    m_btn_upccd_back[0] = m_btn_upccd[0]  ;
    m_btn_upcheckmark_back[0] = m_btn_upcheckmark[0]  ;
    m_btn_downccd_back[0] = m_btn_downccd[0]  ;
    m_btn_downcheckmark_back[0] = m_btn_downcheckmark[0]  ;

    m_btn_upccd_back[1] = m_btn_upccd[1]  ;
    m_btn_upcheckmark_back[1] = m_btn_upcheckmark[1]  ;
    m_btn_downccd_back[1] = m_btn_downccd[1]  ;
    m_btn_downcheckmark_back[1] = m_btn_downcheckmark[1]  ;
    //check box status
    m_bCheckbox_Upcycle_back = m_bCheckbox_Upcycle  ;
    m_bCheckbox_Downcycle_back = m_bCheckbox_Downcycle  ;
    m_bCheckbox_UpCheckDis_back = m_bCheckbox_UpCheckDis  ;
    m_bCheckbox_DownCheckDis_back = m_bCheckbox_DownCheckDis  ;
    m_bCheckbox_Binary_back = m_bCheckbox_Binary  ;

    //initial ui
    InitUIStatus();
}

void dlgBGParameter::InitUIStatus()
{
    ui->checkBox_binarization->setChecked(m_bCheckbox_Binary);
    ui->checkBox_up_checkdistance->setChecked(m_bCheckbox_UpCheckDis);
    ui->checkBox_down_checkdistance->setChecked(m_bCheckbox_DownCheckDis);
    ui->checkBox_down_cycle->setChecked(m_bCheckbox_Downcycle);
    ui->checkBox_up_cycle->setChecked(m_bCheckbox_Upcycle);
    if(m_btn_upcheckmark[0])
    {
        ui->btn_up_checkallmarks->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_up_checkallmarks->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
    if(m_btn_downcheckmark[0])
    {
        ui->btn_down_checkallmarks->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_down_checkallmarks->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }

    if(m_btn_upcheckmark[1])
    {
        ui->btn_up_checkallmarks_2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_up_checkallmarks_2->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
    if(m_btn_downcheckmark[1])
    {
        ui->btn_down_checkallmarks_2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_down_checkallmarks_2->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
    //page btn
    ui->btn_up_ccd1234->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    ui->btn_down_ccd1234->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    switch (m_btn_upccd[0]) {
    case 0:
        ui->btn_up_ccd1234->setText("CCD 1-2");
        break;
    case 1:
        ui->btn_up_ccd1234->setText("CCD 3-4");
        break;
    case 2:
        ui->btn_up_ccd1234->setText("CCD 1-4");
        break;
    case 3:
        ui->btn_up_ccd1234->setText("CCD 2-3");
        break;
    default:
        break;
    }
    switch (m_btn_downccd[0]) {
    case 0:
        ui->btn_down_ccd1234->setText("CCD 1-2");
        break;
    case 1:
        ui->btn_down_ccd1234->setText("CCD 3-4");
        break;
    case 2:
        ui->btn_down_ccd1234->setText("CCD 1-4");
        break;
    case 3:
        ui->btn_down_ccd1234->setText("CCD 2-3");
        break;
    default:
        break;
    }

    ui->btn_up_ccd1234_2->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    ui->btn_down_ccd1234_2->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    switch (m_btn_upccd[1]) {
    case 0:
        ui->btn_up_ccd1234_2->setText("CCD 1-2");
        break;
    case 1:
        ui->btn_up_ccd1234_2->setText("CCD 3-4");
        break;
    case 2:
        ui->btn_up_ccd1234_2->setText("CCD 1-4");
        break;
    case 3:
        ui->btn_up_ccd1234_2->setText("CCD 2-3");
        break;
    default:
        break;
    }
    switch (m_btn_downccd[1]) {
    case 0:
        ui->btn_down_ccd1234_2->setText("CCD 1-2");
        break;
    case 1:
        ui->btn_down_ccd1234_2->setText("CCD 3-4");
        break;
    case 2:
        ui->btn_down_ccd1234_2->setText("CCD 1-4");
        break;
    case 3:
        ui->btn_down_ccd1234_2->setText("CCD 2-3");
        break;
    default:
        break;
    }
    //full or half
    if(m_bfull==0)
    {
        ui->btn_fullpower->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_halfpower->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_halfpower->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_fullpower->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    //energy or time
    if(m_benergyortime==0)
    {
        ui->btn_energy->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_time->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_time->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_energy->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    //auto or manual
    if(m_bmannual==0)
    {
        ui->btn_auto->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_manual->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_manual->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_auto->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    //up or down
    if(m_iFrameSelect==0)
    {
        ui->btn_upframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_downframe->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    else if(m_iFrameSelect==1)
    {
        ui->btn_downframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_upframe->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    }
    else if(m_iFrameSelect==2)
    {
        ui->btn_upframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_downframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    //权限
    if(g_settings_kind == 10)//低级权限,只能查看参数
    {
        ui->checkBox_binarization->setEnabled(false);
        ui->checkBox_up_checkdistance->setEnabled(false);
        ui->checkBox_down_checkdistance->setEnabled(false);
        ui->checkBox_down_cycle->setEnabled(false);
        ui->checkBox_up_cycle->setEnabled(false);
        ui->btn_up_checkallmarks->setEnabled(false);
        ui->btn_down_checkallmarks->setEnabled(false);
        //page btn
        ui->btn_up_ccd1234->setEnabled(false);
        ui->btn_down_ccd1234->setEnabled(false);
        ui->btn_up_ccd1234->setEnabled(false);
        ui->btn_up_ccd1234->setEnabled(false);
        ui->btn_up_ccd1234->setEnabled(false);
        ui->btn_up_ccd1234->setEnabled(false);
        ui->btn_down_ccd1234->setEnabled(false);
        ui->btn_down_ccd1234->setEnabled(false);
        ui->btn_down_ccd1234->setEnabled(false);
        ui->btn_down_ccd1234->setEnabled(false);

        ui->btn_up_ccd1234_2->setEnabled(false);
        ui->btn_down_ccd1234_2->setEnabled(false);
        ui->btn_up_ccd1234_2->setEnabled(false);
        ui->btn_up_ccd1234_2->setEnabled(false);
        ui->btn_up_ccd1234_2->setEnabled(false);
        ui->btn_up_ccd1234_2->setEnabled(false);
        ui->btn_down_ccd1234_2->setEnabled(false);
        ui->btn_down_ccd1234_2->setEnabled(false);
        ui->btn_down_ccd1234_2->setEnabled(false);
        ui->btn_down_ccd1234_2->setEnabled(false);

        ui->btn_fullpower->setEnabled(false);
        ui->btn_halfpower->setEnabled(false);
        ui->btn_energy->setEnabled(false);
        ui->btn_time->setEnabled(false);
        ui->btn_auto->setEnabled(false);
        ui->btn_manual->setEnabled(false);
        ui->btn_manual->setEnabled(false);
        ui->btn_auto->setEnabled(false);
        ui->btn_upframe->setEnabled(false);
        ui->btn_downframe->setEnabled(false);
        ui->btn_ccd2->setEnabled(false);
        ui->btn_ccd3->setEnabled(false);
        ui->btn_ccd4->setEnabled(false);
        ui->spinBox_zhenkongbaochi->setEnabled(false);

        ui->lineEdit_upbg_delay->setEnabled(false);
        ui->lineEdit_upbg_time->setEnabled(false);//(new QIntValidator(0, 90000, this));
        ui->lineEdit_upbg_energy->setEnabled(false);//(new QIntValidator(0, 5000, this));
        ui->lineEdit_updpclear_count->setEnabled(false);//(new QIntValidator(0,10000, this));
        ui->lineEdit_updpbg_count->setEnabled(false);//(new QIntValidator(0,10000, this));

        ui->lineEdit_downbg_delay->setEnabled(false);//(new QIntValidator(0, 30000, this));
        ui->lineEdit_downbg_time->setEnabled(false);//(new QIntValidator(0, 90000, this));
        ui->lineEdit_downbg_energy->setEnabled(false);//(new QIntValidator(0, 5000, this));
        ui->lineEdit_downdpclear_count->setEnabled(false);//(new QIntValidator(0,10000, this));
        ui->lineEdit_downdpbg_count->setEnabled(false);//(new QIntValidator(0,10000, this));
        ui->lineEdit_switchboard_time->setEnabled(false);//(new QIntValidator(1000,10000, this));
        //上框影像检测装置
        ui->lineEdit_up_checkdelay->setEnabled(false);//(new QIntValidator(0,10000, this));//上框检测延时时间
        ui->lineEdit_up_dperrorx->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-X
        ui->lineEdit_up_dperrory->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-Y
        ui->lineEdit_up_dpdivx->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dpdivy->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_filmdia->setEnabled(false);//(new QIntValidator(500,5000, this));//上菲林直径
        ui->lineEdit_up_maxjb->setEnabled(false);//(new QIntValidator(1,10, this));//上框最大拒曝次数
        ui->lineEdit_up_boarddia->setEnabled(false);//(new QIntValidator(500,5000, this));//上底板直径
        ui->lineEdit_up_dwjingdu->setEnabled(false);//(new QIntValidator(3,30, this));//上框对位完成精度

        ui->lineEdit_down_checkdelay->setEnabled(false);//(new QIntValidator(0,10000, this));//下框检测延时时间
        ui->lineEdit_down_dperrorx->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-X
        ui->lineEdit_down_dperrory->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-Y
        ui->lineEdit_down_dpdivx->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dpdivy->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_filmdia->setEnabled(false);//(new QIntValidator(500,5000, this));//下菲林直径
        ui->lineEdit_down_maxjb->setEnabled(false);//(new QIntValidator(1,10, this));//下框最大拒曝次数
        ui->lineEdit_down_boarddia->setEnabled(false);//(new QIntValidator(500,5000, this));//下底板直径
        ui->lineEdit_down_dwjingdu->setEnabled(false);//(new QIntValidator(3,30, this));//下框对位完成精度

        ui->lineEdit_up_checkdelay_2->setEnabled(false);//(new QIntValidator(0,10000, this));//上框检测延时时间
        ui->lineEdit_up_dperrorx_2->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-X
        ui->lineEdit_up_dperrory_2->setEnabled(false);//(new QIntValidator(5,999, this));//上框底片误差值+-Y
        ui->lineEdit_up_dpdivx_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_dpdivy_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_up_filmdia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//上菲林直径
        ui->lineEdit_up_maxjb_2->setEnabled(false);//(new QIntValidator(1,10, this));//上框最大拒曝次数
        ui->lineEdit_up_boarddia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//上底板直径
        ui->lineEdit_up_dwjingdu_2->setEnabled(false);//(new QIntValidator(3,30, this));//上框对位完成精度

        ui->lineEdit_down_checkdelay_2->setEnabled(false);//(new QIntValidator(0,10000, this));//下框检测延时时间
        ui->lineEdit_down_dperrorx_2->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-X
        ui->lineEdit_down_dperrory_2->setEnabled(false);//(new QIntValidator(5,999, this));//下框底片误差值+-Y
        ui->lineEdit_down_dpdivx_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_dpdivy_2->setEnabled(false);//(new QIntValidator(3,999, this));
        ui->lineEdit_down_filmdia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//下菲林直径
        ui->lineEdit_down_maxjb_2->setEnabled(false);//(new QIntValidator(1,10, this));//下框最大拒曝次数
        ui->lineEdit_down_boarddia_2->setEnabled(false);//(new QIntValidator(500,5000, this));//下底板直径
        ui->lineEdit_down_dwjingdu_2->setEnabled(false);//(new QIntValidator(3,30, this));//下框对位完成精度
        //平台设置
        ui->lineEdit_up_piecehightmil->setEnabled(false);//(new QIntValidator(0,119, this));//上框工件板厚mil
        ui->lineEdit_up_piecehight->setEnabled(false);//(new QIntValidator(0,3000, this));//上框工件板厚um
        ui->lineEdit_up_dwdistance->setEnabled(false);//(new QIntValidator(0,3000, this));//上框对位间距
        ui->lineEdit_down_piecehightmil->setEnabled(false);//(new QIntValidator(0,119, this));//上框工件板厚mil
        ui->lineEdit_down_piecehight->setEnabled(false);//(new QIntValidator(0,3000, this));//上框工件板厚um
        ui->lineEdit_down_dwdistance->setEnabled(false);//(new QIntValidator(0,3000, this));//上框对位间距

        ui->lineEdit_up_xlength->setEnabled(false);//(new QIntValidator(0,1999, this));
        ui->lineEdit_up_ylength->setEnabled(false);//(new QIntValidator(0,1999, this));
        ui->lineEdit_down_xlength->setEnabled(false);//(new QIntValidator(0,1999, this));
        ui->lineEdit_down_ylength->setEnabled(false);//(new QIntValidator(0,1999, this));
    }
}

void dlgBGParameter::on_bginput_okEvent()
{
    g_beeptype = 1;
    if(IsParaChanged())
    {
       SavePara();
       g_reloadpara = true;//update para
    }
//    qDebug()<<QString::number(g_par.quick_door_time,10);
}

void dlgBGParameter::on_bginput_cancelEvent()
{
    g_beeptype = 1;
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

void dlgBGParameter::on_radioButton_bgpara_clicked()
{
   g_beeptype = 1;
   ui->stackedWidget_bgpara->setCurrentIndex(0);
   ui->btn_ccd2->hide();
   ui->btn_ccd3->hide();
   ui->btn_ccd4->hide();
}
//上框上底片
void dlgBGParameter::on_radioButton_upccd_check_clicked()
{
   g_beeptype = 1;
   ui->stackedWidget_bgpara->setCurrentIndex(1);
   ui->btn_ccd2->show();
   ui->btn_ccd3->show();
   ui->btn_ccd4->show();
   //ccd 2 3 4
   switch (m_iUpCCDSelect[0]) {
   case 0:
       ui->btn_ccd2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
       ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   case 1:
       ui->btn_ccd3->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
       ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   case 2:
       ui->btn_ccd4->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
       ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   default:
       ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   }
}

//下框上底片
void dlgBGParameter::on_radioButton_downccd_check_clicked()
{
   g_beeptype = 1;
   ui->stackedWidget_bgpara->setCurrentIndex(2);
   ui->btn_ccd2->show();
   ui->btn_ccd3->show();
   ui->btn_ccd4->show();
   switch (m_iUpCCDSelect[1]) {
   case 0:
       ui->btn_ccd2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
       ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   case 1:
       ui->btn_ccd3->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
       ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   case 2:
       ui->btn_ccd4->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
       ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   default:
       ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       break;
   }
}

void dlgBGParameter::on_radioButton_ptsetting_clicked()
{
   g_beeptype = 1;
   ui->stackedWidget_bgpara->setCurrentIndex(4);
   ui->btn_ccd2->hide();
   ui->btn_ccd3->hide();
   ui->btn_ccd4->hide();
}

void dlgBGParameter::on_btn_fullpower_clicked()
{
    g_beeptype = 1;
    ui->btn_fullpower->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    ui->btn_halfpower->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    m_bfull = 0;
}

void dlgBGParameter::on_btn_halfpower_clicked()
{
    g_beeptype = 1;
    ui->btn_halfpower->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    ui->btn_fullpower->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    m_bfull = 1;
}

void dlgBGParameter::on_btn_energy_clicked()
{
    g_beeptype = 1;
    if((m_sUpBG_energy.toInt()==0)||(m_sDownBG_energy.toInt()==0))
    {
           QMessageBox::information(this, "TOP", QObject::tr("设定能量模式，曝光能量必须大于0"));
           return;
    }
    ui->btn_energy->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    ui->btn_time->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    m_benergyortime = 0;
}

void dlgBGParameter::on_btn_time_clicked()
{
    g_beeptype = 1;
    ui->btn_time->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    ui->btn_energy->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    m_benergyortime = 1;
}

void dlgBGParameter::on_btn_auto_clicked()
{
    g_beeptype = 1;
    ui->btn_auto->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    ui->btn_manual->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    m_bmannual = 0;
}

void dlgBGParameter::on_btn_manual_clicked()
{
    g_beeptype = 1;
    ui->btn_manual->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    ui->btn_auto->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
    m_bmannual = 1;
}

void dlgBGParameter::on_btn_upframe_clicked()
{
    g_beeptype = 1;
//    if((m_iFrameSelect<0)||(m_iFrameSelect>2))
//    {
//        m_iFrameSelect = 2;
//    }
//    else
//    {
//        if(m_iFrameSelect==2) m_iFrameSelect = 1;
//        else if(m_iFrameSelect==1) m_iFrameSelect = 2;
//        else if(m_iFrameSelect==0) m_iFrameSelect = 1;
//    }
    switch(m_iFrameSelect)
    {
    case 0:
        ui->btn_upframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_downframe->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        break;
    case 1:
        ui->btn_downframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_upframe->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        break;
    case 2:
        ui->btn_upframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_downframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        break;
    }
}

void dlgBGParameter::on_btn_downframe_clicked()
{

    g_beeptype = 1;
//    if((m_iFrameSelect<0)||(m_iFrameSelect>2))
//    {
//        m_iFrameSelect = 2;
//    }
//    else
//    {
//        if(m_iFrameSelect==2) m_iFrameSelect = 0;
//        else if(m_iFrameSelect==1) m_iFrameSelect = 0;
//        else if(m_iFrameSelect==0) m_iFrameSelect = 2;
//    }
    switch(m_iFrameSelect)
    {
    case 0:
        ui->btn_upframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_downframe->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        break;
    case 1:
        ui->btn_downframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_upframe->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        break;
    case 2:
        ui->btn_upframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        ui->btn_downframe->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
        break;
    }
}

void dlgBGParameter::on_btn_ccd2_clicked()
{
   g_beeptype = 1;
   if(m_bCancelTow)
   {
       return;
   }
   if(ui->radioButton_upccd_check->isChecked())
   {
       if(m_iUpCCDSelect[0]==0)
       {
           m_iUpCCDSelect[0] = 3;
           ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
           ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
           ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       }
       else
       {
           m_iUpCCDSelect[0] =0;
           ui->btn_ccd2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
           ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
           ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       }
   }
   if(ui->radioButton_downccd_check->isChecked())
   {
       if(m_iUpCCDSelect[1]==0)
       {
           m_iUpCCDSelect[1] = 3;
           ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
           ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
           ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       }
       else
       {
           m_iUpCCDSelect[1] =0;
           ui->btn_ccd2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
           ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
           ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
       }
   }
}

void dlgBGParameter::on_btn_ccd3_clicked()
{
    g_beeptype = 1;
    if(m_bCancelThree)
    {
        return;
    }
    if(ui->radioButton_upccd_check->isChecked())
    {
        if(m_iUpCCDSelect[0]==1)
        {
            m_iUpCCDSelect[0] = 3;
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
        else
        {
            m_iUpCCDSelect[0] =1;
            ui->btn_ccd3->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
    }
    if(ui->radioButton_downccd_check->isChecked())
    {
        if(m_iUpCCDSelect[1]==1)
        {
            m_iUpCCDSelect[1] = 3;
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
        else
        {
            m_iUpCCDSelect[1] =1;
            ui->btn_ccd3->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
    }
}

void dlgBGParameter::on_btn_ccd4_clicked()
{
    g_beeptype = 1;
    if(ui->radioButton_upccd_check->isChecked())
    {
        if(m_iUpCCDSelect[0]==2)
        {
            m_iUpCCDSelect[0] = 3;
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
        else
        {
            m_iUpCCDSelect[0] =2;
            ui->btn_ccd4->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
            ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
    }
    if(ui->radioButton_downccd_check->isChecked())
    {
        if(m_iUpCCDSelect[1]==2)
        {
            m_iUpCCDSelect[1] = 3;
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd4->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
        else
        {
            m_iUpCCDSelect[1] =2;
            ui->btn_ccd4->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
            ui->btn_ccd3->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
            ui->btn_ccd2->setStyleSheet("background:darkgray;font-size:13px;font:bold;");
        }
    }
}
void dlgBGParameter::on_btn_up_ccd1234_clicked()
{
    g_beeptype = 1;
    m_btn_upccd[0]+=1;
    if(m_btn_upccd[0]>3)
    {
        m_btn_upccd[0] =0;
        ui->btn_up_ccd1234->setText("CCD 1-2");
    }
    else if(m_btn_upccd[0]==1)
    {
        ui->btn_up_ccd1234->setText("CCD 3-4");
    }
    else if(m_btn_upccd[0]==2)
    {
        ui->btn_up_ccd1234->setText("CCD 1-4");
    }
    else if(m_btn_upccd[0]==3)
    {
        ui->btn_up_ccd1234->setText("CCD 2-3");
    }
}

void dlgBGParameter::on_btn_up_ccd1234_2_clicked()
{
    g_beeptype = 1;
    m_btn_upccd[1]+=1;
    if(m_btn_upccd[1]>3)
    {
        m_btn_upccd[1] =0;
        ui->btn_up_ccd1234_2->setText("CCD 1-2");
    }
    else if(m_btn_upccd[1]==1)
    {
        ui->btn_up_ccd1234_2->setText("CCD 3-4");
    }
    else if(m_btn_upccd[1]==2)
    {
        ui->btn_up_ccd1234_2->setText("CCD 1-4");
    }
    else if(m_btn_upccd[1]==3)
    {
        ui->btn_up_ccd1234_2->setText("CCD 2-3");
    }
}

void dlgBGParameter::on_btn_up_checkallmarks_clicked()
{
    g_beeptype = 1;
    m_btn_upcheckmark[0] = !m_btn_upcheckmark[0];
    if(m_btn_upcheckmark[0])
    {
        ui->btn_up_checkallmarks->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_up_checkallmarks->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
}
void dlgBGParameter::on_btn_up_checkallmarks_2_clicked()
{
    g_beeptype = 1;
    m_btn_upcheckmark[1] = !m_btn_upcheckmark[1];
    if(m_btn_upcheckmark[1])
    {
        ui->btn_up_checkallmarks_2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_up_checkallmarks_2->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
}
void dlgBGParameter::on_btn_down_ccd1234_clicked()
{
    g_beeptype = 1;
    m_btn_downccd[0]+=1;
    if(m_btn_downccd[0]>3)
    {
        m_btn_downccd[0] =0;
        ui->btn_down_ccd1234->setText("CCD 1-2");
    }
    else if(m_btn_downccd[0]==1)
    {
        ui->btn_down_ccd1234->setText("CCD 3-4");
    }
    else if(m_btn_downccd[0]==2)
    {
        ui->btn_down_ccd1234->setText("CCD 1-4");
    }
    else if(m_btn_downccd[0]==3)
    {
        ui->btn_down_ccd1234->setText("CCD 2-3");
    }
}

void dlgBGParameter::on_btn_down_ccd1234_2_clicked()
{
    g_beeptype = 1;
    m_btn_downccd[1]+=1;
    if(m_btn_downccd[1]>3)
    {
        m_btn_downccd[1] =0;
        ui->btn_down_ccd1234_2->setText("CCD 1-2");
    }
    else if(m_btn_downccd[1]==1)
    {
        ui->btn_down_ccd1234_2->setText("CCD 3-4");
    }
    else if(m_btn_downccd[1]==2)
    {
        ui->btn_down_ccd1234_2->setText("CCD 1-4");
    }
    else if(m_btn_downccd[1]==3)
    {
        ui->btn_down_ccd1234_2->setText("CCD 2-3");
    }
}

void dlgBGParameter::on_btn_down_checkallmarks_clicked()
{
    g_beeptype = 1;
    m_btn_downcheckmark[0] = !m_btn_downcheckmark[0];
    if(m_btn_downcheckmark[0])
    {
        ui->btn_down_checkallmarks->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_down_checkallmarks->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
}

void dlgBGParameter::on_btn_down_checkallmarks_2_clicked()
{
    g_beeptype = 1;
    m_btn_downcheckmark[1] = !m_btn_downcheckmark[1];
    if(m_btn_downcheckmark[1])
    {
        ui->btn_down_checkallmarks_2->setStyleSheet("background-color: rgb(0, 254, 0);font-size:13px;font:bold;");
    }
    else
    {
        ui->btn_down_checkallmarks_2->setStyleSheet("background-color: rgb(196, 196, 196);font-size:13px;font:bold;");
    }
}

void dlgBGParameter::on_checkBox_up_checkdistance_clicked()
{
   g_beeptype = 1;
   if(ui->checkBox_up_checkdistance->isChecked())
   {
       m_bCheckbox_UpCheckDis = 1;
   }
   else
   {
       m_bCheckbox_UpCheckDis = 0;
   }
}

void dlgBGParameter::on_checkBox_down_checkdistance_clicked()
{
    g_beeptype = 1;
    if(ui->checkBox_down_checkdistance->isChecked())
    {
        m_bCheckbox_DownCheckDis = 1;
    }
    else
    {
        m_bCheckbox_DownCheckDis = 0;
    }
}

void dlgBGParameter::on_checkBox_up_cycle_clicked()
{
    g_beeptype = 1;
    if(ui->checkBox_up_cycle->isChecked())
    {
        m_bCheckbox_Upcycle = 1;
    }
    else
    {
        m_bCheckbox_Upcycle = 0;
    }
}

void dlgBGParameter::on_checkBox_down_cycle_clicked()
{
    g_beeptype = 1;
    if(ui->checkBox_down_cycle->isChecked())
    {
        m_bCheckbox_Downcycle = 1;
    }
    else
    {
        m_bCheckbox_Downcycle = 0;
    }
}

void dlgBGParameter::on_checkBox_binarization_clicked()
{
    g_beeptype = 1;
    if(ui->checkBox_binarization->isChecked())
    {
        m_bCheckbox_Binary = 1;
    }
    else
    {
        m_bCheckbox_Binary = 0;
    }
}

bool dlgBGParameter::IsParaChanged()
{
    bool bChangeflg = false;
    int m_iIndex = -1;
    m_sUpBG_timedelay = ui->lineEdit_upbg_delay->text();
    m_sUpBG_time = ui->lineEdit_upbg_time->text();
    m_sUpBG_energy = ui->lineEdit_upbg_energy->text();
    m_sUpDP_clearcnt = ui->lineEdit_updpclear_count->text();
    m_sUpDP_bgcnt = ui->lineEdit_updpbg_count->text();
    //下框
    m_sDownBG_timedelay = ui->lineEdit_downbg_delay->text();
    m_sDownBG_time = ui->lineEdit_downbg_time->text();
    m_sDownBG_energy = ui->lineEdit_downbg_energy->text();
    m_sDownDP_clearcnt = ui->lineEdit_downdpclear_count->text();
    m_sDownDP_bgcnt = ui->lineEdit_downdpbg_count->text();
    m_sBG_finishair = ui->spinBox_zhenkongbaochi->text();
    m_sBG_switchboardtime = ui->lineEdit_switchboard_time->text();
    /**上框影像检测装置**/
    m_sUp_checkdelay = ui->lineEdit_up_checkdelay->text();
    m_sUp_maxjb = ui->lineEdit_up_maxjb->text();
    m_sUp_dwjingdu = ui->lineEdit_up_dwjingdu->text();
    m_sUp_dperror_x = ui->lineEdit_up_dperrorx->text();
    m_sUp_dperror_y = ui->lineEdit_up_dperrory->text();
    m_sUp_dpdiv_x = ui->lineEdit_up_dpdivx->text();
    m_sUp_dpdiv_y = ui->lineEdit_up_dpdivy->text();
    m_sUp_filmDia = ui->lineEdit_up_filmdia->text();
    m_sUp_boardDia = ui->lineEdit_up_boarddia->text();

    m_sUp_checkdelay_2 = ui->lineEdit_up_checkdelay_2->text();
    m_sUp_maxjb_2 = ui->lineEdit_up_maxjb_2->text();
    m_sUp_dwjingdu_2 = ui->lineEdit_up_dwjingdu_2->text();
    m_sUp_dperror_x_2 = ui->lineEdit_up_dperrorx_2->text();
    m_sUp_dperror_y_2 = ui->lineEdit_up_dperrory_2->text();
    m_sUp_dpdiv_x_2 = ui->lineEdit_up_dpdivx_2->text();
    m_sUp_dpdiv_y_2 = ui->lineEdit_up_dpdivy_2->text();
    m_sUp_filmDia_2 = ui->lineEdit_up_filmdia_2->text();
    m_sUp_boardDia_2 = ui->lineEdit_up_boarddia_2->text();
    /**下框影像检测装置**/
    m_sDown_checkdelay = ui->lineEdit_down_checkdelay->text();
    m_sDown_maxjb = ui->lineEdit_down_maxjb->text();
    m_sDown_dwjingdu = ui->lineEdit_down_dwjingdu->text();
    m_sDown_dperror_x = ui->lineEdit_down_dperrorx->text();
    m_sDown_dperror_y = ui->lineEdit_down_dperrory->text();
    m_sDown_dpdiv_x = ui->lineEdit_down_dpdivx->text();
    m_sDown_dpdiv_y = ui->lineEdit_down_dpdivy->text();
    m_sDown_filmDia = ui->lineEdit_down_filmdia->text();
    m_sDown_boardDia = ui->lineEdit_down_boarddia->text();

    m_sDown_checkdelay_2 = ui->lineEdit_down_checkdelay_2->text();
    m_sDown_maxjb_2 = ui->lineEdit_down_maxjb_2->text();
    m_sDown_dwjingdu_2 = ui->lineEdit_down_dwjingdu_2->text();
    m_sDown_dperror_x_2 = ui->lineEdit_down_dperrorx_2->text();
    m_sDown_dperror_y_2 = ui->lineEdit_down_dperrory_2->text();
    m_sDown_dpdiv_x_2 = ui->lineEdit_down_dpdivx_2->text();
    m_sDown_dpdiv_y_2 = ui->lineEdit_down_dpdivy_2->text();
    m_sDown_filmDia_2 = ui->lineEdit_down_filmdia_2->text();
    m_sDown_boardDia_2 = ui->lineEdit_down_boarddia_2->text();
    /**平台设置**/
    m_sUp_boardHeightMil = ui->lineEdit_up_piecehightmil->text();
    m_sDown_boardHeightMil = ui->lineEdit_down_piecehightmil->text();
    //
    if(false==ui->lineEdit_up_piecehight->hasAcceptableInput())
    {
         ui->lineEdit_up_piecehight->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(254, 0, 0);");
         QMessageBox::information(this, "TOP", QObject::tr("参数输入错误或超出范围，请检查!"));
         return bChangeflg;
    }
    ui->lineEdit_up_piecehight->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(255, 255, 255);");
    m_sUp_boardHeight = ui->lineEdit_up_piecehight->text();
    //
    if(false==ui->lineEdit_down_piecehight->hasAcceptableInput())
    {
         ui->lineEdit_down_piecehight->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(254, 0, 0);");
         QMessageBox::information(this, "TOP", QObject::tr("参数输入错误或超出范围，请检查!"));
         return bChangeflg;
    }
    ui->lineEdit_down_piecehight->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(255, 255, 255);");
    m_sDown_boardHeight = ui->lineEdit_down_piecehight->text();
    //
    if(false==ui->lineEdit_up_dwdistance->hasAcceptableInput())
    {
         ui->lineEdit_up_dwdistance->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(254, 0, 0);");
         QMessageBox::information(this, "TOP", QObject::tr("参数输入错误或超出范围，请检查!"));
         return bChangeflg;
    }
    ui->lineEdit_up_dwdistance->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(255, 255, 255);");
    m_sUp_dwDistance = ui->lineEdit_up_dwdistance->text();
    //
    if(false==ui->lineEdit_down_dwdistance->hasAcceptableInput())
    {
         ui->lineEdit_down_dwdistance->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(254, 0, 0);");
         QMessageBox::information(this, "TOP", QObject::tr("参数输入错误或超出范围，请检查!"));
         return bChangeflg;
    }
    ui->lineEdit_down_dwdistance->setStyleSheet("font-family:Sans Serif;font-size:17px;background-color: rgb(255, 255, 255);");
    m_sDown_dwDistance = ui->lineEdit_down_dwdistance->text();
    m_sUp_xlength = ui->lineEdit_up_xlength->text();
    m_sDown_xlength = ui->lineEdit_down_xlength->text();
    m_sUp_ylength = ui->lineEdit_up_ylength->text();
    m_sDown_ylength = ui->lineEdit_down_ylength->text();
    if(m_sUpBG_timedelay.compare(m_sUpBG_timedelay_back))
    {
        bChangeflg = true;
        m_iIndex = 0;
    }
    if(m_sUpBG_time.compare(m_sUpBG_time_back))
    {
        bChangeflg = true;
        m_iIndex = 1;
    }
    if(m_sUpBG_energy.compare(m_sUpBG_energy_back))
    {
        bChangeflg = true;
        m_iIndex = 2;
    }
    if(m_sUpDP_clearcnt.compare(m_sUpDP_clearcnt_back))
    {
        bChangeflg = true;
        m_iIndex = 3;
    }
    if(m_sUpDP_bgcnt.compare(m_sUpDP_bgcnt_back))
    {
        bChangeflg = true;
        m_iIndex = 4;
    }
    if(m_sDownBG_timedelay.compare(m_sDownBG_timedelay_back))
    {
        bChangeflg = true;
        m_iIndex = 5;
    }
    if(m_sDownBG_time.compare(m_sDownBG_time_back))
    {
        bChangeflg = true;
        m_iIndex = 6;
    }
    if(m_sDownBG_energy.compare(m_sDownBG_energy_back))
    {
        bChangeflg = true;
        m_iIndex = 7;
    }
    if(m_sDownDP_clearcnt.compare(m_sDownDP_clearcnt_back))
    {
        bChangeflg = true;
        m_iIndex = 8;
    }
    if(m_sDownDP_bgcnt.compare(m_sDownDP_bgcnt_back))
    {
        bChangeflg = true;
        m_iIndex = 9;
    }
    if(m_sBG_finishair.compare(m_sBG_finishair_back))
    {
        bChangeflg = true;
        m_iIndex = 10;
    }
    if(m_sBG_switchboardtime.compare(m_sBG_switchboardtime_back))
    {
        bChangeflg = true;
        m_iIndex = 11;
    }
    //
    if(m_sUp_checkdelay.compare(m_sUp_checkdelay_back))
    {
        bChangeflg = true;
        m_iIndex = 12;
    }
    if(m_sUp_maxjb.compare(m_sUp_maxjb_back))
    {
        bChangeflg = true;
        m_iIndex = 13;
    }
    if(m_sUp_dwjingdu.compare(m_sUp_dwjingdu_back))
    {
        bChangeflg = true;
        m_iIndex = 14;
    }
    if(m_sUp_dperror_x.compare(m_sUp_dperror_x_back))
    {
        bChangeflg = true;
        m_iIndex = 15;
    }

    if(m_sUp_dperror_y.compare(m_sUp_dperror_y_back))
    {
        bChangeflg = true;
        m_iIndex = 16;
    }
    if(m_sUp_dpdiv_x.compare(m_sUp_dpdiv_x_back))
    {
        bChangeflg = true;
        m_iIndex = 17;
    }
    if(m_sUp_dpdiv_y.compare(m_sUp_dpdiv_y_back))
    {
        bChangeflg = true;
        m_iIndex = 18;
    }
    if(m_sUp_filmDia.compare(m_sUp_filmDia_back))
    {
        bChangeflg = true;
        m_iIndex = 19;
    }
    if(m_sUp_boardDia.compare(m_sUp_boardDia_back))
    {
        bChangeflg = true;
        m_iIndex = 20;
    }
    //
    if(m_sUp_checkdelay_2.compare(m_sUp_checkdelay_back_2))
    {
        bChangeflg = true;
        m_iIndex = 41;
    }
    if(m_sUp_maxjb_2.compare(m_sUp_maxjb_back_2))
    {
        bChangeflg = true;
        m_iIndex = 42;
    }
    if(m_sUp_dwjingdu_2.compare(m_sUp_dwjingdu_back_2))
    {
        bChangeflg = true;
        m_iIndex = 43;
    }
    if(m_sUp_dperror_x_2.compare(m_sUp_dperror_x_back_2))
    {
        bChangeflg = true;
        m_iIndex = 44;
    }

    if(m_sUp_dperror_y_2.compare(m_sUp_dperror_y_back_2))
    {
        bChangeflg = true;
        m_iIndex = 45;
    }
    if(m_sUp_dpdiv_x_2.compare(m_sUp_dpdiv_x_back_2))
    {
        bChangeflg = true;
        m_iIndex = 46;
    }
    if(m_sUp_dpdiv_y_2.compare(m_sUp_dpdiv_y_back_2))
    {
        bChangeflg = true;
        m_iIndex = 47;
    }
    if(m_sUp_filmDia_2.compare(m_sUp_filmDia_back_2))
    {
        bChangeflg = true;
        m_iIndex = 48;
    }
    if(m_sUp_boardDia_2.compare(m_sUp_boardDia_back_2))
    {
        bChangeflg = true;
        m_iIndex = 49;
    }
    //
    if(m_sDown_checkdelay.compare(m_sDown_checkdelay_back))
    {
        bChangeflg = true;
        m_iIndex = 21;
    }
    if(m_sDown_maxjb.compare(m_sDown_maxjb_back))
    {
        bChangeflg = true;
        m_iIndex = 22;
    }
    if(m_sDown_dwjingdu.compare(m_sDown_dwjingdu_back))
    {
        bChangeflg = true;
        m_iIndex = 23;
    }
    if(m_sDown_dperror_x.compare(m_sDown_dperror_x_back))
    {
        bChangeflg = true;
        m_iIndex = 24;
    }
    if(m_sDown_dperror_y.compare(m_sDown_dperror_y_back))
    {
        bChangeflg = true;
        m_iIndex = 25;
    }
    if(m_sDown_dpdiv_x.compare(m_sDown_dpdiv_x_back))
    {
        bChangeflg = true;
        m_iIndex = 26;
    }
    if(m_sDown_dpdiv_y.compare(m_sDown_dpdiv_y_back))
    {
        bChangeflg = true;
        m_iIndex = 27;
    }
    if(m_sDown_filmDia.compare(m_sDown_filmDia_back))
    {
        bChangeflg = true;
        m_iIndex = 28;
    }
    if(m_sDown_boardDia.compare(m_sDown_boardDia_back))
    {
        bChangeflg = true;
        m_iIndex = 29;
    }
    //
    if(m_sDown_checkdelay_2.compare(m_sDown_checkdelay_back_2))
    {
        bChangeflg = true;
        m_iIndex = 51;
    }
    if(m_sDown_maxjb_2.compare(m_sDown_maxjb_back_2))
    {
        bChangeflg = true;
        m_iIndex = 52;
    }
    if(m_sDown_dwjingdu_2.compare(m_sDown_dwjingdu_back_2))
    {
        bChangeflg = true;
        m_iIndex = 53;
    }
    if(m_sDown_dperror_x_2.compare(m_sDown_dperror_x_back_2))
    {
        bChangeflg = true;
        m_iIndex = 54;
    }
    if(m_sDown_dperror_y_2.compare(m_sDown_dperror_y_back_2))
    {
        bChangeflg = true;
        m_iIndex = 55;
    }
    if(m_sDown_dpdiv_x_2.compare(m_sDown_dpdiv_x_back_2))
    {
        bChangeflg = true;
        m_iIndex = 56;
    }
    if(m_sDown_dpdiv_y_2.compare(m_sDown_dpdiv_y_back_2))
    {
        bChangeflg = true;
        m_iIndex = 57;
    }
    if(m_sDown_filmDia_2.compare(m_sDown_filmDia_back_2))
    {
        bChangeflg = true;
        m_iIndex = 58;
    }
    if(m_sDown_boardDia_2.compare(m_sDown_boardDia_back_2))
    {
        bChangeflg = true;
        m_iIndex = 59;
    }
    //
    if(m_sUp_boardHeight.compare(m_sUp_boardHeight_back))
    {
        bChangeflg = true;
        m_iIndex = 30;
    }
    if(m_sDown_boardHeight.compare(m_sDown_boardHeight_back))
    {
        bChangeflg = true;
        m_iIndex = 31;
    }
    if(m_sUp_boardHeightMil.compare(m_sUp_boardHeightMil_back))
    {
        bChangeflg = true;
        m_iIndex = 60;
    }
    if(m_sDown_boardHeightMil.compare(m_sDown_boardHeightMil_back))
    {
        bChangeflg = true;
        m_iIndex = 61;
    }
    if(m_sUp_dwDistance.compare(m_sUp_dwDistance_back))
    {
        bChangeflg = true;
        m_iIndex = 32;
    }
    if(m_sDown_dwDistance.compare(m_sDown_dwDistance_back))
    {
        bChangeflg = true;
        m_iIndex = 33;
    }
    if(m_sUp_xlength.compare(m_sUp_xlength_back))
    {
        bChangeflg = true;
        m_iIndex = 34;
    }
    if(m_sDown_xlength.compare(m_sDown_xlength_back))
    {
        bChangeflg = true;
        m_iIndex = 35;
    }
    if(m_sUp_ylength.compare(m_sUp_ylength_back))
    {
        bChangeflg = true;
        m_iIndex = 36;
    }
    if(m_sDown_ylength.compare(m_sDown_ylength_back))
    {
        bChangeflg = true;
        m_iIndex = 37;
    }
    if(m_sUpBG_time2.compare(m_sUpBG_time2_back))
    {
        bChangeflg = true;
        m_iIndex = 38;
    }
    if(m_sDownBG_time2.compare(m_sDownBG_time2_back))
    {
        bChangeflg = true;
        m_iIndex = 39;
    }
    if((m_bfull_back!=m_bfull)||(m_bmannual_back!=m_bmannual)||(m_benergyortime_back!=m_benergyortime)
            ||(m_iFrameSelect_back!=m_iFrameSelect)||(m_iUpCCDSelect_back[0]!=m_iUpCCDSelect[0])||(m_iUpCCDSelect_back[1]!=m_iUpCCDSelect[1])
            ||(m_iDownCCDSelect_back[0]!=m_iDownCCDSelect[0])||(m_iDownCCDSelect_back[1]!=m_iDownCCDSelect[1])
            ||(m_btn_upccd_back[0]!=m_btn_upccd[0])||(m_btn_upccd_back[1]!=m_btn_upccd[1])
            ||(m_btn_upcheckmark_back[0]!=m_btn_upcheckmark[0])||(m_btn_downccd_back[0]!=m_btn_downccd[0])
            ||(m_btn_upcheckmark_back[1]!=m_btn_upcheckmark[1])||(m_btn_downccd_back[1]!=m_btn_downccd[1])
            ||(m_btn_downcheckmark_back[0]!=m_btn_downcheckmark[0])||(m_btn_downcheckmark_back[1]!=m_btn_downcheckmark[1])
            ||(m_bCheckbox_Upcycle_back!=m_bCheckbox_Upcycle)
            ||(m_bCheckbox_Downcycle_back!=m_bCheckbox_Downcycle)||(m_bCheckbox_UpCheckDis_back!=m_bCheckbox_UpCheckDis)
            ||(m_bCheckbox_DownCheckDis_back!=m_bCheckbox_DownCheckDis)||(m_bCheckbox_Binary_back!=m_bCheckbox_Binary))
    {
        bChangeflg = true;
        m_iIndex = 40;
    }
    qDebug()<<"m_iIndex="+QString::number(m_iIndex,10);
    return bChangeflg;
}
void dlgBGParameter::SavePara()
{
    CIniRW inifile(TOPPARAMETERPATH);
    if(m_sUpBG_time.toInt()>=g_par.quick_door_time)
    {
        char buf[100];
        sprintf(buf,"无法保存参数!\n上框曝光时间应小于全功限时(%d ms)!\n",g_par.quick_door_time);
        QMessageBox::information(this,"TOP",QObject::tr(buf));
        return;
    }
    else if(m_sDownBG_time.toInt()>=g_par.quick_door_time)
    {
        char buf[100];
        sprintf(buf,"无法保存参数!\n下框曝光时间应小于全功限时(%d ms)!\n",g_par.quick_door_time);
        QMessageBox::information(this,"TOP",QObject::tr(buf));
        return;
    }
    else if(m_sUpBG_time.toInt()<g_par.min_bgtime)
    {
        char buf[100];
        sprintf(buf,"上框曝光时间(%d ms)小于曝光时间下限(%d ms),是否保存?\n",m_sUpBG_time.toInt(),g_par.min_bgtime);
        int res = QMessageBox::information(this, "TOP", QObject::tr(buf),QMessageBox::Yes|QMessageBox::No);
        if(res==QMessageBox::No) return;
    }
//    else if(m_sDownBG_time.toInt()<g_par.min_bgtime)
//    {
//        char buf[100];
//        sprintf(buf,"下框曝光时间(%d ms)小于曝光时间下限(%d ms),是否保存?\n",m_sDownBG_time.toInt(),g_par.min_bgtime);
//        int res = QMessageBox::information(this, "TOP", QObject::tr(buf),QMessageBox::Yes|QMessageBox::No);
//        if(res==QMessageBox::No) return;
//    }
    /**曝光参数设置**/
    //上框
    g_par.bg_up_delaytime = m_sUpBG_timedelay.toFloat();
    g_par.bg_up_time = m_sUpBG_time.toInt();
    g_par.bg_up_time2 = m_sUpBG_time2.toInt();
    g_par.bg_up_energy = m_sUpBG_energy.toFloat();
    g_par.DP_maxclear_num = m_sUpDP_clearcnt.toInt();
    g_par.DP_BG_MAXNUM = m_sUpDP_bgcnt.toInt();
    inifile.WriteIni("TRAJ","BAO_GUANG_DELAY",m_sUpBG_timedelay);
    inifile.WriteIni("TRAJ","SHANG_DI_PIAN",m_sUpBG_time);
    inifile.WriteIni("TRAJ","SHANG_DI_PIAN2",m_sUpBG_time2);
    inifile.WriteIni("TRAJ","BAO_GUANG_ENERGY",m_sUpBG_energy);
    inifile.WriteIni("TRAJ","CLEAN_DIPIAN_COUNT",m_sUpDP_clearcnt);
    inifile.WriteIni("TRAJ","DIPIAN_COUNT",m_sUpDP_bgcnt);
    //下框
    g_par.bg_down_delaytime = m_sDownBG_timedelay.toInt();
    g_par.bg_down_time = m_sDownBG_time.toInt();
    g_par.bg_down_time2 = m_sDownBG_time2.toInt();
    g_par.bg_down_energy = m_sDownBG_energy.toFloat();
    g_par.DP2_maxclear_num = m_sDownDP_clearcnt.toInt();
    g_par.DP2_BG_MAXNUM = m_sDownDP_bgcnt.toInt();
    inifile.WriteIni("TRAJ","BAO_GUANG_DELAY2",m_sDownBG_timedelay);
    inifile.WriteIni("TRAJ","XIA_DI_PIAN",m_sDownBG_time);
    inifile.WriteIni("TRAJ","XIA_DI_PIAN2",m_sDownBG_time2);
    inifile.WriteIni("TRAJ","BAO_GUANG_ENERGY2",m_sDownBG_energy);
    inifile.WriteIni("TRAJ","CLEAN_DIPIAN_COUNT2",m_sDownDP_clearcnt);
    inifile.WriteIni("TRAJ","DIPIAN_COUNT2",m_sDownDP_bgcnt);
    //
    g_par.isbg_finishairkeep=m_sBG_finishair.toInt();
    g_par.bg_switchboardtime = m_sBG_switchboardtime.toInt();
    inifile.WriteIni("TRAJ","AIR_KEEP",m_sBG_finishair);
    inifile.WriteIni("TRAJ","QU_FANG_TIME",m_sBG_switchboardtime);
    /**上框影像检测装置**/
    g_par.CCD_check_time = m_sUp_checkdelay.toInt();
    g_par.DW_max_num = m_sUp_maxjb.toInt();
    g_par.dw_up_jingdu =m_sUp_dwjingdu.toFloat();
    g_par.DP_up_error_x =m_sUp_dperror_x.toFloat();
    g_par.DP_up_error_y =m_sUp_dperror_y.toFloat();
    g_par.DP_up_junfen_x = m_sUp_dpdiv_x.toFloat();
    g_par.DP_up_junfen_y = m_sUp_dpdiv_y.toFloat();
    g_par.film_up_dia =m_sUp_filmDia.toFloat();
    g_par.board_up_dia =m_sUp_boardDia.toFloat();
    inifile.WriteIni("TRAJ","CHECK_DELAY",m_sUp_checkdelay);
    inifile.WriteIni("TRAJ","DW_MAX_NUM",m_sUp_maxjb);
    inifile.WriteIni("TRAJ","DUIWEI_FINISH_PRECISION",m_sUp_dwjingdu);
    inifile.WriteIni("TRAJ","DIPIAN_WUCHA_X",m_sUp_dperror_x);
    inifile.WriteIni("TRAJ","DIPIAN_WUCHA_Y",m_sUp_dperror_y);
    inifile.WriteIni("TRAJ","JUNFEN_WUCHA_X",m_sUp_dpdiv_x);
    inifile.WriteIni("TRAJ","JUNFEN_WUCHA_Y",m_sUp_dpdiv_y);
    inifile.WriteIni("TRAJ","UP_FILM_D",m_sUp_filmDia);
    inifile.WriteIni("TRAJ","BOARD_D",m_sUp_boardDia);

    /**下框影像检测装置**/
    g_par.CCD_check_time2 = m_sDown_checkdelay.toInt();
    g_par.DW_max_num2 = m_sDown_maxjb.toInt();
    g_par.dw_down_jingdu =m_sDown_dwjingdu.toFloat();
    g_par.DP_down_error_x =m_sDown_dperror_x.toFloat();
    g_par.DP_down_error_y =m_sDown_dperror_y.toFloat();
    g_par.DP_down_junfen_x = m_sDown_dpdiv_x.toFloat();
    g_par.DP_down_junfen_y = m_sDown_dpdiv_y.toFloat();
    g_par.film_down_dia =m_sDown_filmDia.toFloat();
    g_par.board_down_dia =m_sDown_boardDia.toFloat();
    inifile.WriteIni("TRAJ","CHECK_DELAY2",m_sDown_checkdelay);
    inifile.WriteIni("TRAJ","DW_MAX_NUM2",m_sDown_maxjb);
    inifile.WriteIni("TRAJ","DUIWEI_FINISH_PRECISION2",m_sDown_dwjingdu);
    inifile.WriteIni("TRAJ","DIPIAN_WUCHA_X2",m_sDown_dperror_x);
    inifile.WriteIni("TRAJ","DIPIAN_WUCHA_Y2",m_sDown_dperror_y);
    inifile.WriteIni("TRAJ","JUNFEN_WUCHA_X2",m_sDown_dpdiv_x);
    inifile.WriteIni("TRAJ","JUNFEN_WUCHA_Y2",m_sDown_dpdiv_y);
    inifile.WriteIni("TRAJ","DOWN_FILM_D",m_sDown_filmDia);
    inifile.WriteIni("TRAJ","BOARD_D2",m_sDown_boardDia);


    /**平台设置**/
    g_par.up_bm_height_mil = m_sUp_boardHeightMil.toFloat();
    g_par.down_bm_height_mil = m_sDown_boardHeightMil.toFloat();
    g_par.up_bm_height = m_sUp_boardHeight.toFloat();
    g_par.down_bm_height = m_sDown_boardHeight.toFloat();
    g_par.up_dw_height = m_sUp_dwDistance.toFloat();
    g_par.down_dw_height = m_sDown_dwDistance.toFloat();
    g_par.up_xlength = m_sUp_xlength.toFloat();
    g_par.up_ylength = m_sUp_ylength.toFloat();
    g_par.down_xlength = m_sDown_xlength.toFloat();
    g_par.down_ylength = m_sUp_ylength.toFloat();

    inifile.WriteIni("TRAJ","PRODUCT_THICKNESS_MIL",m_sUp_boardHeightMil);
    inifile.WriteIni("TRAJ","PRODUCT_THICKNESS_MIL2",m_sDown_boardHeightMil);
    inifile.WriteIni("TRAJ","PRODUCT_THICKNESS_UM",m_sUp_boardHeight);
    inifile.WriteIni("TRAJ","PRODUCT_THICKNESS_UM2",m_sDown_boardHeight);
    inifile.WriteIni("TRAJ","DUIWEI_DISTANCE",m_sUp_dwDistance);
    inifile.WriteIni("TRAJ","DUIWEI_DISTANCE2",m_sDown_dwDistance);
    inifile.WriteIni("TRAJ","WIDTH",m_sUp_xlength);
    inifile.WriteIni("TRAJ","WIDTH2",m_sDown_xlength);
    inifile.WriteIni("TRAJ","HEIGHT",m_sUp_ylength);
    inifile.WriteIni("TRAJ","HEIGHT2",m_sDown_ylength);
    //
    QString tmp;
    tmp = QString::number(m_iFrameSelect,10);
    inifile.WriteIni("TOP","FRAME_SELECT",tmp);
    g_par.Frame_select = 0;

    tmp = QString::number(m_bfull,10);
    inifile.WriteIni("TRAJ","QUANGONG_OR_BANGONG",tmp);
    g_par.full_half = m_bfull;

    tmp = QString::number(m_bmannual,10);
    inifile.WriteIni("TRAJ","AUTO_OR_MANUAL",tmp);
    g_par.auto_manual = m_bmannual;

    tmp = QString::number(m_benergyortime,10);
    inifile.WriteIni("TRAJ","ENERGY_OR_TIME",tmp);
    g_par.bg_mode = m_benergyortime;

    tmp = QString::number(m_iUpCCDSelect[0],10);
    inifile.WriteIni("TRAJ","CCD_234",tmp);
    g_par.ccd_234[0]=m_iUpCCDSelect[0];

    tmp = QString::number(m_iUpCCDSelect[1],10);
    inifile.WriteIni("TRAJ","CCD_234_2",tmp);
    g_par.ccd_234[1]=m_iUpCCDSelect[1];

    tmp = QString::number(m_iDownCCDSelect[0],10);
    inifile.WriteIni("TRAJ","DOWNCCD_234",tmp);
    g_par.downccd_234[0]=m_iDownCCDSelect[0];

    tmp = QString::number(m_iDownCCDSelect[1],10);
    inifile.WriteIni("TRAJ","DOWNCCD_234_2",tmp);
    g_par.downccd_234[1]=m_iDownCCDSelect[1];
    //
    tmp = QString::number(m_btn_upccd[0],10);
    inifile.WriteIni("TRAJ","CCD_SWITCH",tmp);
    g_par.up_ccd_select = m_btn_upccd[0];

    tmp = QString::number(m_btn_upccd[1],10);
    inifile.WriteIni("TRAJ","DOWNCCD_SWITCH",tmp);
    g_par.up_downccd_select = m_btn_upccd[1];

    tmp = QString::number(m_btn_downccd[0],10);
    inifile.WriteIni("TRAJ","CCD_SWITCH2",tmp);
    g_par.down_ccd_select = m_btn_downccd[0];

    tmp = QString::number(m_btn_downccd[1],10);
    inifile.WriteIni("TRAJ","DOWNCCD_SWITCH2",tmp);
    g_par.down_downccd_select = m_btn_downccd[1];

    tmp = QString::number(m_btn_upcheckmark[0],10);
    inifile.WriteIni("TOP","CHECK8",tmp);
    g_par.check8[0]=m_btn_upcheckmark[0];

    tmp = QString::number(m_btn_upcheckmark[1],10);
    inifile.WriteIni("TOP","DOWNCHECK8",tmp);
    g_par.checkdown8[0]=m_btn_upcheckmark[1];

    tmp = QString::number(m_btn_downcheckmark[0],10);
    inifile.WriteIni("TOP","CHECK8_2",tmp);
    g_par.check8[1]=m_btn_downcheckmark[0];

    tmp = QString::number(m_btn_downcheckmark[1],10);
    inifile.WriteIni("TOP","DOWNCHECK8_2",tmp);
    g_par.checkdown8[1]=m_btn_downcheckmark[1];
    //check box
    tmp = QString::number(m_bCheckbox_Upcycle,10);
    inifile.WriteIni("TRAJ","DW_KXY_BASE",tmp);
    g_par.dw_up_kxybase = m_bCheckbox_Upcycle;

    tmp = QString::number(m_bCheckbox_Downcycle,10);
    inifile.WriteIni("TRAJ","DW_KXY_BASE2",tmp);
    g_par.dw_down_kxybase = m_bCheckbox_Downcycle;

    tmp = QString::number(m_bCheckbox_UpCheckDis,10);
    inifile.WriteIni("TRAJ","GC1",tmp);
    g_par.up_CheckDis_GC1 = m_bCheckbox_UpCheckDis;

    tmp = QString::number(m_bCheckbox_DownCheckDis,10);
    inifile.WriteIni("TRAJ","GC2",tmp);
    g_par.down_CheckDis_GC2 = m_bCheckbox_DownCheckDis;

    tmp = QString::number(m_bCheckbox_Binary,10);
    inifile.WriteIni("TOP","EZH",tmp);
    g_par.isBinary = m_bCheckbox_Binary;
    /****备份参数******/
    //上框
    m_sUpBG_timedelay_back = m_sUpBG_timedelay;
    if(m_sUpBG_time_back.compare(m_sUpBG_time))
    {
        m_sUpBG_time_back = m_sUpBG_time  ;
    }
    if(m_sUpBG_time2_back.compare(m_sUpBG_time2))
    {
        m_sUpBG_time2_back = m_sUpBG_time2  ;
    }
    m_sUpBG_energy_back = m_sUpBG_energy  ;
    if(m_sUpDP_clearcnt_back.compare(m_sUpDP_clearcnt))
    {
       m_sUpDP_clearcnt_back = m_sUpDP_clearcnt  ;
    }
    if(m_sUpDP_bgcnt_back.compare(m_sUpDP_bgcnt))
    {
       m_sUpDP_bgcnt_back = m_sUpDP_bgcnt  ;
    }
    //下框
    m_sDownBG_timedelay_back = m_sDownBG_timedelay  ;
    if(m_sDownBG_time_back.compare(m_sDownBG_time))
    {
        m_sDownBG_time_back = m_sDownBG_time  ;

    }
    if(m_sDownBG_time2_back.compare(m_sDownBG_time2))
    {
        m_sDownBG_time2_back = m_sDownBG_time2  ;

    }
    m_sDownBG_energy_back = m_sDownBG_energy  ;
    if(m_sDownDP_clearcnt_back.compare(m_sDownDP_clearcnt))
    {
       m_sDownDP_clearcnt_back = m_sDownDP_clearcnt  ;
    }
    if(m_sDownDP_bgcnt_back.compare(m_sDownDP_bgcnt))
    {
       m_sDownDP_bgcnt_back = m_sDownDP_bgcnt  ;
    }
    m_sBG_finishair_back = m_sBG_finishair  ;
    m_sBG_switchboardtime_back = m_sBG_switchboardtime  ;
    /**上框影像检测装置**/
    m_sUp_checkdelay_back = m_sUp_checkdelay  ;
    m_sUp_maxjb_back = m_sUp_maxjb  ;
    m_sUp_dwjingdu_back = m_sUp_dwjingdu  ;
    m_sUp_dperror_x_back = m_sUp_dperror_x  ;
    m_sUp_dperror_y_back = m_sUp_dperror_y  ;
    m_sUp_dpdiv_x_back = m_sUp_dpdiv_x  ;
    m_sUp_dpdiv_y_back = m_sUp_dpdiv_y  ;
    m_sUp_filmDia_back = m_sUp_filmDia  ;
    m_sUp_boardDia_back = m_sUp_boardDia  ;

    /**下框影像检测装置**/
    m_sDown_checkdelay_back = m_sDown_checkdelay  ;
    m_sDown_maxjb_back = m_sDown_maxjb  ;
    m_sDown_dwjingdu_back = m_sDown_dwjingdu  ;
    m_sDown_dperror_x_back = m_sDown_dperror_x  ;
    m_sDown_dperror_y_back = m_sDown_dperror_y  ;
    m_sDown_dpdiv_x_back = m_sDown_dpdiv_x  ;
    m_sDown_dpdiv_y_back = m_sDown_dpdiv_y  ;
    m_sDown_filmDia_back = m_sDown_filmDia  ;
    m_sDown_boardDia_back = m_sDown_boardDia  ;

    /**平台设置**/
    m_sUp_boardHeightMil_back = m_sUp_boardHeightMil  ;
    m_sDown_boardHeightMil_back = m_sDown_boardHeightMil  ;
    m_sUp_boardHeight_back = m_sUp_boardHeight  ;
    m_sDown_boardHeight_back = m_sDown_boardHeight  ;
    m_sUp_dwDistance_back = m_sUp_dwDistance  ;
    m_sDown_dwDistance_back = m_sDown_dwDistance  ;
    m_sUp_xlength_back = m_sUp_xlength  ;
    m_sDown_xlength_back = m_sDown_xlength  ;
    m_sUp_ylength_back = m_sUp_ylength  ;
    m_sDown_ylength_back = m_sDown_ylength  ;
    //right btn status
    m_bfull_back = m_bfull  ;
    m_bmannual_back = m_bmannual  ;
    m_benergyortime_back = m_benergyortime  ;
    m_iFrameSelect_back = m_iFrameSelect  ;//
    m_iUpCCDSelect_back[0] = m_iUpCCDSelect[0]  ;
    m_iDownCCDSelect_back[0] = m_iDownCCDSelect[0]  ;
    m_iUpCCDSelect_back[1] = m_iUpCCDSelect[1]  ;
    m_iDownCCDSelect_back[1] = m_iDownCCDSelect[1]  ;
    //page btn
    m_btn_upccd_back[0] = m_btn_upccd[0]  ;
    m_btn_upcheckmark_back[0] = m_btn_upcheckmark[0]  ;
    m_btn_downccd_back[0] = m_btn_downccd[0]  ;
    m_btn_downcheckmark_back[0] = m_btn_downcheckmark[0]  ;

    m_btn_upccd_back[1] = m_btn_upccd[1]  ;
    m_btn_upcheckmark_back[1] = m_btn_upcheckmark[1]  ;
    m_btn_downccd_back[1] = m_btn_downccd[1]  ;
    m_btn_downcheckmark_back[1] = m_btn_downcheckmark[1]  ;
    //check box status
    m_bCheckbox_Upcycle_back = m_bCheckbox_Upcycle  ;
    m_bCheckbox_Downcycle_back = m_bCheckbox_Downcycle  ;
    m_bCheckbox_UpCheckDis_back = m_bCheckbox_UpCheckDis  ;
    m_bCheckbox_DownCheckDis_back = m_bCheckbox_DownCheckDis  ;
    m_bCheckbox_Binary_back = m_bCheckbox_Binary  ;
}

void dlgBGParameter::on_lineEdit_up_piecehightmil_textEdited(const QString &arg1)
{
    QString tempstr;
    QString valuestr=ui->lineEdit_up_piecehightmil->text();
    int value = valuestr.toInt()*miltoum;
    tempstr.sprintf("%d",value);
    ui->lineEdit_up_piecehight->setText(tempstr);
}

void dlgBGParameter::on_lineEdit_down_piecehightmil_textEdited(const QString &arg1)
{
    QString tempstr;
    QString valuestr=ui->lineEdit_down_piecehightmil->text();
    int value = valuestr.toInt()*miltoum;
    tempstr.sprintf("%d",value);
    ui->lineEdit_down_piecehight->setText(tempstr);
}

void dlgBGParameter::on_lineEdit_up_piecehight_textEdited(const QString &arg1)
{
    QString tempstr;
    QString valuestr=ui->lineEdit_up_piecehight->text();
    int value = valuestr.toInt()/miltoum;
    tempstr.sprintf("%d",value);
    ui->lineEdit_up_piecehightmil->setText(tempstr);
}

void dlgBGParameter::on_lineEdit_down_piecehight_textEdited(const QString &arg1)
{
    QString tempstr;
    QString valuestr=ui->lineEdit_down_piecehight->text();
    int value = valuestr.toInt()/miltoum;
    tempstr.sprintf("%d",value);
    ui->lineEdit_down_piecehightmil->setText(tempstr);
}


void dlgBGParameter::on_lineEdit_downbg_time_textChanged(const QString &arg1)
{
}
