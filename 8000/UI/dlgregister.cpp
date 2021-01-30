#include "dlgregister.h"
#include "ui_dlgregister.h"
#include"filemanage/cinirw.h"
#include<QRegExp>
#include<QFile>
#include<QDebug>
#include<QMessageBox>
#include"driver/cncsh.hh"
#include"filemanage/cinirw.h"

QString ranstr;
static int timenum=-1;
static int firsttime=1;
static int num2=0;
char jynum[9];//校验码
char sysnum[9],sysnum1[9];//系统号
char keysn0[17]; //输入密码

DlgRegister::DlgRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRegister)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    //规范注册码与系统编码的输入语法  大写字符
    QRegExp regExp("^[A-Za-z0-9]{4}");
    QRegExpValidator *pRegexp = new QRegExpValidator(regExp,this);
    QRegExp sysExp("^[A-Za-z0-9]{8}");
    QRegExpValidator *pSysexp = new QRegExpValidator(sysExp,this);
    ui->lineEdit_regcode->setValidator(pRegexp);
    ui->lineEdit_regcode_2->setValidator(pRegexp);
    ui->lineEdit_regcode_3->setValidator(pRegexp);
    ui->lineEdit_regcode_4->setValidator(pRegexp);
    ui->lineEdit_syscode->setValidator(pSysexp);
    //硬件时间  系统版本信息
    QString str;
    str.sprintf("%04d-%02d-%02d",real_year,real_month,real_day);
    ui->label_reghardwaretime_show->setText(str);
    QString name,ver,modifytime;
    name = "TOP-8000";
    CIniRW version(LOGPATH);
    ver = version.ReadIni(name,"version").toString();
    modifytime = version.ReadIni(name,"date").toString();
    str = ver+" "+modifytime;
    ui->label_systemver_show->setText(str);
    //
    ui->label_regstate_show->setText(reg_info);
    //
    Init_Reg();
    //
    //加载键盘
    m_kb = new DlgKeyboard(false,this);
    m_kb->setParent(ui->frame_keyboard);
    // 把子窗口的信号连接到父窗口来。
    connect(m_kb, SIGNAL(okEvent()),this, SLOT(on_btn_ok_clicked()));
    connect(m_kb, SIGNAL(cancelEvent()),this, SLOT(on_btn_exit_clicked()));
}

int DlgRegister::ByteArrayToInt(QByteArray bytes)
{
    int val = bytes[0] & 0x000000FF;
    val |= ((bytes[1] << 8) & 0x0000FF00);
    val |= ((bytes[2] << 16) & 0x00FF0000);
    val |= ((bytes[3] << 24) & 0xFF000000);
    return val;
}

QByteArray DlgRegister::IntToByteArray(int number)
{
    QByteArray bytes;
    bytes.resize(4);
    bytes[0] = (uchar)  (0x000000ff & number);
    bytes[1] = (uchar) ((0x0000ff00 & number) >> 8);
    bytes[2] = (uchar) ((0x00ff0000 & number) >> 16);
    bytes[3] = (uchar) ((0xff000000 & number) >> 24);
    return bytes;
}

void DlgRegister::rewrite_sysnum()
{
    if(timenum==-1)
    {
        //更新次数
        if(access("/media/sda4/.time.txt",0)==-1)
        {
            system("sudo echo 0 >> /media/sda4/.time.txt");
            usleep(200*1000);
            timenum=0;
        }
        else
        {
            FILE *time;
            time=fopen("/media/sda4/.time.txt","r");
            if(time!=NULL)
            {
                char buf[100];
                fgets(buf, sizeof(buf),time);
                timenum=atoi(buf);
                fclose(time);
            }
        }
    }

    FILE *fp;
    fp=fopen("/usr/bin/.gtf-bittorrent-1.2","r");
    if(fp!=NULL)
    {
            char buffer[400];
            fgets(buffer, sizeof(buffer),fp);
            fclose(fp);
            QString str(buffer);
            QString astr=str.left(8);
            int count=astr.length();
            if(count<8)
            {
                for(int i=0;i<8-count;i++)
                astr.append(QVariant(8).toString());
            }
            int rannum[8];
            for(int i=0;i<8;i++)
            {
                rannum[i]=(astr.at(i).toAscii())%10;
            }
            ranstr.sprintf("%d%d%d%d%d%d%d%d",\
            (rannum[0]+timenum*rannum[0])%7,(rannum[1]*(timenum+1)+rannum[2]*timenum)%9,\
            (rannum[2]+2*timenum)%4,(rannum[3]+rannum[4]+timenum*2)%10,\
            (rannum[4]+timenum*5+rannum[0]*5)%8,(rannum[5]+5*rannum[1]*timenum)%9,\
            (rannum[6]*2+rannum[3]+timenum*2)%5,timenum);
    }
    else
    {
        ranstr.sprintf("66666666");
    }
}

