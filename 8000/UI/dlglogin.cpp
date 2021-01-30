#include "dlglogin.h"
#include "ui_dlglogin.h"
#include "driver/global.h"
#include"filemanage/cinirw.h"
#include<QMessageBox>
#include "driver/global.h"

DlgLogin::DlgLogin(int keylevel,QString tile,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgLogin)
{
    ui->setupUi(this);
    //
    setWindowTitle(tile);
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    inputkeyboard = new DlgInputkey(this);
    inputkeyboard->setParent(ui->page_key);
    // 把子窗口的信号连接到父窗口来。
    connect(inputkeyboard, SIGNAL(okEvent()),this,SLOT(on_btn_ok_clicked()));
    connect(inputkeyboard, SIGNAL(cancelEvent()),this, SLOT(on_btn_cancel_clicked()));
    //初始化数据
    GetPassword();
    passlevel = keylevel;
    if(passlevel==-1)
    {
        clr_passwd = "";
        ui->stackedWidget_login->setCurrentIndex(1);
        ui->checkBox_debug->setChecked(true);
    }
    else
    {
        ui->stackedWidget_login->setCurrentIndex(0);
        clr_passwd = QString("123123");
    }
    ui->input_password->setText(clr_passwd);
    ui->input_password->setEchoMode(QLineEdit::Password);
    iscorrect = false;
}

DlgLogin::~DlgLogin()
{
//    system("sudo killall onboard &");//退出时关闭软键盘
    delete inputkeyboard;
    delete ui;
}

void DlgLogin::GetPassword()
{
    CIniRW codeini(TOPPARAMETERPATH);
    key[0] = codeini.ReadIni("TRAJ","CODE_OF_SETTINGS_1").toString();
    key[1] = codeini.ReadIni("TRAJ","CODE_OF_SETTINGS_2").toString();
    key[2] = codeini.ReadIni("TRAJ","CODE_OF_HIGH_SETTINGS").toString();
    CIniRW snini(MACHINECONGFILEPATH);
    key[3] = snini.ReadIni("SN","debugsn").toString();
    key[4] = snini.ReadIni("SN","developersn").toString();
}

//五级密码输入
void DlgLogin::on_btn_ok_clicked()
{
    QString tmp = ui->input_password->text();
    int pageindex = ui->stackedWidget_login->currentIndex();
    if(pageindex==0)
    {
        if(passlevel==0)//根据输入的密码设置级别,只针对用户密码
        {
            if( tmp == "")
            {
                g_settings_kind =10;
                iscorrect = true;
            }
            else if((tmp == clr_passwd)||(!tmp.compare(key[0])))
            {
                g_settings_kind =11;
                iscorrect = true;
            }
            else if(!tmp.compare(key[1]))
            {
                g_settings_kind =12;
                iscorrect = true;
            }
            else if(!tmp.compare(key[2]))
            {
                g_settings_kind =13;
                iscorrect = true;
            }
            else
            {
                iscorrect = false;
            }
        }
        else//根据密码级别判断输入的密码是否正确
        {
            g_settings_kind = passlevel;//记录密码级别,全局有效
            switch (passlevel) {
            case 10:
                if( tmp == "")
                {
                    iscorrect = true;
                }
                else
                {
                    iscorrect = false;
                }
                break;
            case 11:
                if((tmp == clr_passwd)||(0==tmp.compare(key[0])))
                {
                    iscorrect = true;
                }
                else
                {
                    iscorrect = false;
                }
                break;
            case 12:
                if(0==tmp.compare(key[1]))
                {
                    iscorrect = true;
                }
                else
                {
                    iscorrect = false;
                }
                break;
            case 13:
                if(0==tmp.compare(key[2]))
                {
                    iscorrect = true;
                }
                else
                {
                    iscorrect = false;
                }
                break;
            default:
                break;
            }
        }
    }
    else if(pageindex==1)
    {
        if(ui->checkBox_debug->isChecked())
        {
            if((0==tmp.compare(key[3]))||(tmp=="runf")||(tmp=="789789"))
            {
                iscorrect = true;
                g_settings_kind = 14;
            }
            else
            {
                iscorrect = false;
            }
        }
        if(ui->checkBox_develop->isChecked())
        {
            if(0==tmp.compare(key[4]))
            {
                iscorrect = true;
                g_settings_kind = 15;
            }
            else
            {
                iscorrect = false;
            }
        }
    }
    if( iscorrect == false)
    {
        QMessageBox::information(this,"TOP",QObject::tr("密码错误!"));
    }
    else
    {
        this->close();
    }
}

void DlgLogin::on_btn_cancel_clicked()
{
    this->close();
}


//void DlgLogin::on_btn_softkeyboard_clicked()
//{
//    system("sudo onboard &");
//}

//void DlgLogin::on_btn_softkeyboard_close_clicked()
//{
//    system("sudo killall onboard &");
//}

void DlgLogin::on_btn_clear_clicked()
{
    ui->input_password->clear();
}
