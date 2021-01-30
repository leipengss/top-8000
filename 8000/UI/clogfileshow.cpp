#include "clogfileshow.h"
#include "ui_clogfileshow.h"
#include<QFile>
#include<QTextStream>
#include<QTextCodec>

CLogFileshow::CLogFileshow(QString file,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLogFileshow)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    //
    filename = file;
    logshow();
}

CLogFileshow::~CLogFileshow()
{
    delete ui;
}

void CLogFileshow::logshow()
{
    LogfileRead();
    tablecount = text_list.size();
    QStringList header;
    header<<QObject::tr("历史信息");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(tablecount);//设置行数
    //设置表格的各列的宽度值
    ui->tableWidget->setColumnWidth(0,400);
    showlog();
}

void CLogFileshow::LogfileRead()
{
    QFile log(filename);
    if (!log.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
    text_list.clear();
    QTextStream out(&log);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!out.atEnd())
    {
        QString text = out.readLine();
        text_list.append(text);
    }
}
void CLogFileshow::showlog()
{
    for(int row=0;row<tablecount;row++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        if(text_list.at(tablecount-row-1).left(3)=="WAR")
        {
           item->setBackgroundColor(QColor(Qt::yellow));
        }
        else if(text_list.at(tablecount-row-1).left(3)=="ERR")
        {
           item->setBackgroundColor(QColor(0xFF,0xB6,0xC1));//浅红
        }
        else if(text_list.at(tablecount-row-1).left(3)=="NOR")
        {
           item->setBackgroundColor(QColor(Qt::lightGray));
        }
        item->setText(text_list.at(tablecount-row-1));//从文件末尾开始显示历史最新数据
        ui->tableWidget->setItem(row,0,item);
    }
}
void CLogFileshow::clear()
{
    QFile file(filename);
    file.open(QFile::WriteOnly|QFile::Truncate );
    file.close();
    ui->tableWidget->clearContents();
    //清除打印
    QFile debugfile("log.txt");
    debugfile.open(QFile::WriteOnly|QFile::Truncate );
    debugfile.close();
}
