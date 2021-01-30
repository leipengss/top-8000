#ifndef CLOGFILESHOW_H
#define CLOGFILESHOW_H

#include <QWidget>
#include<QVector>

namespace Ui {
class CLogFileshow;
}

class CLogFileshow : public QWidget
{
    Q_OBJECT
    
public:
    explicit CLogFileshow(QString file,QWidget *parent = 0);
    ~CLogFileshow();
    void logshow();
    void clear();
private:
    Ui::CLogFileshow *ui;
    QString filename;
    int tablecount;
    QVector<QString> text_list;
    void LogfileRead();
    void showlog();
};

#endif // CLOGFILESHOW_H
