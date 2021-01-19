#include "userAccount.h"
#include "userDataFunc.cpp"
#include <iostream>

using namespace std;

//developer:GitHub_CSTHenry(zhengke@bytecho.net)
userAccount *searchUid(userAccount *head, char *id);

bool rememberUid(char *uid, char rememberCh);

void printMenu() {
    cout << "        �칫�Զ��� V1.3       " << endl;
    cout << "===============================" << endl;
    cout << "=         01.�˻�ע��         =" << endl;
    cout << "=        02.����Ա����        =" << endl;
    cout << "=         03.�˻���¼         =" << endl;
    cout << "===============================" << endl;
    cout << "������ѡ���Ž���ѡ��" << endl;
}

[[noreturn]] void print_usersMenu(userAccount *head, attendance *ahead, char *uid) {
    userAccount *point;
    point = searchUid(head, uid);
    uHead:
    point->welcomeUser();
    cout << "��ǰʱ�䣺" << nowTime() << endl;
    cout << endl;
    cout << "             �û��˵�            " << endl;
    cout << "================================" << endl;
    cout << "=          01.������Ϣ         =" << endl;
    cout << "=          02.����ϵͳ         =" << endl;
    cout << "=          03.��������         =" << endl;
    cout << "=        04.�鿴��˽���       =" << endl;
    cout << "================================" << endl;
    cout << "������ѡ���Ž���ѡ��" << endl;
    if (userMenuChoice(head, ahead, point)) {
        userAccount::saveUserData(head);
        ahead->savaAttendance(ahead);
        goto uHead;
    } else
        goto uHead;
}

[[noreturn]] void print_adminMenu(userAccount *head, attendance *ahead, char *uid);

void userAccount::print_userInf() {
    cout << "    ��ǰ�û���Ϣ" << endl;
    cout << "====================" << endl;
    cout << "-> UID��" << uid << endl;
    cout << "-> ������" << name << endl;
    cout << "-> ְλ��" << search_Situation() << endl;
    cout << "====================" << endl;
}

userAccount *search_userInf(userAccount *p, char *id)//���ض�ӦUID��ָ��
{//developer:GitHub_CSTHenry(zhengke@bytecho.net)
    userAccount *head = nullptr;
    do {
        if (!strcmp(p->uid, id)) {
            p = p->next;
        } else {
            head = p;
            break;
        }
    } while (p != nullptr);
    return head;
}

void
addUser(userAccount *head, attendance *ahead, userAccount *last, attendance *alast) //��Ҫ�������ݿ���β����ĵ�ַ,����loadUserData()��ʹ��
{
    int newGroup;
    char pass[17] = "\0", newName[10] = "\0", newUID[12] = "\0";
    auto *newP = new userAccount();
    cout << "     �밴����ʾ�����˻�" << endl;//�Ѹ���Ϊ�������UID
    cout << "===========================" << endl;
    cout << "= �������û�����(2-3)�� =" << endl;
    cout << "========�Զ�����UID========" << endl;
    cin >> newGroup;
    if (newGroup != 2 && newGroup != 3) {
        cout << "���û��鲻���ڣ����������룺" << endl;
        cin >> newGroup;
    }
    randUID(newGroup, newUID);
    while (!cheakUidInAdvance(head, newUID))
        randUID(newGroup, newUID);
    cout << "[ UID������ɣ����Ʊ��ܣ���" << newUID << " ]" << endl;
    cout << endl;
    cout << "===========================" << endl;
    cout << "= ������������            =" << endl;
    cout << "===========================" << endl;
    cin >> newName;
    cout << "===========================" << endl;
    cout << "= �봴�����루<=16λ����  =" << endl;
    cout << "===========================" << endl;
    cin >> pass;
    newP->signUp(newUID, newName, pass, newGroup);
    last->next = newP;
    userAccount::saveUserData(head); //������������
    addAttendance(newUID, alast);//���¿��ڱ�
    ahead->savaAttendance(ahead);
}

userAccount *createUser() //����userData���ݱ����ڳ�ʼ�����򣬲���������ͷָ��
{
    int newGroup = 1;
    char pass[17] = "\0", newName[10] = "\0", newUID[12] = "\0";
    auto *newP = new userAccount();//�Ѹ���Ϊ�������UID
    randUID(newGroup, newUID);
    cout << "    �밴����ʾ��������Ա�˻�" << endl;
    cout << endl;
    cout << "[ UID������ɣ����Ʊ��ܣ���" << newUID << " ]" << endl;
    cout << endl;
    cout << "=========UID�Զ�����=========" << endl;//UID�Զ�����
    cout << "= ������������              =" << endl;
    cout << "=============================" << endl;
    cin >> newName;
    cout << "=============================" << endl;
    cout << "= �봴�����루<=16λ����    =" << endl;
    cout << "=============================" << endl;
    cin >> pass;
    newP->signUp(newUID, newName, pass, newGroup);
    newP->next = nullptr;
    userAccount::saveUserData(newP); //������������
    return newP;
}

