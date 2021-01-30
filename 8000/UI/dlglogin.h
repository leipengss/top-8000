#ifndef DLGLOGIN_H
#define DLGLOGIN_H

#include <QDialog>
#include"dlginputkey.h"
#include<QString>

namespace Ui {
class DlgLogin;
}

class DlgLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLogin(int keylevel,QString tile,QWidget *parent = 0);
    ~DlgLogin();

    bool iscorrect ;
private slots:
    void on_btn_ok_clicked();
    void on_btn_cancel_clicked();

//    void on_btn_softkeyboard_clicked();

//    void on_btn_softkeyboard_close_clicked();

    void on_btn_clear_clicked();    
private:
    Ui::DlgLogin *ui;
    DlgInputkey *inputkeyboard;
    QString key[5];
    QString clr_passwd;
    void GetPassword();
    int passlevel;
};

#endif // DLGLOGIN_H
