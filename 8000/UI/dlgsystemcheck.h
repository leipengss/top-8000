#ifndef DLGSYSTEMCHECK_H
#define DLGSYSTEMCHECK_H
#include <QDialog>
#include"cinifileshow.h"
#include"clogfileshow.h"

namespace Ui {
class DlgSystemcheck;
}

class DlgSystemcheck : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSystemcheck(QWidget *parent = 0);
    ~DlgSystemcheck();

private:
    Ui::DlgSystemcheck *ui;
    CIniFileshow *inifileshow;
    CLogFileshow *histroyshow;
    int nCount;
    int nIndex;
    int m_output_maxnum;
    unsigned int outtablecount;
    QTableWidgetItem *item;
    vector<int> newpos_list;
    QVector<QString> pol_list;
    QVector<QString> group_list;
    QVector<QString> name_list;
    QVector<QString> describe_list;
    QVector<QString> portnumback_list;
    QVector<QPushButton*> pushbutton_list;
    vector<int> portnum_list;
    bool ParseINIFile();
    //端口是否可操作
    bool IsOperated(QString name);
private slots:
    void onTableBtnClicked();
    void click_btn_exit();
    void on_tabWidget_syscheck_currentChanged(int index);
    void on_btn_clear_clicked();
};

#endif // DLGSYSTEMCHECK_H
