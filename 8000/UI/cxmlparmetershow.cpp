#include "cxmlparmetershow.h"
#include "ui_cxmlparmetershow.h"
#include<QDebug>
#include <QTableWidget>
#include "driver/global.h"
#include"filemanage/cinirw.h"
#include"filemanage/cinputrangecheck.h"
#include"driver/global.h"

Cxmlparmetershow::Cxmlparmetershow(int num,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cxmlparmetershow)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    //
    offset = num;
    xmlparse = new CXMLParameter(CNCPARAMETERPATH);
    if(xmlparse->readXml()<0)
    {
        qDebug()<<"para.xml is broken!";
    }
    QVector<QString> temp_list(xmlparse->rootname_list);
    parrootname_list.swap(temp_list);
    //获取PPMC中参数实际值
    int group_num = xmlparse->groupname_list.size();
    QString readpath;
    if(offset==0)
    {
        readpath = TOPPARAMETERPATH;
    }
    else
    {
        readpath = CONGFILEPATH;
    }
    CIniRW inifile(readpath);
    for(int n=0;n<group_num;n++)
    {
       QString tmp =inifile.ReadIni(xmlparse->groupname_list.at(n),xmlparse->keyname_list.at(n)).toString();
       parvalue_list.append(tmp);
    }
//    qDebug()<<"value="<<parvalue_list[1];
    isrestart = false;
}

Cxmlparmetershow::~Cxmlparmetershow()
{
    delete xmlparse;
    delete ui;
}

void Cxmlparmetershow::xmlparshow()
{
    firstnodenum = xmlparse->rootname_list.size();
//        qDebug()<<"tab="<<QString::number(firstnodenum,10);
    ui->tableWidget->setColumnCount(4); //设置列数
    //设置表格行标题
    QStringList headerLabels;
    headerLabels<<QObject::tr("编号")<<QObject::tr("名称")<<QObject::tr("值")<<QObject::tr("生效时间");
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    //设置表格的各列的宽度值
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,300);
    ui->tableWidget->setColumnWidth(2,140);
    ui->tableWidget->setColumnWidth(3,100);
    //设置表格行标题的对齐方式
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    parrow = xmlparse->firstnodename_list.size()/firstnodenum;
//    qDebug()<<"parrow="<<QString::number(parrow,10);
    ui->tableWidget->setRowCount(parrow);//设置行数
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为时每次选择一行
    for(int r=0;r<parrow;r++)
    {
        // 设置单元格不可编辑
        QTableWidgetItem *item1 = new QTableWidgetItem();
        item1->setFlags(item1->flags() & (~Qt::ItemIsEditable));
        item1->setText(xmlparse->firstnodename_list[r+offset*parrow]);
        ui->tableWidget->setItem(r, 0, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem();
        item2->setFlags(item2->flags() & (~Qt::ItemIsEditable));
        item2->setText(xmlparse->chinaname_list[r+offset*parrow]);
        ui->tableWidget->setItem(r, 1, item2);
        //值列可以编辑
        ui->tableWidget->setItem(r, 2, new QTableWidgetItem(parvalue_list[r+offset*parrow]));

        QTableWidgetItem *item3 = new QTableWidgetItem();
        item3->setFlags(item3->flags() & (~Qt::ItemIsEditable));
        if(!xmlparse->reboot_list[r+offset*parrow].compare("1"))
        {
            item3->setText(QObject::tr("重启生效"));
        }
        else if(!xmlparse->reboot_list[r+offset*parrow].compare("0"))
        {
            item3->setText(QObject::tr("立即生效"));
        }
        ui->tableWidget->setItem(r, 3,item3);
    }
}
//显示参数描述
void Cxmlparmetershow::on_tableWidget_cellClicked(int row, int column)
{
    if(column>3 || column<0)
    {
        qDebug()<<"error column="<<QString::number(column,10);
        return;
    }
    int pos = row+offset*parrow;
    QString text = xmlparse->describe_list.at(pos);
    ui->textEdit_describe->setText(text);
}

