#include "dlgmove.h"
#include "ui_dlgmove.h"
#include "function/cbaoguangflow.h"
extern     CBaoguangFlow *bgflow;
DlgMove::DlgMove(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMove)
{
    ui->setupUi(this);

}

DlgMove::~DlgMove()
{
    delete ui;
}
//ch: 0 跳过 1重复对位  2 退出

void DlgMove::on_btn_skip_clicked()
{
   bgflow->run_move(0);
  // bgflow-> run();
   // reject();
}

void DlgMove::on_btn_duiwei_clicked()
{
    bgflow->run_move(1);
    //reject();

}

void DlgMove::on_btn_BG_clicked()
{
    this->accept();
    //reject();
}

void DlgMove::on_btn_quit_clicked()
{
    bgflow->run_move(2);
   // reject();

}
