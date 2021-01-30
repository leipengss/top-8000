#include "dlglanguage.h"
#include "ui_dlglanguage.h"
#include <QMessageBox>
#include "filemanage/cinirw.h"
#include "driver/global.h"

static CIniRW lanconf(MACHINECONGFILEPATH);

DlgLanguage::DlgLanguage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLanguage)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    int language = lanconf.ReadIni("LANGUAGE","language").toInt();
    if(language==0)//简体
    {
        ui->checkBox_ch_simplified->setChecked(true);
    }
    else if(language==1)//繁体
    {
        ui->checkBox_ch_traditional->setChecked(true);
    }
    else if(language==2)//英文
    {
        ui->checkBox_english->setChecked(true);
    }
    else if(language==3)//韩文
    {
        ui->checkBox_korea->setChecked(true);
    }
    else if(language==4)//日文
    {
        ui->checkBox_japan->setChecked(true);
    }
    else if(language==5)//越南文
    {
        ui->checkBox_vietnam->setChecked(true);
    }
}

DlgLanguage::~DlgLanguage()
{
    delete ui;
}

void DlgLanguage::on_btn_ok_clicked()
{
    int lan = -1;
    if(ui->checkBox_ch_simplified->isChecked())
    {
        lan = 0;
    }
    else if(ui->checkBox_ch_traditional->isChecked())
    {
        lan = 1;
    }
    else if(ui->checkBox_english->isChecked())
    {
        lan = 2;
    }
    else if(ui->checkBox_korea->isChecked())
    {
        lan = 3;
    }
    else if(ui->checkBox_japan->isChecked())
    {
        lan = 4;
    }
    else if(ui->checkBox_vietnam->isChecked())
    {
        lan = 5;
    }
    if(lan>=0)
    {
        lanconf.WriteIni("LANGUAGE","language",QString::number(lan));
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("重启系统参数生效！"));
    }
}

void DlgLanguage::on_btn_cancel_clicked()
{
    this->close();
}


