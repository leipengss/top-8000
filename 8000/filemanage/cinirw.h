#ifndef CINIRW_H
#define CINIRW_H
#include<QString>
#include<QVariant>
#include<QReadWriteLock>
class CIniRW
{
public:
    CIniRW(QString filepath);

    void WriteIni(QString group,QString keyname,QString val);
    QVariant ReadIni(QString group,QString keyname);
    //通过键名及其值 字段类型 获取字段下其它键值,确保键名其值的唯一性
    QVariant ReadIniKeyVal(QString keyname, QString keyvalue,int grouptype,int keytype);//grouptype 0:output 1:input  keytype 0:signal name 1: default val
private:
    QString filename;
    QReadWriteLock filelock;
    int writefile(const char *filename,const char *seckey, const char *text);
};

#endif // CINIRW_H
