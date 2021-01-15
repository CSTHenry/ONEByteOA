#ifndef _APPROVE_H_
#define _APPROVE_H_

#include <cstring>
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
class approve
{
    public:
        char Uid[12] = "\0";
        approve *next = nullptr;
        char *reContent()//获取申请内容，可能为\0，取决于审批流程
        {
            return content;
        }
        char *reTitle()
        {
            return title;
        }
        char *reApply()
        {
            return apply;
        }
        char *reReply()
        {
            return reply;
        }
        void cheakStatu()//获取审核状态
        {
            if(statu)
            {
                if(flag)
                    cout << "审核通过" << endl;
                else
                    cout << "审核终止" << endl;
            }
            else
                cout << "审核中" << endl;
        }
        void printStatu()
        {
            cout << "项目：" << title << " 审核状态：";
            cheakStatu();
            cout << endl;
        }

    private:
        bool statu = false;//审批状态：f审核中，t审核完成，用于个人菜单
        bool flag = false;//审核结果：t通过，f拒绝，用于个人菜单
        char content[200] = "\0";//申请内容（可选）
        char title[50] = "\0";//流程名
        char apply[200] = "\0";//申请理由
        char reply[101] = "\0";//审核意见
};

#endif