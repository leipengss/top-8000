#ifndef CINIFILESHOW_H
#define CINIFILESHOW_H

#include <QWidget>
#include <QTableWidgetItem>
#include<QComboBox>
#include<QVector>
//#include<QTimer>
#include<QThread>
#include <QMutexLocker>
#include <QtAlgorithms>
#include <vector>
#include <algorithm>//注意要包含该头文件
using namespace std;

namespace Ui {
class CIniFileshow;
}

class UpdateIOThread : public QThread
{
    Q_OBJECT

public:
    explicit UpdateIOThread(QObject *parent = 0)
        : QThread(parent),
          m_bStopped(false)
    {
//        qDebug() << "Update Thread : " << QThread::currentThreadId();
    }

    ~UpdateIOThread()
    {
        stop();
        quit();
        wait();
    }
    void stop();
    void run();
    QWidget *ptr_thread;
signals:
    void send_message(int,QString);

private:
    bool m_bStopped;
    QMutex m_mutex;
};

class CIniFileshow : public QWidget
{
    Q_OBJECT

public:
    explicit CIniFileshow(QWidget *parent = 0);
    ~CIniFileshow();

private:
    Ui::CIniFileshow *ui;
    UpdateIOThread *updatethread;
    QVector<QComboBox*> combox_list;
    QVector<QString> pol_list;
    QVector<QString> group_list;
    QVector<QString> name_list;
    QVector<QString> describe_list;
    QVector<QString> portnumback_list;
    vector<int> portnum_list;
    vector<int> newpos_list;
    QVector<QString> modifygroup_list;
    QVector<QString> modifypol_list;
    QVector<int> modifypolpos_list;
    //
    QVector<QString> modifyportgroup_list;
    QVector<QString> modifyport_list;
    QVector<int> modifyportpos_list;

    QTableWidgetItem *item;
    QComboBox *combox;
    unsigned int tablecount;
    bool isPOLEdit;
    void Init_List();
    bool ParseINIFile();
    void addItemContent(int row, int column, QString content);
    void addItemCombox(int row, int column, QString content);
    void setui();
public:
    bool m_bPolModifyFlg;
    bool m_bInPortModifyFlg;
    bool isPolChanged();
    int isPortChanged();
    bool PortSaveParameter();
    void ui_exit();
    void ModelShow(bool isedit);//isedit 0:show  1:io parameter
    bool SaveParameter();
    int m_input_maxnum;
private slots:
//    void combox_process(int index);
    void update_io(int row,QString tr);
};

#endif // CINIFILESHOW_H
