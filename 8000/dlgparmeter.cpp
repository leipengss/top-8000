#include "dlgparmeter.h"
#include "ui_dlgparmeter.h"
#include <QDebug>
#include "driver/global.h"
#include"filemanage/cinirw.h"
#include <QMessageBox>
dlgparmeter::dlgparmeter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgparmeter)
{
    ui->setupUi(this);
    list.clear();//初始化
    qmap.clear();
     ui->lineEdit_DWaccuracy->setValidator(new QIntValidator(0, 1000, this));
     ui->lineEdit_junfenX->setValidator(new QIntValidator(0, 1000, this));
     ui->lineEdit_junfenY->setValidator(new QIntValidator(0, 1000, this));
    // ui->lineEdit_movedistance->setValidator(new QIntValidator(0, 1000, this));
     ui->lineEdit_wuchaX->setValidator(new QIntValidator(0, 1000, this));
     ui->lineEdit_wuchaY->setValidator(new QIntValidator(0, 1000, this));
     input = new DlgInputkey(this);
     input->setParent(ui->frame_key);
     // 把子窗口的信号连接到父窗口来。
     connect(input, SIGNAL(okEvent()),this, SLOT(on_bginput_okEvent()));
     connect(input, SIGNAL(cancelEvent()),this, SLOT(on_bginput_cancelEvent()));
     CIniRW inifile(TOPPARAMETERPATH);
       QString  QLIST_PA = inifile.ReadIni("MOVEPATH","LIST_PA").toString();
       int check = inifile.ReadIni("MOVEPATH","BM_KG").toInt();
       if(1==check){
        ui->checkBox->setChecked(true);
       }else{
        ui->checkBox->setChecked(false);
       }
        int cf = inifile.ReadIni("MOVEPATH","BM_CF").toInt();
        if(1==cf){
         ui->checkBox_2->setChecked(true);
        }else{
         ui->checkBox_2->setChecked(false);
        }
       if(QLIST_PA.size()>0){
           QStringList group_list =  QLIST_PA.split(",");

           for(int i=1;i<=group_list.size();i++){
               qmap.insert(QString::number(i),group_list.at(i-1));
           }
           QStringList List_parmeter =  group_list.at(0).split("/");
            if(List_parmeter.size()<6)return;
            //界面数据
            ui->lineEdit_DWaccuracy->setText(List_parmeter.at(0));
            ui->lineEdit_junfenX->setText(List_parmeter.at(1));
            ui->lineEdit_junfenY->setText(List_parmeter.at(2));
            ui->lineEdit_movedistance->setText(List_parmeter.at(3));
            ui->lineEdit_wuchaX->setText(List_parmeter.at(4));
            ui->lineEdit_wuchaY->setText(List_parmeter.at(5));
       }

       qDebug()<<qmap.value("1")<<qmap.value("2")<<qmap.value("3")<<QLIST_PA;


}

dlgparmeter::~dlgparmeter()
{
    delete ui;
}
void dlgparmeter::on_bginput_okEvent()
{
    int currhh =  ui->label_duan->text().toInt();
    if(currhh==1&&g_par.bg_down_time>1){
        on_btn_next_clicked();
        on_btn_previous_clicked();

    }else{
        on_btn_next_clicked();

    }

      on_btn_save_clicked();

}

