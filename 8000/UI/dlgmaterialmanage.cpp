#include "dlgmaterialmanage.h"
#include "ui_dlgmaterialmanage.h"
#include<QDebug>
#include<QSqlError>
#include<QMessageBox>
#include<QDateTime>
#include"filemanage/cinirw.h"
#include"UI/dlgabselect.h"

QString tablename = "Material";
QString create_sql = "create table Material (code text primary key,date text,frameid int,type text,abmian text)";
//QString select_max_sql = "select max(num) from Material";
QString insert_sql = "insert into Material (code,date,frameid,type,abmian) values (?, ?, ?, ?, ?)";
//QString update_sql = "update Material set num = :num where id = :id";
QString select_sql = "select * from Material where code = ?";
QString select_col_sql = "select code from Material";
QString delete_sql = "delete from Material where code = ?";
QString clear_sql = "delete from Material";
QString create_table ="create table parameter (code text primary key,"
                       "filmdia text,pcbdia text,bgtime text,banheight text,"
                       "dwdistance text,ccd_swtich text,ccd234 text,"
                       "dpwuchaX text,dpwuchaY text,dpjunfenX text,dpjunfenY text,"
                       "mainmark1x text,mainmark1y text,mainmark2x text,mainmark2y text,"
                       "mainmark3x text,mainmark3y text,mainmark4x text,mainmark4y text,"
                       "othermark1x text,othermark1y text,othermark2x text,othermark2y text,"
                       "othermark3x text,othermark3y text,othermark4x text,othermark4y text)";
QString insert_table = "insert into parameter values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
QString select_table = "select * from parameter where code = ?";
QString select_col_table = "select code from parameter";
QString delete_table = "delete from parameter where code = ?";

DlgMaterialManage::DlgMaterialManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMaterialManage)
{
    //屏蔽最大化
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);setFixedSize(this->width(), this->height());
    //加载小键盘
    input = new DlgKeyboard(true,this);
    input->setParent(ui->frame_keyboard);
    connect(input, SIGNAL(cancelEvent()),this, SLOT(on_btn_exit_clicked()));
    //
    ui->tableView_database->setColumnWidth(0,25);//列宽
    ui->tableView_database->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_database->setSelectionMode(QAbstractItemView::SingleSelection);
    sqlmodel = new QSqlQueryModel;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName(MATERIALPATH);
    if(!db.open())
    {
        QMessageBox::information(this,"TOP",QObject::tr("料号文件不存在！"));
    }
    else
    {
        QSqlQuery query(db);
        query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(tablename));
        if(query.next())
        {
            if(query.value(0).toInt()==0)
            {
               qDebug()<<"create materialinfo table!";
               query.prepare(create_sql);
               query.exec();
               qDebug()<<"create parameter table!";
               query.prepare(create_table);
               query.exec();
            }
            update_view();
        }
        else
        {
            QMessageBox::information(this,"TOP",QObject::tr("料号文件已损坏！"));
        }
    }
    //默认选择A面
    isAB = true;
}
void DlgMaterialManage::update_view()
{
    //update
    sqlmodel->setQuery("select * from Material");
//    sqlmodel->setHeaderData(0,Qt::Horizontal,QObject::tr("序号"));
    sqlmodel->setHeaderData(0,Qt::Horizontal,QObject::tr("料号编码"));
    sqlmodel->setHeaderData(1,Qt::Horizontal,QObject::tr("创建时间"));
    sqlmodel->setHeaderData(2,Qt::Horizontal,QObject::tr("框架ID"));
    sqlmodel->setHeaderData(3,Qt::Horizontal,QObject::tr("种类"));
    sqlmodel->setHeaderData(4,Qt::Horizontal,QObject::tr("板面"));
    ui->tableView_database->setModel(sqlmodel);
    ui->tableView_database->show();
}

