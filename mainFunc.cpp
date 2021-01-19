#include "userAccount.h"
#include "userDataFunc.cpp"
#include <iostream>

using namespace std;

//developer:GitHub_CSTHenry(zhengke@bytecho.net)
userAccount *searchUid(userAccount *head, char *id);

bool rememberUid(char *uid, char rememberCh);

void printMenu() {
    cout << "        办公自动化 V1.3       " << endl;
    cout << "===============================" << endl;
    cout << "=         01.账户注册         =" << endl;
    cout << "=        02.管理员申请        =" << endl;
    cout << "=         03.账户登录         =" << endl;
    cout << "===============================" << endl;
    cout << "请输入选项编号进行选择：" << endl;
}

[[noreturn]] void print_usersMenu(userAccount *head, attendance *ahead, char *uid) {
    userAccount *point;
    point = searchUid(head, uid);
    uHead:
    point->welcomeUser();
    cout << "当前时间：" << nowTime() << endl;
    cout << endl;
    cout << "             用户菜单            " << endl;
    cout << "================================" << endl;
    cout << "=          01.个人信息         =" << endl;
    cout << "=          02.考勤系统         =" << endl;
    cout << "=          03.发起申请         =" << endl;
    cout << "=        04.查看审核进度       =" << endl;
    cout << "================================" << endl;
    cout << "请输入选项编号进行选择：" << endl;
    if (userMenuChoice(head, ahead, point)) {
        userAccount::saveUserData(head);
        ahead->savaAttendance(ahead);
        goto uHead;
    } else
        goto uHead;
}

[[noreturn]] void print_adminMenu(userAccount *head, attendance *ahead, char *uid);

void userAccount::print_userInf() {
    cout << "    当前用户信息" << endl;
    cout << "====================" << endl;
    cout << "-> UID：" << uid << endl;
    cout << "-> 姓名：" << name << endl;
    cout << "-> 职位：" << search_Situation() << endl;
    cout << "====================" << endl;
}

userAccount *search_userInf(userAccount *p, char *id)//返回对应UID的指针
{//developer:GitHub_CSTHenry(zhengke@bytecho.net)
    userAccount *head = nullptr;
    do {
        if (!strcmp(p->uid, id)) {
            p = p->next;
        } else {
            head = p;
            break;
        }
    } while (p != nullptr);
    return head;
}

void
addUser(userAccount *head, attendance *ahead, userAccount *last, attendance *alast) //需要传入数据库首尾链表的地址,调用loadUserData()后使用
{
    int newGroup;
    char pass[17] = "\0", newName[10] = "\0", newUID[12] = "\0";
    auto *newP = new userAccount();
    cout << "     请按照提示创建账户" << endl;//已更改为随机生成UID
    cout << "===========================" << endl;
    cout << "= 请输入用户组编号(2-3)： =" << endl;
    cout << "========自动生成UID========" << endl;
    cin >> newGroup;
    if (newGroup != 2 && newGroup != 3) {
        cout << "该用户组不存在，请重新输入：" << endl;
        cin >> newGroup;
    }
    randUID(newGroup, newUID);
    while (!cheakUidInAdvance(head, newUID))
        randUID(newGroup, newUID);
    cout << "[ UID创建完成（妥善保管）：" << newUID << " ]" << endl;
    cout << endl;
    cout << "===========================" << endl;
    cout << "= 请输入姓名：            =" << endl;
    cout << "===========================" << endl;
    cin >> newName;
    cout << "===========================" << endl;
    cout << "= 请创建密码（<=16位）：  =" << endl;
    cout << "===========================" << endl;
    cin >> pass;
    newP->signUp(newUID, newName, pass, newGroup);
    last->next = newP;
    userAccount::saveUserData(head); //保存整个链表
    addAttendance(newUID, alast);//更新考勤表
    ahead->savaAttendance(ahead);
}

userAccount *createUser() //创建userData数据表，用于初始化程序，并返回链表头指针
{
    int newGroup = 1;
    char pass[17] = "\0", newName[10] = "\0", newUID[12] = "\0";
    auto *newP = new userAccount();//已更新为随机生成UID
    randUID(newGroup, newUID);
    cout << "    请按照提示创建管理员账户" << endl;
    cout << endl;
    cout << "[ UID创建完成（妥善保管）：" << newUID << " ]" << endl;
    cout << endl;
    cout << "=========UID自动生成=========" << endl;//UID自动生成
    cout << "= 请输入姓名：              =" << endl;
    cout << "=============================" << endl;
    cin >> newName;
    cout << "=============================" << endl;
    cout << "= 请创建密码（<=16位）：    =" << endl;
    cout << "=============================" << endl;
    cin >> pass;
    newP->signUp(newUID, newName, pass, newGroup);
    newP->next = nullptr;
    userAccount::saveUserData(newP); //保存整个链表
    return newP;
}

