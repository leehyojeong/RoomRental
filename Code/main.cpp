#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <cstring>
#include <algorithm>
#include <conio.h>
#include "sha256.h"
using namespace std;

void init();
int login();
bool file_open(string name, vector<string>* data);
void printList(vector<string> data);
vector<vector<string>> token(vector<string> data);
vector<string> split(string line, char delimiter);
string find_txt_name(int date);
void writeList(string name, vector<string> data);
void MMenu();
void showMmenu();
void addRoom();
void showReservation();
void startMmenu(int num);
void UMenu();
void reserve(vector<string>* temp);
void showMyReserve(vector<string>* temp);
void update();
////////////������ �Ǵ� �κ�! �׳� �Լ��� �� �Q���!////////////
void makeTxt(vector<string>* date);

#define MANAGER_PHONE "01000000000"
#define MANAGER_PASS "9999"

vector<vector<string>> user;//����ڵ��� ������ ��� ����(��ȭ��ȣ�� ��й�ȣ)
vector<string> tmp_user;//����
vector<string> tName;//�ؽ�Ʈ������ �̸�
vector<string> room;
int START_DAY;
int LAST_DAY;
int START_TIME;
int LAST_TIME;
string phone;

enum {
   day,
   space,
   stime,
   utime,
   id
};

int main() {

   while (1) {
      init();
      int login_return = login();
      switch (login_return) {
      case 0:
         update();//���� ������Ʈ(���� ������ ������Ʈ)
      case 1://�����
         UMenu();
         break;
      case 2://������
         MMenu();
         break;
      }
   }
   return 0;
}

void init() {//�ʱ�ȭ�ϴ� �Լ�
   START_DAY = 20190304;
   LAST_DAY = 20190621;
   START_TIME = 9;
   LAST_TIME = 17.5;
   file_open("room.txt", &room);
   file_open("user.txt", &tmp_user);
   if (tmp_user[0] != "")
      user = token(tmp_user);
   makeTxt(&tName);
}

void update() {
   room.clear();
   user.clear();
   tmp_user.clear();
   file_open("room.txt", &room);
   file_open("user.txt", &tmp_user);
   user = token(tmp_user);
}

void makeTxt(vector<string> * date) {//txt���� �̸� �����
   for (int i = START_DAY; i <= LAST_DAY; i += 7) {
      int year = i / 10000;//��
      int month = (i % 10000) / 100;//��
      int day = (i % 100);//��
      int Mday = 30;
      switch (month)
      {
      case 1:
      case 3:
      case 5:
      case 7:
      case 8:
      case 10:
      case 12:
         Mday = 31;
         break;
      case 2://���⵵ �����ߵſ�
         Mday = 28;
         break;
      case 4:
      case 6:
      case 9:
      case 11:
         Mday = 30;
         break;
      }
      if (day > Mday) {//�����޷�
         month++;
         day -= Mday;
      }
      if (month >= 13) {//����������
         month = 1;
         year++;
      }
      if (year % 100 > 99) {//���ο� ����
         year = 0;
      }
      i = (year / 1000) * 10000000 + ((year % 1000) / 100) * 1000000 + (((year % 1000) % 100) / 10) * 100000 + (((year % 1000) % 100) % 10) * 10000 + (month / 10) * 1000 + (month % 10) * 100 + (day / 10) * 10 + (day % 10);
      date->push_back(to_string(i));
   }
}

