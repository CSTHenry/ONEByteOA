#include "userAccount.h"
#include "approve.h"
#include "approveList.h"
#include <fstream>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
approveList* createList(char *title, char *tips, bool content);
void addList(approveList *aphead, char *title, char *tips, bool content);
void saveList(approveList *aphead);
bool deleteList(approveList *aphead, int listNumber);

approveList* createList(char *title, char *tips, bool content)//创建审批流程
{
    approveList *aphead = new approveList();
    if(aphead->listNum >= 1)
        addList(aphead, title, tips, content);
    else
    {
        ofstream saveList("./src/appList.dat", ios::binary | ios::out);
        saveList.write((char *)title, sizeof(title));
        saveList.write((char *)tips, sizeof(tips));
        saveList.write((char *)&content, sizeof(content));
    }
    saveList(aphead);
}
void addList(approveList *aphead, char *title, char *tips, bool content)
{
    approveList *head = aphead, *temp = nullptr;
    while(aphead->netx)
    {
        temp = aphead;//前一个链
        aphead = aphead->netx;//转换为链表尾地址
    }
    aphead->listNum = temp->listNum + 1;
    aphead->setTitle(title);
    aphead->setTips(tips);
    aphead->isContent = content;
    saveList(head);
}
void saveList(approveList *aphead)
{
    ofstream saveList("./src/appList.dat", ios::binary | ios::out);
    while(aphead->netx)
    {
        saveList.write((char *)aphead->reListTitle(), sizeof(aphead->reListTitle()));
        saveList.write((char *)aphead->reListTips(), sizeof(aphead->reListTips()));
        saveList.write((char *)&aphead->isContent, sizeof(aphead->isContent));
    }
}
approveList* approveList::loadList(approveList* aphead)
{
    //approveList *aphead = new approveList();
    approveList *next = nullptr, *temp = nullptr;
    ifstream loadList("./src/appList.dat", ios::binary | ios::in);
    if(!loadList.fail())
    {
        while(!loadList.eof())
        {
            loadList.read((char *)aphead->listTitle, sizeof(aphead->listTitle));
            loadList.read((char *)aphead->listTips, sizeof(aphead->listTips));
            loadList.read((char *)aphead->isContent, sizeof(aphead->isContent));
            if(aphead->listNum)
            {
                delete[] aphead;
                aphead = temp;
                aphead->netx = nullptr;
                break;
            }else
            {
                approveList *next = new approveList();
                aphead->netx = next;
                temp = aphead;
                aphead = next;
            }
        }
        return aphead;//尾指针
    }
    else
    {
        return nullptr;//打开文件失败返回nullptr
    }

}
bool deleteList(approveList *aphead, int listNumber)//返回flase即未找到相应流程
{
    approveList *temp = nullptr;
    while(aphead)
    {
        if(listNumber == aphead->listNum)
            break;
        temp = aphead;
        aphead = aphead->netx;
    }
    if(aphead)
        return false;
    else{
        temp->netx = aphead->netx;
        return true;
    }
}