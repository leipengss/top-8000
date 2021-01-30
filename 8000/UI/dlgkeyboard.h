#ifndef DLGKEYBOARD_H
#define DLGKEYBOARD_H

#include <QWidget>

namespace Ui {
class DlgKeyboard;
}

class DlgKeyboard : public QWidget
{
    Q_OBJECT
    
public:
    explicit DlgKeyboard(bool isshowzero,QWidget*pWidget,QWidget *parent = 0);
    ~DlgKeyboard();
private:
    bool m_bcapslock;
private slots:
    void on_btn_num1_clicked();

    void on_btn_num2_clicked();

    void on_btn_num3_clicked();

    void on_btn_num4_clicked();

    void on_btn_num5_clicked();

    void on_btn_num6_clicked();

    void on_btn_num7_clicked();

    void on_btn_num8_clicked();

    void on_btn_num9_clicked();

    void on_btn_num0_clicked();

    void on_btn_q_clicked();

    void on_btn_w_clicked();

    void on_btn_e_clicked();

    void on_btn_r_clicked();

    void on_btn_t_clicked();

    void on_btn_y_clicked();

    void on_btn_u_clicked();

    void on_btn_i_clicked();

    void on_btn_o_clicked();

    void on_btn_p_clicked();

    void on_btn_caps_clicked();

    void on_btn_a_clicked();

    void on_btn_s_clicked();

    void on_btn_d_clicked();

    void on_btn_f_clicked();

    void on_btn_g_clicked();

    void on_btn_h_clicked();

    void on_btn_j_clicked();

    void on_btn_k_clicked();

    void on_btn_l_clicked();

    void on_btn_left_clicked();

    void on_btn_right_clicked();

    void on_btn_z_clicked();

    void on_btn_x_clicked();

    void on_btn_c_clicked();

    void on_btn_v_clicked();

    void on_btn_b_clicked();

    void on_btn_n_clicked();

    void on_btn_m_clicked();

    void on_btn_backspace_clicked();

    void on_btn_clear_clicked();

    void on_btn_ok_clicked();

    void on_btn_back_clicked();
signals:
    // 这个信号是发给父窗口的。
    void okEvent();
    void cancelEvent();
private:
    Ui::DlgKeyboard *ui;
    QWidget* pWidgetObj;
};

#endif // DLGKEYBOARD_H
