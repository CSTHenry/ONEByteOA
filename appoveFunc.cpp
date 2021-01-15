#include "approve.h"
#include "approveList.h"
#include "userAccount.h"
#include <fstream>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
//approveList* createList(char *title, char *tips, bool content);
approveList *appSearch(approveList *aphead, int num);
void addList(approveList *aphead, char *title, char *tips, bool content);
void editList(approveList *target, char *title, char *tips, bool content);
void saveList(approveList *aphead);
bool deleteList(approveList *aphead, int listNumber);

void addList(approveList *aphead, char *title, char *tips, bool content)
{
    approveList *head = aphead;
    if(aphead->listNum == 0)
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
        approveList *nextPtr = new approveList();
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
    char title[21] = "\0", tips[100] = "\0", ch='\0';
    approveList *target = nullptr;
    cout << "请输入需要编辑的流程编号：" << endl;
    cin >> num;
    while (!(target = appSearch(aphead, num)))
    {
        cout << "未检索到对于的流程，请重新输入：" << endl;
        cin >> num;
    }
    cout << "正在编辑 [" << target->listNum << " " << target->reListTitle()<<"] 流程" << endl;
    cout << "请输入审批项目名称（10字以内）：" << endl;
    cin >> title;
    target->setTitle(title);
    cout << "请输入审批提示/指引（50字以内）：" << endl;
    cin >> tips;
    target->setTips(tips);
    cout << "是否开启审批内容选填框（y开启n关闭）？" << endl;
    cin >> ch;
    if (ch == 'y'||ch == 'Y')
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
            }
            else
            {
                next = new approveList();
                aphead->next = next;
                temp = aphead;
                aphead = next;
            }
        }
        loadList.close();
        return aphead; //尾指针
    }
    else
    {
        loadList.close();
        return nullptr; //打开文件失败返回nullptr
    }
}
bool deleteList(approveList *aphead, int listNumber) //返回flase即未找到相应流程
{
    approveList *temp = nullptr;
    while (aphead)
    {
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