//参数修改
bool Cxmlparmetershow::isParChanged()
{
     m_bModifyFlg = false;
     modify_position.clear();
     newvalue_list.clear();
     QTableWidgetItem *item = new QTableWidgetItem();
     for(int m=0;m<parrow;m++)
     {
         item = ui->tableWidget->item(m,2);//获取值这一列所有数据
         newvalue_list.append(item->text());
//         qDebug() <<"m="+QString::number(m,10)<<"newvalue="+newvalue_list.at(m)<<"parvalue="+parvalue_list.at(m+offset*parrow);
         if(newvalue_list.at(m).compare(parvalue_list.at(m+offset*parrow)))
         {
             m_bModifyFlg = true;
             modify_position.append(m);//记录newvalue_list修改项的位置
             qDebug() <<"isPolChanged";
         }
     }
     return m_bModifyFlg;
}
//更新全局参数,重启生效的参数不在此更新
void Cxmlparmetershow::updatepara(int tableindex,int pos)
{
    if(tableindex!=0) return;
    int code = xmlparse->firstnodename_list[pos].right(4).toInt();
    switch(code)
    {
    case 0://"P0000":
        g_par.x_speed = parvalue_list[pos].toFloat();
        break;
    case 1://"P0001":
        g_par.y_speed = parvalue_list[pos].toFloat();
        break;
    case 2://"P0002":
        g_par.z_minspeed = parvalue_list[pos].toFloat();
        break;
    case 3://"P0003":
        g_par.z_midspeed = parvalue_list[pos].toFloat();
        break;
    case 4://"P0004":
        g_par.z_speed = parvalue_list[pos].toFloat();
        break;
    case 5://"P0005":
        g_par.pt_speed = parvalue_list[pos].toFloat();
        break;
//    case 6://"P0006":
//        g_par.downpt_speed = parvalue_list[pos].toFloat();
//        break;
    case 6://"P0006":
        g_par.w_speed = parvalue_list[pos].toFloat();
        break;
    case 7://"P0007":
        g_par.bg_up_energyoveride = parvalue_list[pos].toFloat();
        break;
    case 8://"P0008":
        g_par.bg_down_energyoveride = parvalue_list[pos].toFloat();
        break;
    case 9://"P0009":
        g_par.bg_openzg = parvalue_list[pos].toInt();
        break;
    case 10://"P0010":
        g_par.move_delay = parvalue_list[pos].toFloat();
        break;
    case 11://"P0011":
        g_par.bg_movedelay = parvalue_list[pos].toFloat();
        break;
    case 12://"P0012":
        g_par.frame_close_delay_a = parvalue_list[pos].toInt();
        break;
    case 13://"P0013":
        g_par.frame_close_delay_b = parvalue_list[pos].toInt();
        break;
    case 14://"P0014":
        g_par.frame2_close_delay_a = parvalue_list[pos].toInt();
        break;
    case 15://"P0015":
        g_par.frame2_close_delay_b = parvalue_list[pos].toInt();
        break;
    case 16://"P0016":
        g_par.screen_waittime = parvalue_list[pos].toInt();
        break;
    case 18://"P0018":
        g_par.light_speed = parvalue_list[pos].toFloat();
        break;
    case 19://"P0019":
        g_par.scan_length[0] = parvalue_list[pos].toFloat();
        break;
//    case 20://"P0020":
//        g_par.scan_length[1] = parvalue_list[pos].toFloat();
//        break;
    case 20://"P0020":
        g_par.ischecksafe = parvalue_list[pos].toInt();
        break;
    case 23://"P0023":
        g_par.islightsetting = parvalue_list[pos].toInt();
        break;
    }
    qDebug("code=P00%d x_speed=%.3f bg_movedelay=%.3f",code,g_par.x_speed,g_par.bg_movedelay);
}

//保存参数
int Cxmlparmetershow::XmlParSave()
{
    int ret = 1;
    int len = modify_position.size();
    if(len<=0)
    {
        ret = -1;
    }
    QString writepath;
    if(offset==0)
    {
        writepath = TOPPARAMETERPATH;
    }
    else
    {
        writepath = CONGFILEPATH;
    }
    CIniRW inifilewrite(writepath);
    for(int pos=0;pos<len;pos++)
    {
        int xmllist_pos = modify_position.at(pos)+offset*parrow;//映射到parvalue_list
        if(isParOutRange(xmllist_pos,newvalue_list.at(modify_position.at(pos))))
        {
            inifilewrite.WriteIni(xmlparse->groupname_list.at(xmllist_pos),xmlparse->keyname_list.at(xmllist_pos),newvalue_list.at(modify_position.at(pos)));
            parvalue_list[xmllist_pos] = newvalue_list.at(modify_position.at(pos));//修改完参数后更新值
            if(!xmlparse->reboot_list.at(xmllist_pos).compare("1"))
            {
                g_isRestartSys = true;//判断参数项是否需要重启生效
                isrestart = true;
                qDebug()<<"isrestart";
            }
            else
            {
                updatepara(offset,xmllist_pos);
            }
        }
        else
        {
            ret = 0;
        }
    }
    return ret;
}

//检查参数范围
bool Cxmlparmetershow::isParOutRange(int pos,QString val)
{
    bool ret = true;
    CInputRangeCheck inputcheck;
    int type = xmlparse->typevalue_list.at(pos).toInt();
    int retval = inputcheck.checkrange(type,xmlparse->minvalue_list.at(pos),xmlparse->maxvalue_list.at(pos),val);
    if(retval<1)
    {
        ret = false;
        qDebug() <<"outrange:"<<"min="+xmlparse->minvalue_list.at(pos)<<"max="+xmlparse->maxvalue_list.at(pos)<<"max="+val;
    }
    return ret;
}