void dlgparmeter::on_bginput_cancelEvent()
{
    on_bginput_okEvent();
    reject();

//    if(true)
//    {
//        QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("参数有更改，是否保存?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
//        if(rb == QMessageBox::Yes)
//        {
//            on_btn_save_clicked();
//             this->close();
//        }
//        else
//        {
//            this->close();
//        }
//    }
//    else
//    {
//        this->close();
//    }
}
//下一页
void dlgparmeter::on_btn_next_clicked()
{
//    ui->lineEdit_DWaccuracy->clear();
//    ui->lineEdit_junfenX->clear();
//    ui->lineEdit_junfenY->clear();
//    ui->lineEdit_movedistance->clear();
//    ui->lineEdit_wuchaX->clear();
//    ui->lineEdit_wuchaY->clear();
    list.clear();
    int int_duan =  ui->label_duan->text().toInt();
    if(int_duan==g_par.bg_down_time){
        if(int_duan==1){
            //获取界面数据保存
            QString dw = ui->lineEdit_DWaccuracy->text()+"/";
           QString jfx =  ui->lineEdit_junfenX->text()+"/";
           QString jfy =  ui->lineEdit_junfenY->text()+"/";
            QString move = ui->lineEdit_movedistance->text()+"/";
            QString wcx = ui->lineEdit_wuchaX->text()+"/";
            QString wcy = ui->lineEdit_wuchaY->text();
            list.append(dw);
            list.append(jfx);
            list.append(jfy);
            list.append(move);
            list.append(wcx);
            list.append(wcy);
            qmap.insert(QString::number(int_duan),list);
            list.clear();
        }
     return;
    }
    //获取界面数据保存
    QString dw = ui->lineEdit_DWaccuracy->text()+"/";
   QString jfx =  ui->lineEdit_junfenX->text()+"/";
   QString jfy =  ui->lineEdit_junfenY->text()+"/";
    QString move = ui->lineEdit_movedistance->text()+"/";
    QString wcx = ui->lineEdit_wuchaX->text()+"/";
    QString wcy = ui->lineEdit_wuchaY->text();
    list.append(dw);
    list.append(jfx);
    list.append(jfy);
    list.append(move);
    list.append(wcx);
    list.append(wcy);
    qmap.insert(QString::number(int_duan),list);
    list.clear();


    if(qmap.contains(QString::number(int_duan+1))){
        qDebug()<<qmap.value("1")<<qmap.value("2")<<qmap.value("3")<<"#1";
        list =  qmap.value(QString::number(int_duan+1));
       QStringList List_parmeter = list.split("/");
        if(List_parmeter.size()<6)return;
        //界面数据
        ui->lineEdit_DWaccuracy->setText(List_parmeter.at(0));
        ui->lineEdit_junfenX->setText(List_parmeter.at(1));
        ui->lineEdit_junfenY->setText(List_parmeter.at(2));
        ui->lineEdit_movedistance->setText(List_parmeter.at(3));
        ui->lineEdit_wuchaX->setText(List_parmeter.at(4));
        ui->lineEdit_wuchaY->setText(List_parmeter.at(5));
    }else{
        qDebug()<<qmap.value("1")<<qmap.value("2")<<qmap.value("3")<<"#2";

        //清除界面数据
        ui->lineEdit_DWaccuracy->setText(QString::number(0));
        ui->lineEdit_junfenX->setText(QString::number(0));
        ui->lineEdit_junfenY->setText(QString::number(0));
        ui->lineEdit_movedistance->setText(QString::number(0));
        ui->lineEdit_wuchaX->setText(QString::number(0));
        ui->lineEdit_wuchaY->setText(QString::number(0));
    }
    ui->label_duan->setText(QString::number(int_duan+1));
}
//上一页
void dlgparmeter::on_btn_previous_clicked()
{

    list.clear();
   int int_duan =  ui->label_duan->text().toInt();
   if(int_duan==1){
       if(qmap.contains(QString::number(1))){
           list =  qmap.value(QString::number(1));
           QStringList List_parmeter = list.split("/");
           if(List_parmeter.size()<6)return;
            //界面数据
            ui->lineEdit_DWaccuracy->setText(List_parmeter.at(0));
            ui->lineEdit_junfenX->setText(List_parmeter.at(1));
            ui->lineEdit_junfenY->setText(List_parmeter.at(2));
            ui->lineEdit_movedistance->setText(List_parmeter.at(3));
            ui->lineEdit_wuchaX->setText(List_parmeter.at(4));
            ui->lineEdit_wuchaY->setText(List_parmeter.at(5));
        }
    return;
   }

   //获取界面数据保存
   QString dw = ui->lineEdit_DWaccuracy->text()+"/";
  QString jfx =  ui->lineEdit_junfenX->text()+"/";
  QString jfy =  ui->lineEdit_junfenY->text()+"/";
   QString move = ui->lineEdit_movedistance->text()+"/";
   QString wcx = ui->lineEdit_wuchaX->text()+"/";
   QString wcy = ui->lineEdit_wuchaY->text();
   list.append(dw);
   list.append(jfx);
   list.append(jfy);
   list.append(move);
   list.append(wcx);
   list.append(wcy);
   qmap.insert(QString::number(int_duan),list);
   list.clear();
   if(qmap.contains(QString::number(int_duan-1))){
       list =  qmap.value(QString::number(int_duan-1));
       QStringList List_parmeter = list.split("/");
       if(List_parmeter.size()<6)return;
        //界面数据
        ui->lineEdit_DWaccuracy->setText(List_parmeter.at(0));
        ui->lineEdit_junfenX->setText(List_parmeter.at(1));
        ui->lineEdit_junfenY->setText(List_parmeter.at(2));
        ui->lineEdit_movedistance->setText(List_parmeter.at(3));
        ui->lineEdit_wuchaX->setText(List_parmeter.at(4));
        ui->lineEdit_wuchaY->setText(List_parmeter.at(5));
    }else{
        //清除界面数据
//        ui->lineEdit_DWaccuracy->setText(QString::number(0));
//        ui->lineEdit_junfenX->setText(QString::number(0));
//        ui->lineEdit_junfenY->setText(QString::number(0));
//        ui->lineEdit_movedistance->setText(QString::number(0));
//        ui->lineEdit_wuchaX->setText(QString::number(0));
//        ui->lineEdit_wuchaY->setText(QString::number(0));
    }


   ui->label_duan->setText(QString::number(int_duan-1));
}

