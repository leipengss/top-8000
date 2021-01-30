#ifndef DLGLANGUAGE_H
#define DLGLANGUAGE_H

#include <QDialog>

namespace Ui {
class DlgLanguage;
}

class DlgLanguage : public QDialog
{
    Q_OBJECT

public:
    explicit DlgLanguage(QWidget *parent = 0);
    ~DlgLanguage();

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

   // void on_pushButton_2_clicked();

private:
    Ui::DlgLanguage *ui;
};

#endif // DLGLANGUAGE_H
