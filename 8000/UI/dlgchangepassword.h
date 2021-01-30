#ifndef DLGCHANGEPASSWORD_H
#define DLGCHANGEPASSWORD_H

#include <QDialog>

namespace Ui {
class DlgChangePassword;
}

class DlgChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit DlgChangePassword(int type,QWidget *parent = 0);//type 0 change setting code;type 1 change setting hi
    ~DlgChangePassword();
    QString old_setting_code;
    int Type;
private slots:
      void on_radioButton_lowkey_clicked();

      void on_radioButton_midkey_clicked();

      void on_radioButton_upkey_clicked();

      void on_radioButton_debugkey_clicked();

      void on_radioButton_developkey_clicked();

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

private:
    Ui::DlgChangePassword *ui;
     void GetPassword();
     void SavePassword();
     int passwordtype;//0:chuji  1:zhongji 2:gaoji 3:debug 4:operator
};

#endif // DLGCHANGEPASSWORD_H