bool DlgMaterialManage::MaterialWrite(MaterialInfo data)
{
    int filmdia,pcbdia,dpbgtime,banheight,dwdistance;
    int ccd_swtich,ccd234;
    int dpwuchaX,dpwuchaY,dpjunfenX,dpjunfenY;
    Mark_pos ccd_mainmark,ccd_othermark;
    if(data.frameid==0)
    {
       if(data.ab)
       {
           filmdia = (int)g_par.film_up_dia;
           pcbdia = (int)g_par.board_up_dia;
           dpbgtime = (int)g_par.bg_up_time;
           banheight = (int)g_par.up_bm_height;
           dwdistance = (int)g_par.up_dw_height;
           ccd_swtich = g_par.up_ccd_select;
           ccd234 = g_par.ccd_234[0];
           dpwuchaX = (int)g_par.DP_up_error_x;
           dpwuchaY = (int)g_par.DP_up_error_y;
           dpjunfenX = (int)g_par.DP_up_junfen_x;
           dpjunfenY = (int)g_par.DP_up_junfen_y;
           ccd_mainmark = g_par.main_markpos[0];
           ccd_othermark = g_par.other_markpos[0];
       }
       else
       {
           filmdia = (int)g_par.Downfilm_up_dia;
           pcbdia = (int)g_par.Downboard_up_dia;
           dpbgtime = (int)g_par.bg_up_time2;
           banheight = (int)g_par.up_bm_height;
           dwdistance = (int)g_par.up_dw_height;
           ccd_swtich = g_par.up_downccd_select;
           ccd234 = g_par.downccd_234[0];
           dpwuchaX = (int)g_par.DownDP_up_error_x;
           dpwuchaY = (int)g_par.DownDP_up_error_y;
           dpjunfenX = (int)g_par.DownDP_up_junfen_x;
           dpjunfenY = (int)g_par.DownDP_up_junfen_y;
           ccd_mainmark = g_par.main_downmarkpos[0];
           ccd_othermark = g_par.other_downmarkpos[0];
       }
    }
    else if(data.frameid==1)
    {
        if(data.ab)
        {
            filmdia = (int)g_par.film_down_dia;
            pcbdia = (int)g_par.board_down_dia;
            dpbgtime = (int)g_par.bg_down_time;
            banheight = (int)g_par.down_bm_height;
            dwdistance = (int)g_par.down_dw_height;
            ccd_swtich = g_par.down_ccd_select;
            ccd234 = g_par.ccd_234[1];
            dpwuchaX = (int)g_par.DP_down_error_x;
            dpwuchaY = (int)g_par.DP_down_error_y;
            dpjunfenX = (int)g_par.DP_down_junfen_x;
            dpjunfenY = (int)g_par.DP_down_junfen_y;
            ccd_mainmark = g_par.main_markpos[1];
            ccd_othermark = g_par.other_markpos[1];
        }
        else
        {
            filmdia = (int)g_par.Downfilm_down_dia;
            pcbdia = (int)g_par.Downboard_down_dia;
            dpbgtime = (int)g_par.bg_down_time2;
            banheight = (int)g_par.down_bm_height;
            dwdistance = (int)g_par.down_dw_height;
            ccd_swtich = g_par.down_downccd_select;
            ccd234 = g_par.downccd_234[1];
            dpwuchaX = (int)g_par.DownDP_down_error_x;
            dpwuchaY = (int)g_par.DownDP_down_error_y;
            dpjunfenX = (int)g_par.DownDP_down_junfen_x;
            dpjunfenY = (int)g_par.DownDP_down_junfen_y;
            ccd_mainmark = g_par.main_downmarkpos[1];
            ccd_othermark = g_par.other_downmarkpos[1];
        }
    }
    QString value;
    QSqlQuery wquery(db);
    wquery.prepare(insert_table);
    wquery.addBindValue(data.name);
    wquery.addBindValue(value.sprintf("%d",filmdia));
    wquery.addBindValue(value.sprintf("%d",pcbdia));
    wquery.addBindValue(value.sprintf("%d",dpbgtime));
    wquery.addBindValue(value.sprintf("%d",banheight));
    wquery.addBindValue(value.sprintf("%d",dwdistance));
    wquery.addBindValue(value.sprintf("%d",ccd_swtich));
    wquery.addBindValue(value.sprintf("%d",ccd234));
    wquery.addBindValue(value.sprintf("%d",dpwuchaX));
    wquery.addBindValue(value.sprintf("%d",dpwuchaY));
    wquery.addBindValue(value.sprintf("%d",dpjunfenX));
    wquery.addBindValue(value.sprintf("%d",dpjunfenY));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.x1));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.y1));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.x2));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.y2));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.x3));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.y3));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.x4));
    wquery.addBindValue(value.sprintf("%.3f",ccd_mainmark.y4));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.x1));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.y1));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.x2));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.y2));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.x3));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.y3));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.x4));
    wquery.addBindValue(value.sprintf("%.3f",ccd_othermark.y4));
    if(!wquery.exec())
    {
        qDebug()<<wquery.lastError();
        qDebug()<<"write db:error";
        return false;
    }
    qDebug()<<"write db:ok";
    return true;
}

