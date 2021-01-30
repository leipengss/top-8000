#include "dlgkeyboard.h"
#include "ui_dlgkeyboard.h"
#include<QLineEdit>

DlgKeyboard::DlgKeyboard(bool isshowzero,QWidget*pWidget,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DlgKeyboard)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    m_bcapslock = false;
    pWidgetObj = pWidget;
    ui->btn_num0->setVisible(isshowzero);
}

DlgKeyboard::~DlgKeyboard()
{
    delete ui;
}

void DlgKeyboard::on_btn_num0_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("0");
        }
    }
}

void DlgKeyboard::on_btn_num1_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("1");
        }
    }
}

void DlgKeyboard::on_btn_num2_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("2");
        }
    }
}

void DlgKeyboard::on_btn_num3_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("3");
        }
    }
}

void DlgKeyboard::on_btn_num4_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("4");
        }
    }
}

void DlgKeyboard::on_btn_num5_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("5");
        }
    }
}

void DlgKeyboard::on_btn_num6_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("6");
        }
    }
}

void DlgKeyboard::on_btn_num7_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("7");
        }
    }
}

void DlgKeyboard::on_btn_num8_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("8");
        }
    }
}

void DlgKeyboard::on_btn_num9_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->insert("9");
        }
    }
}

void DlgKeyboard::on_btn_left_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->cursorBackward(false);
        }
    }
}

void DlgKeyboard::on_btn_right_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->cursorForward(false);
        }
    }
}

void DlgKeyboard::on_btn_clear_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->clear();
        }
    }
}

void DlgKeyboard::on_btn_backspace_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            w->backspace();
        }
    }
}

void DlgKeyboard::on_btn_ok_clicked()
{
    // 触发子窗口的信号给父窗口接收。
    emit okEvent();
}

void DlgKeyboard::on_btn_back_clicked()
{
    m_bcapslock = false;
    emit cancelEvent();
}

void DlgKeyboard::on_btn_q_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("Q");
            }
            else
            {
                w->insert("q");
            }
        }
    }
}

void DlgKeyboard::on_btn_w_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("W");
            }
            else
            {
                w->insert("w");
            }
        }
    }
}

void DlgKeyboard::on_btn_e_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("E");
            }
            else
            {
                w->insert("e");
            }
        }
    }
}

void DlgKeyboard::on_btn_r_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("R");
            }
            else
            {
                w->insert("r");
            }
        }
    }
}

void DlgKeyboard::on_btn_t_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("T");
            }
            else
            {
                w->insert("r");
            }
        }
    }
}

void DlgKeyboard::on_btn_y_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("Y");
            }
            else
            {
                w->insert("y");
            }
        }
    }
}

void DlgKeyboard::on_btn_u_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("U");
            }
            else
            {
                w->insert("u");
            }
        }
    }
}

void DlgKeyboard::on_btn_i_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("I");
            }
            else
            {
                w->insert("i");
            }
        }
    }
}

void DlgKeyboard::on_btn_o_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("O");
            }
            else
            {
                w->insert("o");
            }
        }
    }
}

void DlgKeyboard::on_btn_p_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("P");
            }
            else
            {
                w->insert("p");
            }
        }
    }
}

void DlgKeyboard::on_btn_caps_clicked()
{
    if(m_bcapslock)
    {
        m_bcapslock = false;
        ui->btn_caps->setStyleSheet("background:darkgray;");
    }
    else
    {
        m_bcapslock = true;
        ui->btn_caps->setStyleSheet("background:green;");
    }
}

void DlgKeyboard::on_btn_a_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("A");
            }
            else
            {
                w->insert("a");
            }
        }
    }
}

void DlgKeyboard::on_btn_s_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("S");
            }
            else
            {
                w->insert("s");
            }
        }
    }
}

void DlgKeyboard::on_btn_d_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("D");
            }
            else
            {
                w->insert("d");
            }
        }
    }
}

void DlgKeyboard::on_btn_f_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("F");
            }
            else
            {
                w->insert("f");
            }
        }
    }
}

void DlgKeyboard::on_btn_g_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("G");
            }
            else
            {
                w->insert("g");
            }
        }
    }
}

void DlgKeyboard::on_btn_h_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("H");
            }
            else
            {
                w->insert("h");
            }
        }
    }
}

void DlgKeyboard::on_btn_j_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("J");
            }
            else
            {
                w->insert("j");
            }
        }
    }
}

void DlgKeyboard::on_btn_k_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("K");
            }
            else
            {
                w->insert("k");
            }
        }
    }
}

void DlgKeyboard::on_btn_l_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("L");
            }
            else
            {
                w->insert("l");
            }
        }
    }
}

void DlgKeyboard::on_btn_z_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("Z");
            }
            else
            {
                w->insert("z");
            }
        }
    }
}

void DlgKeyboard::on_btn_x_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("X");
            }
            else
            {
                w->insert("x");
            }
        }
    }
}

void DlgKeyboard::on_btn_c_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("C");
            }
            else
            {
                w->insert("c");
            }
        }
    }
}

void DlgKeyboard::on_btn_v_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("V");
            }
            else
            {
                w->insert("v");
            }
        }
    }
}

void DlgKeyboard::on_btn_b_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("B");
            }
            else
            {
                w->insert("b");
            }
        }
    }
}

void DlgKeyboard::on_btn_n_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("N");
            }
            else
            {
                w->insert("n");
            }
        }
    }
}

void DlgKeyboard::on_btn_m_clicked()
{
    if(pWidgetObj->focusWidget()!=NULL)
    {
        if(pWidgetObj->focusWidget()->inherits("QLineEdit"))
        {
            QLineEdit *w = (QLineEdit *)(pWidgetObj->focusWidget());
            if(m_bcapslock)
            {
                w->insert("M");
            }
            else
            {
                w->insert("m");
            }
        }
    }
}

