#ifndef DLGLIGHTZERO_H
#define DLGLIGHTZERO_H

#include <QDialog>

namespace Ui {
class dlgLightZero;
}

class dlgLightZero : public QDialog
{
    Q_OBJECT
    
public:
    explicit dlgLightZero(bool isdown,QWidget *parent = 0);
    ~dlgLightZero();
    
private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::dlgLightZero *ui;
    bool isdownlight;
};

#endif // DLGLIGHTZERO_H
