#ifndef LIGHT_H
#define LIGHT_H

#include <QDialog>
#include <QString>
#include <QList>


namespace Ui {
class light;
}

class light : public QDialog
{
    Q_OBJECT

public:
    explicit light(QWidget *parent = 0);
    ~light();

private:
    Ui::light *ui;
    void readink(QString ink);
    int ee;
private slots:
   void on_login_Btn_clicked();
   void on_lightBtn_clicked();
   void on_light_saveBtn_clicked();
   void on_communicationBtn_clicked();

   void on_comboBox_up_currentIndexChanged(int index);
   void on_pushButton_clicked();
   void on_showbtn_clicked();
public:
   bool returnlightTCP(int i);//发送 0/1
   QString returnlightLOWER();
   QString returnlightUP();

};

#endif // LIGHT_H
