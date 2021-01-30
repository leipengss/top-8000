#ifndef CFILEOPERATE_H
#define CFILEOPERATE_H

#include<QString>

class CFileOperate
{
public:
    CFileOperate();
public:
    bool copyFileToPath(QString sourceFile ,QString toDir,bool copyorrecover);//copyorrecover  false:backup true:recover
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir);
    bool isFileExist(QString filepath,QString filename);
};

#endif // CFILEOPERATE_H
