#ifndef DLGSUPERPARASETTING_H
#define DLGSUPERPARASETTING_H

#include <QDialog>
#include"dlginputkey.h"

namespace Ui {
class DlgSuperParaSetting;
}

class DlgSuperParaSetting : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSuperParaSetting(QWidget *parent = 0);
    ~DlgSuperParaSetting();

private slots:
    void on_superparainput_okEvent();
    void on_superparainput_cancelEvent();

    void on_radioButton_upframe_clicked();

    void on_radioButton_downframe_clicked();

    void on_radioButton_light_clicked();

    void on_radioButton_dwlock_clicked();

    void on_radioButton_supersetting_clicked();

    void on_radioButton_otherpara_clicked();

    void on_btn_light_clear_clicked();

    void on_btn_dwlock_cw_clicked();

    void on_btn_dwlock_ccw_clicked();

    void on_btn_dwlock_yplus_clicked();

    void on_btn_dwlock_xplus_clicked();

    void on_btn_dwlock_yminus_clicked();

    void on_btn_dwlock_xminus_clicked();

    void on_checkBox_canceltwo_clicked();

    void on_checkBox_cancelthree_clicked();

    void on_checkBox_issuperman_clicked();

    void on_checkBox_isdpload_clicked();

private:
    Ui::DlgSuperParaSetting *ui;
    DlgInputkey *input;
    void SetTextInputRange();
    void InitParameter();
    bool IsParaChanged();
    //save parameter
    void SavePara();
    //para
    QVector<QString> para;
    QVector<QString> paraback;
    bool m_bCancelTwo;
    bool m_bCancelTwoback;
    bool m_bOpenSuperman;
    bool m_bOpenSupermanback;
    bool m_bCancelThree;
    bool m_bCancelThreeback;
    int  m_iLightCnt;
    float  m_iLightTime;
    int  m_iLightCntback;
    float  m_iLightTimeback;
};

#endif // DLGSUPERPARASETTING_H
