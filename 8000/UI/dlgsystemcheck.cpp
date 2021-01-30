#include "dlgsystemcheck.h"
#include "ui_systemcheck.h"
#include<QMessageBox>
#include<QDebug>
#include "driver/global.h"
#include<QFile>
#include"driver/cupdatedatathread.h"
#include <QSettings>
#include <QTextCodec>

DlgSystemcheck::DlgSystemcheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSystemcheck)
{
    ui->setupUi(this);
    vector<int> outportnum_list;
    ParseINIFile();
    vector<int> temp_list(portnum_list);
    QString temp = QString::number(temp_list.size(),10);
    qDebug()<<"temp="+temp;
    std::sort(portnum_list.begin(),portnum_list.end());//排序
    vector<int> temp2_list(portnum_list);
    outportnum_list.swap(temp2_list);
    QString portnum = QString::number(portnum_list.size(),10);
    qDebug()<<"portnum="+portnum;
    outtablecount = portnum_list.size();
    for(unsigned int m=0;m<temp_list.size();m++)
    {
        vector<int>::iterator iter=std::find(temp_list.begin(),temp_list.end(),portnum_list.at(m));//返回的是一个迭代器指针
        int index = std::distance(temp_list.begin(), iter);
        newpos_list.push_back(index);//记录排序后的下标
    }
//    QString print = QString::number(newpos_list.size(),10);
//    qDebug()<<print;
//    qDebug()<<newpos_list;
//    qDebug()<<outtablecount;
    if(newpos_list.size()!=outtablecount)
    {
        qDebug() <<"newpos_list is error!";
        return;
    }
    //
    QColor tablecolor;
    QFont tablefont;
    tablefont.setPointSize(11);
    ui->tableWidget->setColumnCount(4);//设置列数
    QStringList header;
    header<<QObject::tr("编码")<<QObject::tr("描述")<<QObject::tr("硬件端口")<<QObject::tr("状态");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setRowCount(outtablecount);//设置行数
    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,140);
    ui->tableWidget->setColumnWidth(2,70);
    ui->tableWidget->setColumnWidth(3,68);
    ui->tableWidget->setFont(tablefont);
    ui->tableWidget->horizontalHeader()->setFont(tablefont);
    //设置表格的单元为不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pushbutton_list.clear();
    QString state;
    for(unsigned int k=0;k<outtablecount;k++)
    {
          ui->tableWidget->setItem(k,0,new QTableWidgetItem(name_list.at(newpos_list.at(k))));
          ui->tableWidget->setItem(k,1,new QTableWidgetItem(describe_list.at(newpos_list.at(k))));
          ui->tableWidget->setItem(k,2,new QTableWidgetItem(portnumback_list.at(newpos_list.at(k))));
          if(1==k%2)
          {
              tablecolor = QColor(184,215,255);
          }
          else
          {
              tablecolor = QColor(191,191,191);
          }
          ui->tableWidget->item(k,0)->setBackgroundColor(tablecolor);
          ui->tableWidget->item(k,0)->setFont(tablefont);
          ui->tableWidget->item(k,1)->setBackgroundColor(tablecolor);
          ui->tableWidget->item(k,1)->setFont(tablefont);
          ui->tableWidget->item(k,2)->setBackgroundColor(tablecolor);
          ui->tableWidget->item(k,2)->setFont(tablefont);
          int value = update_thread->threaddriver->Get_PortVal(portnumback_list.at(newpos_list.at(k)).toInt(),PORT_OUT);
          QPushButton *button = new QPushButton;
          if(1==value)
          {
              state = QObject::tr("打开");
              button->setStyleSheet("QPushButton{background-color:green; color: white;   border-radius: 10px;  border: 2px groove gray;border-style: inset;}");
          }
          else if (0==value)
          {
              state = QObject::tr("关闭");
              button->setStyleSheet("QPushButton{background-color:gray; color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}");
          }
          button->setFont(tablefont);
          button->setText(state);
          button->setProperty("row", k);
          pushbutton_list.append(button);
          ui->tableWidget->setCellWidget(k,3, pushbutton_list.at(k));
          QObject::connect(pushbutton_list.at(k), SIGNAL(clicked()), this, SLOT(onTableBtnClicked()));
    }

    //
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    //
    connect(ui->btn_exit,SIGNAL(clicked()),this,SLOT(click_btn_exit()));

    ui->tabWidget_syscheck->setFont(tablefont);
    inifileshow = new CIniFileshow;
    inifileshow->ModelShow(false);//第一个false:只显示，不可改变数据,控制显示
    inifileshow->setParent(ui->tab_io);
    //histroy tab
    histroyshow = new CLogFileshow(LOGFILEPATH);
    histroyshow->setParent(ui->tab_history);
    //
    ui->btn_clear->setVisible(false);
    //
    nIndex = 0;
    ui->tabWidget_syscheck->setCurrentIndex(nIndex);
}

