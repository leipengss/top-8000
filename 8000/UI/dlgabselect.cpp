#include "dlgabselect.h"
#include "ui_dlgabselect.h"

DlgABSelect::DlgABSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgABSelect)
{
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    m_absel = true;
    ui->checkBox_a->setChecked(m_absel);
}

DlgABSelect::~DlgABSelect()
{
    delete ui;
}

void DlgABSelect::on_btn_ok_clicked()
{
    if(ui->checkBox_a->isChecked())
    {
        m_absel = true;
    }
    if(ui->checkBox_b->isChecked())
    {
        m_absel = false;
    }
    this->accept();
}

void DlgABSelect::on_btn_cancel_clicked()
{
    this->reject();
}
