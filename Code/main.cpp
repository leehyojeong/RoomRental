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
////////////지워도 되는 부분! 그냥 함수로 또 뻈어요!////////////
void makeTxt(vector<string>* date);

#define MANAGER_PHONE "01000000000"
#define MANAGER_PASS "9999"

vector<vector<string>> user;//사용자들의 정보가 담길 벡터(전화번호와 비밀번호)
vector<string> tmp_user;//한줄
vector<string> tName;//텍스트파일의 이름
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
         update();//정보 업데이트(현재 값들을 업데이트)
      case 1://사용자
         UMenu();
         break;
      case 2://관리자
         MMenu();
         break;
      }
   }
   return 0;
}

void init() {//초기화하는 함수
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

void makeTxt(vector<string> * date) {//txt파일 이름 만들기
   for (int i = START_DAY; i <= LAST_DAY; i += 7) {
      int year = i / 10000;//년
      int month = (i % 10000) / 100;//월
      int day = (i % 100);//일
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
      case 2://윤년도 따져야돼요
         Mday = 28;
         break;
      case 4:
      case 6:
      case 9:
      case 11:
         Mday = 30;
         break;
      }
      if (day > Mday) {//다음달로
         month++;
         day -= Mday;
      }
      if (month >= 13) {//다음년으로
         month = 1;
         year++;
      }
      if (year % 100 > 99) {//새로운 세기
         year = 0;
      }
      i = (year / 1000) * 10000000 + ((year % 1000) / 100) * 1000000 + (((year % 1000) % 100) / 10) * 100000 + (((year % 1000) % 100) % 10) * 10000 + (month / 10) * 1000 + (month % 10) * 100 + (day / 10) * 10 + (day % 10);
      date->push_back(to_string(i));
   }
}

