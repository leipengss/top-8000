#include "caxispositionshow.h"
#include "ui_caxispositionshow.h"
#include"driver/cupdatedatathread.h"

CAxisPositionShow::CAxisPositionShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CAxisPositionShow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::WindowModal);
    updatetimer=new QTimer(this);
    updatetimer->start(50);//坐标刷新周期50ms
    connect(updatetimer,SIGNAL(timeout()),this,SLOT(slot_positionupdate()));
}

CAxisPositionShow::~CAxisPositionShow()
{
    delete updatetimer;
    delete ui;
}
void CAxisPositionShow::ShowCurPos()
{
    QString str;
    str.sprintf("%.3f",update_thread->m_axispos[AXIS_Z]);
    ui->value_ptz_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_M]);
    ui->value_ptm_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_L]);
    ui->value_ptl_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_R]);
    ui->value_ptr_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_UPSCAN]);
    ui->value_upscan_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD1X]);
    ui->value_ccd1x_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD1Y]);
    ui->value_ccd1y_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD2X]);
    ui->value_ccd2x_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD2Y]);
    ui->value_ccd2y_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD3X]);
    ui->value_ccd3x_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD3Y]);
    ui->value_ccd3y_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD4X]);
    ui->value_ccd4x_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_CCD4Y]);
    ui->value_ccd4y_2->setText(str);

    str.sprintf("%.3f",update_thread->m_axispos[AXIS_W]);
    ui->value_w_2->setText(str);
}

void CAxisPositionShow::ShowCmdPos()
{
    QString str;

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_Z]);
    ui->value_ptz_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_M]);
    ui->value_ptm_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_L]);
    ui->value_ptl_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_R]);
    ui->value_ptr_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_UPSCAN]);
    ui->value_upscan_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD1X]);
    ui->value_ccd1x_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD1Y]);
    ui->value_ccd1y_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD2X]);
    ui->value_ccd2x_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD2Y]);
    ui->value_ccd2y_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD3X]);
    ui->value_ccd3x_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD3Y]);
    ui->value_ccd3y_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD4X]);
    ui->value_ccd4x_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_CCD4Y]);
    ui->value_ccd4y_3->setText(str);

    str.sprintf("%.3f",update_thread->m_axiscmdpos[AXIS_W]);
    ui->value_w_3->setText(str);
}

void CAxisPositionShow::ShowFeedbackPos()
{
    QString str;

    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_Z]);
    ui->value_ptz_4->setText(str);

    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_M]);
    ui->value_ptm_4->setText(str);

    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_L]);
    ui->value_ptl_4->setText(str);

    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_R]);
    ui->value_ptr_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_UPSCAN]);
//    ui->value_upscan_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD1X]);
//    ui->value_ccd1x_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD1Y]);
//    ui->value_ccd1y_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD2X]);
//    ui->value_ccd2x_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD2Y]);
//    ui->value_ccd2y_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD3X]);
//    ui->value_ccd3x_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD3Y]);
//    ui->value_ccd3y_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD4X]);
//    ui->value_ccd4x_4->setText(str);

//    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_CCD4Y]);
//    ui->value_ccd4y_4->setText(str);

    str.sprintf("%.3f",update_thread->m_axisfeedbackpos[AXIS_W]);
    ui->value_w_4->setText(str);
}

void CAxisPositionShow::slot_positionupdate()
{
    ShowCurPos();
    ShowCmdPos();
    ShowFeedbackPos();
}

void CAxisPositionShow::on_btn_showpos_exit_clicked()
{
    updatetimer->stop();
    delete updatetimer;
    this->close();
}
