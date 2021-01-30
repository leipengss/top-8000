#include "cinioutfile.h"
#include "ui_cinioutfile.h"
#include <QTableWidget>
#include<QSettings>
#include<QDebug>
#include<QTextCodec>
#include<QThread>
#include "driver/global.h"
#include"driver/cupdatedatathread.h"
#include"filemanage/cinirw.h"

int outnum = 0;
vector<int> outportnum_list;
cinioutfile::cinioutfile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cinioutfile)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    //
    Init_List();
    m_bOutPortModifyFlg = false;
    //
    updatethread=new UpdateOUTThread;
    updatethread->ptr_thread=this;
//    updatethread->start(QThread::NormalPriority);
    connect(updatethread,SIGNAL(send_outmessage(int,QString)),this,SLOT(update_io(int,QString)));
    //
    m_output_maxnum = 0;
}

cinioutfile::~cinioutfile()
{
    delete updatethread;
    delete ui;
}

void cinioutfile::Init_List()
{
    pol_list.clear();
    group_list.clear();
    portnum_list.clear();
    name_list.clear();
    describe_list.clear();
    newpos_list.clear();
    portnumback_list.clear();
    modifyportgroup_list.clear();
    modifyport_list.clear();
    modifyportpos_list.clear();
}
void cinioutfile::setui()
{
    vector<int> temp_list(portnum_list);
    QString temp = QString::number(temp_list.size(),10);
    qDebug()<<"temp="+temp;
    std::sort(portnum_list.begin(),portnum_list.end());//排序
    vector<int> temp2_list(portnum_list);
    outportnum_list.swap(temp2_list);
    QString portnum = QString::number(portnum_list.size(),10);
    qDebug()<<"portnum="+portnum;
    for(unsigned int m=0;m<temp_list.size();m++)
    {
        vector<int>::iterator iter=std::find(temp_list.begin(),temp_list.end(),portnum_list.at(m));//返回的是一个迭代器指针
        int index = std::distance(temp_list.begin(), iter);
        newpos_list.push_back(index);//记录排序后的下标
    }
    QString print = QString::number(newpos_list.size(),10);
    qDebug()<<print;
    if(newpos_list.size()!=outtablecount)
    {
        qDebug() <<"newpos_list is error!";
        return;
    }
    for(unsigned int k=0;k<outtablecount;k++)
    {
          addItemContent(k, 0, name_list.at(newpos_list.at(k)));
          addItemContent(k, 1, describe_list.at(newpos_list.at(k)));
          addItemContent(k, 2, portnumback_list.at(newpos_list.at(k)));
          addItemContent(k, 3, pol_list.at(newpos_list.at(k)));
          addItemContent(k, 4, "0");//初始化时不获取信号值
          addItemContent(k, 5, group_list.at(newpos_list.at(k)));
    }
    qDebug() <<"set ui!";
}
void UpdateOUTThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_bStopped = true;
    exit(0);
}
void UpdateOUTThread::run()
{
    int num = outportnum_list.size();
    while(1)
    {
        for(int k=0;k<num;k++)
        {
//            qDebug()<<QString::number(portnum_list.at(k),10);
            int val = update_thread->threaddriver->Get_PortVal(outportnum_list.at(k),PORT_OUT);
            QString tr = QString::number(val,10);
//            usleep(10000);//10ms
            emit send_outmessage(k,tr);
        }
        usleep(10000);//10ms
        // 检测是否停止
        {
            QMutexLocker locker(&m_mutex);
            if (m_bStopped)
                break;
        }
        // locker超出范围并释放互斥锁
    }
    exit(0);
}
void cinioutfile::ui_exit()
{
    if(updatethread->isRunning())
    {
//        updatethread->disconnect(SIGNAL(send_outmessage(int,QString)));
        this->disconnect();
        updatethread->stop();
//        updatethread->terminate();//线程强制退出，数据不需保存不影响UI主线程，这样可以防止退出界面时卡顿
        updatethread->wait();
        updatethread->quit();
    }
    this->close();
}
//更新信号数据
void cinioutfile::update_io(int row,QString tr)
{
//     qDebug()<<QString::number(row,10)+"   val="+tr;
    if(!tr.compare("1"))
    {
        ui->tableWidget->item(row,4)->setTextColor(QColor("red"));
    }
    else if(!tr.compare("0"))
    {
        ui->tableWidget->item(row,4)->setTextColor(QColor("green"));
    }
    ui->tableWidget->item(row,4)->setText(tr);
}
void cinioutfile::ModelShow()
{
    Init_List();
    ParseINIFile();//遍历INI文件获取有效IO的个数
    QFont tablefont;
    tablefont.setPointSize(11);
    QStringList header;
    header<<QObject::tr("编码")<<QObject::tr("描述")<<QObject::tr("硬件端口")<<QObject::tr("极性")<<QObject::tr("信号")<<QObject::tr("字段");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setFont(tablefont);
    ui->tableWidget->setColumnCount(6);
    outtablecount = portnum_list.size();//有效IO的个数
    ui->tableWidget->setRowCount(outtablecount);//设置行数
    //设置表格的各列的宽度值
    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,180);
    ui->tableWidget->setColumnWidth(2,70);
    ui->tableWidget->setColumnWidth(3,50);
    ui->tableWidget->setColumnWidth(4,50);
    ui->tableWidget->setColumnHidden(5,true);//hide zhiduan
    //设置表格的单元为能编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    //设置选中时为整行选中
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bOutPortModifyFlg = false;
    setui();//显示数据
    updatethread->start(QThread::NormalPriority);//update data
}

