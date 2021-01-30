#include "cinirw.h"
#include<QSettings>
#include<QStringList>
#include<QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
CIniRW::CIniRW(QString filepath)
{
    filename = filepath;
}
int CIniRW::writefile(const char *filename,const char *seckey, const char *text)	//写入ini配置
{
    FILE *fp;
    filelock.lockForRead();
    if((fp=fopen(filename,"r"))==NULL)
    {
        qDebug("cannot open file %s\n",filename);
        return -1;
    }

    char file[10000][500];
    int line=0;

    while(fgets(file[line],1000,fp)!=NULL)
    {
        line++;	//获取行数
    }
    fclose(fp);
    filelock.unlock();
    if(line>=20000)
    {
        qDebug("ini path too long!");
        return -1;
    }
    int flagset=0;  //判断是否存在section
    char *p,buf[100],buf1[50],buf2[50];
    strcpy(buf,seckey);
    p=strtok(buf,"//");
    if(p) strcpy(buf1,p);
    p=strtok(NULL,"//");
    if(p) strcpy(buf2,p);
    char compar1[50],compar2[50];
    sprintf(compar1,"%s ",buf2);
    sprintf(compar2,"%s=",buf2);
    for(int i=0;i<line;i++)
    {
        char ss[51];
        sprintf(ss,"[%s]",buf1);
        if(!strncmp(ss,file[i],strlen(ss))) flagset=1;
        if((!strncmp(compar1,file[i],strlen(compar1))||!strncmp(compar2,file[i],strlen(compar2))) && flagset==1) {sprintf(file[i],"%s = %s\n",buf2,text);break;}
    }
    filelock.lockForWrite();
    FILE *tp;
    if((tp=fopen(filename,"w"))==NULL)
    {
        qDebug("cannot open file %s\n",filename);
        return -1;
    }
    for(int i=0;i<line;i++)
    {
        fprintf(tp,"%s",file[i]);
    }
    filelock.unlock();
    fclose(tp);
    return 0;
}
 //inifile.WriteIni("TRAJ","XIA_DI_PIAN",m_sDownBG_time);    CIniRW inifile(TOPPARAMETERPATH);
void CIniRW::WriteIni(QString group, QString keyname,QString val)
{
    //按照QT的封装函数写法，INI文件会出现乱码，主要是因为PPMC中有相同键  以及符号#
    QString ppmc_path = "/home/u/cnc/configs/ppmc/ppmc.ini";
    if(filename==ppmc_path)
    {
        QString group_key = group+"/"+keyname;
        writefile(filename.toStdString().c_str(),group_key.toStdString().c_str(),val.toStdString().c_str());
    }
    else
    {
        QSettings *ini=new QSettings(filename,QSettings::IniFormat);
        ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
        ini->beginGroup(group);
        ini->setValue(keyname,val);
        ini->endGroup();
        delete ini;
    }
}
//字段 键值
QVariant CIniRW::ReadIni(QString group, QString keyname)
{
    QSettings *ini=new QSettings(filename,QSettings::IniFormat);
    filelock.lockForRead();
    ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
    ini->beginGroup(group);
    QVariant val= ini->value(keyname);
    ini->endGroup();
    filelock.unlock();
    delete ini;
    return val;
}
//通过键名及其值 字段类型 获取字段下其它键值,确保键名其值的唯一性
QVariant CIniRW::ReadIniKeyVal(QString keyname, QString keyvalue,int grouptype,int keytype)
{
    QSettings *ini=new QSettings(filename,QSettings::IniFormat);
    ini->setIniCodec(QTextCodec::codecForName("UTF-8"));
    filelock.lockForRead();
    QVariant val;
    QString tmp1,tmp2;
    switch(grouptype)
    {
    case 0:
        tmp1 = "OUTPUT";
        break;
    case 1:
        tmp1 = "INPUT_";
        break;
    }
    switch(keytype)
    {
    case 0:
        tmp2 = "SIGNAL_NAME";
        break;
    case 1:
        tmp2 = "DEFAULT";
        break;
    }
    foreach (QString group, ini->childGroups())
    {
        ini->beginGroup(group);
        if(group.left(6)==tmp1)
        {
            foreach (QString key, ini->childKeys())
            {
                 QTextCodec *codec=QTextCodec::codecForName("UTF-8");
                 QString outStr = codec->toUnicode(key.toLatin1());
                 if (outStr == keyname)
                 {
                    QString outnumtmp = ini->value(outStr).toString();
                    if(outnumtmp==keyvalue)
                    {
                        val = ini->value(tmp2);
                    }
                 }
            }
        }
        ini->endGroup();
    }
    filelock.unlock();
    delete ini;
    return val;
}
