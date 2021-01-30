#include "dlgchangepassword.h"
#include "ui_changepassword.h"
#include <QMessageBox>
#include <stdio.h>
#include"filemanage/cinirw.h"
#include "driver/global.h"

DlgChangePassword::DlgChangePassword(int type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgChangePassword)
{
    ui->setupUi(this);
    //
    setWindowTitle(QObject::tr("修改密码"));
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    Type=type;
    ui->stackedWidget_keytype->setCurrentIndex(type);
    if(type==0)
    {
        ui->radioButton_lowkey->setChecked(true);
        passwordtype =0;
    }
    else
    {
        ui->radioButton_debugkey->setChecked(true);
        passwordtype =3;
    }
    GetPassword();
    ui->lineEdit_newpassword->setEchoMode(QLineEdit::Password);
    ui->lineEdit_newpassword_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_oldpssword->setEchoMode(QLineEdit::Password);
//    inputkey = new DlgChangePassword(this);
//    inputkey->setParent(ui->frame_key);
//    // 把子窗口的信号连接到父窗口来。
//    connect(inputkey, SIGNAL(okEvent()),this, SLOT(on_inputkey_okEvent()));
//    connect(inputkey, SIGNAL(cancelEvent()),this, SLOT(on_inputkey_cancelEvent()));
    //去除焦点
    ui->btn_0->setFocusPolicy(Qt::NoFocus);
    ui->btn_1->setFocusPolicy(Qt::NoFocus);
    ui->btn_2->setFocusPolicy(Qt::NoFocus);
    ui->btn_3->setFocusPolicy(Qt::NoFocus);
    ui->btn_4->setFocusPolicy(Qt::NoFocus);
    ui->btn_5->setFocusPolicy(Qt::NoFocus);
    ui->btn_6->setFocusPolicy(Qt::NoFocus);
    ui->btn_7->setFocusPolicy(Qt::NoFocus);
    ui->btn_8->setFocusPolicy(Qt::NoFocus);
    ui->btn_9->setFocusPolicy(Qt::NoFocus);
    ui->btn_left->setFocusPolicy(Qt::NoFocus);
    ui->btn_right->setFocusPolicy(Qt::NoFocus);
    ui->btn_clear->setFocusPolicy(Qt::NoFocus);
    ui->btn_backspace->setFocusPolicy(Qt::NoFocus);
    ui->btn_back->setFocusPolicy(Qt::NoFocus);
    ui->btn_ok->setFocusPolicy(Qt::NoFocus);
}

DlgChangePassword::~DlgChangePassword()
{
//    delete inputkey;
    delete ui;
}

void DlgChangePassword::GetPassword()
{
    CIniRW codeini(TOPPARAMETERPATH);
    if(Type==0&&(ui->stackedWidget_keytype->currentIndex()==0))
    {
        switch (passwordtype) 
        {
        case 0:
            old_setting_code = codeini.ReadIni("TRAJ","CODE_OF_SETTINGS_1").toString();
            break;
        case 1:
            old_setting_code = codeini.ReadIni("TRAJ","CODE_OF_SETTINGS_2").toString();
            break;
        case 2:
            old_setting_code = codeini.ReadIni("TRAJ","CODE_OF_HIGH_SETTINGS").toString();
            break;
        default:
            break;
        }
    }
    else if(Type==1&&(ui->stackedWidget_keytype->currentIndex()==1))
    {
        switch (passwordtype) 
        {
        case 3:
            old_setting_code = codeini.ReadIni("TRAJ","CODE_OF_DEBUG").toString();
            break;
        case 4:
            old_setting_code = codeini.ReadIni("TRAJ","CODE_OF_DEVELOP").toString();
            break;
        default:
            break;
        }
    }
}