bool DlgMaterialManage::MaterialRead(MaterialInfo data,bool isload)
{
    QSqlQuery rquery(db);
    rquery.prepare(select_table);
    rquery.addBindValue(data.name);
    rquery.exec();
    if((true==rquery.next())&&(data.name==rquery.value(0).toString()))
    {
        CIniRW material(TOPPARAMETERPATH);
        if(data.frameid==0)
        {
            if(data.ab)
            {
                g_par.film_up_dia = rquery.value(1).toFloat();
                g_par.board_up_dia = rquery.value(2).toFloat();
                g_par.bg_up_time = rquery.value(3).toInt();
                g_par.up_bm_height = rquery.value(4).toFloat();
                g_par.up_dw_height = rquery.value(5).toFloat();
                g_par.up_ccd_select = rquery.value(6).toInt();
                g_par.ccd_234[0] = rquery.value(7).toInt();
                g_par.DP_up_error_x = rquery.value(8).toFloat();
                g_par.DP_up_error_y = rquery.value(9).toFloat();
                g_par.DP_up_junfen_x = rquery.value(10).toFloat();
                g_par.DP_up_junfen_y = rquery.value(11).toFloat();
                g_par.main_markpos[0].x1 = rquery.value(12).toFloat();
                g_par.main_markpos[0].y1 = rquery.value(13).toFloat();
                g_par.main_markpos[0].x2 = rquery.value(14).toFloat();
                g_par.main_markpos[0].y2 = rquery.value(15).toFloat();
                g_par.main_markpos[0].x3 = rquery.value(16).toFloat();
                g_par.main_markpos[0].y3 = rquery.value(17).toFloat();
                g_par.main_markpos[0].x4 = rquery.value(18).toFloat();
                g_par.main_markpos[0].y4 = rquery.value(19).toFloat();
                g_par.other_markpos[0].x1 = rquery.value(20).toFloat();
                g_par.other_markpos[0].y1 = rquery.value(21).toFloat();
                g_par.other_markpos[0].x2 = rquery.value(22).toFloat();
                g_par.other_markpos[0].y2 = rquery.value(23).toFloat();
                g_par.other_markpos[0].x3 = rquery.value(24).toFloat();
                g_par.other_markpos[0].y3 = rquery.value(25).toFloat();
                g_par.other_markpos[0].x4 = rquery.value(26).toFloat();
                g_par.other_markpos[0].y4 = rquery.value(27).toFloat();
                if(isload)
                {
                    material.WriteIni("TRAJ","UP_FILM_D",rquery.value(1).toString());
                    material.WriteIni("TRAJ","BOARD_D",rquery.value(2).toString());
                    material.WriteIni("TRAJ","SHANG_DI_PIAN",rquery.value(3).toString());
                    material.WriteIni("TRAJ","PRODUCT_THICKNESS_UM",rquery.value(4).toString());
                    material.WriteIni("TRAJ","DUIWEI_DISTANCE",rquery.value(5).toString());
                    material.WriteIni("TRAJ","CCD_SWITCH",rquery.value(6).toString());
                    material.WriteIni("TRAJ","CCD_234",rquery.value(7).toString());
                    material.WriteIni("TRAJ","DIPIAN_WUCHA_X",rquery.value(8).toString());
                    material.WriteIni("TRAJ","DIPIAN_WUCHA_Y",rquery.value(9).toString());
                    material.WriteIni("TRAJ","JUNFEN_WUCHA_X",rquery.value(10).toString());
                    material.WriteIni("TRAJ","JUNFEN_WUCHA_Y",rquery.value(11).toString());
                }
            }
            else
            {
                g_par.Downfilm_up_dia = rquery.value(1).toFloat();
                g_par.Downboard_up_dia = rquery.value(2).toFloat();
                g_par.bg_up_time2 = rquery.value(3).toInt();
                g_par.up_bm_height = rquery.value(4).toFloat();
                g_par.up_dw_height = rquery.value(5).toFloat();
                g_par.up_downccd_select = rquery.value(6).toInt();
                g_par.downccd_234[0] = rquery.value(7).toInt();
                g_par.DownDP_up_error_x = rquery.value(8).toFloat();
                g_par.DownDP_up_error_y = rquery.value(9).toFloat();
                g_par.DownDP_up_junfen_x = rquery.value(10).toFloat();
                g_par.DownDP_up_junfen_y = rquery.value(11).toFloat();
                g_par.main_downmarkpos[0].x1 = rquery.value(12).toFloat();
                g_par.main_downmarkpos[0].y1 = rquery.value(13).toFloat();
                g_par.main_downmarkpos[0].x2 = rquery.value(14).toFloat();
                g_par.main_downmarkpos[0].y2 = rquery.value(15).toFloat();
                g_par.main_downmarkpos[0].x3 = rquery.value(16).toFloat();
                g_par.main_downmarkpos[0].y3 = rquery.value(17).toFloat();
                g_par.main_downmarkpos[0].x4 = rquery.value(18).toFloat();
                g_par.main_downmarkpos[0].y4 = rquery.value(19).toFloat();
                g_par.other_downmarkpos[0].x1 = rquery.value(20).toFloat();
                g_par.other_downmarkpos[0].y1 = rquery.value(21).toFloat();
                g_par.other_downmarkpos[0].x2 = rquery.value(22).toFloat();
                g_par.other_downmarkpos[0].y2 = rquery.value(23).toFloat();
                g_par.other_downmarkpos[0].x3 = rquery.value(24).toFloat();
                g_par.other_downmarkpos[0].y3 = rquery.value(25).toFloat();
                g_par.other_downmarkpos[0].x4 = rquery.value(26).toFloat();
                g_par.other_downmarkpos[0].y4 = rquery.value(27).toFloat();
                if(isload)
                {
                    material.WriteIni("TRAJ","DOWNUP_FILM_D",rquery.value(1).toString());
                    material.WriteIni("TRAJ","DOWNBOARD_D",rquery.value(2).toString());
                    material.WriteIni("TRAJ","SHANG_DI_PIAN2",rquery.value(3).toString());
                    material.WriteIni("TRAJ","PRODUCT_THICKNESS_UM",rquery.value(4).toString());
                    material.WriteIni("TRAJ","DUIWEI_DISTANCE",rquery.value(5).toString());
                    material.WriteIni("TRAJ","DOWNCCD_SWITCH",rquery.value(6).toString());
                    material.WriteIni("TRAJ","DOWNCCD_234",rquery.value(7).toString());
                    material.WriteIni("TRAJ","DOWNDIPIAN_WUCHA_X",rquery.value(8).toString());
                    material.WriteIni("TRAJ","DOWNDIPIAN_WUCHA_Y",rquery.value(9).toString());
                    material.WriteIni("TRAJ","DOWNJUNFEN_WUCHA_X",rquery.value(10).toString());
                    material.WriteIni("TRAJ","DOWNJUNFEN_WUCHA_Y",rquery.value(11).toString());
                }
            }

        }
        else if(data.frameid==1)
        {
            if(data.ab)
            {
                g_par.film_down_dia = rquery.value(1).toFloat();
                g_par.board_down_dia = rquery.value(2).toFloat();
                g_par.bg_down_time = rquery.value(3).toInt();
                g_par.down_bm_height = rquery.value(4).toFloat();
                g_par.down_dw_height = rquery.value(5).toFloat();
                g_par.down_ccd_select = rquery.value(6).toInt();
                g_par.ccd_234[1] = rquery.value(7).toInt();
                g_par.DP_down_error_x = rquery.value(8).toFloat();
                g_par.DP_down_error_y = rquery.value(9).toFloat();
                g_par.DP_down_junfen_x = rquery.value(10).toFloat();
                g_par.DP_down_junfen_y = rquery.value(11).toFloat();
                g_par.main_markpos[1].x1 = rquery.value(12).toFloat();
                g_par.main_markpos[1].y1 = rquery.value(13).toFloat();
                g_par.main_markpos[1].x2 = rquery.value(14).toFloat();
                g_par.main_markpos[1].y2 = rquery.value(15).toFloat();
                g_par.main_markpos[1].x3 = rquery.value(16).toFloat();
                g_par.main_markpos[1].y3 = rquery.value(17).toFloat();
                g_par.main_markpos[1].x4 = rquery.value(18).toFloat();
                g_par.main_markpos[1].y4 = rquery.value(19).toFloat();
                g_par.other_markpos[1].x1 = rquery.value(20).toFloat();
                g_par.other_markpos[1].y1 = rquery.value(21).toFloat();
                g_par.other_markpos[1].x2 = rquery.value(22).toFloat();
                g_par.other_markpos[1].y2 = rquery.value(23).toFloat();
                g_par.other_markpos[1].x3 = rquery.value(24).toFloat();
                g_par.other_markpos[1].y3 = rquery.value(25).toFloat();
                g_par.other_markpos[1].x4 = rquery.value(26).toFloat();
                g_par.other_markpos[1].y4 = rquery.value(27).toFloat();
                if(isload)
                {
                    material.WriteIni("TRAJ","DOWN_FILM_D",rquery.value(1).toString());
                    material.WriteIni("TRAJ","BOARD_D2",rquery.value(2).toString());
                    material.WriteIni("TRAJ","XIA_DI_PIAN",rquery.value(3).toString());
                    material.WriteIni("TRAJ","PRODUCT_THICKNESS_UM2",rquery.value(4).toString());
                    material.WriteIni("TRAJ","DUIWEI_DISTANCE2",rquery.value(5).toString());
                    material.WriteIni("TRAJ","CCD_SWITCH2",rquery.value(6).toString());
                    material.WriteIni("TRAJ","CCD_234_2",rquery.value(7).toString());
                    material.WriteIni("TRAJ","DIPIAN_WUCHA_X2",rquery.value(8).toString());
                    material.WriteIni("TRAJ","DIPIAN_WUCHA_Y2",rquery.value(9).toString());
                    material.WriteIni("TRAJ","JUNFEN_WUCHA_X2",rquery.value(10).toString());
                    material.WriteIni("TRAJ","JUNFEN_WUCHA_Y2",rquery.value(11).toString());
                }
            }
            else
            {
                g_par.Downfilm_down_dia = rquery.value(1).toFloat();
                g_par.Downboard_down_dia = rquery.value(2).toFloat();
                g_par.bg_down_time2 = rquery.value(3).toInt();
                g_par.down_bm_height = rquery.value(4).toFloat();
                g_par.down_dw_height = rquery.value(5).toFloat();
                g_par.down_downccd_select = rquery.value(6).toInt();
                g_par.downccd_234[1] = rquery.value(7).toInt();
                g_par.DownDP_down_error_x = rquery.value(8).toFloat();
                g_par.DownDP_down_error_y = rquery.value(9).toFloat();
                g_par.DownDP_down_junfen_x = rquery.value(10).toFloat();
                g_par.DownDP_down_junfen_y = rquery.value(11).toFloat();
                g_par.main_downmarkpos[1].x1 = rquery.value(12).toFloat();
                g_par.main_downmarkpos[1].y1 = rquery.value(13).toFloat();
                g_par.main_downmarkpos[1].x2 = rquery.value(14).toFloat();
                g_par.main_downmarkpos[1].y2 = rquery.value(15).toFloat();
                g_par.main_downmarkpos[1].x3 = rquery.value(16).toFloat();
                g_par.main_downmarkpos[1].y3 = rquery.value(17).toFloat();
                g_par.main_downmarkpos[1].x4 = rquery.value(18).toFloat();
                g_par.main_downmarkpos[1].y4 = rquery.value(19).toFloat();
                g_par.other_downmarkpos[1].x1 = rquery.value(20).toFloat();
                g_par.other_downmarkpos[1].y1 = rquery.value(21).toFloat();
                g_par.other_downmarkpos[1].x2 = rquery.value(22).toFloat();
                g_par.other_downmarkpos[1].y2 = rquery.value(23).toFloat();
                g_par.other_downmarkpos[1].x3 = rquery.value(24).toFloat();
                g_par.other_downmarkpos[1].y3 = rquery.value(25).toFloat();
                g_par.other_downmarkpos[1].x4 = rquery.value(26).toFloat();
                g_par.other_downmarkpos[1].y4 = rquery.value(27).toFloat();
                if(isload)
                {
                    material.WriteIni("TRAJ","DOWNDOWN_FILM_D",rquery.value(1).toString());
                    material.WriteIni("TRAJ","DOWNBOARD_D2",rquery.value(2).toString());
                    material.WriteIni("TRAJ","XIA_DI_PIAN2",rquery.value(3).toString());
                    material.WriteIni("TRAJ","PRODUCT_THICKNESS_UM2",rquery.value(4).toString());
                    material.WriteIni("TRAJ","DUIWEI_DISTANCE2",rquery.value(5).toString());
                    material.WriteIni("TRAJ","DOWNCCD_SWITCH2",rquery.value(6).toString());
                    material.WriteIni("TRAJ","DOWNCCD_234_2",rquery.value(7).toString());
                    material.WriteIni("TRAJ","DOWNDIPIAN_WUCHA_X2",rquery.value(8).toString());
                    material.WriteIni("TRAJ","DOWNDIPIAN_WUCHA_Y2",rquery.value(9).toString());
                    material.WriteIni("TRAJ","DOWNJUNFEN_WUCHA_X2",rquery.value(10).toString());
                    material.WriteIni("TRAJ","DOWNJUNFEN_WUCHA_Y2",rquery.value(11).toString());
                }
            }
        }
        //靶标数据
        if(isload)
        {
            CIniRW markfile(MARKPARAMETERPATH);
            QString mainccdx[4],mainccdy[4],maingroup;
            QString otherccdx[4],otherccdy[4],othergroup;
            if(data.frameid==0)
            {
                maingroup = "MAIN_MARK1";
                othergroup = "OTHER_MARK1";
            }
            else if(data.frameid==1)
            {
                maingroup = "MAIN_MARK2";
                othergroup = "OTHER_MARK2";
            }
            if(data.ab)
            {
                //主靶标
                mainccdx[0].sprintf("%.3f",g_par.main_markpos[data.frameid].x1);
                mainccdx[1].sprintf("%.3f",g_par.main_markpos[data.frameid].x2);
                mainccdx[2].sprintf("%.3f",g_par.main_markpos[data.frameid].x3);
                mainccdx[3].sprintf("%.3f",g_par.main_markpos[data.frameid].x4);
                mainccdy[0].sprintf("%.3f",g_par.main_markpos[data.frameid].y1);
                mainccdy[1].sprintf("%.3f",g_par.main_markpos[data.frameid].y2);
                mainccdy[2].sprintf("%.3f",g_par.main_markpos[data.frameid].y3);
                mainccdy[3].sprintf("%.3f",g_par.main_markpos[data.frameid].y4);
                markfile.WriteIni(maingroup,"ccd1x",mainccdx[0]);
                markfile.WriteIni(maingroup,"ccd2x",mainccdx[1]);
                markfile.WriteIni(maingroup,"ccd3x",mainccdx[2]);
                markfile.WriteIni(maingroup,"ccd4x",mainccdx[3]);
                markfile.WriteIni(maingroup,"ccd1y",mainccdy[0]);
                markfile.WriteIni(maingroup,"ccd2y",mainccdy[1]);
                markfile.WriteIni(maingroup,"ccd3y",mainccdy[2]);
                markfile.WriteIni(maingroup,"ccd4y",mainccdy[3]);
                //次靶标
                otherccdx[0].sprintf("%.3f",g_par.other_markpos[data.frameid].x1);
                otherccdx[1].sprintf("%.3f",g_par.other_markpos[data.frameid].x2);
                otherccdx[2].sprintf("%.3f",g_par.other_markpos[data.frameid].x3);
                otherccdx[3].sprintf("%.3f",g_par.other_markpos[data.frameid].x4);
                otherccdy[0].sprintf("%.3f",g_par.other_markpos[data.frameid].y1);
                otherccdy[1].sprintf("%.3f",g_par.other_markpos[data.frameid].y2);
                otherccdy[2].sprintf("%.3f",g_par.other_markpos[data.frameid].y3);
                otherccdy[3].sprintf("%.3f",g_par.other_markpos[data.frameid].y4);

                markfile.WriteIni(othergroup,"ccd1x",otherccdx[0]);
                markfile.WriteIni(othergroup,"ccd2x",otherccdx[1]);
                markfile.WriteIni(othergroup,"ccd3x",otherccdx[2]);
                markfile.WriteIni(othergroup,"ccd4x",otherccdx[3]);
                markfile.WriteIni(othergroup,"ccd1y",otherccdy[0]);
                markfile.WriteIni(othergroup,"ccd2y",otherccdy[1]);
                markfile.WriteIni(othergroup,"ccd3y",otherccdy[2]);
                markfile.WriteIni(othergroup,"ccd4y",otherccdy[3]);
            }
            else
            {
                //主靶标
                mainccdx[0].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].x1);
                mainccdx[1].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].x2);
                mainccdx[2].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].x3);
                mainccdx[3].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].x4);
                mainccdy[0].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].y1);
                mainccdy[1].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].y2);
                mainccdy[2].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].y3);
                mainccdy[3].sprintf("%.3f",g_par.main_downmarkpos[data.frameid].y4);
                markfile.WriteIni(maingroup,"ccd5x",mainccdx[0]);
                markfile.WriteIni(maingroup,"ccd6x",mainccdx[1]);
                markfile.WriteIni(maingroup,"ccd7x",mainccdx[2]);
                markfile.WriteIni(maingroup,"ccd8x",mainccdx[3]);
                markfile.WriteIni(maingroup,"ccd5y",mainccdy[0]);
                markfile.WriteIni(maingroup,"ccd6y",mainccdy[1]);
                markfile.WriteIni(maingroup,"ccd7y",mainccdy[2]);
                markfile.WriteIni(maingroup,"ccd8y",mainccdy[3]);
                //次靶标
                otherccdx[0].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].x1);
                otherccdx[1].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].x2);
                otherccdx[2].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].x3);
                otherccdx[3].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].x4);
                otherccdy[0].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].y1);
                otherccdy[1].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].y2);
                otherccdy[2].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].y3);
                otherccdy[3].sprintf("%.3f",g_par.other_downmarkpos[data.frameid].y4);

                markfile.WriteIni(othergroup,"ccd5x",otherccdx[0]);
                markfile.WriteIni(othergroup,"ccd6x",otherccdx[1]);
                markfile.WriteIni(othergroup,"ccd7x",otherccdx[2]);
                markfile.WriteIni(othergroup,"ccd8x",otherccdx[3]);
                markfile.WriteIni(othergroup,"ccd5y",otherccdy[0]);
                markfile.WriteIni(othergroup,"ccd6y",otherccdy[1]);
                markfile.WriteIni(othergroup,"ccd7y",otherccdy[2]);
                markfile.WriteIni(othergroup,"ccd8y",otherccdy[3]);
            }
            //触发数据更新
            g_loadmaterial = true;
            return true;
        }
    }
    else
    {
        qDebug()<<"read:error";
        return false;
    }
}