void userAccount::saveUserData(userAccount *head) //�����ڴ��е���������,�������ļ�./src/userData.dat
{
    ofstream openUserFile("./src/userData.dat", ios::binary | ios::out);
    if (!openUserFile.fail()) {
        do {
            openUserFile.write((const char *) head->uid, sizeof(head->uid));
            openUserFile.write((const char *) head->name, sizeof(head->name));
            openUserFile.write((const char *) head->password, sizeof(head->password));
            openUserFile.write((const char *) &head->group, sizeof(head->group));
            head = head->next;
        } while (head);
    } else {
        cout << "fault,code:fun_184." << endl;
        return;
    }
    openUserFile.close();
}

userAccount *userAccount::loadUserData(userAccount *head) //����������βָ�룬����next==nullptr�������ݱ����ڣ����ؿ�ָ��
{
    //userAccount *head = new userAccount();
    int flag = 0;
    userAccount *temp = head;
    userAccount *last = nullptr;
    ifstream loadUserFile("./src/userData.dat",
                          ios::binary | ios::in); // F:\\VSCode_Projects\\VSCode-C++\\github_oa\\src\\userData.dat
    if (!loadUserFile.fail()) {
        do {
            loadUserFile.read((char *) temp->uid, sizeof(temp->uid));
            loadUserFile.read((char *) temp->name, sizeof(head->name));
            loadUserFile.read((char *) temp->password, sizeof(head->password));
            loadUserFile.read((char *) &temp->group, sizeof(int));
            if (!temp->group) //���loadUserFile.eof()������һ�ε�����
            {
                flag = 1;
                last->next = nullptr;
            }
            auto *next = new userAccount();
            if (!flag)
                last = temp;
            temp->next = next;
            temp = temp->next;
        } while (!loadUserFile.eof());
    }//developer:GitHub_CSTHenry(zhengke@bytecho.net)
    else {
        //cout << "fault,code:fun_146." << endl;
        delete[] temp;
        head = nullptr;
        loadUserFile.close();
        return head;
    }
    loadUserFile.close();
    return last;
}

void mainSignup() //��ҳ��ע�ắ���������ж�userData
{
    auto *ahead = new attendance();
    auto *head = new userAccount();
    userAccount *last = nullptr;
    attendance *alast = nullptr;
    alast = ahead->loadData(ahead);
    if (!(last = head->loadUserData(head))) //���뵱ǰ���ݿ�
    {
        cout << "�봴������Ա�˻����û����ţ�1����" << endl;
        createUser();
    } else
        addUser(head, ahead, last, alast);
    system("CLS");
    cout << "ע����ɣ��������˵���" << endl;
    cout << endl;
    delete head;
}

void adminSignup() //��ҳ��ע�ắ���������ж�userData
{
    auto *ahead = new attendance();
    auto *head = new userAccount();
    userAccount *last = nullptr;
    attendance *alast = nullptr;
    alast = ahead->loadData(ahead);
    if (!(last = head->loadUserData(head))) //���뵱ǰ���ݿ�
    {
        cout << "�봴������Ա�˻����û����ţ�1����" << endl;
        createUser();
    } else
        addUser(head, ahead, last, alast);
    system("CLS");
    cout << "ע����ɣ��������˵���" << endl;
    cout << endl;
    delete head;
}

