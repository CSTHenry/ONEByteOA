/***********************************************/
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
//2021-01
//VSCode(GCC)
/***********************************************/
#include "src\userAccount.h"
#include "src\mainFunc.cpp"
#include <iostream>

using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
int main()
{
menu:
    int choice = 0;
    printMenu();
    cin >> choice;
    while(!(choice == 1 || choice == 2))
    {
        cout << "选项不存在，请重新输入：" << endl;
        cin >> choice;
    }
    switch (choice)
    {
    case 1:
        system("CLS");
        mainSignup();
        //cout << "注册完成，返回主菜单。" << endl;
        goto menu;
        break;
    case 2:
        system("CLS");
        if(!(bool)mainLogin())
            goto menu;
        break;
    default:
        system("CLS");
        break;
    }
    /*userAccount *head = new userAccount();
    head->loadUserData(head);
    cout << "数据表测试工具" << endl;
    cout << endl;
    if(!(bool)head)
        return 0;
    while(head)
    {
        head->print_userInf();
        head = head->next;
    }
    system("pause");*/
    return 0;
}