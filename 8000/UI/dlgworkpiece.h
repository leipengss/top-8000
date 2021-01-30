#ifndef DLGWORKPIECE_H
#define DLGWORKPIECE_H

#include <QDialog>

namespace Ui {
class DlgWorkpiece;
}

class DlgWorkpiece : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWorkpiece(bool isupframe,QWidget *parent = 0);
    ~DlgWorkpiece();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DlgWorkpiece *ui;
    bool m_bisUpframe;
};

#endif // DLGWORKPIECE_H
