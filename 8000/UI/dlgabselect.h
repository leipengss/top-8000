#ifndef DLGABSELECT_H
#define DLGABSELECT_H

#include <QDialog>

namespace Ui {
class DlgABSelect;
}

class DlgABSelect : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgABSelect(QWidget *parent = 0);
    ~DlgABSelect();
    bool m_absel;
private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::DlgABSelect *ui;
};

#endif // DLGABSELECT_H
