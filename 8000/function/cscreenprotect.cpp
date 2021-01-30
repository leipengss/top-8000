#include "cscreenprotect.h"
#include <QEvent>
#include <QApplication>
#include <QPixmap>
#include<QDesktopWidget>
#include "driver/global.h"
#include"filemanage/cinirw.h"

CScreenProtect::CScreenProtect(QObject *parent):
    QObject(parent)
{
    QString urlPath = ":/images/image/top.gif";
    CIniRW waittimefile(TOPPARAMETERPATH);
    int waittime = waittimefile.ReadIni("TOP","SCREEN_WAIT").toInt()*1000;
    timer = new QTimer;
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    timer->start(waittime);
    //屏保窗口
    QRect screenRect = QApplication::desktop()->screenGeometry(0);
    label = new QLabel();
    label->setGeometry(screenRect);
    label->setWindowFlags(Qt::FramelessWindowHint);
    //拉伸背景图片
    label->setScaledContents(true);
    QPixmap pmp;
    pmp.load(urlPath);
    label->setPixmap(pmp);
    label->hide();
}

CScreenProtect::~CScreenProtect()
{

}

bool CScreenProtect::eventFilter(QObject *obj, QEvent *event)
{
    //判断事件类型
    if (event->type() == QEvent::KeyPress /*|| event->type() == QEvent::MouseMove*/
        || event->type() == QEvent::MouseButtonPress)
    {
        //有鼠标或键盘事件则重置timer
        int time = g_par.screen_waittime*1000;
        timer->start(time);
        label->hide();
        g_isScreenStart = false;
    }
    return QObject::eventFilter(obj, event);
}

void CScreenProtect::slot_timeout()
{
    //显示屏保
    label->activateWindow();
    label->show();
    g_isScreenStart = true;
}
