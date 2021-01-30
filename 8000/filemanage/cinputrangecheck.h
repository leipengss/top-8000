#ifndef CINPUTRANGECHECK_H
#define CINPUTRANGECHECK_H
#include<QString>

#define VALID_INTEGER (0)
#define VALID_REAL_NUMBER (1)
#define VALID_IP (2)
#define VALID_REGSYNTAX (3)

class CInputRangeCheck
{
public:
    CInputRangeCheck();
    bool regExpCheck(int nType,QString strValue);
    int checkrange(int nType,QString min,QString max,QString value);
};

#endif // CINPUTRANGECHECK_H