int login() {//�α��� �Լ�

   string pass;
   while (1) {
      cout << "��ȭ��ȣ�� �Է��ϼ���(-����) : ";
      cin >> phone;

      if (phone.length() != 11) {//11�ڸ����� ���� �Է����� ��
         cout << "��ȭ��ȣ�� ������ Ʋ�Ƚ��ϴ�. �ٽ��Է��ϼ���" << endl;
      }
      else if (phone.find("010") != 0 && phone.find("011") != 0) {//010�Ǵ� 011�� �������� ���� ��
         cout << "��ȭ��ȣ�� 010�Ǵ� 011�� �����ؾ��մϴ�. �ٽ��Է��ϼ���" << endl;
      }
      else {
         bool check = true;
         for (int i = 0; i < phone.length(); i++) {
            if (phone[i] >= 'A' && phone[i] <= 'Z' || phone[i] >= 'a' && phone[i] <= 'z') {
               cout << "���ڸ� �Է��ؾ� �մϴ�. �ٽ� �Է��ϼ���" << endl;
               check = false;
               break;
            }
         }
         if (check) {
            break;
         }
      }
   }
   while (1) {
      cout << "��й�ȣ�� �Է��ϼ���(4�ڸ� ����) : ";
      cin >> pass;
      bool check = true;
      for (int i = 0; i < pass.length(); i++) {
         if (pass[i] >= 'A' && pass[i] <= 'Z' || pass[i] >= 'a' && pass[i] <= 'z') {
            cout << "��й�ȣ�� ���ڷθ� �Է��ؾ� �մϴ�." << endl;
            check = false;
            break;
         }
      }
      if (check) {
         break;
      }
   }
   ////////////////////////////////////////////
   /////////��ȣȭ�ؾ��ؿ�////////////////////
   ///////////////////////////////////////////
   if (phone == MANAGER_PHONE && pass == MANAGER_PASS) {//�������� ���
      return 2;
   }
   pass = sha256(pass);
   if (user.size() != 0) {
      for (int i = 0; i < user.size(); i++)
      {
         if (user[i][0] == phone) {
            if (user[i][1] == pass) {
               cout << "����ڷ� �α����մϴ�" << endl;
               return 1;//������� ���
            }
         }
      }
   }
   //������ ���� ���
   if (user.size() == 0) {//ó�� ����ϴ� ������� ���
      tmp_user.clear();
   }
   cout << "������ ���� �����մϴ�" << endl;
   tmp_user.push_back(phone + " " + pass);
   writeList("user.txt", tmp_user);//���� ���� ���
   return 0;
}

bool file_open(string name, vector<string> * data) {//���� ���� �Լ�
   ifstream file(name);
   if (file.is_open()) {
      char line[100];
      while (!file.eof()) {
         file.getline(line, 100);
         data->push_back(line);
      }
      return true;
   }
   return false;
}

void printList(vector<string> data) {//���͸� ����ϴ� �Լ�
   for (int i = 0; i < data.size(); i++) {
      cout << data[i] << endl;
   }
}

vector<vector<string>> token(vector<string> data) {//�ش� �����ڷ� ��ū���ִ� �Լ�
   vector<vector<string>> str;
   for (int i = 0; i < data.size(); i++) {
      vector<string> v = split(data[i], ' ');
      str.push_back(v);
   }
   return str;
}

vector<string> split(string line, char delimiter) {
   vector<string> output;
   stringstream ss(line);
   string tmp;
   while (getline(ss, tmp, delimiter)) {
      output.push_back(tmp);
   }

   return output;
}

string find_txt_name(int date) {//�Է��� ��¥ txt�̸� ã�� �Լ�
   for (int i = 0; i < tName.size() - 1; i++) {
      if (atoi(tName[i].c_str()) <= date && atoi(tName[i + 1].c_str()) > date) {
         return tName[i];
      }
   }
}

void writeList(string name, vector<string> data) {//txt���Ͽ� �����ϴ� �Լ�
   ofstream file(name);
   for (int i = 0; i < data.size(); i++) {
      if (i == data.size() - 1) {
         file << data[i];
      }
      else {
         file << data[i] << endl;
      }
   }
}

void MMenu() {//������ �޴�
   showMmenu();
}
void showMmenu() {
   cout << "==================== ��   �� ===================" << endl;
   cout << "1. �� �߰�" << endl;
   cout << "2. ������Ȳ" << endl;
   cout << "3. ����" << endl;
   cout << endl << "* ���ϴ� �޴��� �������ּ���." << endl;
   cout << "->";
   int num;
   cin >> num;
   startMmenu(num);
}
void startMmenu(int num) {
   if (num == 1) {//�� �߰��ϱ�
      addRoom();
   }
   else if (num == 2) {//������Ȳ ����
      showReservation();
   }
   else if (num == 3) {//���α׷� ����
      exit(1);
   }
   else {//1, 2, 3 �̿��� ���ڸ� �Է��� ���
      cout << endl;
      cout << "�޴��� �ٽ� �Է����ּ���." << endl;
      cout << "����� ������ �޴��� ���ư��ϴ�." << endl;
      Sleep(1000);
      system("cls");
      showMmenu();
   }
}

