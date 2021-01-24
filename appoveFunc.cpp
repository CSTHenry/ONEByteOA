#include "approve.h"
#include "approveList.h"
#include <fstream>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
//approveList* createList(char *title, char *tips, bool content);
approveList *appSearch(approveList *aphead, int num);
void addList(approveList *aphead, char *title, char *tips, bool content);
void editApprove(approve *uahead, char *uid);
void saveApprove(approve *uahead);
void saveList(approveList *aphead);
bool deleteList(approveList *aphead, int listNumber);
bool getList(approve *uahead);
void adminApprove(approve *target);
void approveAdmin();
bool getUserApprove(userAccount *point, approve *uahead);
void addList(approveList *aphead, char *title, char *tips, bool content)
{
    approveList *head = aphead;
    if (aphead->listNum == 0)
    {
        aphead->listNum += 1;
        aphead->setTitle(title);
        aphead->setTips(tips);
        aphead->isContent = content;
    }
    else
    {
        while (aphead->next)
            aphead = aphead->next; //转换为链表尾地址
        auto *nextPtr = new approveList();
        nextPtr->listNum = aphead->listNum + 1;
        nextPtr->setTitle(title);
        nextPtr->setTips(tips);
        nextPtr->isContent = content;
        aphead->next = nextPtr;
    }
    saveList(head);
}
void editList(approveList *aphead)
{
    int num = 0;
    char title[21] = "\0", tips[100] = "\0", ch = '\0';
    approveList *target = nullptr;
    cout << "请输入需要编辑的流程编号：" << endl;
    cin >> num;
    while (!(target = appSearch(aphead, num))) {
        cout << "未检索到对于的流程，请重新输入：" << endl;
        cin >> num;
    }
    cout << "正在编辑 [" << target->listNum << " " << target->reListTitle() << "] 流程" << endl;
    cout << "请输入审批项目名称（10字以内）：" << endl;
    cin >> title;
    target->setTitle(title);
    cout << "请输入审批提示/指引（50字以内）：" << endl;
    cin >> tips;
    target->setTips(tips);
    cout << "是否开启审批内容选填框（y开启n关闭）？" << endl;
    cin >> ch;
    if (ch == 'y' || ch == 'Y')
        target->isContent = true;
    else
        target->isContent = false;
    cout << "编辑流程完成。" << endl;
    saveList(aphead);
}
void saveList(approveList *aphead) //./src/appList.dat
{
    ofstream saveList("./src/appList.dat", ios::binary | ios::out); //F:\\VSCode_Projects\\VSCode-C++\\oa\\src\\appList.dat
    while (aphead)
    {
        saveList.write((char *)&aphead->listNum, sizeof(aphead->listNum));
        saveList.write((char *)aphead->reListTitle(), 21);
        saveList.write((char *)aphead->reListTips(), 100);
        saveList.write((char *)&aphead->isContent, sizeof(aphead->isContent));
        aphead = aphead->next;
    }
    saveList.close();
}
approveList *approveList::loadList(approveList *aphead)
{
    //approveList *aphead = new approveList();
    approveList *next = nullptr, *temp = nullptr;
    ifstream loadList("./src/appList.dat", ios::binary | ios::in);
    if (!loadList.fail())
    {
        while (!loadList.eof())
        {
            loadList.read((char *)&aphead->listNum, sizeof(aphead->listNum));
            loadList.read((char *)aphead->listTitle, sizeof(aphead->listTitle));
            loadList.read((char *)aphead->listTips, sizeof(aphead->listTips));
            loadList.read((char *)&aphead->isContent, sizeof(aphead->isContent));
            if (!aphead->listNum)
            {
                delete[] aphead;
                aphead = temp;
                aphead->next = nullptr;
                break;
            } else {
                next = new approveList();
                aphead->next = next;
                temp = aphead;
                aphead = next;
            }
        }
        loadList.close();
        return aphead; //尾指针
    } else {
        loadList.close();
        return nullptr; //打开文件失败返回nullptr
    }
}

