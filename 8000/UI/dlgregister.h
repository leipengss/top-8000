#ifndef DLGREGISTER_H
#define DLGREGISTER_H

#include <QDialog>
#include"driver/cupdatedatathread.h"
#include"UI/dlgkeyboard.h"

namespace Ui {
class DlgRegister;
}

class DlgRegister : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgRegister(QWidget *parent = 0);
    ~DlgRegister();
    
private slots:
    void on_btn_ok_clicked();

    void on_btn_reset_clicked();

    void on_btn_exit_clicked();

    void on_lineEdit_syscode_textChanged(const QString &arg1);

    void on_lineEdit_regcode_textChanged(const QString &arg1);

    void on_lineEdit_regcode_2_textChanged(const QString &arg1);

    void on_lineEdit_regcode_3_textChanged(const QString &arg1);

    void on_lineEdit_regcode_4_textChanged(const QString &arg1);

private:
    Ui::DlgRegister *ui;
    QString m_checksum;
    QString m_syscode;
    QString m_regcode[4];
    //
    int ByteArrayToInt(QByteArray bytes);
    QByteArray IntToByteArray(int number);
    void Init_Reg();
    void rewrite_sysnum();
    //
    DlgKeyboard *m_kb;
};

#endif // DLGREGISTER_H
