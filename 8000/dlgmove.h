#ifndef DLGMOVE_H
#define DLGMOVE_H
#include "function/cbaoguangflow.h"
#include <QDialog>

namespace Ui {
class DlgMove;
}

class DlgMove : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgMove(QWidget *parent = 0);
    ~DlgMove();
    
private slots:
    void on_btn_skip_clicked();

    void on_btn_duiwei_clicked();

    void on_btn_BG_clicked();

    void on_btn_quit_clicked();

private:
    Ui::DlgMove *ui;

};

#endif // DLGMOVE_H