void userAccount::saveUserData(userAccount *head) //保存内存中的整个链表,并覆盖文件./src/userData.dat
{
    ofstream openUserFile("./src/userData.dat", ios::binary | ios::out);
    if (!openUserFile.fail()) {
        do {
            openUserFile.write((const char *) head->uid, sizeof(head->uid));
            openUserFile.write((const char *) head->name, sizeof(head->name));
            openUserFile.write((const char *) head->password, sizeof(head->password));
            openUserFile.write((const char *) &head->group, sizeof(head->group));
            head = head->next;
        } while (head);
    } else {
        cout << "fault,code:fun_184." << endl;
        return;
    }
    openUserFile.close();
}

userAccount *userAccount::loadUserData(userAccount *head) //将返回链表尾指针，即其next==nullptr，若数据表不存在，返回空指针
{
    //userAccount *head = new userAccount();
    int flag = 0;
    userAccount *temp = head;
    userAccount *last = nullptr;
    ifstream loadUserFile("./src/userData.dat",
                          ios::binary | ios::in); // F:\\VSCode_Projects\\VSCode-C++\\github_oa\\src\\userData.dat
    if (!loadUserFile.fail()) {
        do {
            loadUserFile.read((char *) temp->uid, sizeof(temp->uid));
            loadUserFile.read((char *) temp->name, sizeof(head->name));
            loadUserFile.read((char *) temp->password, sizeof(head->password));
            loadUserFile.read((char *) &temp->group, sizeof(int));
            if (!temp->group) //解决loadUserFile.eof()会多操作一次的问题
            {
                flag = 1;
                last->next = nullptr;
            }
            auto *next = new userAccount();
            if (!flag)
                last = temp;
            temp->next = next;
            temp = temp->next;
        } while (!loadUserFile.eof());
    }//developer:GitHub_CSTHenry(zhengke@bytecho.net)
    else {
        //cout << "fault,code:fun_146." << endl;
        delete[] temp;
        head = nullptr;
        loadUserFile.close();
        return head;
    }
    loadUserFile.close();
    return last;
}

void mainSignup() //主页面注册函数，可以判断userData
{
    auto *ahead = new attendance();
    auto *head = new userAccount();
    userAccount *last = nullptr;
    attendance *alast = nullptr;
    alast = ahead->loadData(ahead);
    if (!(last = head->loadUserData(head))) //载入当前数据库
    {
        cout << "请创建管理员账户（用户组编号：1）：" << endl;
        createUser();
    } else
        addUser(head, ahead, last, alast);
    system("CLS");
    cout << "注册完成！返回主菜单。" << endl;
    cout << endl;
    delete head;
}

void adminSignup() //主页面注册函数，可以判断userData
{
    auto *ahead = new attendance();
    auto *head = new userAccount();
    userAccount *last = nullptr;
    attendance *alast = nullptr;
    alast = ahead->loadData(ahead);
    if (!(last = head->loadUserData(head))) //载入当前数据库
    {
        cout << "请创建管理员账户（用户组编号：1）：" << endl;
        createUser();
    } else
        addUser(head, ahead, last, alast);
    system("CLS");
    cout << "注册完成！返回主菜单。" << endl;
    cout << endl;
    delete head;
}

