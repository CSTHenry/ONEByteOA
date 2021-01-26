#include "appoveFunc.cpp"
#include "attendanceFunc.cpp"
#include <ctime>
#include <random>
#include <iostream>

using namespace std;

//developer:GitHub_CSTHenry(zhengke@bytecho.net)
void randUID(int group, char *newUid);

void userDataChange(userAccount *head, userAccount *point);

bool userMenuChoice1(userAccount *head, userAccount *point);

bool userMenuChoice2(attendance *apoint);

bool userMenuChoice3(userAccount *head, userAccount *point);

bool userMenuChoice4(userAccount *point);

bool cheakUidInAdvance(userAccount *head, char *id); //无重复UID返回TRUE
void deleteUser(userAccount *head, attendance *ahead, char *uid);

bool adminMenuChoice2(userAccount *head, attendance *ahead, userAccount *point);

bool adminMenuChoice3();

bool adminMenuChoice4(userAccount *head, attendance *ahead);

bool adminMenuChoice5(userAccount *head, attendance *ahead, attendance *apoint);

void adminUserDataChange(userAccount *head, userAccount *target);

bool adminMenuChoice(userAccount *head, attendance *ahead, userAccount *point); //返回true表示对链表进行了修改，false代表直接返回
void uidApprove(userAccount *head, userAccount *target);

bool deleteApp(userAccount *head, userAccount *target);

