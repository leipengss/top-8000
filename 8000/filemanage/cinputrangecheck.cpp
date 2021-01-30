#include "cinputrangecheck.h"
#include<QRegExp>
#include<QRegExpValidator>

CInputRangeCheck::CInputRangeCheck()
{
}

/***************************************************************************
*breif：strValue有效性判断
*param：nType [in] 有效性的类型(实数/整数等等 此处只做了整数与小数的判断)
*param: strValue [in] 需要判断的内容
*return: 如果strValue 符合该类型的正则表达式 则返回ture
****************************************************************************/
bool CInputRangeCheck::regExpCheck(int nType,QString strValue)
{
    bool bRet = true;
    int pos = 0;
    QRegExp int_reg("^[-?\\d+]?\\d*$");
    QRegExpValidator int_valid(int_reg,0);
    QRegExp float_reg("^-?\\d+(\\.\\d+)?$");
    QRegExpValidator float_valid(float_reg,0);
    QRegExp ip_reg("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[\\.]){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    QRegExpValidator ip_valid(ip_reg,0);
    QRegExp syntax_reg("^[A-Za-z0-9]{4}");
    QRegExpValidator reg_valid(syntax_reg,0);
    switch (nType)
    {
     case VALID_INTEGER:
        pos = 0;
        if(QValidator::Acceptable!=int_valid.validate(strValue,pos))
        {
            bRet = false;
        }
        break;
     case VALID_REAL_NUMBER:
        pos = 0;
        if (QValidator::Acceptable!=float_valid.validate(strValue,pos))
        {
            bRet = false;
        }
        break;
    case VALID_IP:
       pos = 0;
       if (QValidator::Acceptable!=ip_valid.validate(strValue,pos))
       {
           bRet = false;
       }
       break;
    case VALID_REGSYNTAX:
       pos = 0;
       if (QValidator::Acceptable!=reg_valid.validate(strValue,pos))
       {
           bRet = false;
       }
       break;
     }
     return bRet;
}

int CInputRangeCheck::checkrange(int nType,QString min,QString max,QString value)
{
    int bRet = 1;
    int int_min,int_max,int_val;
    int float_min,float_max,float_val;
    if(regExpCheck(nType,value))
    {
        switch (nType)
        {
         case VALID_INTEGER:
            int_min = min.toInt();
            int_max = max.toInt();
            int_val = value.toInt();
            if((int_val<int_min)||(int_val>int_max))
            {
                bRet=0;
            }
            break;
         case VALID_REAL_NUMBER:
            float_min = min.toFloat();
            float_max = max.toFloat();
            float_val = value.toFloat();
            if((float_val<float_min)||(float_val>float_max))
            {
                bRet=0;
            }
            break;
         }
    }
    else
    {
        bRet=-1;
    }
    return bRet;
}