/*给单元格添加内容*/
void cinioutfile::addItemContent(int row, int column, QString content)
{
    QColor tablecolor;
    QFont tablefont;
    tablefont.setPointSize(11);
    item = new QTableWidgetItem(content);
    if(1==row%2)
    {
        tablecolor = QColor(184,215,255);
    }
    else
    {
        tablecolor = QColor(191,191,191);
    }
    switch(column)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 5:
         item->setBackgroundColor(tablecolor);
         break;
    }
    item->setFont(tablefont);
    if(column!=2)
    {
       item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    }
    ui->tableWidget->setItem(row, column, item);
}

bool cinioutfile::ParseINIFile()
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

//判断端口号是否改变
int cinioutfile::isPortChanged()
{
    int ret = -1;
    modifyportgroup_list.clear();
    modifyport_list.clear();
    modifyportpos_list.clear();
    QVector<QString> itemvalue_list;
    itemvalue_list.clear();
    for(unsigned int m=0;m<outtablecount;m++)
    {
        itemvalue_list.push_back(ui->tableWidget->item(m,2)->text());
    }
    for(unsigned int i=0;i<outtablecount;i++)
    {
        if(portnumback_list.at(newpos_list.at(i)).compare(itemvalue_list.at(i)))
        {
            for(unsigned int j=0;j<outtablecount;j++)
            {
                if((0==itemvalue_list.at(i).compare(itemvalue_list.at(j)))&&(i!=j))
                {
                   ret = 0;
                   return ret;
                }
                if(itemvalue_list.at(i).toInt()==-1)//无效端口
                {
                   ret = 1;
                   return ret;
                }
                else if(itemvalue_list.at(i).toInt()>m_output_maxnum)//无效端口
                {
                   ret = 3;
                   return ret;
                }
            }
            m_bOutPortModifyFlg = true;
            QString print = group_list.at(newpos_list.at(i));
            modifyportgroup_list.append(print);
            modifyport_list.append(itemvalue_list.at(i));
            modifyportpos_list.append(newpos_list.at(i));
            qDebug() <<"Output number is changed";
            ret = 2;
        }
    }
    return ret;
}

bool cinioutfile::SaveParameter()
{
    bool ret= false;
    if(!m_bOutPortModifyFlg)
    {
        qDebug() <<"no outitem is changed!";
        return true;
    }
    qDebug() <<"Outport SaveParameter";
    CIniRW outconfigIniWrite(CONGFILEPATH);
    QVector<QString>::iterator iter;
    int offset=0;
    for(iter=modifyportgroup_list.begin();iter!=modifyportgroup_list.end();iter++)
    {
        outconfigIniWrite.WriteIni(*iter,"NUM",modifyport_list.at(offset));
        qDebug() <<*iter;
        qDebug() <<modifyport_list.at(offset);
        //修改后更新值
        portnumback_list[modifyportpos_list.at(offset)]=modifyport_list.at(offset);
        offset++;
    }
    if(offset>0)
    {
        ret= true;
    }
    m_bOutPortModifyFlg = false;//保存数据后恢复标志位
    return ret;
}


