#ifndef DLGMATERIALMANAGE_H
#define DLGMATERIALMANAGE_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include "driver/global.h"
#include"dlgkeyboard.h"

struct MaterialInfo{
    QString name;
    QString date;
    int frameid;
    bool ab;
};

namespace Ui {
class DlgMaterialManage;
}

class DlgMaterialManage : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgMaterialManage(QWidget *parent = 0);
    ~DlgMaterialManage();
private:
    bool MaterialWrite(MaterialInfo data);
    bool MaterialRead(MaterialInfo data,bool isload);
    void update_view();
private slots:
    void on_btn_search_clicked();

    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_btn_fetch_clicked();

    void on_btn_loadmark_clicked();

    void on_btn_exit_clicked();

    void on_btn_liaohaoclear_clicked();

private:
    Ui::DlgMaterialManage *ui;
    QSqlQueryModel *sqlmodel;
    QSqlDatabase db;
    MaterialInfo newtable;
    DlgKeyboard *input;
    bool isAB;
};

#endif // DLGMATERIALMANAGE_H
