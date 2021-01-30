#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <mainwindow.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include<QDesktopWidget>
#include"function/cscreenprotect.h"
#include<QTranslator>
#include<QTextCodec>
#include<QMutex>
#include<QTextStream>
#include "modbus/mainwindowRTU.h"
#include "modbus/mainwindowTCP.h"

#define CH_PATH              "/usr/share/runf/language/translator_CH.qm"
#define CH_FAN_PATH          "/usr/share/runf/language/translator_FAN.qm"
#define EN_PATH              "/usr/share/runf/language/translator_EN.qm"
#define KOREA_PATH           "/usr/share/runf/language/translator_Korea.qm"
#define JAPAN_PATH           "/usr/share/runf/language/translator_Japan.qm"
#define VIETNAM_PATH         "/usr/share/runf/language/translator_Vietnam.qm"

MainWindowRTU * globalMainWinRTU = NULL;
MainWindowTCP * globalMainWinTCP = NULL;
void outputMessage(QtMsgType type,const char *msg)
{
    static QMutex mutex;
    mutex.lock();
    //控制台输出
    switch(type)
    {
    case QtDebugMsg:
        fprintf(stderr,"%s\n",msg);
        break;
    case QtWarningMsg:
        fprintf(stderr,"Warning:%s\n",msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr,"Critical:%s\n",msg);
        break;
    case QtFatalMsg:
        fprintf(stderr,"Fatal:%s\n",msg);
        abort();
    }
    //保存日志文件
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:%1").arg(msg);
        break;
    case QtWarningMsg:
        text = QString("Warning:%1").arg(msg);
        break;
    case QtCriticalMsg:
        text = QString("Critical:%1").arg(msg);
        break;
    case QtFatalMsg:
        text = QString("Fatal:%1").arg(msg);
        abort();
    }
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = text+current_date;
    QFile file("log.txt");
//    if(file.size()>1024*1024*3)
//    {
//        qDebug()<<"logfile is 100M";
//        file.open(QFile::WriteOnly|QFile::Truncate );
//        file.close();
//    }
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream.setCodec(QTextCodec::codecForName("UTF-8"));
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    MainWindowRTU mRTU;
    globalMainWinRTU=&mRTU;
    MainWindowTCP mTCP;
    globalMainWinTCP= &mTCP;
    QTranslator translator;
    CIniRW lanconf( MACHINECONGFILEPATH);
    int language = lanconf.ReadIni("LANGUAGE","language").toInt();
    if(language==0)//简体
    {
        translator.load(CH_PATH);
    }
    else if(language==1)//繁体
    {
        translator.load(CH_FAN_PATH);
    }
    else if(language==2)//英文
    {
        translator.load(EN_PATH);
    }
    else if(language==3)//
    {
        translator.load(KOREA_PATH);
    }
    else if(language==4)//
    {
        translator.load(JAPAN_PATH);
    }
    else if(language==5)//
    {
        translator.load(VIETNAM_PATH);
    }
    app.installTranslator(&translator);
    //日志文件大于3M时开机清除
    QFile debugfile("log.txt");
    if(debugfile.size()>1024*1024*10)
    {
        qDebug()<<QObject::tr("清除日志");
        //清除打印
        debugfile.open(QFile::WriteOnly|QFile::Truncate );
        debugfile.close();
    }
    //注册MessageHandler
//    qInstallMsgHandler(outputMessage);
    qDebug()<<QObject::tr("开机启动");
    //splash screen
    QPixmap pixmap(":/images/image/top.gif");
    QSplashScreen screen(pixmap);
    screen.show();
    screen.showMessage(QObject::tr("正在载入，请稍等..."), Qt::AlignCenter, Qt::blue);
    //启动画面停留时间
    int delayTime = 3;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
         app.processEvents();
    }
    MainWindow w;
    //设置程序名
    w.setWindowTitle("TOP-FPA");
    w.setWindowIcon(QIcon(":/images/image/logo.png"));
    w.move((QApplication::desktop()->width() - w.width())/2,
                (QApplication::desktop()->height() - w.height())/2);//居中显示
    w.show();
    screen.finish(&w);//启动画面停留结束
    screen.close();
    //屏蔽屏保功能
//    CScreenProtect *screenprotect = new CScreenProtect;
//    app.installEventFilter(screenprotect);
    return app.exec();
}
