#include "cinifileshow.h"
#include "ui_cinifileshow.h"
#include <QTableWidget>
#include<QSettings>
#include<QDebug>
#include<QTextCodec>
#include<QThread>
#include "driver/global.h"
#include"filemanage/cinirw.h"
#include"driver/cupdatedatathread.h"

vector<int> inportnum_list;

CIniFileshow::CIniFileshow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CIniFileshow)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());
    //
    Init_List();
    m_bPolModifyFlg = false;
    m_bInPortModifyFlg = false;
    //
    updatethread=new UpdateIOThread;
    updatethread->ptr_thread=this;
//    updatethread->start(QThread::NormalPriority);
    connect(updatethread,SIGNAL(send_message(int,QString)),this,SLOT(update_io(int,QString)));
    //
    m_input_maxnum = 0;
}
void CIniFileshow::Init_List()
{
    combox_list.clear();
    pol_list.clear();
    group_list.clear();
    modifygroup_list.clear();
    modifypol_list.clear();
    portnum_list.clear();
    name_list.clear();
    describe_list.clear();
    newpos_list.clear();
    portnumback_list.clear();
}

CIniFileshow::~CIniFileshow()
{
    delete item;
//    for(QVector<QComboBox *>::iterator it = combox_list.begin(); it != combox_list.end(); it ++)
//    {
//        if (NULL != *it)
//        {
//           delete *it;
//           *it = NULL;
//        }
//    }
//    combox_list.clear();
    delete updatethread;
    qDeleteAll(combox_list);
    delete ui;
}
void UpdateIOThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_bStopped = true;
    exit(0);
}
void UpdateIOThread::run()
{
    qDebug()<<"update run";
    int ionum = inportnum_list.size();
    while(1)
    {
        for(int k=0;k<ionum;k++)
        {
            int val = update_thread->threaddriver->Get_PortVal(inportnum_list.at(k),PORT_IN);
            QString tr = QString::number(val,10);
            emit send_message(k,tr);
//            usleep(10000);//10ms
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
void CIniFileshow::ui_exit()
{
    if(updatethread->isRunning())
    {
//        updatethread->disconnect(SIGNAL(send_message(int,QString)));
        this->disconnect();
        updatethread->stop();
        updatethread->wait();
        updatethread->quit();
//        updatethread->terminate();//线程强制退出，数据不需保存不影响UI主线程，这样可以防止退出界面时卡顿
    }
    this->close();
}
//更新信号数据
void CIniFileshow::update_io(int row,QString tr)
{
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
void CIniFileshow::ModelShow(bool isedit)
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
    tablecount = portnum_list.size();//有效IO的个数
    ui->tableWidget->setRowCount(tablecount);//设置行数
    //设置表格的各列的宽度值
    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,180);
    ui->tableWidget->setColumnWidth(2,70);
    ui->tableWidget->setColumnWidth(3,50);
    ui->tableWidget->setColumnWidth(4,50);
    ui->tableWidget->setColumnHidden(5,true);//hide zhiduan
    if(isedit==false)
    {
        //设置表格的单元为不可编辑
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //设置选中时为整行选中
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    }
    else
    {
        //设置表格的单元为能编辑
        ui->tableWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    }
    isPOLEdit=isedit;
    m_bPolModifyFlg = false;
    setui();
//    ui->tableWidget->sortByColumn(0,Qt::AscendingOrder);
    updatethread->start(QThread::NormalPriority);//update data
}
void CIniFileshow::setui()
{
    vector<int> temp_list(portnum_list);
    QString temp = QString::number(temp_list.size(),10);
    qDebug()<<"temp="+temp;
    std::sort(portnum_list.begin(),portnum_list.end());//排序
    vector<int> temp2_list(portnum_list);
    inportnum_list.swap(temp2_list);
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
    if(newpos_list.size()!=tablecount)
    {
        qDebug() <<"newpos_list is error!";
    }
    for(unsigned int k=0;k<tablecount;k++)
    {
        addItemContent(k, 0, name_list.at(newpos_list.at(k)));
        addItemContent(k, 1, describe_list.at(newpos_list.at(k)));
        addItemContent(k, 2, portnumback_list.at(newpos_list.at(k)));
        if(isPOLEdit==true)
        {
            addItemCombox(k,3,pol_list.at(newpos_list.at(k)));//添加组合框，并初始化内容
        }
        else
        {
            addItemContent(k, 3, pol_list.at(newpos_list.at(k)));
        }
        addItemContent(k, 4, "0");//初始化时不获取信号值
        addItemContent(k, 5, group_list.at(newpos_list.at(k)));
    }
    qDebug() <<"set ui!";
}

/*给单元格添加内容*/
void CIniFileshow::addItemContent(int row, int column, QString content)
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
    //可修改极性不设置底色  信号值不设置底色
    switch(column)
    {
    case 0:
    case 1:
    case 2:
    case 5:
         item->setBackgroundColor(tablecolor);
         break;
    case 3:
         if(!isPOLEdit)
            item->setBackgroundColor(tablecolor);
         break;
    }
    item->setFont(tablefont);
    if(isPOLEdit==true)
    {
        if(column!=2)
        {
           item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        }
    }
    ui->tableWidget->setItem(row, column, item);
}
///*给单元格添加内容*/
void CIniFileshow::addItemCombox(int row, int column,QString content)
{
    combox = new QComboBox(); // 下拉选择框控件
    combox->addItem("0");
    combox->addItem("1");
    combox_list.append(combox);
    QFont tablefont;
    tablefont.setPointSize(11);
    combox->setFont(tablefont);
    ui->tableWidget->setCellWidget(row, column, combox_list.at(row));
    combox_list.at(row)->setCurrentIndex(content.toInt());
//    QString print = QString::number(row,10);
//    qDebug()<<print;
//    connect(combox_list.at(row), SIGNAL(currentIndexChanged(int)), this, SLOT(combox_process(int)));
}

bool CIniFileshow::ParseINIFile()
{
    QSettings *configIniRead = new QSettings(CONGFILEPATH, QSettings::IniFormat);
    configIniRead->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString name,pol,describe,number;
    foreach (QString group, configIniRead->childGroups())
    {
      configIniRead->beginGroup(group);
      if(group=="PORT_MAX")
      {
         m_input_maxnum = configIniRead->value("INPUTNUMBER").toInt();
      }
      if(group.left(6)=="INPUT_")//input
      {
          foreach (QString key, configIniRead->childKeys())
          {
               QTextCodec *codec=QTextCodec::codecForName("UTF-8");
               QString outStr = codec->toUnicode(key.toLatin1());
               if (outStr == "NUM")
               {
                  number = configIniRead->value(outStr).toString();
                  int numtmp = number.toInt();
                  if(numtmp>=0)
                  {
                      QString str1 = configIniRead->value("NAME").toString();
                      if(!str1.isEmpty())
                      {
                         if(str1.left(1)=="I")
                         {
                           name=str1.left(5);
                           describe=str1.mid(6);
                         }
                         else
                         {
                             name ="";
                             describe=str1;
                         }
                      }
                      QString  str2 = configIniRead->value("POL").toString();
                      if(!str2.isEmpty())
                      {
                         pol=str2.left(2);
                      }
                      portnum_list.push_back(numtmp);
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

//void CIniFileshow::combox_process(int index)
//{
//    int currentrow = 0;
////    qDebug() <<"slot is actived!";
//    for(unsigned int i=0;i<tablecount;i++)
//    {
//        if(sender()==combox_list.at(i))
//        {
//            currentrow = i;
////            qDebug() <<"combox i!";
//        }
//    }
//    QString combox_text = combox_list.at(currentrow)->currentText();
//    //极性组合框值改变且与初始值不同，触发记录改变的值
//    if(pol_list.at(currentrow).compare(combox_text))
//    {
//        qDebug() <<"combox is changed!";
//        QString poltext = QString::number(index,10);//极性值
//        if(initialpol_list.at(currentrow).compare(combox_text))
//        {
//            QString print = group_list.at(currentrow);//QString::number(index, 10);
//            modifygroup_list.append(print);
//            qDebug() <<print;
//            modifypol_list.append(poltext);//改变极性时临时保存，按保存按钮时写入文件
//            pol_list[currentrow]=poltext;//改变极性后更新当前值
//            qDebug() <<poltext;
//        }
//        else
//        {
//            pol_list[currentrow]=poltext;
//            qDebug() <<poltext;
//        }
//    }
//}

bool CIniFileshow::isPolChanged()
{
    bool ret = false;
    modifygroup_list.clear();
    modifypol_list.clear();
    modifypolpos_list.clear();
    for(unsigned int i=0;i<tablecount;i++)
    {
        QString combox_text = combox_list.at(i)->currentText();//获取值这一列所有数据
        if(pol_list.at(newpos_list.at(i)).compare(combox_text))
        {
            m_bPolModifyFlg = true;
            QString print = group_list.at(newpos_list.at(i));
            modifygroup_list.append(print);
            modifypol_list.append(combox_text);
            modifypolpos_list.append(newpos_list.at(i));
            qDebug() <<"isPolChanged";
            ret = true;
        }
    }
    return ret;
}

bool CIniFileshow::SaveParameter()
{
    bool ret= false;
    if(!m_bPolModifyFlg)
    {
        qDebug() <<"no item pol is changed!";
        return true;
    }
    qDebug() <<"Input pol:SaveParameter";
    CIniRW configIniWrite(CONGFILEPATH);
    QVector<QString>::iterator iter;
    int offset=0;
    for(iter=modifygroup_list.begin();iter!=modifygroup_list.end();iter++)
    {
//        int default_val = configIniWrite.ReadIni(*iter,"DEFAULT").toInt();
//        int newdefault_val = 0;
//        if(default_val==0) newdefault_val = 1;
//        else if(default_val==1) newdefault_val = 0;
        configIniWrite.WriteIni(*iter,"POL",modifypol_list.at(offset));
//        configIniWrite.WriteIni(*iter,"DEFAULT",QString::number(newdefault_val,10));
        qDebug() <<*iter;
        qDebug() <<modifypol_list.at(offset);
        //修改后更新值
        pol_list[modifypolpos_list.at(offset)]=modifypol_list.at(offset);
        offset++;
    }
    if(offset>0)
    {
        ret= true;
    }
    m_bPolModifyFlg = false;//保存数据后恢复标志位
    return ret;
}

//判断端口号是否改变
int CIniFileshow::isPortChanged()
{
    int ret = -1;
    modifyportgroup_list.clear();
    modifyport_list.clear();
    modifyportpos_list.clear();
    QVector<QString> itemvalue_list;
    itemvalue_list.clear();
    for(unsigned int m=0;m<tablecount;m++)
    {
        itemvalue_list.push_back(ui->tableWidget->item(m,2)->text());
    }
    for(unsigned int i=0;i<tablecount;i++)
    {
        if(portnumback_list.at(newpos_list.at(i)).compare(itemvalue_list.at(i)))
        {
            for(unsigned int j=0;j<tablecount;j++)
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
                else if(itemvalue_list.at(i).toInt()>m_input_maxnum)//无效端口
                {
                   ret = 3;
                   return ret;
                }
            }
            m_bInPortModifyFlg = true;
            QString print = group_list.at(newpos_list.at(i));
            modifyportgroup_list.append(print);
            modifyport_list.append(itemvalue_list.at(i));
            modifyportpos_list.append(newpos_list.at(i));
            qDebug() <<"Input number is changed";
            ret = 2;
        }
    }
    return ret;
}

bool CIniFileshow::PortSaveParameter()
{
    bool ret= false;
    if(!m_bInPortModifyFlg)
    {
        qDebug() <<"no inport item is changed!";
        return true;
    }
    qDebug() <<"Inport SaveParameter";
    CIniRW inconfigIniWrite(CONGFILEPATH);
    QVector<QString>::iterator iter;
    int offset=0;
    for(iter=modifyportgroup_list.begin();iter!=modifyportgroup_list.end();iter++)
    {
        inconfigIniWrite.WriteIni(*iter,"NUM",modifyport_list.at(offset));
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
    m_bInPortModifyFlg = false;//保存数据后恢复标志位
    return ret;
}
