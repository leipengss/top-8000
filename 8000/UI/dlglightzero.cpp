#include "dlglightzero.h"
#include "ui_dlglightzero.h"
#include "driver/global.h"
#include"filemanage/cinirw.h"

dlgLightZero::dlgLightZero(bool isdown,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLightZero)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    ui->checkBox_count->setChecked(true);
    ui->checkBox_time->setChecked(true);
    isdownlight = isdown;
    QString str;
    if(isdownlight)
    {
        str.sprintf("%d",g_par.downlight_num);
        ui->label_count->setText(str);
        str.sprintf("%.1f",g_par.downlight_time/3600.0);
        ui->label_time->setText(str);
    }
    else
    {
        str.sprintf("%d",g_par.light_num);
        ui->label_count->setText(str);
        str.sprintf("%.1f",g_par.light_time/3600.0);
        ui->label_time->setText(str);
    }
}

dlgLightZero::~dlgLightZero()
{
    delete ui;
}

void dlgLightZero::on_btn_ok_clicked()
{
    CIniRW inifile(TOPPARAMETERPATH);
    QString str_val;
    if(ui->checkBox_count->isChecked())
    {
        if(isdownlight)
        {
            g_par.downlight_num = 0;
            str_val.sprintf("%d",g_par.downlight_num);
            inifile.WriteIni("TOP","DOWNLIGHT_NUM",str_val);
            ui->label_count->setText(str_val);
        }
        else
        {
            g_par.light_num = 0;
            str_val.sprintf("%d",g_par.light_num);
            inifile.WriteIni("TOP","LIGHT_NUM",str_val);
            ui->label_count->setText(str_val);
        }
    }
    if(ui->checkBox_time->isChecked())
    {
        if(isdownlight)
        {
            g_par.downlight_time = 0.0;
            str_val.sprintf("%.1f",g_par.downlight_time);
            inifile.WriteIni("TOP","DOWNLIGHT_TIME",str_val);
            ui->label_time->setText(str_val);
        }
        else
        {
            g_par.light_time = 0.0;
            str_val.sprintf("%.1f",g_par.light_time);
            inifile.WriteIni("TOP","LIGHT_TIME",str_val);
            ui->label_time->setText(str_val);
        }
    }
}

void dlgLightZero::on_btn_cancel_clicked()
{
    this->close();
}