void DlgRegister::Init_Reg()
{
    num2=0;
    rewrite_sysnum();
    strcpy(jynum,ranstr.left(8).toLatin1().data());
    ui->label_checknum_show->setText(ranstr.left(8));
    //
    cnc_sysnum(0,"");//读可用时间
    QString str;
    str.sprintf("%s",limitdate_back);
    str = str.left(4)+"-"+str.mid(4,2)+"-"+str.right(2);//时间格式
    ui->label_regtime_show->setText(str);
    //系统号
    FILE *fp;
    if((fp=fopen("/usr/bin/.gtf-bittorrent-1.2","r"))==NULL )
    {
        ui->lineEdit_syscode->clear();
        ui->lineEdit_syscode->setEnabled(true);//无系统号,可输入系统号
    }
    else if(!strcmp(limitdate_back,"")||!strcmp(limitdate_back,"OK"))
    {
        if(fgets(sysnum1,9,fp)!=NULL)
            ui->lineEdit_syscode->setEnabled(true);//没有系统号,不锁定输入框
        str.sprintf("%s",sysnum1);
        ui->lineEdit_syscode->setText(str);
        fclose(fp);
        if (!strcmp(limitdate_back,"OK")){
            system("sudo rm /usr/bin/.gtf-bittorrent-1.3");
            system("sudo rm /usr/bin/.gtf-bittorrent-0.1");
        }
        ui->lineEdit_syscode->setEnabled(true);//无系统号,可输入系统号
    }
    else
    {
        if(fgets(sysnum1,9,fp)!=NULL)
        {
            ui->lineEdit_syscode->setEnabled(false);//如有系统号,读入且锁定输入框
        }
        str.sprintf("%s",sysnum1);
        ui->lineEdit_syscode->setText(str);
        fclose(fp);
    }
    strcpy(sysnum,sysnum1);
    //
    ui->lineEdit_regcode->clear();
    ui->lineEdit_regcode_2->clear();
    ui->lineEdit_regcode_3->clear();
    ui->lineEdit_regcode_4->clear();
    ui->lineEdit_syscode->setEnabled(false);
}

DlgRegister::~DlgRegister()
{
    delete ui;
}

void DlgRegister::on_btn_ok_clicked()
{
    cnc_sysnum(1,sysnum);
    if(strcmp(limitdate_back,"FAIL")==0)
    {
        if(strlen(sysnum)!=8)
        {
            QMessageBox::information(this,"TOP",QObject::tr("系统编号必须为8位！"));
        }
        else
        {
            QMessageBox::information(this,"TOP",QObject::tr("系统编号错误"));
        }
        return;
    }
    else
    {
        QString regcodeall= m_regcode[0]+m_regcode[1]+m_regcode[2]+m_regcode[3];
        strcpy(keysn0,regcodeall.toLatin1().data());
        if(strlen(keysn0)!=16)
        {
            QMessageBox::information(this,"TOP",QObject::tr("输入密码长度不足"));
            return;
        }
        cnc_psw(jynum,keysn0);
        if(strcmp(psw_back,"FAIL")==0)
        {
            QMessageBox::information(this,"TOP",QObject::tr("输入密码错误"));
            return;
        }
        else
        {
            int tmp = diff_time();
//            is_reg = tmp;
            g_registertype = tmp;

            int clear_delay = 0;
            CIniRW reginifile(TOPPARAMETERPATH);
            reginifile.WriteIni("TOP","UNREG_DELAY",QString::number(clear_delay,10));
            if(firsttime==1)
            {
                firsttime=0;
                FILE *recordtime;
                recordtime=fopen("/media/sda4/.time.txt","w");
                if(recordtime!=NULL)
                {
                    timenum++;
                    fprintf(recordtime,"%d",timenum);
                    fclose(recordtime);
                }
            }
            QMessageBox::information(this,"TOP",QObject::tr("注册成功,重启生效!"));
        }
    }
    cnc_sysnum(0,"");
    QString str;
    str.sprintf("%s",limitdate_back);
    str = str.left(4)+"-"+str.mid(4,2)+"-"+str.right(2);//时间格式
    ui->label_regtime_show->setText(str);
}

