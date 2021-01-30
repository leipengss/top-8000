#ifndef CXMLPARMETERSHOW_H
#define CXMLPARMETERSHOW_H

#include <QWidget>
#include"filemanage/cxmlparameter.h"

namespace Ui {
class Cxmlparmetershow;
}

class Cxmlparmetershow : public QWidget
{
    Q_OBJECT

public:
    explicit Cxmlparmetershow(int num,QWidget *parent = 0);
    ~Cxmlparmetershow();
    void xmlparshow();
    QVector<QString> parrootname_list;
    bool isParChanged();
    int XmlParSave();
    bool m_bModifyFlg;
    bool isrestart;
private slots:
    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::Cxmlparmetershow *ui;
    CXMLParameter *xmlparse;
    QVector<QString> parfirstnodename_list;
//    QVector<QString> pargroupname_list;
//    QVector<QString> parkeyname_list;
//    QVector<QString> parminvalue_list;
//    QVector<QString> parmaxvalue_list;
//    QVector<QString> parchinaname_list;
//    QVector<QString> pardescribe_list;
//    QVector<QString> parreboot_list;
//    QVector<QString> parspeed_list;
    QVector<QString> parvalue_list;
    QVector<QString> newvalue_list;//新值的索引值+offset*firstnodenum 才对应上初始值parvalue_list
    int offset;//第几项参数，该值为tab的索引号
    int firstnodenum;//参数类别项，即tab的总数
    int parrow;//每个参数类别的项数，即当前tab下的行数
    QVector<int> modify_position;
    bool isParOutRange(int pos,QString val);
    void updatepara(int tableindex,int pos);
};

#endif // CXMLPARMETERSHOW_H
