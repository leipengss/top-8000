#ifndef LIGHTRTU_H
#define LIGHTRTU_H

#include <QDialog>
#include <QString>
#include <QList>
//#include "inkRTU.h"
#include <QTimer>
#include <QSettings>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
namespace Ui {
class lightRTU;
}

class lightRTU : public QDialog
{
    Q_OBJECT

public:
    explicit lightRTU(QWidget *parent = 0);
    ~lightRTU();
virtual bool sendframe(int uporlower);
    void closeEvent(QCloseEvent *);
    //bool eventFilter(QObject *, QEvent *);

private:
    Ui::lightRTU *ui;
    void readink(QString ink);
    void readink2(QString ink,QString ink2);
    bool   peizhiand(int slave);
    bool  peizhibool;
    //inkRTU *in;
   // void sendrtuData(int slave,int addr,int da);
      bool sendrtuData(int slave,int addr ,int data);
     bool sendrtuData2(int slave,int addr, QList<int> list);
     QTcpServer *server;
     QTcpSocket *socket;
     QTimer *m_pollTimer;
     int slave1,slave2,slave3,slave4,slave5,slave6,slave7,slave8,slave9,slave10,slave11,slave12,slave13,slave14,slave15,slave16;
     QList<QStringList> Glist;
     int SendCount;
     int gLDsize,gUPsize;
     bool flag;
     QByteArray m_buffer;//读取的缓存数据
       int m_buff_size;//文件大小  上下框 0 10
public slots:
      // QStringList sendthree(int slave,int addr,int num);
private slots:
   void on_login_Btn_clicked();
   void on_lightBtn_clicked();
   void on_light_saveBtn_clicked();//发送
   void on_communicationBtn_clicked();
   void on_comboBox_up_currentTextChanged(const QString &arg1);
   void on_communicationBtn_2_clicked();

    void on_light_saveBtn_2_clicked();
    void on_light_saveBtn_3_clicked();
    void on_pushButton_clicked();

    void on_deleteBtn_clicked();
    void on_addBtn_clicked();
    void on_comboBox_up_currentIndexChanged(int index);
    void on_lightText_selectionChanged();
    void on_addBtn_2_clicked();
    void on_deleteBtn_2_clicked();


    //void SocketConnect();
    void heartCheckedTimeout();
    void on_sysBtn_2_clicked();
    void on_iniBtn_clicked();
    void on_iniBtn_2_clicked();
    void on_checkedBtn_clicked();
    void on_deleteBtn_3_clicked();

    void on_addBtn_3_clicked();

    void on_deleteBtn_4_clicked();

    void on_addBtn_4_clicked();

    void SocketConnect();
    void SocketReadData();
    void SocketDisconnet();
    void on_sendButton_clicked(QByteArray json);
    void startServer();
    void on_iniBtn_3_clicked();

signals:
   void sendSigalData(bool b);//发送数据信号
public slots:
   bool returnlightRTU(int i);//933地址发送 0/8
   void returnlightRTU(bool b);//脉冲发送
    QString  returnlightUpInkRTU();
    QString  returnlightLowerInkRTU();
   // bool cmd_setAbsFramePost( int frame );//设置电源箱在上框或下框时灯管曝光参数

    void slot_showUI();
    void slot_showTimeRate(double,double);
public:

     bool returnlightRTU2(int inkint_1,int inkint_2,int inkint_3,int inkint_4,int inkint_5,int m_buff_size);//高级模式单灯模式 0获取时间比例  1代表上框发送数据 2代表下框发送数据 返回0 代表失败 1代表成功
    // void set_timeValue(double time,double time2);//设置 曝光时间 time=上下框最滴曝光时间 time2=最低限制曝光时间
  //   QMap<int,QList<int> > mapSlave_SendData,mapSlave_SendData2;
      QString gaojiModel;
      bool hhh(QList<int> inkList,QStringList list,QMap<int,QList<int> > mapSlave_SendData,QString text,QString text2,int upinkIndex,int lowerink,int Frame);

};

#endif // LIGHT_H
