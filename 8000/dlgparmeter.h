#ifndef DLGPARMETER_H
#define DLGPARMETER_H

#include <QDialog>
#include "UI/dlginputkey.h"
#include <QMap>
namespace Ui {
class dlgparmeter;
}

class dlgparmeter : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlgparmeter(QWidget *parent = 0);
    ~dlgparmeter();
    
private slots:
    void on_btn_next_clicked();

    void on_btn_previous_clicked();

    void on_btn_save_clicked();

    void on_btn_leave_clicked();
    void on_bginput_okEvent();
    void on_bginput_cancelEvent();

    void on_btn_bigbm_toggled(bool checked);


private:
    Ui::dlgparmeter *ui;
     QVector<QString> qlist;
     QString list;
     QMap<QString,QString> qmap;
      DlgInputkey *input;
};

#endif // DLGPARMETER_H
