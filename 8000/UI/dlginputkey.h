#ifndef DLGINPUTKEY_H
#define DLGINPUTKEY_H

#include <QWidget>

namespace Ui {
class DlgInputkey;
}

class DlgInputkey : public QWidget
{
    Q_OBJECT

public:
    explicit DlgInputkey(QWidget*pWidget,QWidget *parent = 0);
    ~DlgInputkey();

private slots:
    void on_btn_0_clicked();

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_left_clicked();

    void on_btn_right_clicked();

    void on_btn_clear_clicked();

    void on_btn_backspace_clicked();

    void on_btn_ok_clicked();

    void on_btn_back_clicked();
signals:
    // 这个信号是发给父窗口的。
    void okEvent();
    void cancelEvent();
private:
    Ui::DlgInputkey *ui;
    QWidget* pWidgetObj;
};

#endif // DLGINPUTKEY_H
