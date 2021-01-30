#ifndef CDLGMESSAGE_H
#define CDLGMESSAGE_H

#include <QDialog>

namespace Ui {
class CDlgMessage;
}

class CDlgMessage : public QDialog
{
    Q_OBJECT
    
public:
    explicit CDlgMessage(QWidget *parent = 0);
    ~CDlgMessage();
    void MessageClose();
    void MessageShow(bool isnormal,QString code);
private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::CDlgMessage *ui;
};

#endif // CDLGMESSAGE_H