void addRoom() {//�����ڰ� ���߰��ϴ� �Լ�
   cout << "==================== ��  �߰��ϱ� ===================" << endl;
   cout << "* ���� �߰��� �ǹ��� �����ϼ���." << endl;

   int ebuildingcount = 0;//���� ���� �� �߰��� �� �ִ� �ǹ� ����
   int fbuildingcount = 0;//���� �߰��� �� ���� �ǹ� ����
   int bcount = 0;//�� �ǹ�����

   vector<vector<string>> ebuilding;//�� �߰������� �ǹ�
   //vector<vector<string>> fbuilding;//�� �߰��Ұ��� �ǹ�
   vector<int> eachbuildingfloor;//�� �ǹ� �� �� ����
   vector<vector<string>> allbinfo;//�ǹ��̸��� ����� �����ϴ� ����

   for (int i = 0; i < room.size(); i++) {
      const char* charstr = room[i].c_str();
      if (strchr(charstr, '@') != NULL) {//@�� �����ϴ� ���
         bcount++;
         ebuildingcount++;
         vector<string> ebcount;
         ebcount.push_back(charstr);//�ǹ��̸� �ε���[0]
         ebcount.push_back("0");//������ �ε���[1]
         ebuilding.push_back(ebcount);
      }
      else if (strchr(charstr, '$') != NULL) {//$�� �����ϴ� ���
         bcount++;
         fbuildingcount++;
         vector<string> ebcount;
         ebcount.push_back(charstr);//�ǹ��̸� �ε���[0]
         ebcount.push_back("0");//������ �ε���[1]
         ebuilding.push_back(ebcount);
      }
      else if (strchr(charstr, '#') != NULL) {
         eachbuildingfloor.push_back(atoi(charstr + 1));
      }
   }
   //cout << typeid(eachbuildingfloor[0]).name() << endl;
   int ebm = 0;
   for (int i = 0; i < ebuilding.size(); i++) {
      ebuilding[i][1] = to_string(eachbuildingfloor[i]);

      const char* bname = ebuilding[i][0].c_str();

      if (strchr(bname, '@') != NULL) {
         ebm++;
         cout << ebm << "." << bname + 1 << " �ǹ� " << ebuilding[i][1] << " ������ �ֽ��ϴ�." << endl;
      }
   }

   for (int i = 0; i < ebuilding.size(); i++) {
      vector<string> tmp;
      for (int j = 0; j < eachbuildingfloor[i]; j++) {
         //cout << eachbuildingfloor[i] << endl;
         tmp.push_back(ebuilding[i][0]);//�ǹ� �̸� ���� �ְ�
         tmp.push_back("0");//�� ���� �� ���� ���� ����
         allbinfo.push_back(tmp);
      }
   }
   for (int i = bcount * 2; i < room.size(); i++) {
      //cout << room[i] << endl;
      const char* line = room[i].c_str();
      for (int j = 0; j < ebuilding.size(); j++) {//3
         const char* bname = ebuilding[j][0].c_str() + 1;
         if (strstr(line, bname) != NULL) {
            //cout << (line + strlen(bname))[0] << endl;
            int floor = (int)((line + strlen(bname))[0]) - '0';
            int index = 0;

            for (int k = 0; k < j; k++) {
               index += atoi(ebuilding[k][1].c_str());
            }
            int c = atoi((allbinfo[index + floor - 1][1]).c_str());

            allbinfo[index + floor - 1][1] = to_string(++c);
            break;
         }
      }
   }

   //�ǹ� ����
   cout << "->";
   int mnum = 0;
   cin >> mnum;
   while (mnum<1 || mnum>ebuilding.size()) {
      cout << "�߸��� �Է��Դϴ�. �ٽ� �������ּ���." << endl;
      cout << "->";
      cin >> mnum;
   }

   //�� ����
   cout << "���� �Է��ϼ���." << endl;
   cout << "->";
   int floor = 0;
   cin >> floor;

   //���� �� ����
   while (floor<1 || floor>atoi(ebuilding[mnum - 1][1].c_str())) {
      cout << "�߸��� �Է��Դϴ�. �ٽ� �������ּ���." << endl;
      cout << "->";
      cin >> floor;
   }

   //10�� ���� Ȯ���ϴ� �κ�
   int index = 0;
   for (int i = 0; i < mnum - 1; i++) {
      index += atoi(ebuilding[i][1].c_str());
   }

   if (atoi(allbinfo[index + (floor - 1)][1].c_str()) >= 10) {
      cout << "�ش� ���� �� �̻� ���� �߰��� �� �����ϴ�." << endl;

      cout << "q �Է� �� ������ �޴��� ���ư��ϴ�." << endl;
      char remenu;
      bool exit = false;
      do {
         cin >> remenu;

         if (remenu == 'q') {
            exit = true;
            system("cls");
            MMenu();
         }
      } while (!exit);
   }
   else {
      //10�� ������ �� ȣ�� �Է½�Ű��
      cout << "ȣ���� �Է��ϼ���." << endl;
      cout << "->";
      string rname;
      cin >> rname;

      string check = ((ebuilding[mnum - 1][0]).c_str()[1]) + to_string(floor) + rname;
      cout << check << endl;

      int flag = 0;
      for (int i = bcount * 2; i < room.size(); i++) {
         if (room[i] == check) {
            cout << "�̹� �ִ� ���Դϴ�." << endl;
            flag = 1;
            cout << "q �Է� �� ������ �޴��� ���ư��ϴ�." << endl;
            char remenu;
            bool exit = false;
            do {
               cin >> remenu;

               if (remenu == 'q') {
                  exit = true;
                  system("cls");
                  MMenu();
               }
            } while (!exit);
         }
      }
      if (flag == 0) {
         //���� �ְ� ����

         room.push_back(check);
         sort(room.begin(), room.end());

         //allbinfo ���� ����
         int p2 = atoi(allbinfo[index + (floor - 1)][1].c_str());
         allbinfo[index + (floor - 1)][1] = to_string(++p2);

         cout << "���� �߰� �Ǿ����ϴ�." << endl;

         //�ǹ��̶� ȣ�Ǹ� ��ġ(���Ʒ�)�ٲ��ֱ�
         //�����ϸ� #�� @���� ������ ����
         for (int i = 0; i < ebuilding.size(); i++) {
            room[i] = ebuilding[i][0];
            room[i + bcount] = "#" + ebuilding[i][1];
         }

         //���Ͽ� ����
         writeList("room.txt", room);

         cout << "q �Է� �� ������ �޴��� ���ư��ϴ�." << endl;
         char remenu;
         bool exit = false;
         do {
            cin >> remenu;

            if (remenu == 'q') {
               exit = true;
               system("cls");
               MMenu();
            }
         } while (!exit);
      }
   }
}

