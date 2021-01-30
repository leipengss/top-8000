#ifndef DLGIOPARAMETER_H
#define DLGIOPARAMETER_H

#include <QDialog>
#include"cinifileshow.h"
#include"cinioutfile.h"

namespace Ui {
class DlgIOParameter;
}

class DlgIOParameter : public QDialog
{
    Q_OBJECT

public:
    explicit DlgIOParameter(QWidget *parent = 0);
    ~DlgIOParameter();

private slots:
    void on_btn_iobackup_clicked();

    void on_btn_iorecover_clicked();

    void on_btn_iosave_clicked();

    void on_btn_ioleave_clicked();

    void on_tabWidget_iopara_currentChanged(int index);

private:
    Ui::DlgIOParameter *ui;
    CIniFileshow *inifileshowInput;
    cinioutfile *inifileshowOutput;
    void SysRestart();
};

#endif // DLGIOPARAMETER_H