bool mainLogin() //��¼��������¼ʧ�ܷ���false����¼�ɹ�����true
{
    auto *head = new userAccount();
    auto *ahead = new attendance();
    userAccount *target = head;
    attendance *aTarget = ahead;
    char loginID[12];//ȫ�ֱ���
    int flag = 0;
    char pass[17], rememberCh = 'n';
    if (!target->loadUserData(target)) //���뵱ǰ���ݿ�
    {
        cout << "δ���������ݱ�����ע���˻���" << endl;
        cout << endl;
        return false;
    } else {
        cout << " ��¼ϵͳ������������UID������" << endl;
        if (!rememberUid(loginID, 'n')) {
            cout << "===============================" << endl;
            cout << "= ������UID��                 =" << endl;
            cout << "===============================" << endl;
            cin >> loginID;
        } else {
            cout << " ɾ��uidInf.ini�Ե�¼�����˻�" << endl;
            cout << "=========�ѻ�ȡ���UID=========" << endl;
            cout << "-> " << loginID << endl;
            cout << "===============================" << endl;
            cout << endl;
        }
        cout << "===============================" << endl;
        cout << "= ���������루<=16λ����      =" << endl;
        cout << "===============================" << endl;
        cin >> pass;
        if (!rememberUid(loginID, 'n')) {
            cout << "��ʾ����(y)��(n)���浱ǰUID [" << loginID << "] �Ա��´ε�¼��" << endl;
            cout << "���� \"y\" ���� \"n\" " << endl;
            cin >> rememberCh;
            rememberUid(loginID, rememberCh);
            if (rememberCh == 'y') {
                cout << "UID������ɣ��´ε�¼�Զ���ȡ��" << endl;
                cout << "����Ҫ��¼�����˻�����ɾ����Ŀ¼�µ�uidInf.ini��" << endl;
                cout << endl;
                system("pause");
            }
        }
        while (target) {
            if (target->cheakUid(loginID) && target->logIn(pass)) {
                flag = 1;
                break;
            }
            target = target->next;
        }
        if (!flag) {
            system("CLS");
            cout << "UID�����ڻ����벻ƥ�䣬�������˵�" << endl;
            cout << endl;
            return false;
        } else {
            if (!aTarget->loadData(aTarget))//��ʼ������ϵͳ,aheadΪ���ڱ�ͷָ��
            {
                ahead = aTarget = userToData(head);
                aTarget->savaAttendance(aTarget);
                ahead->loadData(ahead);
            } else {
                ahead = new attendance();
                ahead->loadData(ahead);//����βָ�룬����ͷָ��
            }
            switch (target->getGroup()) {
                case 0:
                    system("CLS");
                    cout << "UID�����ڻ����벻ƥ�䣬�������˵�" << endl;
                    cout << endl;
                    return false;
                case 1:
                    system("CLS");
                    print_adminMenu(head, ahead, loginID);
                default:
                    system("CLS");
                    print_usersMenu(head, ahead, loginID);
            }
        }
    }
}//developer:GitHub_CSTHenry(zhengke@bytecho.net)
userAccount *searchUid(userAccount *head, char *id) //����uid��������ָ��ָ����Ӧuid
{
    //userAccount *head = new userAccount();
    userAccount *point = nullptr;
    head->loadUserData(head);
    while (head) {
        if (head->cheakUid(id))
            point = head;
        head = head->next;
    }
    return point;
}

void userAccount::print_userInfSimple() {
    if (search_Situation() != "����Ա")
        cout << "UID: " << uid << "\t" << name << "\t" << search_Situation() << endl;
}

bool rememberUid(char *uid, char rememberCh) {
    int i;
    char loadUID[32] = "\0";
    ifstream loadUidInf("uidInf.ini", ios::in);
    if (loadUidInf.fail()) {
        if (rememberCh != 'y')
            return false;
        else {
            ofstream savaUidInf("uidInf.ini", ios::out | ios::trunc);
            savaUidInf << "UID = " << uid;
            savaUidInf.close();
        }
    } else {
        loadUidInf.getline(loadUID, sizeof(loadUID));
        for (i = 6; loadUID[i]; i++) {
            loadUID[i - 6] = loadUID[i];
            loadUID[i] = '\0';
        }
        strcpy(uid, loadUID);
        return true;
    }
    loadUidInf.close();
    return false;
}

[[noreturn]] void print_adminMenu(userAccount *head, attendance *ahead, char *uid) {
    userAccount *point;
    point = searchUid(head, uid);
    aHead:
    point->welcomeUser();
    cout << "��ǰʱ�䣺" << nowTime() << endl;
    cout << endl;
    cout << "           ����Ա�˵�           " << endl;
    cout << "===============================" << endl;
    cout << "=         01.������Ϣ         =" << endl;
    cout << "=         02.�û�����         =" << endl;
    cout << "=    03.�����������������    =" << endl;
    cout << "=       04.�鿴��������       =" << endl;
    cout << "=       05.�û�����״̬       =" << endl;
    cout << "===============================" << endl;
    cout << "������ѡ���Ž���ѡ��" << endl;
    if (adminMenuChoice(head, ahead, point)) {
        head->saveUserData(head);
        ahead->savaAttendance(ahead);
        goto aHead;
    } else
        goto aHead;
}