DlgMaterialManage::~DlgMaterialManage()
{
    db.close();
    delete sqlmodel;
    delete ui;
}

void DlgMaterialManage::on_btn_search_clicked()
{
    newtable.name = ui->lineEdit_name->text();
    if(newtable.name.isEmpty())
    {
        QMessageBox::information(this,"TOP",QObject::tr("料号编码不能为空！"));
        return;
    }
    QSqlQuery searchquery(db);
    searchquery.prepare(select_sql);
    searchquery.addBindValue(newtable.name);
    searchquery.exec();
    int serach_row = 0;
    if(searchquery.next())
    {
        if(searchquery.value(0).toString().isEmpty())
        {
            QMessageBox::information(this,"TOP",QObject::tr("料号编码不存在！"));
            return;
        }
        else
        {
            qDebug()<<"search:ok";
            searchquery.prepare(select_col_sql);
            searchquery.exec();
            while(searchquery.next())
            {
                if(searchquery.value(0).toString()==newtable.name)
                {
                    serach_row = searchquery.at();
                    break;
                }
            }
            ui->tableView_database->selectRow(serach_row);
        }
    }
}

void DlgMaterialManage::on_btn_add_clicked()
{
    newtable.name = ui->lineEdit_name->text();
    if(newtable.name.isEmpty())
    {
        QMessageBox::information(this,"TOP",QObject::tr("料号编码不能为空！"));
        return;
    }
    QSqlQuery addquery(db);
    //查询是否有重名
    addquery.prepare(select_sql);
    addquery.addBindValue(newtable.name);
    if(!addquery.exec())
    {
        qDebug()<<addquery.lastError();
        return;
    }
    else
    {
        if(addquery.next())
        {
            if(!addquery.value(0).toString().isEmpty())
            {
                QMessageBox::information(this,"TOP",QObject::tr("料号编码重复！"));
                return;
            }
        }
        qDebug()<<"add:new name";
    }
    if(g_now_frame>=0)
    {
//        DlgABSelect absel(this);
//        if(0==absel.exec())
//        {
//            return;
//        }
        addquery.prepare(insert_sql);
        //当前日期
        QDateTime CurrentTime=QDateTime::currentDateTime();
        newtable.date=CurrentTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示的格式
        addquery.addBindValue(newtable.name);
        addquery.addBindValue(newtable.date);
        newtable.frameid = g_now_frame;
        addquery.addBindValue(newtable.frameid);
        QString str;
        if(g_now_frame==0)
        {
           str = QObject::tr("上框");
        }
        else
        {
           str = QObject::tr("下框");
        }
        addquery.addBindValue(str);
        if(isAB)
        {
           str = QObject::tr("A");
        }
        else
        {
           str = QObject::tr("B");
        }
        newtable.ab = isAB;
        addquery.addBindValue(str);
        if(!addquery.exec())
        {
            qDebug()<<addquery.lastError();
            QMessageBox::information(this,"TOP",QObject::tr("料号文件访问失败！"));
            return;
        }
        if(false==MaterialWrite(newtable))
        {
            QMessageBox::information(this,"TOP",QObject::tr("料号保存失败！"));
            return;
        }
        update_view();
    }
    else
    {
        qDebug()<<"add:no such frameID";
        QMessageBox::information(this,"TOP",QObject::tr("无效框架ID"));
        return;
    }
}