void showReservation() {//�����ڰ� �ش� ��¥ ���೻������ �Լ�
   cout << "==================== ���� ��Ȳ ��ȸ ===================" << endl;
   cout << endl << "* ��ȸ�� ��¥�� �Է����ּ���." << endl;
   cout << "->";
   int date;
   cin >> date;
   vector<string> data;
   vector<vector<string>> data2;
   string fName = find_txt_name(date);
   if (file_open(fName + ".txt", &data)) {
      data2 = token(data);
      for (int i = 0; i < data2.size(); i++) {
         if (atoi(data2[i][0].c_str()) != date) {
            data[i].erase();
         }
      }
      printList(data);
   }

}

void UMenu() {//����� �޴�   //��
   int select;
   vector<string> temp;

   while (true) {
      Sleep(1000);
      system("cls");
      cout << "1. �����ϱ�" << endl;
      cout << "2. ������ȸ" << endl;
      cout << "3. �ڷΰ���" << endl;
      cin >> select;
      switch (select) {
      case 1:
         reserve(&temp);
         break;
      case 2://�����
         showMyReserve(&temp);
         break;
      case 3://������
         return;
      }
   }
}//~��


void reserve(vector<string>* temp) {//�����ϱ� �Լ�
   vector<vector<string>> vtemp;
   string fname;
   int date, start;
   double use;
   string sdate;
   int input;
   int totalUse = 0;

   do {
      cout << "��¥�� �Է����ֽʽÿ� : ";
      cin >> date;
   } while ((date < START_DAY || date > LAST_DAY));
   fname = find_txt_name(date) + ".txt";
   file_open(fname+".txt", temp);
   vtemp = token(*temp);
   do {
      cout << "�����ϰ��� �ϴ� ���� �ð��� �Է����ֽʽÿ� : ";
      cin >> start;
   } while (start < START_TIME || start >= LAST_TIME);

   while(true) {
      double utemp;
      cout << "�̿��ϰ��� �ϴ� �ð��� �Է����ֽʽÿ�(0.5(30��)������ �Է�) : ";
      cin >> use;
      utemp = use * 2;
      if (utemp == (int)utemp) break;
   }
   if (use > 3) {
      cout << "�̿��� 3�ð������� �� �� �ֽ��ϴ�." << endl;
      _getch();
      return;
   }

   for (int i = 0; i < vtemp.size(); i++) {
      if (vtemp[i][day] == to_string(date))
         totalUse += stoi(vtemp[i][utime]);
   }
   if (totalUse + use > 6) {
      cout << endl << "������ ���� �̿��� ��  6�ð������� �� �� �ֽ��ϴ�." << endl;
      _getch();
      return;
   }
   sdate = to_string(date);

   int i, j, l, m = 0;

   int r = 0;
   for (i = 0; i < room.size(); i++) {
      for (j = 0; j < vtemp.size(); j++) {
         if (room[i][0] == '@' || room[i][0] == '#' || room[i][0] == '$') break;
         if (vtemp[j][space] == room[i]) break;
      }
      if (j == vtemp.size()) {
         vector<string> tmp;
         tmp.push_back(sdate);
         tmp.push_back(room[i]);
         tmp.push_back("0");
         tmp.push_back("0");
         vtemp.push_back(tmp);
      }
   }

   for (i = 0; i < vtemp.size();) {
      if (start + use < stoi(vtemp[i][stime]) || start > stoi(vtemp[i][stime]) + stoi(vtemp[i][utime])) i++;
      else {
         vtemp.erase(vtemp.begin() + i);
      }
   }

   for (i = 0; i < room.size(); i++) {
      if (room[i][0] == '@' || room[i][0] == '#' || room[i][0] == '$') m++;
      for (int k = 0; k < vtemp.size(); k++){
         if(vtemp[k][space] == room[i]){
         cout << r + 1 << ". " << room[i] << endl;
         r++;
         for (j = 0; j < 18; j++) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            for (l = 0; l < vtemp.size(); l++) {
               if (vtemp[l][stime] == "0");
               else if (vtemp[l][space] == room[i]) {
                  if (stoi(vtemp[l][stime]) <= j * 0.5 + 9.0 && stoi(vtemp[l][stime]) + stoi(vtemp[l][utime]) >= j * 0.5 + 9.0) {
                     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                  }
               }
            }
            cout << 9.0 + j * 0.5 << " ";
         }
         cout << endl;
         }
      }
   }


   cout << r+1 << ". ����" << endl;

   if (vtemp.empty()) {
      cout << "������ �� �ִ� ���� �����ϴ�." << endl;
      return;
   }
   while (1) {
      cin >> input;
      input--;
      if (input < 0) cout << endl << "���Է��Ͻʽÿ�." << endl;
      else if (input > r) cout << endl << "���Է��Ͻʽÿ�." << endl;
      else break;
   }
   if (input == r) {
      cout << "������ �����մϴ�." << endl;
      _getch();
      return;
   }
   else {
      (*temp).push_back(to_string(date) + " " + vtemp[input][space] + " " + to_string(start) + " " + to_string(use) + " " + phone); // login �Լ��� phone string ������ �������� �����ϸ� �˴ϴ�.
      sort(temp->begin(), temp->end());
   }
   writeList(fname+".txt", *temp);
   // ���೻������ ���� ���� �ʿ��ҵ�
   return;
}


void showMyReserve(vector<string>* temp) {//������ȸ �Լ�
   vector<vector<string>> vtemp;
   string fname;
   int date, i;

   do {
      cout << "��¥�� �Է����ֽʽÿ� : ";
      cin >> date;
   } while (date < START_DAY || date > LAST_DAY);
   fname = find_txt_name(date) + ".txt";
   vtemp = token(*temp);
   file_open(fname, temp);
   for (i = 0; i < vtemp.size(); i++) {
      if (vtemp[i][id] == phone && vtemp[i][day] == to_string(date)) {
         cout << vtemp[i][day] << ", " << vtemp[i][space] << " : " << vtemp[i][stime] << " ~ " << stoi(vtemp[i][stime]) + stoi(vtemp[i][utime]) << endl;
      }
   }
   _getch();
}