void dlgparmeter::on_btn_save_clicked()
{
    list.clear();
    int int_duan =  ui->label_duan->text().toInt();
    if(int_duan==g_par.bg_down_time){
        //获取界面数据保存
        QString dw = ui->lineEdit_DWaccuracy->text()+"/";
       QString jfx =  ui->lineEdit_junfenX->text()+"/";
       QString jfy =  ui->lineEdit_junfenY->text()+"/";
        QString move = ui->lineEdit_movedistance->text()+"/";
        QString wcx = ui->lineEdit_wuchaX->text()+"/";
        QString wcy = ui->lineEdit_wuchaY->text();
        list.append(dw);
        list.append(jfx);
        list.append(jfy);
        list.append(move);
        list.append(wcx);
        list.append(wcy);
        qmap.insert(QString::number(int_duan),list);
    }
    list.clear();
    for(int i=1;i<=g_par.bg_down_time;i++){
       QString mapvulue =  qmap.value(QString::number(i));
       if(i==g_par.bg_down_time){
       list+=mapvulue;
       }else{
           list+=mapvulue+",";
       }
    }
    CIniRW inifile(TOPPARAMETERPATH);
    inifile.WriteIni("MOVEPATH","LIST_PA",list);
    if(ui->checkBox->isChecked()){
        inifile.WriteIni("MOVEPATH","BM_KG","1");
    }else{
        inifile.WriteIni("MOVEPATH","BM_KG","0");
    }
    if(ui->checkBox_2->isChecked()){
        inifile.WriteIni("MOVEPATH","BM_CF","1");
    }else{
        inifile.WriteIni("MOVEPATH","BM_CF","0");
    }
}

void dlgparmeter::on_btn_leave_clicked()
{
    reject();
}



void dlgparmeter::on_btn_bigbm_toggled(bool checked)
{

    if(checked){
        qDebug()<<checked<<"1";
    }else{
        qDebug()<<checked<<"2";
    }

}


