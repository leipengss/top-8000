#include "dlgworkpiece.h"
#include "ui_workpiece.h"
#include<QPushButton>
#include "driver/global.h"
#include"filemanage/cinirw.h"
#include<QDebug>

DlgWorkpiece::DlgWorkpiece(bool isupframe,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWorkpiece)
{
    ui->setupUi(this);
    //
    m_bisUpframe = isupframe;
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    //
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QObject::tr("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QObject::tr("离开"));
    //
    bool clear=false;
    bool bg=false;
    int dp_clearnum=0;
    int dp_bgnum=0;
    int max_clear_num =0;
    int max_bg_num =0;
    QString str_dpclear,str_total,str_dpbg,str_jbtotal;
    if(m_bisUpframe)
    {
        max_clear_num = g_par.DP_maxclear_num;
        dp_clearnum = g_par.DP_clear_num;
        str_dpclear=QString::number(g_par.DP_clear_num,10);
        str_dpclear="("+str_dpclear+")";

        str_total=QString::number(g_par.Frame_finish_num,10);
        str_total="("+str_total+")";

        max_bg_num = g_par.DP_BG_MAXNUM;
        dp_bgnum = g_par.DP_BG_NUM;
        str_dpbg=QString::number(g_par.DP_BG_NUM,10);
        str_dpbg="("+str_dpbg+")";

        str_jbtotal=QString::number(g_par.JB_finish_num,10);
        str_jbtotal="("+str_jbtotal+")";
    }
    else
    {
        max_clear_num = g_par.DP2_maxclear_num;
        dp_clearnum = g_par.DP2_clear_num;
        str_dpclear=QString::number(g_par.DP2_clear_num,10);
        str_dpclear="("+str_dpclear+")";

        str_total=QString::number(g_par.Frame2_finish_num,10);
        str_total="("+str_total+")";

        max_bg_num = g_par.DP2_BG_MAXNUM;
        dp_bgnum = g_par.DP2_BG_NUM;
        str_dpbg=QString::number(g_par.DP2_BG_NUM,10);
        str_dpbg="("+str_dpbg+")";

        str_jbtotal=QString::number(g_par.JB2_finish_num,10);
        str_jbtotal="("+str_jbtotal+")";
    }
    ui->label_dp_clear->setText(str_dpclear);
    ui->label_dp_baoguang->setText(str_dpbg);
    ui->label_total_number->setText(str_total);
    ui->label_jubaonumber->setText(str_jbtotal);
    if(dp_clearnum>=max_clear_num) clear=true;
    if(dp_bgnum>=max_bg_num) bg=true;
    if(clear==false&&bg==false) ui->label_number_tip->setText(QObject::tr(""));
    else if(clear==true&&bg==false)
    {
      ui->label_number_tip->setText(QObject::tr("请进行底片清洁"));
      ui->box_dp_clear->setChecked(true);
    }
    else if(clear==false&&bg==true)
    {
        ui->label_number_tip->setText(QObject::tr("请更换菲林"));
        ui->box_dp_baoguang->setChecked(true);
    }
    else
    {
        ui->label_number_tip->setText(QObject::tr("请进行底片清洁与更换菲林"));
        ui->box_dp_baoguang->setChecked(true);
        ui->box_dp_clear->setChecked(true);
    }
}

DlgWorkpiece::~DlgWorkpiece()
{
    delete ui;
}

void DlgWorkpiece::on_buttonBox_accepted()
{
    if(ui->box_dp_clear->isChecked())
    {
        if(m_bisUpframe)
        {
            g_par.DP_clear_num = 0;
        }
        else
        {
            g_par.DP2_clear_num = 0;
        }
    }
    if(ui->box_dp_baoguang->isChecked())
    {
        if(m_bisUpframe)
        {
            g_par.DP_BG_NUM = 0;
        }
        else
        {
            g_par.DP2_BG_NUM = 0;
        }
    }
    if(ui->box_bg_totalnumber->isChecked())
    {
        if(m_bisUpframe)
        {
            g_par.Frame_finish_num = 0;
        }
        else
        {
            g_par.Frame2_finish_num = 0;
        }
    }
    if(ui->box_bg_jubaonumber->isChecked())
    {
        if(m_bisUpframe)
        {
            g_par.JB_finish_num = 0;
        }
        else
        {
            g_par.JB2_finish_num = 0;
        }
    }
    QString str_dpclear,str_total,str_dpbg,str_jbtotal;
    if(m_bisUpframe)
    {
        str_dpclear=QString::number(g_par.DP_clear_num,10);
        str_dpclear="("+str_dpclear+")";

        str_total=QString::number(g_par.Frame_finish_num,10);
        str_total="("+str_total+")";

        str_dpbg=QString::number(g_par.DP_BG_NUM,10);
        str_dpbg="("+str_dpbg+")";

        str_jbtotal=QString::number(g_par.JB_finish_num,10);
        str_jbtotal="("+str_jbtotal+")";
    }
    else
    {
        str_dpclear=QString::number(g_par.DP2_clear_num,10);
        str_dpclear="("+str_dpclear+")";

        str_total=QString::number(g_par.Frame2_finish_num,10);
        str_total="("+str_total+")";

        str_dpbg=QString::number(g_par.DP2_BG_NUM,10);
        str_dpbg="("+str_dpbg+")";

        str_jbtotal=QString::number(g_par.JB2_finish_num,10);
        str_jbtotal="("+str_jbtotal+")";
    }
    ui->label_dp_clear->setText(str_dpclear);
    ui->label_dp_baoguang->setText(str_dpbg);
    ui->label_total_number->setText(str_total);
    ui->label_jubaonumber->setText(str_jbtotal);
    //保存参数
    CIniRW filesave(TOPPARAMETERPATH);
    if(m_bisUpframe)
    {
        filesave.WriteIni("TOP","JB_FINISH_NUM",QString::number(g_par.JB_finish_num,10));
        filesave.WriteIni("TOP","FRAME_FINISH_NUM",QString::number(g_par.Frame_finish_num,10));
        filesave.WriteIni("TOP","DP_BG_NUM",QString::number(g_par.DP_BG_NUM,10));
        filesave.WriteIni("TOP","DP_CLEAR_NUM",QString::number(g_par.DP_clear_num,10));
    }
    else
    {
        filesave.WriteIni("TOP","JB2_FINISH_NUM",QString::number(g_par.JB2_finish_num,10));
        filesave.WriteIni("TOP","FRAME2_FINISH_NUM",QString::number(g_par.Frame2_finish_num,10));
        filesave.WriteIni("TOP","DP2_BG_NUM",QString::number(g_par.DP2_BG_NUM,10));
        filesave.WriteIni("TOP","DP2_CLEAR_NUM",QString::number(g_par.DP2_clear_num,10));
    }
}

void DlgWorkpiece::on_buttonBox_rejected()
{
    this->close();
}