void DlgMaterialManage::on_btn_delete_clicked()
{
    int row = ui->tableView_database->currentIndex().row();
    QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("确定删除料号?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QAbstractItemModel *model = ui->tableView_database->model();
        QModelIndex index = model->index(row,0);//选中行第一列的内容
        QVariant code = model->data(index);
        QSqlQuery delquery(db);
        delquery.prepare(delete_sql);
        delquery.addBindValue(code);
        delquery.exec();
        //删除数据
        delquery.prepare(delete_table);
        delquery.addBindValue(code);
        delquery.exec();
        //
        update_view();
    }
}

void DlgMaterialManage::on_btn_fetch_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("确定取出料号?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::No)
    {
       return;
    }
    int row = ui->tableView_database->currentIndex().row();
    QAbstractItemModel *model = ui->tableView_database->model();
    QModelIndex index = model->index(row,0);//选中行第一列的内容
    QVariant code = model->data(index);
    QSqlQuery fetchquery(db);
    fetchquery.prepare(select_sql);
    fetchquery.addBindValue(code);
    fetchquery.exec();
    if(fetchquery.next())
    {
        MaterialInfo toread;
        toread.name = fetchquery.value(0).toString();
        toread.frameid = fetchquery.value(2).toInt();
        if(fetchquery.value(4).toString()=="A")
        {
            toread.ab = true;
        }
        else if(fetchquery.value(4).toString()=="B")
        {
            toread.ab = false;
        }
        if(MaterialRead(toread,false))
        {
            qDebug()<<"fetch:ok";
        }
        else
        {
            qDebug()<<toread.name+"fetch:read error";
            QMessageBox::information(this,"TOP",QObject::tr("料号读取失败！"));
        }
    }
    else
    {
        qDebug()<<"fetch:error";
        QMessageBox::information(this,"TOP",QObject::tr("料号文件访问失败！"));
    }
}