int login() {//로그인 함수

   string pass;
   while (1) {
      cout << "전화번호를 입력하세요(-제외) : ";
      cin >> phone;

      if (phone.length() != 11) {//11자리보다 많이 입력했을 때
         cout << "전화번호의 형식이 틀렸습니다. 다시입력하세요" << endl;
      }
      else if (phone.find("010") != 0 && phone.find("011") != 0) {//010또는 011로 시작하지 않을 때
         cout << "전화번호는 010또는 011로 시작해야합니다. 다시입력하세요" << endl;
      }
      else {
         bool check = true;
         for (int i = 0; i < phone.length(); i++) {
            if (phone[i] >= 'A' && phone[i] <= 'Z' || phone[i] >= 'a' && phone[i] <= 'z') {
               cout << "숫자만 입력해야 합니다. 다시 입력하세요" << endl;
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
      cout << "비밀번호를 입력하세요(4자리 정수) : ";
      cin >> pass;
      bool check = true;
      for (int i = 0; i < pass.length(); i++) {
         if (pass[i] >= 'A' && pass[i] <= 'Z' || pass[i] >= 'a' && pass[i] <= 'z') {
            cout << "비밀번호는 숫자로만 입력해야 합니다." << endl;
            check = false;
            break;
         }
      }
      if (check) {
         break;
      }
   }
   ////////////////////////////////////////////
   /////////암호화해야해요////////////////////
   ///////////////////////////////////////////
   if (phone == MANAGER_PHONE && pass == MANAGER_PASS) {//관리자인 경우
      return 2;
   }
   pass = sha256(pass);
   if (user.size() != 0) {
      for (int i = 0; i < user.size(); i++)
      {
         if (user[i][0] == phone) {
            if (user[i][1] == pass) {
               cout << "사용자로 로그인합니다" << endl;
               return 1;//사용자인 경우
            }
         }
      }
   }
   //정보가 없는 경우
   if (user.size() == 0) {//처음 등록하는 사용자의 경우
      tmp_user.clear();
   }
   cout << "정보를 새로 저장합니다" << endl;
   tmp_user.push_back(phone + " " + pass);
   writeList("user.txt", tmp_user);//새로 정보 등록
   return 0;
}

bool file_open(string name, vector<string> * data) {//파일 여는 함수
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

void printList(vector<string> data) {//벡터를 출력하는 함수
   for (int i = 0; i < data.size(); i++) {
      cout << data[i] << endl;
   }
}

vector<vector<string>> token(vector<string> data) {//해당 구분자로 토큰해주는 함수
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

string find_txt_name(int date) {//입력한 날짜 txt이름 찾기 함수
   for (int i = 0; i < tName.size() - 1; i++) {
      if (atoi(tName[i].c_str()) <= date && atoi(tName[i + 1].c_str()) > date) {
         return tName[i];
      }
   }
}

void writeList(string name, vector<string> data) {//txt파일에 저장하는 함수
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

void MMenu() {//관리자 메뉴
   showMmenu();
}
void showMmenu() {
   cout << "==================== 메   뉴 ===================" << endl;
   cout << "1. 방 추가" << endl;
   cout << "2. 예약현황" << endl;
   cout << "3. 종료" << endl;
   cout << endl << "* 원하는 메뉴를 선택해주세요." << endl;
   cout << "->";
   int num;
   cin >> num;
   startMmenu(num);
}
void startMmenu(int num) {
   if (num == 1) {//방 추가하기
      addRoom();
   }
   else if (num == 2) {//예약현황 보기
      showReservation();
   }
   else if (num == 3) {//프로그램 종료
      exit(1);
   }
   else {//1, 2, 3 이외의 숫자를 입력한 경우
      cout << endl;
      cout << "메뉴를 다시 입력해주세요." << endl;
      cout << "잠시후 관리자 메뉴로 돌아갑니다." << endl;
      Sleep(1000);
      system("cls");
      showMmenu();
   }
}

void addRoom() {//관리자가 방추가하는 함수
   cout << "==================== 방  추가하기 ===================" << endl;
   cout << "* 방을 추가할 건물을 선택하세요." << endl;

   int ebuildingcount = 0;//아직 방을 더 추가할 수 있는 건물 개수
   int fbuildingcount = 0;//방을 추가할 수 없는 건물 개수
   int bcount = 0;//총 건물개수

   vector<vector<string>> ebuilding;//방 추가가능한 건물
   //vector<vector<string>> fbuilding;//방 추가불가한 건물
   vector<int> eachbuildingfloor;//각 건물 총 층 개수
   vector<vector<string>> allbinfo;//건물이름과 방들을 저장하는 벡터

   for (int i = 0; i < room.size(); i++) {
      const char* charstr = room[i].c_str();
      if (strchr(charstr, '@') != NULL) {//@을 포함하는 경우
         bcount++;
         ebuildingcount++;
         vector<string> ebcount;
         ebcount.push_back(charstr);//건물이름 인덱스[0]
         ebcount.push_back("0");//층정보 인덱스[1]
         ebuilding.push_back(ebcount);
      }
      else if (strchr(charstr, '$') != NULL) {//$을 포함하는 경우
         bcount++;
         fbuildingcount++;
         vector<string> ebcount;
         ebcount.push_back(charstr);//건물이름 인덱스[0]
         ebcount.push_back("0");//층정보 인덱스[1]
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
         cout << ebm << "." << bname + 1 << " 건물 " << ebuilding[i][1] << " 층까지 있습니다." << endl;
      }
   }

   for (int i = 0; i < ebuilding.size(); i++) {
      vector<string> tmp;
      for (int j = 0; j < eachbuildingfloor[i]; j++) {
         //cout << eachbuildingfloor[i] << endl;
         tmp.push_back(ebuilding[i][0]);//건물 이름 먼저 넣고
         tmp.push_back("0");//각 층별 방 개수 정보 저장
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

   //건물 선택
   cout << "->";
   int mnum = 0;
   cin >> mnum;
   while (mnum<1 || mnum>ebuilding.size()) {
      cout << "잘못된 입력입니다. 다시 선택해주세요." << endl;
      cout << "->";
      cin >> mnum;
   }

   //층 선택
   cout << "층을 입력하세요." << endl;
   cout << "->";
   int floor = 0;
   cin >> floor;

   //범위 밖 선택
   while (floor<1 || floor>atoi(ebuilding[mnum - 1][1].c_str())) {
      cout << "잘못된 입력입니다. 다시 선택해주세요." << endl;
      cout << "->";
      cin >> floor;
   }

   //10개 인지 확인하는 부분
   int index = 0;
   for (int i = 0; i < mnum - 1; i++) {
      index += atoi(ebuilding[i][1].c_str());
   }

   if (atoi(allbinfo[index + (floor - 1)][1].c_str()) >= 10) {
      cout << "해당 층에 더 이상 방을 추가할 수 없습니다." << endl;

      cout << "q 입력 시 관리자 메뉴로 돌아갑니다." << endl;
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
      //10개 이하일 때 호실 입력시키기
      cout << "호실을 입력하세요." << endl;
      cout << "->";
      string rname;
      cin >> rname;

      string check = ((ebuilding[mnum - 1][0]).c_str()[1]) + to_string(floor) + rname;
      cout << check << endl;

      int flag = 0;
      for (int i = bcount * 2; i < room.size(); i++) {
         if (room[i] == check) {
            cout << "이미 있는 방입니다." << endl;
            flag = 1;
            cout << "q 입력 시 관리자 메뉴로 돌아갑니다." << endl;
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
         //새값 넣고 정렬

         room.push_back(check);
         sort(room.begin(), room.end());

         //allbinfo 정보 업뎃
         int p2 = atoi(allbinfo[index + (floor - 1)][1].c_str());
         allbinfo[index + (floor - 1)][1] = to_string(++p2);

         cout << "방이 추가 되었습니다." << endl;

         //건물이랑 호실만 위치(위아래)바꿔주기
         //정렬하면 #이 @보다 먼저기 때문
         for (int i = 0; i < ebuilding.size(); i++) {
            room[i] = ebuilding[i][0];
            room[i + bcount] = "#" + ebuilding[i][1];
         }

         //파일에 저장
         writeList("room.txt", room);

         cout << "q 입력 시 관리자 메뉴로 돌아갑니다." << endl;
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

void showReservation() {//관리자가 해당 날짜 예약내역보는 함수
   cout << "==================== 예약 현황 조회 ===================" << endl;
   cout << endl << "* 조회할 날짜를 입력해주세요." << endl;
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

void UMenu() {//사용자 메뉴   //백
   int select;
   vector<string> temp;

   while (true) {
      Sleep(1000);
      system("cls");
      cout << "1. 예약하기" << endl;
      cout << "2. 예약조회" << endl;
      cout << "3. 뒤로가기" << endl;
      cin >> select;
      switch (select) {
      case 1:
         reserve(&temp);
         break;
      case 2://사용자
         showMyReserve(&temp);
         break;
      case 3://관리자
         return;
      }
   }
}//~백


void reserve(vector<string>* temp) {//예약하기 함수
   vector<vector<string>> vtemp;
   string fname;
   int date, start;
   double use;
   string sdate;
   int input;
   int totalUse = 0;

   do {
      cout << "날짜를 입력해주십시오 : ";
      cin >> date;
   } while ((date < START_DAY || date > LAST_DAY));
   fname = find_txt_name(date) + ".txt";
   file_open(fname+".txt", temp);
   vtemp = token(*temp);
   do {
      cout << "예약하고자 하는 시작 시간을 입력해주십시오 : ";
      cin >> start;
   } while (start < START_TIME || start >= LAST_TIME);

   while(true) {
      double utemp;
      cout << "이용하고자 하는 시간을 입력해주십시오(0.5(30분)단위로 입력) : ";
      cin >> use;
      utemp = use * 2;
      if (utemp == (int)utemp) break;
   }
   if (use > 3) {
      cout << "이용은 3시간까지만 할 수 있습니다." << endl;
      _getch();
      return;
   }

   for (int i = 0; i < vtemp.size(); i++) {
      if (vtemp[i][day] == to_string(date))
         totalUse += stoi(vtemp[i][utime]);
   }
   if (totalUse + use > 6) {
      cout << endl << "일주일 동안 이용은 총  6시간까지만 할 수 있습니다." << endl;
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


   cout << r+1 << ". 종료" << endl;

   if (vtemp.empty()) {
      cout << "예약할 수 있는 방이 없습니다." << endl;
      return;
   }
   while (1) {
      cin >> input;
      input--;
      if (input < 0) cout << endl << "재입력하십시오." << endl;
      else if (input > r) cout << endl << "재입력하십시오." << endl;
      else break;
   }
   if (input == r) {
      cout << "예약을 종료합니다." << endl;
      _getch();
      return;
   }
   else {
      (*temp).push_back(to_string(date) + " " + vtemp[input][space] + " " + to_string(start) + " " + to_string(use) + " " + phone); // login 함수의 phone string 변수를 전역으로 선언하면 됩니다.
      sort(temp->begin(), temp->end());
   }
   writeList(fname+".txt", *temp);
   // 예약내역저장 조금 수정 필요할듯
   return;
}


void showMyReserve(vector<string>* temp) {//예약조회 함수
   vector<vector<string>> vtemp;
   string fname;
   int date, i;

   do {
      cout << "날짜를 입력해주십시오 : ";
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