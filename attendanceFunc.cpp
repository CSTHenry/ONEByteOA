#include "attendance.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

//developer:GitHub_CSTHenry(zhengke@bytecho.net)
char *simpleTime();

char *nowTime() {
    static char nowTime[50];
    time_t now = time(nullptr);
    strftime(nowTime, 50, "%Y年%m月%d日 %H:%M:%S", localtime(&now));
    return nowTime;
}

char *simpleTime() {
    static char simpleTime[25];
    time_t now = time(nullptr);
    strftime(simpleTime, 50, "%Y年%m月%d日", localtime(&now));
    return simpleTime;
}

attendance *searchUserAttendance(attendance *head, char *uid)//找到用户签到数据返回指针，否则返回Nullptr
{
    while (head) {
        if (head->cheakUid(uid))
            break;
        head = head->next;
    }
    return head;
}

attendance *userToData(userAccount *head)//将当前所有用户ID写入签到数据表，用于程序初始化
{
    auto *ahead = new attendance();
    attendance *nextPtr;
    nextPtr = nullptr;
    auto ptr = ahead;
    while (head)//将用户数据表中的所有UID写入考勤数据表UID，并构建考勤数据表长链
    {
        nextPtr = new attendance();
        strcpy(ahead->Uid, head->uid);
        ahead->next = nextPtr;
        ahead = ahead->next;
        head = head->next;
    }
    return ptr;
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
void attendance::savaAttendance(attendance *ahead)//将考勤链表保存至二进制,在对考勤进行操作后，均要执行此函数
{
    ofstream openUserFile("./src/attendanceData.dat", ios::binary | ios::out);//./src/attendanceData.dat
    if (!openUserFile.fail()) {
        do {
            openUserFile.write((char *) ahead->Uid, sizeof(ahead->Uid));
            openUserFile.write((char *) &ahead->method, sizeof(bool));
            openUserFile.write((char *) ahead->attendanceTime, sizeof(ahead->attendanceTime));
            openUserFile.write((char *) ahead->backTime, sizeof(ahead->backTime));
            openUserFile.write((char *) &ahead->simpleTime, sizeof(ahead->simpleTime));
            ahead = ahead->next;
        } while (ahead);
    } else {
        cout << "fault,code:attendanceSaveError." << endl;
        openUserFile.close();
        return;
    }
    openUserFile.close();
}

[[maybe_unused]] attendance *searchUserPtr(attendance *ahead, char *id)//查找考勤数据中的相应UID，并返回对应指针，无此UID返回Nullptr
{
    while (ahead) {
        if (ahead->cheakUid(id))
            break;
        ahead = ahead->next;
    }
    return ahead;
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
attendance *attendance::loadData(attendance *ahead)//载入考勤数据，处理头指针，返回尾指针，载入失败返回Nullptr
{
    int flag = 0;
    attendance *temp = ahead;
    attendance *last = nullptr;
    ifstream loadUserFile("./src/attendanceData.dat",
                          ios::binary | ios::in); // F:\\VSCode_Projects\\VSCode-C++\\oa\\src\\attendanceData.dat
    if (!loadUserFile.fail()) {
        do {
            loadUserFile.read((char *) temp->Uid, sizeof(temp->Uid));
            loadUserFile.read((char *) &temp->method, sizeof(bool));
            loadUserFile.read((char *) temp->attendanceTime, sizeof(temp->attendanceTime));
            loadUserFile.read((char *) temp->backTime, sizeof(temp->backTime));
            loadUserFile.read((char *) &temp->simpleTime, sizeof(temp->simpleTime));
            if (!strlen(temp->Uid)) //解决loadUserFile.eof()会多操作一次的问题
            {
                flag = 1;
                last->next = nullptr;
            }
            auto *next = new attendance();
            if (!flag)
                last = temp;
            temp->next = next;
            temp = temp->next;
        } while (!loadUserFile.eof());
    } else {
        //cout << "fault,code:fun_146." << endl;
        delete[] temp;
        temp = nullptr;
        ahead = nullptr;
        loadUserFile.close();
        return ahead;
    }
    loadUserFile.close();
    return last;
}

string attendance::getMethod() const {
    if (!method)
        return "管理员补签";
    return "自行签到";
}

/*
bool cheakToday(attendance* target)//判断当天用户是否签到，传入目标用户考勤链表指针
{

}*/
void userAttendance(attendance *target)//签到函数
{
    target->takeAttendance(nowTime(), simpleTime(), true);
}

void userBack(attendance *target)//签退函数
{
    target->takeBack(nowTime());
}

void adminAttendance(attendance *target)//补签函数
{
    target->takeAttendance(nowTime(), simpleTime(), false);
}

void printAttendance(userAccount *head, attendance *ahead)//用于管理员菜单05
{
    while (ahead && head) {
        cout << "[UID：" << head->uid << "] " << head->uName() << " " << head->search_Situation() << " 签到状态->";
        ahead->printAdminInf(simpleTime());
        cout << endl;
        head = head->next;
        ahead = ahead->next;
    }
}

void addAttendance(char *newUid, attendance *last)//注册新用户后，务必调用此函数，且调用此函数后需调用saveAttendance()
{
    auto *nextPtr = new attendance();
    strcpy(nextPtr->Uid, newUid);
    last->next = nextPtr;
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
void deleteAttendance(attendance *ahead, char *uid)//注销用户后，调用此函数，且调用此函数后需调用saveAttendance()
{
    attendance *temp = ahead;
    while (ahead)//定位目标UID
    {
        if (ahead->cheakUid(uid))
            break;
        temp = ahead;//记录前一节的地址
        ahead = ahead->next;
    }
    temp->next = ahead->next;
}