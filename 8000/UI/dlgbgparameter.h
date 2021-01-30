#ifndef DLGBGPARAMETER_H
#define DLGBGPARAMETER_H

#include <QDialog>
#include"dlginputkey.h"

namespace Ui {
class dlgBGParameter;
}

class dlgBGParameter : public QDialog
{
    Q_OBJECT

public:
    explicit dlgBGParameter(QWidget *parent = 0);
    ~dlgBGParameter();

private slots:
    void on_radioButton_bgpara_clicked();

    void on_radioButton_upccd_check_clicked();

    void on_radioButton_downccd_check_clicked();

    void on_radioButton_ptsetting_clicked();

    void on_btn_fullpower_clicked();

    void on_btn_halfpower_clicked();

    void on_btn_energy_clicked();

    void on_btn_time_clicked();

    void on_btn_auto_clicked();

    void on_btn_manual_clicked();

    void on_btn_upframe_clicked();

    void on_btn_downframe_clicked();

    void on_btn_ccd2_clicked();

    void on_btn_ccd3_clicked();

    void on_btn_ccd4_clicked();

    void on_btn_down_ccd1234_clicked();

    void on_btn_down_checkallmarks_clicked();

    void on_checkBox_up_checkdistance_clicked();

    void on_checkBox_down_checkdistance_clicked();

    void on_checkBox_up_cycle_clicked();

    void on_checkBox_down_cycle_clicked();

    void on_checkBox_binarization_clicked();
private slots:
      void on_bginput_okEvent();
      void on_bginput_cancelEvent();
      void on_btn_up_ccd1234_clicked();
      void on_btn_up_checkallmarks_clicked();
      void on_btn_up_ccd1234_2_clicked();

      void on_btn_up_checkallmarks_2_clicked();

      void on_btn_down_ccd1234_2_clicked();

      void on_btn_down_checkallmarks_2_clicked();
      void on_lineEdit_up_piecehightmil_textEdited(const QString &arg1);

      void on_lineEdit_down_piecehightmil_textEdited(const QString &arg1);

      void on_lineEdit_up_piecehight_textEdited(const QString &arg1);

      void on_lineEdit_down_piecehight_textEdited(const QString &arg1);

      void on_lineEdit_downbg_time_textChanged(const QString &arg1);
public:
      /**上框影像检测装置**/
      QString m_sUp_checkdelay_back;
      QString m_sUp_maxjb_back;
      QString m_sUp_dwjingdu_back;
      QString m_sUp_dperror_x_back;
      QString m_sUp_dperror_y_back;
      QString m_sUp_dpdiv_x_back;
      QString m_sUp_dpdiv_y_back;
      QString m_sUp_filmDia_back;
      QString m_sUp_boardDia_back;
private:
    Ui::dlgBGParameter *ui;
    DlgInputkey *input;
    void InitUIStatus();
    void SetTextInputRange();
    void InitParameter();
    bool IsParaChanged();
    //save parameter
    void SavePara();
    /**曝光参数设置**/
    //上框
    QString m_sUpBG_timedelay;//0
    QString m_sUpBG_time;//1
    QString m_sUpBG_time2;//38
    QString m_sUpBG_energy;//2
    QString m_sUpDP_clearcnt;//3
    QString m_sUpDP_bgcnt;//4
    //下框
    QString m_sDownBG_timedelay;//5
    QString m_sDownBG_time;//6
    QString m_sDownBG_time2;//39
    QString m_sDownBG_energy;//7
    QString m_sDownDP_clearcnt;//8
    QString m_sDownDP_bgcnt;//9
    QString m_sBG_finishair;//10
    QString m_sBG_switchboardtime;//11
    /**上框影像检测装置**/
    QString m_sUp_checkdelay;//12
    QString m_sUp_maxjb;//13
    QString m_sUp_dwjingdu;//14
    QString m_sUp_dperror_x;//15
    QString m_sUp_dperror_y;//16
    QString m_sUp_dpdiv_x;//17
    QString m_sUp_dpdiv_y;//18
    QString m_sUp_filmDia;//19
    QString m_sUp_boardDia;//20

    QString m_sUp_checkdelay_2;//12
    QString m_sUp_maxjb_2;//13
    QString m_sUp_dwjingdu_2;//14
    QString m_sUp_dperror_x_2;//15
    QString m_sUp_dperror_y_2;//16
    QString m_sUp_dpdiv_x_2;//17
    QString m_sUp_dpdiv_y_2;//18
    QString m_sUp_filmDia_2;//19
    QString m_sUp_boardDia_2;//20
    /**下框影像检测装置**/
    QString m_sDown_checkdelay;//21
    QString m_sDown_maxjb;//22
    QString m_sDown_dwjingdu;//23
    QString m_sDown_dperror_x;//24
    QString m_sDown_dperror_y;//25
    QString m_sDown_dpdiv_x;//26
    QString m_sDown_dpdiv_y;//27
    QString m_sDown_filmDia;//28
    QString m_sDown_boardDia;//29

