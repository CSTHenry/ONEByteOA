#ifndef _ATTENDANCE_H_
#define _ATTENDANCE_H_
#include <cstring>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
class attendance {
public:
    static void savaAttendance(attendance *ahead);

    static attendance *loadData(attendance *ahead);

    char Uid[12] = "\0";
    attendance *next = nullptr;

    bool cheakUid(char *id)//ID匹配返回true
    {
        if (!strcmp(Uid, id))
            return true;
        return false;
    }

    char *getSimpleTime() {
        return simpleTime;
    }
        char *getAttendanceTime()
        {
            return attendanceTime;
        }

    char *getBackTime() {
        return backTime;
    }

    void takeAttendance(char *Time, char *simple, bool todayMethod)//需要当前时间和签到方式（签到接口）
    {
        strcpy(attendanceTime, Time);
        strcpy(simpleTime, simple);
        method = todayMethod;
    }

    void takeBack(char *Time)//签退接口
    {
        strcpy(backTime, Time);
    }

    void printInf(char *today)//综合打印签到、签退状态，用于个人菜单，且自动清除过期记录
    {
        if (!strcmp(simpleTime, today)) {
            printAttendance();
            if (strlen(backTime)) {
                cout << endl;
                printBack();
            } else
                cout << "[未签退]" << endl;
        } else {
            deleteYesterday();
            cout << "UID：" << Uid << "[未签到]" << endl;
        }
    }

    void printAdminInf(char *today)//简略打印考勤状态，用于管理菜单，在输出用户信息后调用，且自动清除过期记录
    {
        if (!strcmp(simpleTime, today)) {
            cout << "[已签到]" << "签到时间：" << attendanceTime << " ";
        } else {
            deleteYesterday();//清除过期签到记录
            cout << "[当天未签到]" << endl;
            return;
        }
        if (method) {
            if (strlen(backTime))
                cout << "签退时间：" << backTime;
            else
                cout << "[未签退]";
        }
        cout << " 签到方式：" << getMethod() << endl;
    }

    void expAdminInf(char *today, userAccount *head, int flag)//简略打印考勤状态，用于管理菜单，在输出用户信息后调用，且自动清除过期记录
    {
        ofstream expFile("expAttendance.csv", ios::out | ios::app);
        if (flag == 1) {
            expFile << "考勤表导出日期：," << today << "\n";
            expFile << "UID,姓名,职位,签到状态,签到时间,签退状态,签退时间,签到方式" << endl;
        }
        expFile << "[" << head->uid << "]," << head->uName() << "," << head->search_Situation() << ",";
        if (!strcmp(simpleTime, today)) {
            expFile << "[已签到]," << attendanceTime << ",";
        } else {
            expFile << "[当天未签到],NULL,NULL,NULL,NULL" << "\n";
            return;
        }
        if (method) {
            if (strlen(backTime))
                expFile << "[已签退]," << backTime << ",";
            else
                expFile << "[未签退],NULL,";
        } else {
            expFile << "NULL,NULL,";
        }
        expFile << getMethod() << "\n";
    }

    void printAttendance() {
        cout << "UID：" << Uid << endl << "[已签到]"
             << " 签到方式：" << getMethod() << endl;
        cout << "签到时间：" << attendanceTime << endl;
    }

    void printBack() {
        cout << "[已签退]" << endl;
        cout << "签退时间：" << backTime << endl;
    }

    [[nodiscard]] string getMethod() const;

    void deleteYesterday()//用户simpleTime不相同时调用
    {
        method = false;
        for (int i = 0; attendanceTime[i]; i++)
            attendanceTime[i] = '\0';
        for (int i = 0; backTime[i]; i++)
            backTime[i] = '\0';
    }

private://developer:GitHub_CSTHenry(zhengke@bytecho.net)
    bool method = false;//false为补签，true为自行签到
    char attendanceTime[50] = "\0";
    char backTime[50] = "\0";
    char simpleTime[25] = "\0";//仅显示日期，不显示具体时间，用于判断当天是否签到
};
#endif