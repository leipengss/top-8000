#ifndef DLGCNCPARMETER_H
#define DLGCNCPARMETER_H

#include <QDialog>
#include<QVector>
#include"cxmlparmetershow.h"
namespace Ui {
class DlgCNCParmeter;
}

class DlgCNCParmeter : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCNCParmeter(QWidget *parent = 0);
    ~DlgCNCParmeter();

private slots:
    void on_btn_cncpar_exit_clicked();

    void on_btn_cncpar_backup_clicked();

    void on_btn_cncpar_rec_clicked();

    void on_btn_cncpar_save_clicked();

private:
    Ui::DlgCNCParmeter *ui;
    QVector<QWidget*> tablecell_list;
    QVector<Cxmlparmetershow*> uixmlshow_list;
    int tabindex;
    void SysRestart();
};

#endif // DLGCNCPARMETER_H
