#ifndef CXMLPARAMETER_H
#define CXMLPARAMETER_H
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include<QVector>

class CXMLParameter
{
public:
    CXMLParameter(QString file);
    int writeXml();
    int readXml();
    int readXml2();
    void RemoveXml();
    void AddXml();
    void UpdateXml();
    //save data
    QVector<QString> rootname_list;
    QVector<QString> firstnodename_list;
    QVector<QString> groupname_list;
    QVector<QString> keyname_list;
    QVector<QString> typevalue_list;//0:整型 1：浮点数
    QVector<QString> minvalue_list;
    QVector<QString> maxvalue_list;
    QVector<QString> chinaname_list;
    QVector<QString> describe_list;
    QVector<QString> reboot_list;
    QVector<QString> speed_list;
private:
    QString filename;

};

#endif // CXMLPARAMETER_H
