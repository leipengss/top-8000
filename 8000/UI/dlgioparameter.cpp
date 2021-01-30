#include "dlgioparameter.h"
#include "ui_ioparameter.h"
#include<QDebug>
#include<QProcess>
#include<QDir>
#include<QFileDialog>
#include"filemanage/cfileoperate.h"
#include<QMessageBox>
#include "driver/global.h"

DlgIOParameter::DlgIOParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgIOParameter)
{
    ui->setupUi(this);
    //
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    //
    QFont tablefont;
    tablefont.setPointSize(11);
    ui->tabWidget_iopara->setFont(tablefont);
    inifileshowInput = new CIniFileshow;
    inifileshowInput->ModelShow(true);//第一个true:可改变数据,控制显示；第二个false为输入端口
    inifileshowInput->setParent(ui->tab_input);

    inifileshowOutput = new cinioutfile;
    inifileshowOutput->ModelShow();//
    inifileshowOutput->setParent(ui->tab_output);
    //
    QString str;
    str.sprintf("%d",inifileshowInput->m_input_maxnum);
    ui->label_input_maxnum->setText(str);
    str.sprintf("%d",inifileshowOutput->m_output_maxnum);
    ui->label_output_maxnum->setText(str);
}

DlgIOParameter::~DlgIOParameter()
{
//    delete inifileshowInput;
//    delete inifileshowOutput;
    if(inifileshowInput)
    {
        inifileshowInput->deleteLater();
        inifileshowInput = NULL;
    }
    if(inifileshowOutput)
    {
        inifileshowOutput->deleteLater();
        inifileshowOutput = NULL;
    }
    delete ui;
}

void DlgIOParameter::on_btn_iobackup_clicked()
{
     CFileOperate fileoperate;
     if(!fileoperate.copyFileToPath(CONGFILEPATH,CONGFILEBACKUPPATH,false))
     {
         qDebug()<<"copy is failed!";
     }
     else
     {
         qDebug()<<"backup is ok!";
         QMessageBox::information(NULL, QString("TOP"), QObject::tr("备份成功!"));
     }
}

void DlgIOParameter::on_btn_iorecover_clicked()
{
    CFileOperate fileoperate;
    QString fileName = QFileDialog::getOpenFileName(
                     this,
                     tr("Open File"),
                     CONGFILEBACKUPPATH,
                     tr("*.ini"));
    qDebug()<<fileName;
    if(fileName.isEmpty())
    {
        return;
    }
    QFileInfo fileinfo = QFileInfo(fileName);
    QFileInfo dirfile = QFileInfo(CONGFILEPATH);
    if(fileinfo.fileName().left(5)==tr("ppmc_") && fileinfo.fileName().length()==23)
    {
//        qDebug()<<dirfile.absolutePath();
        if(!fileoperate.copyFileToPath(fileName,dirfile.absolutePath()+QObject::tr("/"),true))
        {
            qDebug()<<"recover is failed!";
        }
        else
        {
            qDebug()<<"recover is ok!";
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数还原成功!"));
        }
    }
    else
    {
        qDebug()<<"selected file is wrong!";
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("选择文件错误!"));
    }
}

void DlgIOParameter::on_btn_iosave_clicked()
{
    bool isrestart = false;
    bool ispolchange = false;
    int instate = inifileshowInput->isPortChanged();
    if(instate>=0)
    {
        if(instate==0)
        {
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("输入端口重复！"));
        }
        else if(instate==1)
        {
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("无效输入端口！"));
        }
        else if(instate==3)
        {
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("输入端口号超出范围！"));
        }
        else if(instate==2)
        {
            inifileshowInput->PortSaveParameter();
            g_isRestartSys = true;
            isrestart = true;
            if(inifileshowInput->isPolChanged())
            {
                inifileshowInput->SaveParameter();
                g_isRestartSys = true;
                ispolchange = true;
            }
            else
            {
                qDebug() <<"Input polarity:no change";
            }
        }
    }
    else
    {
        qDebug() <<"Input port:no change";
        if(inifileshowInput->isPolChanged())
        {
            inifileshowInput->SaveParameter();
            g_isRestartSys = true;
            ispolchange = true;
        }
        else
        {
            qDebug() <<"Input polarity:no change";
        }
    }
    //
    int outstate = inifileshowOutput->isPortChanged();
    if(outstate>=0)
    {
        if(outstate==0)
        {
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("输出端口重复！"));
        }
        else if(outstate==1)
        {
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("无效输出端口！"));
        }
        else if(outstate==3)
        {
            QMessageBox::information(NULL, QString("TOP"), QObject::tr("输出端口号超出范围！"));
        }
        else if(outstate==2)
        {
            inifileshowOutput->SaveParameter();
            g_isRestartSys = true;
            isrestart = true;
        }
    }
    else
    {
        qDebug() <<"Output port:no change";
    }
    if(isrestart||ispolchange)
    {
        isrestart = false;
        ispolchange = false;
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("重启系统参数生效！"));
    }
}

void DlgIOParameter::on_btn_ioleave_clicked()
{
    int instate = inifileshowInput->isPortChanged();
    int outstate = inifileshowOutput->isPortChanged();
    bool inportpol = inifileshowInput->isPolChanged();
    if((instate>=0)||(outstate>=0)||(inportpol==true))
    {
        QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("参数有更改，是否保存?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            on_btn_iosave_clicked();
            g_isRestartSys = true;
        }
    }
    inifileshowOutput->ui_exit();
    inifileshowInput->ui_exit();
    this->close();
//    if(g_isRestartSys)
//    {
//        g_isRestartSys = false;
//        QMessageBox::StandardButton rb = QMessageBox::question(NULL, "TOP", QObject::tr("参数已保存，是否重启系统生效?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//        if(rb == QMessageBox::Yes)
//        {
//           SysRestart();
//        }
//    }
}

void DlgIOParameter::on_tabWidget_iopara_currentChanged(int index)
{
    if(index==0)
    {
        inifileshowInput->ModelShow(true);//第一个true:可改变数据,控制显示；第二个false为输入端口
    }
    else if(index==1)
    {
        inifileshowOutput->ModelShow();//第二个true为输出端口
    }
}
void DlgIOParameter::SysRestart()
{
    //系统重启
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDirectory = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDirectory);
    QApplication::exit();
    g_isRestartSys =false;
}
