#ifndef _APPROVE_H_
#define _APPROVE_H_

#include <cstring>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
class approve {
public:
    char Uid[12] = "\0", name[10] = "\0";
    int listNum = 0;
    approve *next = nullptr;
    bool statu = false;//审批状态：f审核中，t审核完成，用于个人菜单
    bool flag = false;//审核结果：t通过，f拒绝，用于个人菜单
    static approve *loadList(approve *uahead);

    bool getUserapp(char *uid, int num) {
        if (!strcmp(Uid, uid) && num == listNum)
            return true;
        else
            return false;
    }

    char *reContent()//获取申请内容，可能为\0，取决于审批流程
    {
        return content;
    }

    char *reApply() {
        return apply;
    }
        char *reReply()
        {
            return reply;
        }
        char *reTitle()
        {
            return title;
        }
        void setContent(char *con)
        {
            strcpy(content, con);
        }
        void setTitle(char* tit)
        {
            strcpy(title, tit);
        }
        void setApply(char *app)
        {
            strcpy(apply, app);
        }

    void setReply(char *re) {
        strcpy(reply, re);
    }

    void cheakStatu() const//获取审核状态
    {
        if (statu) {
            if (flag)
                cout << "通 过" << endl;
            else
                cout << "不通过" << endl;
        } else
            cout << "审核中" << endl;
    }

    void printStatu() {
        cout << "项目：" << title << " 审核状态：";
        cheakStatu();
        cout << endl;
    }

private:
    char content[200] = "\0";//申请内容（可选）
    char title[21] = "\0";//流程名
    char apply[200] = "\0";//申请理由
    char reply[101] = "\0";//审核意见
};

#endif