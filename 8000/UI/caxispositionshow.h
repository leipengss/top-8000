#ifndef CAXISPOSITIONSHOW_H
#define CAXISPOSITIONSHOW_H

#include <QWidget>
#include<QTimer>

namespace Ui {
class CAxisPositionShow;
}

class CAxisPositionShow : public QWidget
{
    Q_OBJECT
    
public:
    explicit CAxisPositionShow(QWidget *parent = 0);
    ~CAxisPositionShow();
    
private:
    Ui::CAxisPositionShow *ui;
    QTimer *updatetimer;
    void ShowCurPos();
    void ShowCmdPos();
    void ShowFeedbackPos();
private slots:
    void slot_positionupdate();
    void on_btn_showpos_exit_clicked();
};

#endif // CAXISPOSITIONSHOW_H
