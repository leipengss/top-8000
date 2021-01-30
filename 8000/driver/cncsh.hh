#ifndef CNCSH_HH
#define CNCSH_HH
#include<QString>

enum REG_TYPE{
    UNREG = 0,
    REGED,
    ONE_DAY_LEFT,
    TWO_DAY_LEFT,
    THREE_DAY_LEFT,
    LONGTIME,
    REG_END
};

enum DAY_LEFT{
    ONE_LEFT = 0,
    TWO_LEFT,
    THREE_LEFT,
    FOUR_LEFT,
    FIVE_LEFT
};
extern QString reg_info;
extern int g_registertype;
extern int real_year;
extern int real_month;
extern int real_day;
extern long nDayLeave;
extern int nRemainHour;   // 剩余小时
extern int ADayPassFlag;
extern int AHourPassFlag;
extern char limitdate_back[9]; //cnc_system的返回值
extern char psw_back[9];   //cnc_psw的返回值
extern int cnc_sysnum(int flag, const char *sysnum);	//密码输入读写系统编号
extern int cnc_psw(const char *calibration, const char *psw);
extern int diff_time(void); //比较密码日期和实际时间
#endif
