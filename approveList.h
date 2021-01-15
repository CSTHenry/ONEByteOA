#ifndef _APPROVELIST_H_
#define _APPROVELIST_H_

#include <cstring>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)

class approveList
{
    public:
        approveList* loadList(approveList* aphead);
        approveList *next = nullptr;
        int listNum = 0;//审批流程编号
        bool isContent = false;//是否开启申请内容填写？
        char *reListTips()
        {
            return listTips;
        }
        char* reListTitle()
        {
            return listTitle;
        }
        void setTitle(char *title)
        {
            strcpy(listTitle, title);
        }
        void setTips(char *tips)
        {
            strcpy(listTips, tips);
        }

    private:
        char listTips[100] = "\0";//申请提示
        char listTitle[21] = "\0";//流程名
};

#endif