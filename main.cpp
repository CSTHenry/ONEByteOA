/***********************************************/
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
//2021-01
//VSCode(GCC)
/***********************************************/
#include "mainFunc.cpp"
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
        goto menu;
    case 2:
        system("CLS");
        if(!(bool)mainLogin())
            goto menu;
        break;
    default:
        system("CLS");
        break;
    }
    return 0;
}