void DlgMaterialManage::on_btn_loadmark_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("载入操作会覆盖当前参数，确定载入料号?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::No)
    {
       return;
    }
    int row = ui->tableView_database->currentIndex().row();
    QAbstractItemModel *model = ui->tableView_database->model();
    QModelIndex index = model->index(row,0);//选中行第一列的内容
    QVariant code = model->data(index);
    QSqlQuery fetchquery(db);
    fetchquery.prepare(select_sql);
    fetchquery.addBindValue(code);
    fetchquery.exec();
    if(fetchquery.next())
    {
        MaterialInfo toread;
        toread.name = fetchquery.value(0).toString();
        toread.frameid = fetchquery.value(2).toInt();
        if(fetchquery.value(4).toString()=="A")
        {
            toread.ab = true;
        }
        else if(fetchquery.value(4).toString()=="B")
        {
            toread.ab = false;
        }
        if(MaterialRead(toread,true))
        {
            qDebug()<<"load:ok";
        }
        else
        {
            qDebug()<<toread.name+"load:read error";
            QMessageBox::information(this,"TOP",QObject::tr("料号读取失败！"));
        }
    }
    else
    {
        qDebug()<<"load:error";
        QMessageBox::information(this,"TOP",QObject::tr("料号文件访问失败！"));
    }
}

void DlgMaterialManage::on_btn_exit_clicked()
{
    this->close();
}

void DlgMaterialManage::on_btn_liaohaoclear_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, "TOP", QObject::tr("确定清空所有料号?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
    {
        QSqlQuery delquery(db);
        delquery.prepare(clear_sql);
        delquery.exec();
        //
        update_view();
    }
}