void DlgChangePassword::SavePassword()
{
    CIniRW codeini(TOPPARAMETERPATH);
    QString oldpassword=ui->lineEdit_oldpssword->text();
    QString newpassword=ui->lineEdit_newpassword->text();
    QString newpassword_2=ui->lineEdit_newpassword_2->text();
    if(oldpassword==NULL)
    {
        QMessageBox::information(this,"TOP",QObject::tr("请输入原密码!"));
        return;
    }
    if(newpassword==NULL)
    {
        QMessageBox::information(this,"TOP",QObject::tr("请输入新密码!"));
        return;
    }
    if(!strcmp(old_setting_code.toLatin1().data(),oldpassword.toLatin1().data()))// chech old code
    {
        if(!strcmp(newpassword.toLatin1().data(),newpassword_2.toLatin1().data()))//check new code
        {
            if(Type==0&&(ui->stackedWidget_keytype->currentIndex()==0))
            {
                switch (passwordtype)
                {
                case 0:
                    codeini.WriteIni("TRAJ","CODE_OF_SETTINGS_1",newpassword);
                    break;
                case 1:
                    codeini.WriteIni("TRAJ","CODE_OF_SETTINGS_2",newpassword);
                    break;
                case 2:
                    codeini.WriteIni("TRAJ","CODE_OF_HIGH_SETTINGS",newpassword);
                    break;
                default:
                    break;
                }
            }
            else if(Type==1&&(ui->stackedWidget_keytype->currentIndex()==1))
            {
                switch (passwordtype)
                {
                case 3:
                    codeini.WriteIni("TRAJ","CODE_OF_DEBUG",newpassword);
                    break;
                case 4:
                    codeini.WriteIni("TRAJ","CODE_OF_DEVELOP",newpassword);
                    break;
                default:
                    break;
                }
            }
            QMessageBox::information(this,"TOP",QObject::tr("修改成功!"));
            return;
        }
        else
        {
            QMessageBox::information(this,"TOP",QObject::tr("新密码两次输入不一致!"));
            return;
        }
    }
    else
    {
        QMessageBox::information(this,"TOP",QObject::tr("原密码错误!"));
        return;
    }
}

void DlgChangePassword::on_radioButton_lowkey_clicked()
{
     passwordtype = 0;
     ui->radioButton_lowkey->setChecked(true);
     ui->radioButton_midkey->setChecked(false);
     ui->radioButton_upkey->setChecked(false);
     GetPassword();
}

void DlgChangePassword::on_radioButton_midkey_clicked()
{
     passwordtype = 1;
     ui->radioButton_lowkey->setChecked(false);
     ui->radioButton_midkey->setChecked(true);
     ui->radioButton_upkey->setChecked(false);
     GetPassword();
}

void DlgChangePassword::on_radioButton_upkey_clicked()
{
     passwordtype = 2;
     ui->radioButton_lowkey->setChecked(false);
     ui->radioButton_midkey->setChecked(false);
     ui->radioButton_upkey->setChecked(true);
     GetPassword();
}

void DlgChangePassword::on_radioButton_debugkey_clicked()
{
     passwordtype = 3;
     ui->radioButton_debugkey->setChecked(true);
     ui->radioButton_developkey->setChecked(false);
     GetPassword();
}

void DlgChangePassword::on_radioButton_developkey_clicked()
{
     passwordtype = 4;
     ui->radioButton_debugkey->setChecked(false);
     ui->radioButton_developkey->setChecked(true);
     GetPassword();
}
void DlgChangePassword::on_btn_0_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("0");
        }
    }
}

void DlgChangePassword::on_btn_1_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("1");
        }
    }
}

void DlgChangePassword::on_btn_2_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("2");
        }
    }
}

void DlgChangePassword::on_btn_3_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("3");
        }
    }
}

void DlgChangePassword::on_btn_4_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("4");
        }
    }
}

void DlgChangePassword::on_btn_5_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("5");
        }
    }
}

void DlgChangePassword::on_btn_6_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("6");
        }
    }
}

void DlgChangePassword::on_btn_7_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("7");
        }
    }
}

void DlgChangePassword::on_btn_8_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("8");
        }
    }
}

void DlgChangePassword::on_btn_9_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->insert("9");
        }
    }
}

void DlgChangePassword::on_btn_left_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->cursorBackward(false);
        }
    }
}

void DlgChangePassword::on_btn_right_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->cursorForward(false);
        }
    }
}

void DlgChangePassword::on_btn_clear_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->clear();
        }
    }
}

void DlgChangePassword::on_btn_backspace_clicked()
{
    if(this->focusWidget()!=NULL)
    {
        if(this->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(this->focusWidget());
            w->backspace();
        }
    }
}
void DlgChangePassword::on_btn_ok_clicked()
{
    SavePassword();
}

void DlgChangePassword::on_btn_back_clicked()
{
    this->close();
}
