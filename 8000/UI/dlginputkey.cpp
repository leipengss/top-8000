#include "dlginputkey.h"
#include "ui_dlginputkey.h"
#include<QLineEdit>

DlgInputkey::DlgInputkey(QWidget*pWidget,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DlgInputkey)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    //
    pWidgetObj = pWidget;
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

//    ui->btn_ok->hide();
//    ui->btn_back->hide();
//    ui->frame_3->hide();
}

DlgInputkey::~DlgInputkey()
{
    delete ui;
}

void DlgInputkey::on_btn_0_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("0");
        }
    }
}

void DlgInputkey::on_btn_1_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("1");
        }
    }
}

void DlgInputkey::on_btn_2_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("2");
        }
    }
}

void DlgInputkey::on_btn_3_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("3");
        }
    }
}

void DlgInputkey::on_btn_4_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("4");
        }
    }
}

void DlgInputkey::on_btn_5_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("5");
        }
    }
}

void DlgInputkey::on_btn_6_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("6");
        }
    }
}

void DlgInputkey::on_btn_7_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("7");
        }
    }
}

void DlgInputkey::on_btn_8_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("8");
        }
    }
}

void DlgInputkey::on_btn_9_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("9");
        }
    }
}

void DlgInputkey::on_btn_left_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->cursorBackward(false);
        }
    }
}

void DlgInputkey::on_btn_right_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->cursorForward(false);
        }
    }
}

void DlgInputkey::on_btn_clear_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->clear();
        }
    }
}

void DlgInputkey::on_btn_backspace_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->backspace();
        }
    }
}

void DlgInputkey::on_btn_ok_clicked()
{
    // 触发子窗口的信号给父窗口接收。
    emit okEvent();
}

void DlgInputkey::on_btn_back_clicked()
{
    emit cancelEvent();
//    QObject *obj=sender();
//    qDebug("TestButton is clicked by %s!",obj->objectName().toStdString().c_str());
}
