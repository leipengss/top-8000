#ifndef CSCREENPROTECT_H
#define CSCREENPROTECT_H
#include<QObject>
#include<QTimer>
#include<QLabel>
class CScreenProtect : public QObject
{
    Q_OBJECT

public:
    CScreenProtect(QObject *parent = NULL);
    ~CScreenProtect();
protected slots:
    void slot_timeout();
protected:
    //初始化屏保参数
    void init();
    //事件接收处理函数，由installEventFilter调用方在接收到事件时调用
    bool eventFilter(QObject *watched, QEvent *event);
private:
    //定时器
    QTimer   *timer;
    //用于显示屏保图片的对象
    QLabel   *label;
};
#endif // CSCREENPROTECT_H
