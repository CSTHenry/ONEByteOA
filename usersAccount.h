#ifndef userAccount
#define userAccount
#include <cstring>
#include <string>
using namespace std;
class userAccount
{
    public:
        void signUp(int I,char* N,char* P,int G)
        {
            uid = I;
            strcpy(name, N);
            strcpy(password,P);
            group = G;
        }
        bool logIn(int I,char* password)
        {

        }
        char* uName()
        {
            return name;
        }
        void print_userInf();
        string search_Situation()
        {
            switch(group)
            {
                case 1:
                    return "管理员";
                    break;
                case 2:
                    return "开发者";
                    break;
                case 3:
                    return "普通用户";
                    break;
                default:
                    return 0;
            }
        }
    int uid;
    userAccount *next;
    private:
        char name[10];
        char password[17];
        int group;
};
#endif