bool userMenuChoice(userAccount *head, attendance *ahead, userAccount *point)   //返回true表示对链表进行了修改，false代表直接返回
{
    cin.clear();
    cin.sync();
    int ch = 0;
    cin >> ch;
    while (!(ch >= 1 && ch <= 4)) {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    attendance *apoint = searchUserAttendance(ahead, point->uid);
    switch (ch) {
        case 1:
            if (userMenuChoice1(head, point))
                return true;
            else
                return false;
        case 2:
            if (userMenuChoice2(apoint))
                return true;
            else
                return false;
        case 3:
            if (userMenuChoice3(head, point))
                return true;
            else
                return false;
        default:
            if (userMenuChoice4(point))
                return true;
            else
                return false;
    }
}

bool adminMenuChoice(userAccount *head, attendance *ahead, userAccount *point) {
    cin.clear();
    cin.sync();
    int ch = 0;
    cin >> ch;
    attendance *apoint = searchUserAttendance(ahead, point->uid);
    while (!(ch >= 1 && ch <= 5)) {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    switch (ch) {
        case 1:
            if (userMenuChoice1(head, point))
                return true;
            else
                return false;
        case 2:
            if (adminMenuChoice2(head, ahead, point))
                return true;
            else
                return false;
        case 3:
            if (adminMenuChoice3())
                return true;
            else
                return false;
        case 4:
            if (adminMenuChoice4(head, ahead))
                return true;
            else
                return false;
        default:
            if (adminMenuChoice5(head, ahead, apoint))
                return true;
            else
                return false;
    }
}

bool userMenuChoice1(userAccount *head, userAccount *point) {
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
    while (uch != 1 && uch != 2) {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> uch;
    }
    if (uch == 1) {
        userDataChange(head, point);
        cout << "信息更新成功，返回用户菜单。" << endl;
        system("pause");
        system("CLS");
        return true;
    } else {
        system("CLS");
        return false;
    }
}

bool userMenuChoice2(attendance *apoint) {
    int ch = 0;
    system("CLS");
    cout << "当前用户考勤信息：" << endl;
    cout << endl;
    apoint->printInf(simpleTime()); //输出当日考勤信息，清空过期信息
    cout << endl;
    cout << "输入1 签到，输入2 签退，输入3 返回个人菜单：" << endl;
    cin >> ch;
    if (ch == 1) {
        if (strlen(apoint->getAttendanceTime())) {
            cout << "请勿重复签到。" << endl;
            system("pause");
            system("CLS");
            return false;
        } else {
            userAttendance(apoint);
            cout << "签到成功，签到时间：" << nowTime() << endl;
            system("pause");
            system("CLS");
            return true;
        }
    } else if (ch == 2) {
        if (!strcmp(apoint->getSimpleTime(), simpleTime())) //判断当天是否签到->签退流程
        {
            if (strlen(apoint->getBackTime())) {
                cout << "请勿重复签退。" << endl;
                system("pause");
                system("CLS");
                return false;
            } else {
                userBack(apoint);
                cout << "签退成功，签退时间：" << nowTime() << endl;
                system("pause");
                system("CLS");
                return true;
            }
        } else {
            cout << "请签到后再进行签退。" << endl;
            system("pause");
            system("CLS");
            return false;
        }
    } else {
        system("CLS");
        return false;
    }
}

bool userMenuChoice3(userAccount *head, userAccount *point) {
    system("CLS");
    char ch = 'n';
    int num = 0;
    char content[200] = "\0", apply[200] = "\0";
    auto *aphead = new approveList();
    auto *uahead = new approve();
    approve *cheak = uahead;
    approveList *temp = aphead;
    approve::loadList(uahead);
    approveList::loadList(aphead);
    cout << "可用申请项目：" << endl;
    cout << endl;
    cout << "[编号]"
         << "\t"
         << "[审批项目]"
         << "\t"
         << "[是否填写申请内容]" << endl;
    cout << left << "1025" << "\t" << setw(16) << "申请新UID" << "const" << endl;
    cout << left << "1026" << "\t" << setw(16) << "申请注销账户" << "const" << endl;
    while (aphead) {
        cout << right << setw(4) << setfill('0') << aphead->listNum << "\t" << setw(16) << setfill(' ') << left
             << aphead->reListTitle();
        if (aphead->isContent)
            cout << "是" << endl;
        else
            cout << "否" << endl;
        aphead = aphead->next;
    }
    aphead = temp;
    cout << endl;
    cout << "输入y 创建新的申请，输入n 返回用户菜单：" << endl;
    cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        cout << "请输入需要申请的项目编号：" << endl;
        re:
        cin >> num;
        while (aphead) {
            if (num == aphead->listNum)
                break;
            aphead = aphead->next;
        }
        if (num == 1025) {
            while (cheak->next) {
                if (cheak->listNum == num && !strcmp(cheak->Uid, point->uid) && (!cheak->statu || !cheak->flag)) {
                    cout << "请勿重复申请同一个未审核的项目，请联系管理员及时审批；" << endl;
                    cout << "审核未通过的项目请前往审核进度页面编辑后重新提交。" << endl;
                    system("pause");
                    system("CLS");
                    return false;
                }
                cheak = cheak->next;
            }
            uidApprove(head, point);
            cout << "申请成功，申请进度请返回用户菜单查看。" << endl;
            system("pause");
            system("CLS");
            return true;
        }
        if (num == 1026) {
            while (cheak->next) {
                if (cheak->listNum == num && !strcmp(cheak->Uid, point->uid) && (!cheak->statu || !cheak->flag)) {
                    cout << "请勿重复申请同一个未审核的项目，请联系管理员及时审批；" << endl;
                    cout << "审核未通过的项目请前往审核进度页面编辑后重新提交。" << endl;
                    system("pause");
                    system("CLS");
                    return false;
                }
                cheak = cheak->next;
            }
            if (deleteApp(head, point)) {
                cout << "申请成功，申请进度请返回用户菜单查看。" << endl;
                system("pause");
                system("CLS");
            }
            return true;
        }
        while (!aphead) {
            cout << "该编号不存在，请重新输入：" << endl;
            goto re;
        }
        while (cheak) {
            if (cheak->listNum == num && !strcmp(cheak->Uid, point->uid) && (!cheak->statu || !cheak->flag)) {
                cout << "请勿重复申请同一个未审核的项目，请联系管理员及时审批；" << endl;
                cout << "审核未通过的项目请前往审核进度页面编辑后重新提交。" << endl;
                system("pause");
                system("CLS");
                return false;
            }
            cheak = cheak->next;
        }
        system("CLS");
        cout << "正在申请 [" << aphead->reListTitle() << "] :" << endl;
        cout << "申请说明：" << aphead->reListTips() << endl;
        if (aphead->isContent) {
            cout << "请输入申请内容（100字以内）：" << endl;
            cin >> content;
        }
        cout << "请输入申请理由（100字以内）：" << endl;
        cin >> apply;
        addApprove(uahead, aphead, point->uid, point->uName(), apply, content);
        cout << "申请成功，申请进度请返回用户菜单查看。" << endl;
        system("pause");
        system("CLS");
    } else {
        system("CLS");
        return false;
    }
    return true;
}

bool userMenuChoice4(userAccount *point) {
    auto *uahead = new approve();
    approve::loadList(uahead);
    if (getUserApprove(point, uahead))
        editApprove(uahead, point->uid);
    return true;
}

bool adminMenuChoice2(userAccount *head, attendance *ahead, userAccount *point) {
    re:
    cin.clear();
    cin.sync();
    int ch = 0, flag = 0;
    char uid[12] = "\0";
    userAccount *target = head;
    system("CLS");
    cout << "用户列表（UID 姓名 职位）：" << endl;
    while (target) {
        if (flag)
            target->print_userInfSimple();
        target = target->next;
        flag++;
    }
    cout << endl;
    cout << "输入1 编辑用户信息，输入2 注销用户账号，输入3 返回管理员菜单：" << endl;
    cin >> ch;
    while (ch != 1 && ch != 2 && ch != 3) {
        cout << "选项不存在，请重新输入：";
        cin >> ch;
    }
    if (ch == 1) { //developer:GitHub_CSTHenry(zhengke@bytecho.net)
        cout << "请输入需要编辑的用户UID：" << endl;
        recin:
        cin >> uid;
        target = head;
        while (target) //将target指向需要编辑的用户对象
        {
            if (target->cheakUid(uid))
                break;
            target = target->next;
        }
        if (target)
            adminUserDataChange(head, target);
        else {
            cout << "UID：" << uid << "不存在，请重新输入：" << endl;
            goto recin;
        }
        return true;
    } else if (ch == 2) {
        cout << "请输入需要注销的用户UID（谨慎操作）：" << endl;
        recin2:
        cin >> uid;
        while (!strcmp(uid, head->uid)) {
            cout << '\a' << "鉴权失败，禁止注销主管理员账户，请重新输入：" << endl;
            cin >> uid;
        }
        target = head;
        if (!strcmp(uid, point->uid)) {
            cout << "警告：你正在注销自己的管理员账户，注销成功后会清除你的所有数据！" << endl;
            cout << "请输入：\"我已知晓并确认注销我的管理员账户\"以完成注销：" << endl;
            string entrance;
            cin >> entrance;
            if (entrance != "我已知晓并确认注销我的管理员账户") {
                cout << "内容验证失败，返回用户管理菜单。" << endl;
                system("pause");
                system("CLS");
                goto re;
            }
        }
        while (target) //将target指向需要编辑的用户对象
        {
            if (target->cheakUid(uid))
                break;
            target = target->next;
        }
        if (target)
            deleteUser(head, ahead, uid);
        else {
            cout << "UID：" << uid << "不存在，请重新输入：" << endl;
            goto recin2;
        }
        return true;
    } else {
        system("CLS");
        return false;
    }
}

bool adminMenuChoice3() //这个函数属实写得垃圾。。。但无所谓
{
    int num = 0;
    auto *aphead = new approveList();
    char title[21] = "\0", tips[100] = "\0";
    char ch = '\0';
    system("CLS");
    if (!aphead->loadList(aphead)) {
        cout << "暂无审批流程，输入y 创建审批流程，输入n 返回管理员菜单：" << endl;
        cin >> ch;
        if (!(ch == 'y' || ch == 'Y')) {
            system("CLS");
            return false;
        } else {
            create:
            cout << "请输入审批项目名称（10字以内）：" << endl;
            cin >> title;
            cout << "请输入审批提示/指引（50字以内）：" << endl;
            cin >> tips;
            cout << "是否开启审批内容选填框（y开启n关闭）？" << endl;
            cin >> ch;
            if (!(ch == 'y' || ch == 'Y'))
                addList(aphead, title, tips, false);
            else
                addList(aphead, title, tips, true);
            cout << "创建流程完成。" << endl;
            system("pause");
            system("CLS");
        }
    } else {
        cout << "已创建的审批流程：" << endl;
        cout << "[编号]"
             << "\t"
             << "[审批项目]"
             << "\t"
             << "[是否填写申请内容]" << endl;
        approveList *temp = aphead;
        while (aphead) {
            cout << aphead->listNum << "\t" << setw(16) << left << aphead->reListTitle();
            if (aphead->isContent)
                cout << "是" << endl;
            else
                cout << "否" << endl;
            aphead = aphead->next;
        }
        aphead = temp;
        cout << endl;
        cout << "输入a 编辑审批流程，输入b 添加审批流程，输入c 删除审批流程，输入d 返回管理员菜单：" << endl;
        cin >> ch;
        switch (ch) {
            case 'a':
                editList(aphead);
                system("pause");
                system("CLS");
                break;
            case 'b':
                goto create;
            case 'c':
                cout << "请输入需要删除的流程编号：" << endl;
                cin >> num;
                while (num == 1) {
                    cout << "程序不允许删除第一个审批流程，请重新输入：" << endl;
                    cin >> num;
                }
                while (!appSearch(aphead, num)) {
                    cout << "未检索到对于的流程，请重新输入：" << endl;
                    cin >> num;
                }
                deleteList(aphead, num);
                cout << "删除流程成功。" << endl;
                saveList(aphead);
                system("pause");
                system("CLS");
                break;
            default:
                system("CLS");
                return false;
        }
    }
    return true;
}

bool adminMenuChoice4(userAccount *head, attendance *ahead) {
    int num = 0;
    char targetUid[10] = "\0", ch = 'n';
    auto *uahead = new approve();
    attendance *atarget = ahead;
    approve *Head = uahead;
    approve::loadList(uahead);
    system("CLS");
    cout << "等待的审批队列：" << endl;
    cout << endl;
    string t1 = "[用户UID]", t2 = "[用户姓名]", t3 = "[审批编号及名称]", t4 = "[状态]";
    cout << left << setw(11) << t1 << setw(16) << t2 << setw(27) << t3 << t4 << endl;
    if (!getList(uahead)) {
        system("pause");
        system("CLS");
        return false;
    } else {
        cout << endl;
        cout << "输入y 进行审核，输入n 返回管理员菜单：" << endl;
        cin >> ch;
        if (ch == 'N' || ch == 'n') {
            system("CLS");
            return false;
        }
        cout << "输入用户UID及其对应申请编号进行审核" << endl;
        re:
        uahead = Head;
        cout << "请输入用户UID：" << endl;
        cin >> targetUid;
        cout << "请输入申请编号：" << endl;
        cin >> num;
        while (uahead) {
            if (uahead->getUserapp(targetUid, num))
                break;
            uahead = uahead->next;
        }
        if (!uahead) {
            cout << "未检索到相应申请信息，请重新输入：" << endl;
            goto re;
        }
        while (!atarget->cheakUid(targetUid))//获取相应考勤表
            atarget = atarget->next;
        if (adminApprove(uahead, head)) {
            if (num == 1024) {
                attendance *temp = ahead;
                while (ahead->next)
                    ahead = ahead->next;
                addAttendance(uahead->Uid, ahead);
                attendance::savaAttendance(temp);
            }
            if (num == 1025) {
                strcpy(atarget->Uid, uahead->reContent());
                attendance::savaAttendance(ahead);
            }
            if (num == 1026)
                deleteUser(head, ahead, targetUid);
        }
        cout << endl;
        cout << "审核完成，返回管理员菜单" << endl;
        saveApprove(Head);
        system("pause");
        system("CLS");
    }
    return true;
}

bool adminMenuChoice5(userAccount *head, attendance *ahead, attendance *apoint) {
    cin.clear();
    cin.sync();
    int ch = 0;
    char Uid[12] = "\0";
    attendance *aTarget = ahead;
    system("CLS");
    cout << "用户考勤信息如下：" << endl;
    cout << endl;
    printAttendance(head, ahead); //输出用户当日考勤信息，并清空过期信息
    cout << endl;
    cout << "输入1 补签用户，输入2 管理员签到，输入3 管理员签退，输入4 导出当天考勤表.csv，输入5 返回管理员菜单：" << endl;
    cin >> ch;
    switch (ch) {
        case 1:
            cout << "请输入需要补签的用户UID：" << endl;
        recin:
            cin >> Uid;
            while (aTarget) //将target指向需要编辑的用户对象
            {
                if (aTarget->cheakUid(Uid))
                    break;
                aTarget = aTarget->next;
            }
            if (aTarget) {
                if (!strlen(aTarget->getAttendanceTime())) {
                    adminAttendance(aTarget);
                    cout << "UID：" << Uid << "补签完成，补签时间：" << nowTime() << endl;
                } else {
                    cout << "UID：" << Uid << "已签到，无需补签" << endl;
                    system("pause");
                    system("CLS");
                    return false;
                }
                system("pause");
                system("CLS");
            } else {
                cout << "UID：" << Uid << "不存在，请重新输入：" << endl;
                goto recin;
            }
            return true;
        case 2:
            if (strlen(apoint->getAttendanceTime())) {
                cout << "请勿重复签到。" << endl;
                system("pause");
                system("CLS");
                return false;
            } else {
                userAttendance(apoint);
                cout << "签到成功，签到时间：" << nowTime() << endl;
                system("pause");
                system("CLS");
                return true;
            }
        case 3:
            if (!strcmp(apoint->getSimpleTime(), simpleTime())) //判断当天是否签到->签退流程
            {
                if (strlen(apoint->getBackTime())) {
                    cout << "请勿重复签退。" << endl;
                    system("pause");
                    system("CLS");
                    return false;
                } else {
                    userBack(apoint);
                    cout << "签退成功，签退时间：" << nowTime() << endl;
                    system("pause");
                    system("CLS");
                    return true;
                }
            } else {
                cout << "请签到后再进行签退。" << endl;
                system("pause");
                system("CLS");
                return false;
            }
        case 4: {
            int flag = 1;
            while (ahead && head) {
                ahead->expAdminInf(simpleTime(), head, flag);
                head = head->next;
                ahead = ahead->next;
                flag++;
            }
            cout << "已导出至expAttendance.csv文件。" << endl;
            system("pause");
            system("CLS");
            return false;
        }
        default:
            system("CLS");
            return false;
    }
} //developer:GitHub_CSTHenry(zhengke@bytecho.net)
void userDataChange(userAccount *head, userAccount *point) {
    cin.clear();
    cin.sync();
    int ch = 0;
    char newName[10] = "\0", newPass[17] = "\0";
    system("CLS");
    cout << "输入1 修改姓名，输入2 修改密码：" << endl;
    cin >> ch;
    while (ch != 1 && ch != 2) {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    if (ch == 1) {
        cout << "请输入新的姓名（UID及用户组请联系管理员修改）：" << endl;
        cin >> newName;
        point->nameChange(newName);
    } else {
        cout << "请输入新的密码（<=16位）：" << endl;
        cin >> newPass;
        point->passChange(newPass);
    }
    cout << "修改成功，返回用户菜单。" << endl;
    system("pause");
    system("CLS");
}

void adminUserDataChange(userAccount *head, userAccount *target) {
    cin.clear();
    cin.sync();
    int ch = 0, newgroup = -1;
    char newName[10] = "\0", newPass[17] = "\0";
    system("CLS");
    cout << "正在被编辑的用户信息如下：" << endl;
    cout << endl;
    target->print_userInfSimple();
    cout << endl;
    cout << "输入1 修改姓名，输入2 修改用户组（职位），输入3 修改密码（谨慎操作）：" << endl;
    cin >> ch;
    while (ch < 1 && ch > 3) {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> ch;
    }
    switch (ch) { //developer:GitHub_CSTHenry(zhengke@bytecho.net)
        /*case 1://存在严重风险，已禁用
            system("CLS");
            cout << "请注意：UID为登录凭证，请谨慎修改，修改完成后需使用新UID登录。" << endl;
            cout << "正在更改 " << target->uName() << " 的UID，"
                 << "请输入新的UID：" << endl;
            cin >> newUid;
            while (!cheakUidInAdvance(head, newUid)) {
                cout << "当前UID：" << newUid << " 已存在，请重新输入：" << endl;
                cin >> newUid;
            }
            strcpy(target->uid, newUid);
            break;*/
        case 1:
            system("CLS");
            cout << "正在更改 " << target->uName() << " 的姓名，请输入新的姓名：" << endl;
            cin >> newName;
            target->nameChange(newName);
            break;
        case 2:
            system("CLS");
            cout << "正在更改 " << target->uName() << " 的用户组，"
                 << "当前用户组为：" << target->getGroup() << endl;
            cout << "请输入新的用户组编号：1.管理员（谨慎选择） 2.开发者 3.普通用户：" << endl;
            cin >> newgroup;
            while (newgroup < 1 && newgroup > 3) {
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

void randUID(int group, char *newUid) //随机UID，管理员前缀为A，普通用户为U，开发者为D
{
    const char *strtoC;
    char ufrontUID[12] = "U";
    char afrontUID[12] = "A";
    char dfrontUID[12] = "D";
    string behindUID;
    int randNum;
    int min = 1000000, max = 10000000;
    default_random_engine e(time(nullptr));
    uniform_int_distribution<int> u(min, max);
    randNum = u(e);
    behindUID = to_string(randNum);
    strtoC = behindUID.c_str();
    if (group == 1) {
        strcat(afrontUID, strtoC);
        strcpy(newUid, afrontUID);
    } else if (group == 3) {
        strcat(ufrontUID, strtoC);
        strcpy(newUid, ufrontUID);
    } else {
        strcat(dfrontUID, strtoC);
        strcpy(newUid, dfrontUID);
    }
}

void deleteUser(userAccount *head, attendance *ahead, char *uid) {
    userAccount *flag = nullptr;
    userAccount *target = head;
    while (target) //将target指向需要编辑的用户对象
    {
        if (target->cheakUid(uid))
            break;
        flag = target; //记录目标之前的指针
        target = target->next;
    }
    flag->next = target->next;
    deleteAttendance(ahead, uid); //同时删除考勤表中相应数据
    ahead->savaAttendance(ahead); //保存
    cout << "账号 [UID:" << uid << "] 注销成功。" << endl;
    system("pause");
    system("CLS");
} //developer:GitHub_CSTHenry(zhengke@bytecho.net)
bool cheakUidInAdvance(userAccount *head, char *id) //无重复UID返回TRUE
{
    bool flag = true;
    do {
        if (head->cheakUid(id))
            flag = false;
        head = head->next;
    } while (head);
    if (flag)
        return true;
    return false;
}

void uidApprove(userAccount *head, userAccount *target) //UID申请函数
{
    char ch;
    re:
    system("cls");
    char apply[200] = "\0", newUID[12] = "\0", content[200] = "\0";
    randUID(target->getGroup(), newUID);
    while (!cheakUidInAdvance(head, newUID))
        randUID(target->getGroup(), newUID);
    cout << "    请按照提示申请更改UID" << endl;//已更改为随机生成UID
    cout << "========自动生成UID========" << endl;
    cout << "-> " << newUID << endl;
    cout << "===========================" << endl;
    cout << " -[UID创建完成 妥善保管]-" << endl;
    cout << "输入y 确认申请更改为此UID，输入n 重新生成UID：" << endl;
    cin >> ch;
    if (ch != 'Y' && ch != 'y')
        goto re;
    strcpy(content, newUID);
    cout << endl;
    cout << "请注意，审核通过后，仅能使用新UID [" << newUID << "] 登录，原UID将失效！" << endl;
    cout << "===========================" << endl;
    cout << "= 请输入更改UID的理由：   =" << endl;
    cout << "===========================" << endl;
    cin >> apply;
    auto *uahead = new approve();
    approve::loadList(uahead);
    addStaticApp(uahead, target, 1025, "UID修改申请", apply, content);
    saveApprove(uahead);
}

bool deleteApp(userAccount *head, userAccount *target) {
    char apply[200]{};
    string entrance;
    system("cls");
    cout << "警告：你正在申请注销账户，一旦审核通过，你的所有数据将被清空！" << endl;
    cout << "请输入：\"我已知晓并确认申请注销我的账户\"以进行申请：" << endl;
    cin >> entrance;
    if (entrance != "我已知晓并确认申请注销我的账户") {
        cout << "验证失败，返回主菜单。" << endl;
        system("pause");
        system("CLS");
        return false;
    }
    cout << "请输入注销此账户 [" << target->uid << "] 的理由：" << endl;
    cin >> apply;
    auto *uahead = new approve();
    approve::loadList(uahead);
    addStaticApp(uahead, target, 1026, "账户注销申请", apply, "\0");
    saveApprove(uahead);
    return true;
}