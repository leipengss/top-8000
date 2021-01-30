#ifndef DLGHOME_H
#define DLGHOME_H

#include <QDialog>
#include"driver/cupdatedatathread.h"
#include<QThread>
#include <QMutexLocker>
//#include "driver/global.h"

namespace Ui {
class DlgHome;
}

class HomeThread : public QThread
{
    Q_OBJECT

public:
    explicit HomeThread(QObject *parent = 0)
        : QThread(parent),
          m_bStopped(false)
    {

    }

    ~HomeThread()
    {
        stop();
        quit();
        wait();
    }
    void stop();
    void run();
    void homeall();
    void homept();
signals:
    void send_message(int type,QString text);
private:
    bool wait_aux(int axis);
    bool m_bStopped;
    QMutex m_mutex;
};

class DlgHome : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHome(QWidget *parent = 0);
    ~DlgHome();
    int moorrestardelay;
private slots:
    void on_btn_homeall_clicked();

    void on_btn_homept_clicked();

    void on_btn_home_exit_clicked();
    void showtips(int tipstype, QString str_data);
signals:
    void send_showtip(int type,QString text);
private:
    Ui::DlgHome *ui;
    HomeThread *homeall_thread;
    HomeThread *pthome_thread;
    void btn_enable(bool flg);
    //非阻塞延时 ms
    void ms_sleep(unsigned int msec);
};

#endif // DLGHOME_H