DlgSystemcheck::~DlgSystemcheck()
{
    qDeleteAll(pushbutton_list);
    if(inifileshow)
    {
        inifileshow->deleteLater();
        inifileshow = NULL;
    }
    if(histroyshow)
    {
        histroyshow->deleteLater();
        histroyshow = NULL;
    }
    delete ui;
}

void DlgSystemcheck::click_btn_exit()
{
    inifileshow->ui_exit();
    pushbutton_list.clear();
    this->close();
}

void DlgSystemcheck::onTableBtnClicked()
{
    QPushButton *button = (QPushButton *)sender();
    int row =button->property("row").toInt();
    int portnum =ui->tableWidget->item(row,2)->text().toInt();
    QString io_code= ui->tableWidget->item(row,0)->text();//返回IO编号
    if(button->text()==QObject::tr("关闭"))
    {
        if(false==IsOperated(io_code))  return;
        button->setText(QObject::tr("打开"));
        button->setStyleSheet("QPushButton{background-color:green; color: white;   border-radius: 10px;  border: 2px groove gray;border-style: inset;}");
        update_thread->threaddriver->Ctr_Output(portnum,true);
    }
    else if (button->text()==QObject::tr("打开"))
    {
         button->setText(QObject::tr("关闭"));
         button->setStyleSheet("QPushButton{background-color:gray; color: white;   border-radius: 10px;  border: 2px groove gray;border-style: outset;}");
         update_thread->threaddriver->Ctr_Output(portnum,false);
    }
}


void DlgSystemcheck::on_tabWidget_syscheck_currentChanged(int index)
{
    if(index==1)
    {
        ui->btn_clear->setVisible(true);
        //历史数据实时更新
        histroyshow->logshow();
    }
    else if(index==0)
    {
        inifileshow->ModelShow(false);
        ui->btn_clear->setVisible(false);
    }
}

bool DlgSystemcheck::ParseINIFile()
{
    QSettings *configIniRead = new QSettings(CONGFILEPATH, QSettings::IniFormat);
    configIniRead->setIniCodec("UTF-8");
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString name,pol,describe,number;
    foreach (QString group, configIniRead->childGroups())
    {
      configIniRead->beginGroup(group);
      if(group=="PORT_MAX")
      {
         m_output_maxnum = configIniRead->value("OUTPUTNUMBER").toInt();
      }
      if(group.left(7)=="OUTPUT_")//output
      {
          foreach (QString key, configIniRead->childKeys())
          {
               QTextCodec *codec=QTextCodec::codecForName("GBK");
               QString outStr = codec->toUnicode(key.toLatin1());
               if (outStr == "NUM")
               {
                  number = configIniRead->value(outStr).toString();
                  int outnumtmp = number.toInt();
                  if(outnumtmp>=0)
                  {
                      QString outstr1 = configIniRead->value("NAME").toString();
                      if(!outstr1.isEmpty())
                      {
                         if(outstr1.left(1)=="O")
                         {
                           name=outstr1.left(5);
                           describe=outstr1.mid(6);
                         }
                         else
                         {
                             name ="";
                             describe=outstr1;
                         }
                      }
                      QString  outstr2 = configIniRead->value("POL").toString();
                      if(!outstr2.isEmpty())
                      {
                         pol=outstr2.left(2);
                      }
                      portnum_list.push_back(outnumtmp);
                      portnumback_list.push_back(number);
                      group_list.append(group);
                      pol_list.append(pol);
                      name_list.append(name);
                      describe_list.append(describe);
                  }
               }
          }
      }
      configIniRead->endGroup();
    }
    //读入完成后删除指针
    delete configIniRead;
    return true;
}


void DlgSystemcheck::on_btn_clear_clicked()
{
    histroyshow->clear();
}

//根据号码管编码判断IO属性，判断操作允许:开框  点灯
bool DlgSystemcheck::IsOperated(QString name)
{
    if(name=="O10.1")//遮光板
    {
//        if(g_now_frame<0)
//        {
//            QMessageBox::information(this, QString("TOP"), QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
//            return false;
//        }
    }
    else if(name=="O10.2")//开上框
    {

    }
    else if(name=="O10.4")//开下框
    {

    }
    else if(name=="O13.0")//UP CCD
    {
//        if(g_now_frame<0)
//        {
//            QMessageBox::information(this, QString("TOP"), QObject::tr("框架尚未到达正确停止位置，不能进行此操作!"));
//            return false;
//        }
//        if(g_now_frame!=1)
//        {
//            QMessageBox::information(this, QString("TOP"), QObject::tr("当前为上框，不能进行此操作!"));
//            qDebug()<<QObject::tr("当前为上框，不能进行此操作!");
//            return false;
//        }
//        bool is_downframeclose = update_thread->threaddriver->VarBit_Get(VAR_SIGNAL,"Frame_close_check2");
//        if(is_downframeclose==false)
//        {
//            QMessageBox::information(this, QString("TOP"), QObject::tr("下框框架未关闭好，不能进行此操作!"));
//            qDebug()<<QObject::tr("下框框架未关闭好，不能进行此操作!");
//            return false;
//        }
    }
    return true;
}