bool deleteList(approveList *aphead, int listNumber) //返回flase即未找到相应流程
{
    approveList *temp = nullptr;
    while (aphead) {
        if (listNumber == aphead->listNum)
            break;
        temp = aphead;
        aphead = aphead->next;
    }
    if (!aphead)
        return false;
    else
    {
        temp->next = aphead->next;
        return true;
    }
}
approveList *appSearch(approveList *aphead, int num)
{
    while (aphead)
    {
        if (num == aphead->listNum)
            break;
        aphead = aphead->next;
    }
    return aphead;
}
void addApprove(approve *uahead, approveList *aphead, char *uid, char *uName, char *apply, char *content)
{
    approve *head = uahead;
    if (uahead->listNum == 0)
    {
        uahead->listNum = aphead->listNum;
        strcpy(uahead->Uid, uid);
        strcpy(uahead->name, uName);
        uahead->setTitle(aphead->reListTitle());
        uahead->setContent(content);
        uahead->setApply(apply);
    }
    else {
        while (uahead->next)
            uahead = uahead->next; //转换为链表尾地址
        auto *nextPtr = new approve();
        nextPtr->listNum = aphead->listNum;
        strcpy(nextPtr->Uid, uid);
        strcpy(nextPtr->name, uName);
        nextPtr->setTitle(aphead->reListTitle());
        nextPtr->setContent(content);
        nextPtr->setApply(apply);
        uahead->next = nextPtr;
    }
    saveApprove(head);
}
void addSimpleApp(approve *uahead, char *uid, char *uName, char *apply, char *content)
{
    approve *head = uahead;
    if (uahead->listNum == 0)
    {
        uahead->listNum = 1024;//管理员申请表编号默认为1024
        strcpy(uahead->Uid, uid);
        strcpy(uahead->name, uName);
        uahead->setTitle("主页管理员申请");
        uahead->setContent(content);
        uahead->setApply(apply);
    }
    else {
        while (uahead->next)
            uahead = uahead->next; //转换为链表尾地址
        auto *nextPtr = new approve();
        nextPtr->listNum = 1024;
        strcpy(nextPtr->Uid, uid);
        strcpy(nextPtr->name, uName);
        nextPtr->setTitle("主页管理员申请");
        nextPtr->setContent(content);
        nextPtr->setApply(apply);
        uahead->next = nextPtr;
    }
    saveApprove(head);
}
void saveApprove(approve *uahead) //./src/userApprove.dat
{
    ofstream saveList("./src/userApprove.dat", ios::binary | ios::out); //F:\\VSCode_Projects\\VSCode-C++\\oa\\src\\userApprove.dat
    while (uahead)
    {
        saveList.write((char *)uahead->Uid, sizeof(uahead->Uid));
        saveList.write((char *)uahead->name, sizeof(uahead->name));
        saveList.write((char *)&uahead->listNum, sizeof(uahead->listNum));
        saveList.write((char *)&uahead->statu, sizeof(uahead->statu));
        saveList.write((char *)&uahead->flag, sizeof(uahead->flag));
        saveList.write((char *)uahead->reContent(), 200);
        saveList.write((char *)uahead->reTitle(), 21);
        saveList.write((char *)uahead->reApply(), 200);
        saveList.write((char *)uahead->reReply(), 101);
        uahead = uahead->next;
    }
    saveList.close();
}
approve *approve::loadList(approve *uahead)
{
    approve *next = nullptr, *temp = nullptr;
    ifstream loadList("./src/userApprove.dat", ios::binary | ios::in);
    if (!loadList.fail())
    {
        while (!loadList.eof())
        {
            loadList.read((char *)uahead->Uid, sizeof(uahead->Uid));
            loadList.read((char *)uahead->name, sizeof(uahead->name));
            loadList.read((char *)&uahead->listNum, sizeof(uahead->listNum));
            loadList.read((char *)&uahead->statu, sizeof(uahead->statu));
            loadList.read((char *)&uahead->flag, sizeof(uahead->flag));
            loadList.read((char *)uahead->content, sizeof(uahead->content));
            loadList.read((char *)uahead->title, sizeof(uahead->title));
            loadList.read((char *)uahead->apply, sizeof(uahead->apply));
            loadList.read((char *)uahead->reply, sizeof(uahead->reply));
            if (!uahead->listNum)
            {
                delete[] uahead;
                uahead = temp;
                uahead->next = nullptr;
                break;
            } else {
                next = new approve();
                uahead->next = next;
                temp = uahead;
                uahead = next;
            }
        }
        loadList.close();
        return uahead; //尾指针
    }
    else {
        loadList.close();
        return nullptr; //打开文件失败返回nullptr
    }
}
bool getList(approve *uahead)
{
    bool flag = false;
    system("CLS");
    cout << "等待的审批队列：" << endl;
    cout << endl;
    while (uahead)
    {
        if (!uahead->listNum)
            break;
        if (uahead->statu)
        {
            uahead = uahead->next;
            continue;
        }
        cout << "[" << uahead->Uid << "] " << uahead->name << "->"
             << "[" << uahead->listNum << "]" << uahead->reTitle() << "\t"
             << "未审核" << endl;
        flag = true;
        uahead = uahead->next;
    }
    if (!flag)
    {
        cout << "暂无需要处理的申请。" << endl;
        return false;
    }
    return true;
}
void adminApprove(approve *target)
{
    char ch = 'n', re[101] = "\0";
    cout << endl;
    system("CLS");
    cout << "正在审核 [" << target->name << " " << target->reTitle() << "]" << endl;
    cout << endl;
    if (strlen(target->reContent()))
    {
        cout << "申请内容：" << endl;
        cout << target->reContent() << endl;
    }
    cout << endl;
    cout << "申请理由：" << target->reApply() << endl;
    cout << endl;
    cout << "输入y 通过审核，输入n 拒绝通过：" << endl;
    cin >> ch;
    if (ch == 'Y' || ch == 'y')
        target->flag = true;
    cout << "请输入审核意见（50字以内）：" << endl;
    cin >> re;
    target->setReply(re);
    target->statu = true;
    //if(target->listNum == 1024)
}
bool getUserApprove(userAccount *point, approve *uahead)
{
    system("CLS");
    int num = 0;
    bool flag = false;
    char ch = 'n';
    cout << "当前审批进度：" << endl;
    cout << endl;
    approve *temp = uahead;
    while (uahead)
    {
        if (!strcmp(point->uid, uahead->Uid))
        {
            flag = true;
            cout << "[申请项]-> "
                 << "[" << uahead->listNum << "]" << uahead->reTitle() << "\t"
                 << " [审核状态]-> ";
            uahead->cheakStatu();
        }
        uahead = uahead->next;
    }
    if (!flag)
    {
        cout << endl;
        cout << "暂无创建的申请，返回用户菜单" << endl;
        system("pause");
        system("CLS");
        return false;
    }
    uahead = temp;
    cout << endl;
    cout << "输入y 查看审核详情，输入z 编辑并重新提交未审核或驳回的申请，输入n 返回用户菜单：" << endl;
    cin >> ch;
    if (!(ch == 'y' || ch == 'Y' || ch == 'z' || ch == 'Z'))
    {
        system("CLS");
        return false;
    }
    if (ch == 'z' || ch == 'Z')
    {
        return true;
    }
    cout << "输入对于申请编号查看审核详情：" << endl;
re:
    cin >> num;
    uahead = temp;
    while (uahead)
    {
        if (!strcmp(point->uid, uahead->Uid) && num == uahead->listNum)
            break;
        uahead = uahead->next;
    }
    if (!uahead)
    {
        cout << "此编号对应的申请项不存在，请重新输入：" << endl;
        goto re;
    }
    system("CLS");
    cout << "正在查看 [" << uahead->listNum << "]" << uahead->reTitle() << " 审核详情：" << endl;
    cout << endl;
    cout << "[审核结果]->";
    uahead->cheakStatu();
    if (strlen(uahead->reContent()))
    {
        cout << endl;
        cout << "[申请内容]" << endl;
        cout << uahead->reContent() << endl;
    }
    cout << endl;
    cout << "[申请理由]" << endl;
    cout << uahead->reApply() << endl;
    cout << endl;
    cout << "[审核意见]" << endl;
    if (uahead->statu)
        cout << uahead->reReply() << endl;
    else
        cout << "暂无审核意见。" << endl;
    cout << endl;
    system("pause");
    system("CLS");
    return false;
}
void editApprove(approve *uahead, char *uid)
{
    int num;
    approve *head = uahead;
    char content[200] = "\0", apply[200] = "\0";
    cout << "请输入待审核或被驳回的申请编号以进行编辑：" << endl;
re:
    uahead = head;
    cin >> num;
    while (uahead)
    {
        if (!strcmp(uahead->Uid, uid) && uahead->listNum == num && (!uahead->statu || !uahead->flag))
        {
            system("CLS");
            cout << "正在编辑申请 [" << uahead->reTitle() << "] :" << endl;
            if (strlen(uahead->reContent()))
            {
                cout << "请输入申请内容（100字以内）：" << endl;
                cin >> content;
            }
            cout << "请输入申请理由（100字以内）：" << endl;
            cin >> apply;
            cout << "编辑成功，审批进度请返回用户菜单查看。" << endl;
            uahead->setContent(content);
            uahead->setApply(apply);
            uahead->statu = false;
            saveApprove(head);
            system("pause");
            system("CLS");
            return;
        }
        else
            uahead = uahead->next;
    }
    if (!uahead)
    {
        cout << "输入的申请编号不存在或不满足编辑要求，请重新输入：" << endl;
        goto re;
    }
}
void approveAdmin()
{

}
