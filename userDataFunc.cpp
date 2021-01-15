#include "userAccount.h"
#include "attendanceFunc.cpp"
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
void randUID(int gourp,char* newUid);
void userDataChange(userAccount *head, userAccount *point);
bool userMenuChoice1(userAccount *head, userAccount* point);
bool userMenuChoice2(attendance *ahead, attendance *apoint);
bool cheakUidInAdvance(userAccount *head, char *id); //无重复UID返回TRUE
void deleteUser(userAccount* head, attendance* ahead, char* uid);
bool adminMenuChoice2(userAccount *head, attendance *ahead);
bool adminMenuChoice5(userAccount *head, attendance *ahead, attendance *apoint);
void adminUserDataChange(userAccount *head, userAccount *target);
bool adminMenuChoice(userAccount *head, attendance* ahead, userAccount *point); //返回true表示对链表进行了修改，false代表直接返回
bool userMenuChoice(userAccount* head, attendance* ahead, userAccount* point)//返回true表示对链表进行了修改，false代表直接返回
{
    cin.clear();
    cin.sync();
    int ch = 0;
    cin >> ch;
    while(!(ch >= 1 && ch <= 4))
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    attendance *apoint = searchUserAttendance(ahead, point->uid);
    switch(ch)
    {
        case 1:
            if(userMenuChoice1(head, point))
                return true;
            else
                return false;
            break;
        case 2:
            if(userMenuChoice2(ahead, apoint))
                return true;
            else
                return false;
            break;
        case 3:
            return false;
            break;
        default:
            return false;
            break;
    }
}
bool adminMenuChoice(userAccount *head, attendance* ahead, userAccount *point)
{
    cin.clear();
    cin.sync();
    int ch = 0;
    cin >> ch;
    attendance *apoint = searchUserAttendance(ahead, point->uid);
    while(!(ch >= 1 && ch <= 5))
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    switch(ch)
    {
        case 1:
            if(userMenuChoice1(head, point))
                return true;
            else
                return false;
            break;
        case 2:
            if(adminMenuChoice2(head, ahead))
                return true;
            else
                return false;
            break;
        case 3:
            return false;
            break;
        case 4:
            return false;
        default:
            if(adminMenuChoice5(head, ahead, apoint))
                return true;
            else
                return false;
            break;
    }
}
bool userMenuChoice1(userAccount* head, userAccount* point)
{
    system("CLS");
    cin.clear();
    cin.sync();
    point->welcomeUser();
    cout << "你的个人信息如下：" << endl;
    point->print_userInf();
    int uch = 0;
    cout << endl;
    cout << "输入1 更改个人信息，输入2 返回个人菜单：" << endl;
    cin >> uch;
    while(uch != 1 && uch != 2)
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> uch;
    }
    if(uch == 1)
    {
        userDataChange(head, point);
        cout << "信息更新成功，返回用户菜单。" << endl;
        system("pause");
        system("CLS");
        return true;
    }
    else{
        system("CLS");
        return false;
    }
}
bool userMenuChoice2(attendance* ahead, attendance* apoint)
{
    int ch = 0;
    system("CLS");
    cout << "当前用户考勤信息：" << endl;
    cout << endl;
    apoint->printInf(simpleTime());//输出当日考勤信息，清空过期信息
    cout << endl;
    cout << "输入1 签到，输入2 签退，输入3 返回个人菜单：" << endl;
    cin >> ch;
    if(ch == 1)
    {
        if(strlen(apoint->getAttendanceTime()))
        {
            cout << "请勿重复签到。" << endl;
            system("pause");
            system("CLS");
            return false;
        }
        else{
            userAttendance(apoint);
            cout << "签到成功，签到时间：" << nowTime() << endl;
            system("pause");
            system("CLS");
            return true;
        }
    }
    else if(ch == 2)
    {
        if(!strcmp(apoint->getSimpleTime(),simpleTime()))//判断当天是否签到->签退流程
        {
            if(strlen(apoint->getBackTime()))
            {
                cout << "请勿重复签退。" << endl;
                system("pause");
                system("CLS");
                return false;
            }
            else{
                userBack(apoint);
                cout << "签退成功，签退时间：" << nowTime() << endl;
                system("pause");
                system("CLS");
                return true;
            }
        }else{
            cout << "请签到后再进行签退。" << endl;
            system("pause");
            system("CLS");
            return false;
        }
    }
    else{
        system("CLS");
        return false;
    }
}
bool adminMenuChoice2(userAccount *head, attendance *ahead)
{
    cin.clear();
    cin.sync();
    int ch = 0;
    char uid[12] = "\0";
    userAccount *target = nullptr;
    target = head;
    system("CLS");
    cout << "用户列表（UID 姓名 职位）：" << endl;
    while(target)
    {
        target->print_userInfSimple();
        target = target->next;
    }
    cout << endl;
    cout << "输入1 编辑用户信息，输入2 注销用户账号，输入3 返回管理员菜单：" << endl;
    cin >> ch;
    while(ch!=1 && ch!=2 && ch!=3)
    {
        cout << "选项不存在，请重新输入：";
        cin >> ch;
    }
    if(ch == 1)
    {//developer:GitHub_CSTHenry(zhengke@bytecho.net)
        cout << "请输入需要编辑的用户UID：" << endl;
    recin:
        cin >> uid;
        target = head;
        while(target)//将target指向需要编辑的用户对象
        {
            if(target->cheakUid(uid))
                break;
            target = target->next;
        }
        if(target)
            adminUserDataChange(head, target);
        else
        {
            cout << "UID：" << uid << "不存在，请重新输入：" << endl;
            goto recin;
        }
        return true;
    }
    else if(ch == 2)
    {
        cout << "请输入需要注销的用户UID（谨慎操作）：" << endl;
    recin2:
        cin >> uid;
        target = head;
        while(target)//将target指向需要编辑的用户对象
        {
            if(target->cheakUid(uid))
                break;
            target = target->next;
        }
        if(target)
            deleteUser(head, ahead, uid);
        else
        {
            cout << "UID：" << uid << "不存在，请重新输入：" << endl;
            goto recin2;
        }
        return true;
    }
    else{
        system("CLS");
        return false;
    }
}
bool adminMenuChoice5(userAccount *head, attendance *ahead, attendance *apoint)
{
    cin.clear();
    cin.sync();
    int ch = 0;
    char Uid[12] = "\0";
    attendance *aTarget = ahead;
    system("CLS");
    cout << "用户考勤信息如下：" << endl;
    cout << endl;
    printAttendance(head, ahead);//输出用户当日考勤信息，并清空过期信息
    cout << endl;
    cout << "输入1 补签用户，输入2 管理员签到，输入3 管理员签退，输入4 返回管理员菜单：" << endl;
    cin >> ch;
    while(ch<1 && ch>4)
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    switch(ch)
    {
        case 1:
            cout << "请输入需要补签的用户UID：" << endl;
        recin:
            cin >> Uid;
            while(aTarget)//将target指向需要编辑的用户对象
            {
                if(aTarget->cheakUid(Uid))
                    break;
                aTarget = aTarget->next;
            }
            if(aTarget)
            {
                if(!strlen(aTarget->getAttendanceTime()))
                {
                    adminAttendance(aTarget);
                    cout << "UID：" << Uid << "补签完成，补签时间：" << nowTime() << endl;
                }else{
                    cout << "UID：" << Uid << "已签到，无需补签" << endl;
                    return false;
                }
                system("pause");
                system("CLS");
            }
            else{
                cout << "UID：" << Uid << "不存在，请重新输入：" << endl;
                goto recin;
            }
            return true;
            break;
        case 2:
            if(strlen(apoint->getAttendanceTime()))
            {
                cout << "请勿重复签到。" << endl;
                system("pause");
                system("CLS");
                return false;
            }
            else{
                userAttendance(apoint);
                cout << "签到成功，签到时间：" << nowTime() << endl;
                system("pause");
                system("CLS");
                return true;
            }
            break;
        case 3:
            if(!strcmp(apoint->getSimpleTime(),simpleTime()))//判断当天是否签到->签退流程
            {
                if(strlen(apoint->getBackTime()))
                {
                    cout << "请勿重复签退。" << endl;
                    system("pause");
                    system("CLS");
                    return false;
                }
                else{
                    userBack(apoint);
                    cout << "签退成功，签退时间：" << nowTime() << endl;
                    system("pause");
                    system("CLS");
                    return true;
                    }
            }else{
                cout << "请签到后再进行签退。" << endl;
                system("pause");
                system("CLS");
                return false;
            }
            break;
            default:
                system("CLS");
                return false;
                break;
    }
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
void userDataChange(userAccount* head, userAccount* point)
{
    cin.clear();
    cin.sync();
    int ch = 0;
    char newName[10] = "\0", newPass[17] = "\0", newUid[12] = "\0";
    system("CLS");
    if(point->search_Situation() == "管理员")
    {
        cout << "信息修改（输入1 修改姓名，输入2 修改密码，输入3 修改UID）：" << endl;
        cin >> ch;
        while(ch!=1 && ch!=2 && ch!=3)
        {
            cout << "选项不存在，请重新输入：" << endl;
            cin >> ch;
        }
        if(ch == 3)
        {
            cout << "请注意：UID为登录凭证，请谨慎修改，修改完成后需使用新UID登录。" << endl;
            cout << "请输入新的UID（字母或数字）：" << endl;
            cin >> newUid;
            /*
            while (!(newUid >= 1000 && newUid <= 1000000000))
            {
                cout << "UID越界，请重新输入：" << endl;
                cin >> newUid;
            }*/
            while (!cheakUidInAdvance(head, newUid))
            {
                cout << "当前UID：" << newUid << " 已存在，请重新输入：" << endl;
                cin >> newUid;
            }
            strcpy(point->uid, newUid);
        }
        else
            goto userModel;
    }
    else{
    cout << "输入1 修改姓名，输入2修改密码：" << endl;
    cin >> ch;
userModel:
    while(ch!=1 && ch!=2)
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    if(ch == 1)
    {
    cout << "请输入新的姓名（UID及用户组请联系管理员修改）：" << endl;
    cin >> newName;
    point->nameChange(newName);
    }
    else{
        cout << "请输入新的密码（<=16位）：" << endl;
        cin >> newPass;
        point->passChange(newPass);
    }
    }
    cout << "修改成功，返回用户菜单。" << endl;
    system("pause");
    system("CLS");
}
void adminUserDataChange(userAccount *head, userAccount *target)
{
    cin.clear();
    cin.sync();
    int ch = 0, newgroup = -1;
    char newName[10] = "\0", newPass[17] = "\0", newUid[12] = "\0";
    system("CLS");
    cout << "正在被编辑的用户信息如下：" << endl;
    cout << endl;
    target->print_userInfSimple();
    cout << endl;
    cout << "输入1 修改UID（谨慎操作），输入2 修改姓名，输入3 修改用户组（职位），输入4 修改密码（谨慎操作）：" << endl;
    cin >> ch;
    while(ch<1 && ch>4)
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    switch(ch)
    {//developer:GitHub_CSTHenry(zhengke@bytecho.net)
        case 1:
            system("CLS");
            cout << "请注意：UID为登录凭证，请谨慎修改，修改完成后需使用新UID登录。" << endl;
            cout <<"正在更改 "<<target->uName()<<" 的UID，"<< "请输入新的UID：" << endl;
            cin >> newUid;
            /*
            while (!(newUid >= 1000 && newUid <= 1000000000))
            {
                cout << "UID越界，请重新输入：" << endl;
                cin >> newUid;
            }*/
            while (!cheakUidInAdvance(head, newUid))
            {
                cout << "当前UID：" << newUid << " 已存在，请重新输入：" << endl;
                cin >> newUid;
            }
            strcpy(target->uid, newUid);
            break;
        case 2:
            system("CLS");
            cout << "正在更改 " << target->uName() << " 的姓名，请输入新的姓名：" << endl;
            cin >> newName;
            target->nameChange(newName);
            break;
        case 3:
            system("CLS");
            cout << "正在更改 " << target->uName() << " 的用户组，"
                 << "当前用户组为：" << target->getGroup() << endl;
            cout << "请输入新的用户组编号：1.管理员（谨慎选择） 2.开发者 3.普通用户：" << endl;
            cin >> newgroup;
            while(newgroup<1 && newgroup>3)
            {
                cout << "该用户组编号不存在，请重新输入：" << endl;
                cin >> newgroup;
            }
            target->groupChange(newgroup);
            break;
        default:
            system("CLS");
            cout << "正在更改 " << target->uName() << " 的密码，密码为登录凭证之一，请谨慎操作。" << endl;
            cout << "请输入新的密码（<=16位）：" << endl;
            cin >> newPass;
            target->passChange(newPass);
            break;
    }
    cout << "编辑用户信息成功，返回管理员菜单。" << endl;
    system("pause");
    system("CLS");
}
void randUID(int group, char *newUid)//随机UID，管理员前缀为A，普通用户为U，开发者为D
{
    const char *strtoC = nullptr;
    char ufrontUID[12] = "U";
    char afrontUID[12] = "A";
    char dfrontUID[12] = "D";
    string behindUID = "\0";
    long int randNum = 0;
    srand((int)time(0));
    randNum = 1000000 + rand() % 100000000;
    behindUID = to_string(randNum);
    strtoC = behindUID.c_str();
    if(group == 1)
    {
        strcat(afrontUID, strtoC);
        strcpy(newUid, afrontUID);
    }
    else if (group == 3)
    {
        strcat(ufrontUID, strtoC);
        strcpy(newUid, ufrontUID);
    }
    else
    {
        strcat(dfrontUID, strtoC);
        strcpy(newUid, dfrontUID);
    }
}
void deleteUser(userAccount* head, attendance* ahead, char* uid)
{
    userAccount *flag = nullptr;
    userAccount* target = head;
    while(target)//将target指向需要编辑的用户对象
    {
        if(target->cheakUid(uid))
            break;
        flag = target;//记录目标之前的指针
        target = target->next;
    }
    flag->next = target->next;
    deleteAttendance(ahead, uid);//同时删除考勤表中相应数据
    ahead->savaAttendance(ahead);//保存
    cout << "账号 [UID:" << uid << "] 注销成功。" << endl;
    system("pause");
    system("CLS");
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
bool cheakUidInAdvance(userAccount *head, char *id) //无重复UID返回TRUE
{
    bool flag = true;
    do
    {
        if (head->cheakUid(id))
            flag = false;
        head = head->next;
    } while (head);
    if (flag)
        return true;
    return false;
}