#ifndef _USERACCOUNT_H_
#define _USERACCOUNT_H_
#include <cstring>
#include <iostream>
using namespace std;
//developer:GitHub_CSTHenry(zhengke@bytecho.net)
class userAccount {
public:
    char uid[12]{};
    userAccount *next = nullptr;

    void welcomeUser() {
        cout << "欢迎 " << search_Situation() << " " << name << " (UID: " << uid << ")：" << endl;
    }

    static void saveUserData(userAccount *head);

    [[nodiscard]] int getGroup() const {
        return group;
    }

    static userAccount *loadUserData(userAccount *head);

    void signUp(char *I, char *N, char *P, int G) {
        strcpy(uid, I);
        strcpy(name, N);
        strcpy(password, P);
        group = G;
        }

    bool logIn(char *pass) {
        bool key;
        !strcmp(pass, password) ? key = true : key = false;
        if (key)
            return true;
        return false;
    }

    bool cheakUid(char *id)//有重复ID返回真
    {
        bool key;
        !strcmp(uid, id) ? key = true : key = false;
        if (key)
            return true;
        return false;
    }

    char *uName() {
        return name;
        }
        void print_userInf();
        void print_userInfSimple();

    [[nodiscard]] string search_Situation() const {
        switch (group) {
            case 1:
                return "管理员";
            case 2:
                return "开发者";
            case 3:
                return "普通用户";
            default:
                    return "error,code:searchSituation Not return.";
            }
        }
        void nameChange(char* newName)
        {
            strcpy(name, newName);
        }

    void passChange(char *newPass) {
        strcpy(password, newPass);
    }

    void groupChange(int newGroup) {
        group = newGroup;
    }

private:
    char name[10]{};
    char password[17]{};
    int group = 0;
};
#endif