    QString m_sDown_checkdelay_2;//21
    QString m_sDown_maxjb_2;//22
    QString m_sDown_dwjingdu_2;//23
    QString m_sDown_dperror_x_2;//24
    QString m_sDown_dperror_y_2;//25
    QString m_sDown_dpdiv_x_2;//26
    QString m_sDown_dpdiv_y_2;//27
    QString m_sDown_filmDia_2;//28
    QString m_sDown_boardDia_2;//29
    /**平台设置**/
    QString m_sUp_boardHeightMil;
    QString m_sDown_boardHeightMil;
    QString m_sUp_boardHeight;//30
    QString m_sDown_boardHeight;//31
    QString m_sUp_dwDistance;//32
    QString m_sDown_dwDistance;//33
    QString m_sUp_xlength;//34
    QString m_sDown_xlength;//35
    QString m_sUp_ylength;//36
    QString m_sDown_ylength;//37
    //right btn status
    bool m_bfull;
    int m_bmannual;
    bool m_benergyortime;
    int  m_iFrameSelect;//
    int  m_iUpCCDSelect[2];//0:ccd2 1:ccd3 2:ccd4
    int  m_iDownCCDSelect[2];//0:ccd2 1:ccd3 2:ccd4
    //page btn
    int  m_btn_upccd[2];//0:CCD 1-2 1:CCD 3-4 2:CCD 1-4 3:CCD 2-3
    bool m_btn_upcheckmark[2];
    int  m_btn_downccd[2];//0:CCD 1-2 1:CCD 3-4 2:CCD 1-4 3:CCD 2-3
    bool m_btn_downcheckmark[2];
    //check box status
    bool m_bCheckbox_Upcycle;
    bool m_bCheckbox_Downcycle;
    bool m_bCheckbox_UpCheckDis;
    bool m_bCheckbox_DownCheckDis;
    bool m_bCheckbox_Binary;

    bool m_bCancelTow;
    bool m_bCancelThree;
    /****备份参数******/
    //上框
    QString m_sUpBG_timedelay_back;
    QString m_sUpBG_time_back;
    QString m_sUpBG_time2_back;
    QString m_sUpBG_energy_back;
    QString m_sUpDP_clearcnt_back;
    QString m_sUpDP_bgcnt_back;
    //下框
    QString m_sDownBG_timedelay_back;
    QString m_sDownBG_time_back;
    QString m_sDownBG_time2_back;
    QString m_sDownBG_energy_back;
    QString m_sDownDP_clearcnt_back;
    QString m_sDownDP_bgcnt_back;
    QString m_sBG_finishair_back;
    QString m_sBG_switchboardtime_back;


    QString m_sUp_checkdelay_back_2;
    QString m_sUp_maxjb_back_2;
    QString m_sUp_dwjingdu_back_2;
    QString m_sUp_dperror_x_back_2;
    QString m_sUp_dperror_y_back_2;
    QString m_sUp_dpdiv_x_back_2;
    QString m_sUp_dpdiv_y_back_2;
    QString m_sUp_filmDia_back_2;
    QString m_sUp_boardDia_back_2;
    /**下框影像检测装置**/
    QString m_sDown_checkdelay_back;
    QString m_sDown_maxjb_back;
    QString m_sDown_dwjingdu_back;
    QString m_sDown_dperror_x_back;
    QString m_sDown_dperror_y_back;
    QString m_sDown_dpdiv_x_back;
    QString m_sDown_dpdiv_y_back;
    QString m_sDown_filmDia_back;
    QString m_sDown_boardDia_back;

    QString m_sDown_checkdelay_back_2;
    QString m_sDown_maxjb_back_2;
    QString m_sDown_dwjingdu_back_2;
    QString m_sDown_dperror_x_back_2;
    QString m_sDown_dperror_y_back_2;
    QString m_sDown_dpdiv_x_back_2;
    QString m_sDown_dpdiv_y_back_2;
    QString m_sDown_filmDia_back_2;
    QString m_sDown_boardDia_back_2;
    /**平台设置**/
    QString m_sUp_boardHeightMil_back;
    QString m_sDown_boardHeightMil_back;
    QString m_sUp_boardHeight_back;
    QString m_sDown_boardHeight_back;
    QString m_sUp_dwDistance_back;
    QString m_sDown_dwDistance_back;
    QString m_sUp_xlength_back;
    QString m_sDown_xlength_back;
    QString m_sUp_ylength_back;
    QString m_sDown_ylength_back;
    //right btn status
    bool m_bfull_back;
    int m_bmannual_back;
    bool m_benergyortime_back;
    int  m_iFrameSelect_back;//
    int  m_iUpCCDSelect_back[2];//0:ccd2 1:ccd3 2:ccd4
    int  m_iDownCCDSelect_back[2];//0:ccd2 1:ccd3 2:ccd4
    //page btn
    int  m_btn_upccd_back[2];//0:CCD 1-2 1:CCD 3-4 2:CCD 1-4 3:CCD 2-3
    int  m_btn_upcheckmark_back[2];
    int  m_btn_downccd_back[2];//0:CCD 1-2 1:CCD 3-4 2:CCD 1-4 3:CCD 2-3
    int  m_btn_downcheckmark_back[2];
    //check box status
    bool m_bCheckbox_Upcycle_back;
    bool m_bCheckbox_Downcycle_back;
    bool m_bCheckbox_UpCheckDis_back;
    bool m_bCheckbox_DownCheckDis_back;
    bool m_bCheckbox_Binary_back;
};
#endif // DLGBGPARAMETER_H
