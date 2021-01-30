#include "dlgcncparmeter.h"
#include "ui_dlgcncparmeter.h"
#include<QDebug>
#include<QMessageBox>
#include<QProcess>
#include<QDir>
#include "driver/global.h"
#include<QFileDialog>
#include"filemanage/cfileoperate.h"

DlgCNCParmeter::DlgCNCParmeter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCNCParmeter)
{
    ui->setupUi(this);
    //
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    tablecell_list.clear();
    Cxmlparmetershow *uixmlshow = new Cxmlparmetershow(0);
    tabindex = uixmlshow->parrootname_list.size();
    qDebug()<<"tabindex="<<QString::number(tabindex,10);
    for(int num=0;num<tabindex;num++)
    {
        QWidget *cellwidget = new QWidget();
        ui->tabWidget->addTab(cellwidget,uixmlshow->parrootname_list[num]);
        tablecell_list.append(cellwidget);
        Cxmlparmetershow *uixmlparashow = new Cxmlparmetershow(num);
        uixmlshow_list.append(uixmlparashow);
    }
    delete uixmlshow;
    int shownum = uixmlshow_list.size();
    qDebug()<<"shownum="<<QString::number(tabindex,10);
    for(int k=0;k<shownum;k++)
    {
       uixmlshow_list[k]->xmlparshow();
       uixmlshow_list[k]->setParent(tablecell_list[k]);
    }
//    ui->tabWidget->setStyleSheet("QTabBar::tab::selected{border-color:white;background:green;}");
//    ui->tabWidget->setStyleSheet("QTabBar::tab{width:75;height:35;}");
    ui->tabWidget->setCurrentIndex(0); //设置当前活动页
}

DlgCNCParmeter::~DlgCNCParmeter()
{
    qDeleteAll(uixmlshow_list);//析构时先子对象后父对象
    qDeleteAll(tablecell_list);
    delete ui;
}

void DlgCNCParmeter::on_btn_cncpar_exit_clicked()
{
    bool isParChanged = false;
    int retval;
    int tabnum = uixmlshow_list.size();
//    qDebug()<<"tabnum="<<QString::number(tabnum,10);
    for(int k=0;k<tabnum;k++)
    {
        if(uixmlshow_list[k]->isParChanged())
        {
           isParChanged = true;
           qDebug()<<"changetabnum="<<QString::number(k,10);
        }
    }
    if(isParChanged)
    {
        isParChanged = false;
        bool isreboot = false;
        QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("参数有更改，是否保存?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            for(int m=0;m<tabnum;m++)
            {
                if(uixmlshow_list[m]->isParChanged())
                {
                    retval =uixmlshow_list[m]->XmlParSave();
                    g_reloadpara = true;//update para
                    if(retval<1)
                    {
                        g_reloadpara = false;
                        qDebug()<<"error input!";
                        QString tr = uixmlshow_list[m]->parrootname_list.at(m);
                        QString message = tr+QObject::tr("请检查参数范围或输入是否合法！");
                        QMessageBox::information(NULL, QString("TOP"), QObject::tr("")+message);
                    }
                }
            }
            if(g_reloadpara)
            {
                for(int m=0;m<tabnum;m++)
                {
                    if(uixmlshow_list[m]->isrestart)
                    {
                        isreboot = true;
                    }
                }
                if(isreboot)
                {
                   isreboot = false;
                   QMessageBox::information(NULL, QString("TOP"), QObject::tr("重启系统参数生效！"));
                }
            }
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

void DlgCNCParmeter::on_btn_cncpar_backup_clicked()
{
    CFileOperate fileoperate;
//    if(!fileoperate.copyFileToPath(CNCPARAMETERPATH,CONGFILEBACKUPPATH,false))
    if((!fileoperate.copyFileToPath(CONGFILEPATH,CONGFILEBACKUPPATH,false))||(!fileoperate.copyFileToPath(TOPPARAMETERPATH,CONGFILEBACKUPPATH,false)))
    {
        qDebug()<<"copy is failed!";
    }
    else
    {
        qDebug()<<"backup is ok!";
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("备份成功!"));
    }
}

void DlgCNCParmeter::on_btn_cncpar_rec_clicked()
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
    QFileInfo topdirfile = QFileInfo(TOPPARAMETERPATH);
    if((fileinfo.fileName().left(5)==tr("ppmc_"))||(fileinfo.fileName().left(5)==tr("8787_")))
    {
        if(fileinfo.fileName().left(5)==tr("ppmc_") && fileinfo.fileName().length()==23)
        {
            if(!fileoperate.copyFileToPath(fileName,dirfile.absolutePath()+QObject::tr("/"),true))
            {
                qDebug()<<"recover is failed!";
            }
            else
            {
                qDebug()<<"recover is ok!";
                QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数还原成功!").toLocal8Bit());
            }
        }
        //
        if(fileinfo.fileName().left(5)==tr("8787_") && fileinfo.fileName().length()==23)
        {
            if(!fileoperate.copyFileToPath(fileName,topdirfile.absolutePath()+QObject::tr("/"),true))
            {
                qDebug()<<"recover is failed!";
            }
            else
            {
                qDebug()<<"recover is ok!";
                QMessageBox::information(NULL, QString("TOP"), QObject::tr("参数还原成功!"));
            }
        }
    }
    else
    {
        qDebug()<<"selected file is wrong!";
        QMessageBox::information(NULL, QString("TOP"), QObject::tr("选择文件错误!"));
    }
}

void DlgCNCParmeter::on_btn_cncpar_save_clicked()
{
    bool isParChanged = false;
    int tabnum = uixmlshow_list.size();
    int retval;
//    qDebug()<<"tabnum="<<QString::number(tabnum,10);
    for(int k=0;k<tabnum;k++)
    {
        if(uixmlshow_list[k]->isParChanged())
        {
           isParChanged = true;
        }
    }
    if(isParChanged)
    {
        isParChanged = false;
        bool isreboot = false;
        for(int m=0;m<tabnum;m++)
        {
            if(uixmlshow_list[m]->isParChanged())
            {
               retval =uixmlshow_list[m]->XmlParSave();
               g_reloadpara = true;//update para
               if(retval<1)
               {
                   g_reloadpara = false;
                   qDebug()<<"error input!";
                   QString tr = uixmlshow_list[m]->parrootname_list.at(m);
                   QString message = tr+QObject::tr("请检查参数范围或输入是否合法！");
                   QMessageBox::information(NULL, QString("TOP"), QObject::tr("")+message);
               }
            }
        }
        if(g_reloadpara)
        {
            for(int m=0;m<tabnum;m++)
            {
                if(uixmlshow_list[m]->isrestart)
                {
                    isreboot = true;
                }
            }
            if(isreboot)
            {
               isreboot = false;
               QMessageBox::information(NULL, QString("TOP"), QObject::tr("重启系统参数生效！"));
            }
        }
    }
}

void DlgCNCParmeter::SysRestart()
{
    //系统重启
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDirectory = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDirectory);
    QApplication::exit();
    g_isRestartSys =false;
}

