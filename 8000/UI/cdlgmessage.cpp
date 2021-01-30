#include "cdlgmessage.h"
#include "ui_cdlgmessage.h"
#include "driver/global.h"

CDlgMessage::CDlgMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDlgMessage)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags()& ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
//    QString str = mess.mid(1,4);
}

CDlgMessage::~CDlgMessage()
{
    delete ui;
}

void CDlgMessage::MessageShow(bool isnormal,QString code)
{
//    QString str = code+":"+QObject::tr("错误!");
    if(isnormal)
    {
        ui->label_info->setStyleSheet("color:black;");
    }
    else
    {
        ui->label_info->setStyleSheet("color:red;");
    }
    ui->label_info->setText(code);
}

void CDlgMessage::on_btn_ok_clicked()
{
    g_beeptype = 1;
    this->accept();
}

void CDlgMessage::on_btn_cancel_clicked()
{
    g_beeptype = 1;
    this->reject();
}

void CDlgMessage::MessageClose()
{
    this->close();
}
