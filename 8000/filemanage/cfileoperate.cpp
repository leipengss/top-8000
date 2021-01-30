#include "cfileoperate.h"
#include <QFile>
#include<QDebug>
#include<QDir>
#include<QFileInfo>
#include<QDateTime>

CFileOperate::CFileOperate()
{

}
//拷贝文件：
bool CFileOperate::copyFileToPath(QString sourceFile ,QString toDir,bool copyorrecover)
{
    toDir.replace("\\","/");
    QFileInfo fromFileDir(sourceFile);
    QDir toFileDir(toDir);
    if(fromFileDir.isDir())
    {
        qDebug()<<"sourcefile is a dir!";
        return false;
    }
    if(!fromFileDir.exists())
    {
        qDebug()<<"no sourcefile!";
        return false;
    }
    if(!toFileDir.exists())
    {
        qDebug()<<"no dir,and mkdir";
        if(!toFileDir.mkdir(toDir))
        {
            qDebug()<<"fail to mkdir";
            return false;
        }
    }
    QString newfilename;
    QString file_suffix = fromFileDir.suffix();
    qDebug()<<file_suffix;
    qDebug()<<fromFileDir.fileName();
    int pos = fromFileDir.fileName().indexOf(file_suffix)-1;
    if(copyorrecover)//recover
    {
        newfilename = fromFileDir.fileName().left(pos-15)+QObject::tr(".")+file_suffix;//15:-与yyyyMMddhhmmss
        QString tmpDir = toDir+newfilename;
//        qDebug()<<tmpDir;
        QFileInfo tmpfile(tmpDir);
        if(tmpfile.exists())
        {
            if(!QFile::remove(tmpDir))
            {
                qDebug()<<"fail to remove";
                return false;
            }
        }
    }
    else
    {
        //rename the copy
        QDateTime CurrentTime=QDateTime::currentDateTime();
        QString timestr=CurrentTime.toString("yyyyMMddhhmmss"); //设置显示的格式
        newfilename = fromFileDir.fileName().left(pos)+QObject::tr("_")+timestr+QObject::tr(".")+file_suffix;
    }
    qDebug()<<newfilename;
    if(!QFile::copy(fromFileDir.filePath(),toFileDir.filePath(newfilename/*fromFileDir.fileName()*/)))
    {
        qDebug()<<"fail to copy";
        return false;
    }
    return true;
}

//拷贝文件夹：
bool CFileOperate::copyDirectoryFiles(const QString &fromDir, const QString &toDir)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!sourceDir.exists())
    {
        qDebug()<<"no fromDir!";
        return false;
    }
    if(!targetDir.exists())
    {    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir())
        {
            /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName())))
                return false;
        }
        else
        {
            /**< 将旧文件进行删除操作 */
            if(targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }
            // 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
               targetDir.filePath(fileInfo.fileName())))
            {
               return false;
            }
        }
    }
    return true;
}
//检查文件是否丢失
bool CFileOperate::isFileExist(QString filepath,QString filename)
{
    filepath.replace("\\","/");
    QFileInfo file(filename);
    QDir dir(filepath);
    if(!dir.exists())
    {
        qDebug()<<"no filepath!";
        /**< 如果目标目录不存在，则进行创建 */
        if(!dir.mkdir(dir.absolutePath()))
        {
            qDebug()<<"fail to creat filepath!";
            return false;
        }
        return true;
    }
    if(!file.exists())
    {
        qDebug()<<"no file!";
        return false;
    }
    return true;
}