void DlgRegister::on_btn_reset_clicked()
{
    ui->lineEdit_regcode->clear();
    ui->lineEdit_regcode_2->clear();
    ui->lineEdit_regcode_3->clear();
    ui->lineEdit_regcode_4->clear();
}

void DlgRegister::on_btn_exit_clicked()
{
    this->close();
}

void DlgRegister::on_lineEdit_syscode_textChanged(const QString &arg1)
{
    bool accept = ui->lineEdit_syscode->hasAcceptableInput();
    if(false==accept)
    {
        ui->lineEdit_syscode->setStyleSheet("background-color: rgb(254, 0, 0);");
    }
    else
    {
        ui->lineEdit_syscode->setStyleSheet("background-color: rgb(255, 255, 255);");
        m_syscode = ui->lineEdit_syscode->text();
        QString tmp;
        if(m_syscode.length()>=9)
        {
            tmp.sprintf("%s",sysnum);
            ui->lineEdit_syscode->setText(tmp);
        }
        else
        {
            strcpy(sysnum,m_syscode.toLatin1().data());
        }
    }
}

void DlgRegister::on_lineEdit_regcode_textChanged(const QString &arg1)
{
    bool accept = ui->lineEdit_regcode->hasAcceptableInput();
    if(false==accept)
    {
        ui->lineEdit_regcode->setStyleSheet("background-color: rgb(254, 0, 0);");
    }
    else
    {
        ui->lineEdit_regcode->setStyleSheet("background-color: rgb(255, 255, 255);");
        m_regcode[0] = ui->lineEdit_regcode->text();
        ui->lineEdit_regcode_2->setFocus();
    }
}

void DlgRegister::on_lineEdit_regcode_2_textChanged(const QString &arg1)
{
    bool accept = ui->lineEdit_regcode_2->hasAcceptableInput();
    if(false==accept)
    {
        ui->lineEdit_regcode_2->setStyleSheet("background-color: rgb(254, 0, 0);");
    }
    else
    {
        ui->lineEdit_regcode_2->setStyleSheet("background-color: rgb(255, 255, 255);");
        m_regcode[1] = ui->lineEdit_regcode_2->text();
        ui->lineEdit_regcode_3->setFocus();
    }
}

void DlgRegister::on_lineEdit_regcode_3_textChanged(const QString &arg1)
{
    bool accept = ui->lineEdit_regcode_3->hasAcceptableInput();
    if(false==accept)
    {
        ui->lineEdit_regcode_3->setStyleSheet("background-color: rgb(254, 0, 0);");
    }
    else
    {
        ui->lineEdit_regcode_3->setStyleSheet("background-color: rgb(255, 255, 255);");
        m_regcode[2] = ui->lineEdit_regcode_3->text();
        ui->lineEdit_regcode_4->setFocus();
    }
}

void DlgRegister::on_lineEdit_regcode_4_textChanged(const QString &arg1)
{
    bool accept = ui->lineEdit_regcode_4->hasAcceptableInput();
    if(false==accept)
    {
        ui->lineEdit_regcode_4->setStyleSheet("background-color: rgb(254, 0, 0);");
    }
    else
    {
        ui->lineEdit_regcode_4->setStyleSheet("background-color: rgb(255, 255, 255);");
        m_regcode[3] = ui->lineEdit_regcode_4->text();
    }
}