bool mainLogin() //登录函数，登录失败返回false，登录成功返回true
{
    auto *head = new userAccount();
    auto *ahead = new attendance();
    userAccount *target = head;
    attendance *aTarget = ahead;
    char loginID[12];//全局变量
    int flag = 0;
    char pass[17], rememberCh = 'n';
    if (!target->loadUserData(target)) //载入当前数据库
    {
        cout << "未检索到数据表，请先注册账户。" << endl;
        cout << endl;
        return false;
    } else {
        cout << " 登录系统，请依次输入UID和密码" << endl;
        if (!rememberUid(loginID, 'n')) {
            cout << "===============================" << endl;
            cout << "= 请输入UID：                 =" << endl;
            cout << "===============================" << endl;
            cin >> loginID;
        } else {
            cout << " 删除uidInf.ini以登录其他账户" << endl;
            cout << "=========已获取你的UID=========" << endl;
            cout << "-> " << loginID << endl;
            cout << "===============================" << endl;
            cout << endl;
        }
        cout << "===============================" << endl;
        cout << "= 请输入密码（<=16位）：      =" << endl;
        cout << "===============================" << endl;
        cin >> pass;
        if (!rememberUid(loginID, 'n')) {
            cout << "提示：是(y)否(n)保存当前UID [" << loginID << "] 以便下次登录？" << endl;
            cout << "输入 \"y\" 或者 \"n\" " << endl;
            cin >> rememberCh;
            rememberUid(loginID, rememberCh);
            if (rememberCh == 'y') {
                cout << "UID保存完成，下次登录自动获取。" << endl;
                cout << "如需要登录其他账户，请删除根目录下的uidInf.ini。" << endl;
                cout << endl;
                system("pause");
            }
        }
        while (target) {
            if (target->cheakUid(loginID) && target->logIn(pass)) {
                flag = 1;
                break;
            }
            target = target->next;
        }
        if (!flag) {
            system("CLS");
            cout << "UID不存在或密码不匹配，返回主菜单" << endl;
            cout << endl;
            return false;
        } else {
            if (!aTarget->loadData(aTarget))//初始化考勤系统,ahead为考勤表头指针
            {
                ahead = aTarget = userToData(head);
                aTarget->savaAttendance(aTarget);
                ahead->loadData(ahead);
            } else {
                ahead = new attendance();
                ahead->loadData(ahead);//返回尾指针，处理头指针
            }
            switch (target->getGroup()) {
                case 0:
                    system("CLS");
                    cout << "UID不存在或密码不匹配，返回主菜单" << endl;
                    cout << endl;
                    return false;
                case 1:
                    system("CLS");
                    print_adminMenu(head, ahead, loginID);
                default:
                    system("CLS");
                    print_usersMenu(head, ahead, loginID);
            }
        }
    }
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
userAccount *searchUid(userAccount *head, char *id) //搜索uid，并返回指针指向相应uid
{
    //userAccount *head = new userAccount();
    userAccount *point = nullptr;
    head->loadUserData(head);
    while (head) {
        if (head->cheakUid(id))
            point = head;
        head = head->next;
    }
    return point;
}

void userAccount::print_userInfSimple() {
    if (search_Situation() != "管理员")
        cout << "UID: " << uid << "\t" << name << "\t" << search_Situation() << endl;
}

bool rememberUid(char *uid, char rememberCh) {
    int i;
    char loadUID[32] = "\0";
    ifstream loadUidInf("uidInf.ini", ios::in);
    if (loadUidInf.fail()) {
        if (rememberCh != 'y')
            return false;
        else {
            ofstream savaUidInf("uidInf.ini", ios::out | ios::trunc);
            savaUidInf << "UID = " << uid;
            savaUidInf.close();
        }
    } else {
        loadUidInf.getline(loadUID, sizeof(loadUID));
        for (i = 6; loadUID[i]; i++) {
            loadUID[i - 6] = loadUID[i];
            loadUID[i] = '\0';
        }
        strcpy(uid, loadUID);
        return true;
    }
    loadUidInf.close();
    return false;
}

[[noreturn]] void print_adminMenu(userAccount *head, attendance *ahead, char *uid) {
    userAccount *point;
    point = searchUid(head, uid);
    aHead:
    point->welcomeUser();
    cout << "当前时间：" << nowTime() << endl;
    cout << endl;
    cout << "           管理员菜单           " << endl;
    cout << "===============================" << endl;
    cout << "=         01.个人信息         =" << endl;
    cout << "=         02.用户管理         =" << endl;
    cout << "=    03.创建与管理审批流程    =" << endl;
    cout << "=       04.查看审批队列       =" << endl;
    cout << "=       05.用户考勤状态       =" << endl;
    cout << "===============================" << endl;
    cout << "请输入选项编号进行选择：" << endl;
    if (adminMenuChoice(head, ahead, point)) {
        head->saveUserData(head);
        ahead->savaAttendance(ahead);
        goto aHead;
    } else
        goto aHead;
}