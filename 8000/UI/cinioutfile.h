#ifndef CINIOUTFILE_H
#define CINIOUTFILE_H

#include <QWidget>
#include <QWidget>
#include <QTableWidgetItem>
#include<QComboBox>
#include<QVector>
//#include<QTimer>
#include<QThread>
#include <QMutexLocker>
#include <vector>
#include <algorithm>//注意要包含该头文件

using namespace std;

namespace Ui {
class cinioutfile;
}

class UpdateOUTThread : public QThread
{
    Q_OBJECT

public:
    explicit UpdateOUTThread(QObject *parent = 0)
        : QThread(parent),
          m_bStopped(false)
    {
//        qDebug() << "Update Thread : " << QThread::currentThreadId();
    }

    ~UpdateOUTThread()
    {
        stop();
        quit();
        wait();
    }
    void stop();
    void run();
    QWidget *ptr_thread;
signals:
    void send_outmessage(int,QString);

private:
    bool m_bStopped;
    QMutex m_mutex;
};
class cinioutfile : public QWidget
{
    Q_OBJECT
    
public:
    explicit cinioutfile(QWidget *parent = 0);
    ~cinioutfile();    
private:
    Ui::cinioutfile *ui;
    UpdateOUTThread *updatethread;
    QTableWidgetItem *item;
    unsigned int outtablecount;
    QVector<QString> pol_list;
    QVector<QString> group_list;
    QVector<QString> name_list;
    QVector<QString> describe_list;
    QVector<QString> portnumback_list;
    QVector<QString> modifyportgroup_list;
    QVector<QString> modifyport_list;
    QVector<int> modifyportpos_list;
    vector<int> portnum_list;
    vector<int> newpos_list;
    void Init_List();
    void setui();
    bool ParseINIFile();
    void addItemContent(int row, int column, QString content);
public:
    bool m_bOutPortModifyFlg;
    int isPortChanged();
    void ui_exit();
    void ModelShow();
    bool SaveParameter();
    int m_output_maxnum;
private slots:
    void update_io(int row,QString tr);
};

#endif // CINIOUTFILE_H

