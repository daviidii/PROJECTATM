#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <time.h>
#include <string>
#include <iomanip>
#define ch_MAX 36
using namespace std;

void SetColor(int ForgC);
void gotoxy(int x,int y);
void setFontStyle(int FontSize);
void ShowConsoleCursor(bool showFlag);


typedef struct accRec{
string name;
string pin;
string dob;
string contactNum;
int accNum;
double balance;
struct accRec* next;
}REC;

class project{
private:
    REC* X;
    string randomID(int length);
    string numpad(string msg);
    bool utilSendMoney(int amount, int recipientAccNum);
    int billers();
    int payUtilFunction(REC *acc);
    void receiptGenerator(REC *a, string purpose, string recipient, double amount);
public:
    string pinCode();
    string insertCard();
    void welcomeScr();
    void makeNull();
    void save();
    void retrieve();
    void save2card(int accountNumber, string pin);
    void checker(string holder);
    void rgstr();
    bool checkDupAccountNum(int usrAccNum);
    void displayUsrDetails(REC usr);
    void add(REC info);
    void transac(int accountNumber);
    void balanceInquiry(int usrAccNum);
    void withdraw(int usrAccNum);
    void transfer(int usrAccNum);
    void depositFunds(int usrAccNum);
    void changePin(int usrAccNum);
    void payBills(int usrAccNum);

};

void project::makeNull(){
    X = NULL;
}

string project::randomID(int length){
    char alphaNum[ch_MAX] ={'1','2','3','4','5','6','7','8','9',
                         '0','A','B','C','D','E','F','G','H',
                         'I','J','K','L','M','N','O','P','Q',
                         'R','S','T','U','V','W','X','Y','Z'};
    string result = "";
    for (int i = 0; i<length; i++)
        result = result + alphaNum[rand() % ch_MAX];

    return result;
}

void project::receiptGenerator(REC *a, string purpose, string recipient, double amount){
system("cls");
string accNum = to_string(a->accNum);
time_t now = time(0);
char* dateAndTime = ctime(&now);

    gotoxy(10,3); cout << "TRANSACTION COMPLETE" << endl;
    gotoxy(10,6); cout << "TRANSACTION NUMBER :";
    gotoxy(30,6); cout << randomID(13) << endl;
    gotoxy(10,7); cout << "TRANSACTION DATE   :";
    gotoxy(30,7); cout << dateAndTime << endl;
    gotoxy(10,8); cout << "ACCOUNT NAME       :";
    gotoxy(30,8); cout << a->name << endl;
    gotoxy(10,9); cout << "FROM               :";
    gotoxy(30,9); cout << "xxx" << accNum.substr(3,2) << endl;
    gotoxy(10,10);cout << "SENT TO            :";
    gotoxy(30,10);cout << recipient << endl;
    gotoxy(10,11);cout << "AMOUNT             :";
    gotoxy(30,11);cout << "-$" <<fixed <<setprecision(2) << amount << endl;
    gotoxy(10,12);cout << "PURPOSE            :";
    gotoxy(30,12);cout << purpose << endl;
    gotoxy(10,14);cout << "Press any key to continue banking..." << endl;
    getch();
}

void project::balanceInquiry(int usrAccNum){
system("cls");
REC *p, *q;
p = q = X;
    while(p!=NULL && usrAccNum != p->accNum){
        q = p;
        p = p->next;
    }
    if (p == NULL){
        cout << "Could not locate account" << endl;
        system("pause");
        }
    else{
        gotoxy(10,7);cout << "Account name: ";
        gotoxy(30,7);cout << p->name << endl;
        gotoxy(10,8);cout << "Account number: ";
        gotoxy(30,8);cout << p->accNum << endl;
        gotoxy(10,9);cout << "Account balance: ";
        gotoxy(30,9);cout << "$" << p->balance << endl;
    }
    gotoxy(10,12);cout << "Press any key to go back..." << endl;
    getch();
}

void project::withdraw(int usrAccNum){
REC *p, *q;
p = q = X;
string purpose = "CASH WITHDRAWAL";
string str;
int amount;
while (p!=NULL && usrAccNum != p->accNum){
    q = p;
    p = p->next;
}
if (p == NULL){
    cout << "Could not locate account" << endl;
}
else{
    do{
        str = numpad("AMOUNT");
        amount = stoi(str);
    }while(amount % 100 != 0 || amount > 10000 || amount > p->balance);
    p->balance = p->balance - amount;
    system("cls");
    cout << "Please wait..." << endl;
    Sleep(3000);
    receiptGenerator(p, purpose, "AUTOMATED TELLER MACHINE", amount);

}
}

void project::transfer(int usrAccNum){
REC *p, *q;
p = q = X;
string purpose = "MONEY TRANSFER";
string str, str2;
int accountNum, amount;
while(p != NULL && usrAccNum != p->accNum){
    q = p;
    p = p->next;
}
if (p == NULL){
    cout << "Could not locate account" << endl;
    system("pause");
}
else{
    // LOOP IF AMOUNT IS GREATER THAN USER BALANCE OR AMOUNT IS GREATER THAN 10,000 OR RECIPIENT ACCOUNT NUMBER CANNOT BE LOCATED
    do {
        str = numpad("ACC NO.");
        accountNum = stoi(str);
        str2 = numpad("AMOUNT");
        amount = stoi(str2);
    }while(amount > p->balance || amount > 10000 || utilSendMoney(amount, accountNum) != true);
    p->balance = p->balance - amount;
    receiptGenerator(p, purpose,"xxx" + str.substr(3,2),amount);
    }
}

bool project::utilSendMoney(int amount, int recipientAccNum){
REC *p, *q;
p = q = X;
while(p!=NULL && recipientAccNum != p->accNum){
    q = p;
    p = p->next;
}
if (p == NULL){
    gotoxy(40,6);cout << "Could not locate account" << endl;
    Sleep(1000);
    return false;
}
else {
    p->balance = p->balance + amount;
    return true;
}
}

void project::depositFunds(int usrAccNum){
REC *p, *q;
p = q = X;
string purpose = "CASH DEPOSIT";
string to;
string str;
int amount;
    while(p != NULL && usrAccNum != p->accNum){
        q = p;
        p = p->next;
    }
    if (p == NULL){
        cout << "Could not locate account" << endl;
        system("pause");
    }
    else{
        do{
            str = numpad("AMOUNT");
            amount = stoi(str);
        }while(amount%100 != 0 && amount > 20000);
        p->balance = p->balance + amount;
        to = to_string(p->accNum);
        system("cls");
        cout << "Please wait..." << endl;
        Sleep(3000);
        receiptGenerator(p,purpose,"xxx" + to.substr(3),amount);
}
}

void project::changePin(int usrAccNum){
fstream fp;
REC *p, *q;
p = q  = X;
string newPin;
    while(p != NULL && usrAccNum != p->accNum){
        q = p;
        p = p->next;
    }
    if (p == NULL){
        cout << "Could not locate account" << endl;
        system("pause");
    }
    else{
        newPin = pinCode();
        p->pin = newPin;
        fp.open("E:\\pin.code.txt", ios::out);
        fp << p->accNum << " " << p->pin << endl;
        gotoxy(47,6); cout << "PIN CHANGED" << endl;
        Sleep(2000);
        fp.close();
    }
}

int project::billers(){
setFontStyle(40);
    string billerList[] =  {"MERRAL CO.", "MYKNEELAD", "CONBIRDS ICTEA" , "PLDC", "EXIT"};
    int pointer = 0;
    const char ENTER = 13;
    char ch = ' ';
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while(true){
        system("cls");
        ShowConsoleCursor(false);
        gotoxy(44,8);
        cout << "SELECT YOUR TRANSACTION\n\n";
        gotoxy(31, 16);
        cout<<"USE UP OR DOWN ARROW KEY TO NAVIGATE THROUGH MENU";

        for(int i=0; i<sizeof(billerList)/sizeof(billerList[0]); ++i){
            if(i==pointer){
                gotoxy(48,10+i);
                SetConsoleTextAttribute(hConsole, 1);
                cout << billerList[i] << endl;
            }else{
                gotoxy(48,10+i);
                SetConsoleTextAttribute(hConsole, 15);
                cout << billerList[i] << endl;
            }
             SetConsoleTextAttribute(hConsole, 15);
        }
        while(true){
            if(GetAsyncKeyState(VK_UP) != 0){
                --pointer;
                if(pointer == -1){
                    pointer = 4;
                }
                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_DOWN) != 0){
                ++pointer;
                if(pointer == 5){
                    pointer = 0;
                }
                Sleep(100);
                break;
            }
            else if(ch=getch() == ENTER){
                switch(pointer){
                    case 0:
                    {
                        return 1;
                        break;
                    }
                    case 1:
                    {
                        return 2;
                        break;
                    }
                    case 2:
                    {
                        return 3;
                        break;
                    }
                    case 3:
                    {
                        return 4;
                        break;
                    }
                    case 4:
                        {
                            return 5;
                            break;
                        }
            }
            break;
        }
}
}
}

int project::payUtilFunction(REC *acc){
string str1, str2;
int usrAccNumToBiller,amount;

    do{
        str1 = numpad("ACC NO.");
        str2 = numpad("AMOUNT");
        usrAccNumToBiller = stoi(str1);
        amount = stoi(str2);
    }while(amount > acc->balance);

    return amount;
}

void project::payBills(int usrAccNum){
REC *p, *q;
p = q = X;
string recipients[4] = {"MERRAL CO","MYKNEELAD","CONBIRDS ICTEA","PLDC"};
string purpose = "BILL/S PAYMENT";
int amount,ch;
    while (p != NULL && usrAccNum != p->accNum){
        q = p;
        p = p->next;
    }
    if (p == NULL){
        cout << "Could not locate account" << endl;
    }
    else{
        ch = billers();
        switch (ch){
            case 1:
                {
                    amount = payUtilFunction(p);
                    p->balance = p->balance - amount;
                    receiptGenerator(p, purpose, recipients[0],amount);
                    break;
                }
            case 2:
                {
                    amount = payUtilFunction(p);
                    p->balance = p->balance - amount;
                    receiptGenerator(p, purpose, recipients[1],amount);
                    break;
                }
            case 3:
                {
                    amount = payUtilFunction(p);
                    p->balance = p->balance - amount;
                    receiptGenerator(p, purpose, recipients[2],amount);
                    break;
                }
            case 4:
                {
                    amount = payUtilFunction(p);
                    p->balance = p->balance - amount;
                    receiptGenerator(p, purpose, recipients[3],amount);
                    break;
                }
            case 5:
                {
                    transac(usrAccNum);
                    break;
                }
            }
    }
}

void project::checker(string holder){
REC *p, *q;
p = q = X;
fstream fp;
string pin;
int tempAccNum;
    // GET PIN NUMBER FROM USER ATM
    fp.open("E:\\pin.code.txt",ios::in);
    fp >> tempAccNum >> pin;
    fp.close();
    // TRAVERSE THROUGH LINKED LIST
    while (p!=NULL && tempAccNum != p->accNum){
        q = p;
        p = p->next;
    }
    if (p == NULL){
        gotoxy(40,6);cout << "Could not locate account" << endl;
        Sleep(1000);
        rgstr();
    }
    // CHECK IF PIN FROM ATM AND PIN FROM USER INPUT ARE THE SAME
    else if(pin == holder){
        transac(tempAccNum);
        system("pause");
    }
    else{
        gotoxy(46,6);cout << "Incorrect PIN" << endl;
        Sleep(1000);
        }
}

string project::numpad(string msg){
string holder = "";
string asterisk = "";

setFontStyle(40);
string options[] =  {"1","2","3","4","5","6","7","8","9","<-","0","enter"};
char ch = ' ';
int pointer = 0;
const char ENTER = 13;

HANDLE  hConsole;
hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while(true){
        system("cls");
        ShowConsoleCursor(false);
        gotoxy(50,3);
        cout << msg << endl;
        gotoxy(28, 18);
        cout<<"USE UP OR DOWN ARROW KEY TO NAVIGATE THROUGH MENU";
        gotoxy(50, 6);cout<< setw(5) << holder << endl;


        for(int i=0; i<13; ++i){

            switch(i)
            {
                case 0:
                {
                    if(i==pointer)
                    {
                        gotoxy(48,8);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[0] << endl;
                    }
                    else
                    {
                        gotoxy(48,8);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[0] << endl;
                    }
                }
                case 1:
                {
                    if(i==pointer)
                    {
                        gotoxy(52,8);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[1] << endl;
                    }
                    else
                    {
                        gotoxy(52,8);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[1] << endl;
                    }
                }
                case 2:
                {
                    if(i==pointer)
                    {
                        gotoxy(56,8);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[2] << endl;
                    }
                    else
                    {
                        gotoxy(56,8);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[2] << endl;
                    }
                }
                case 3:
                {
                   if(i==pointer)
                    {
                        gotoxy(48,10);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[3] << endl;
                    }
                    else
                    {
                        gotoxy(48,10);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[3] << endl;
                    }
                }
                case 4:
                {
                   if(i==pointer)
                    {
                        gotoxy(52,10);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[4] << endl;
                    }
                    else
                    {
                        gotoxy(52,10);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[4] << endl;
                    }
                }
                case 5:
                {
                   if(i==pointer)
                    {
                        gotoxy(56,10);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[5] << endl;
                    }
                    else
                    {
                        gotoxy(56,10);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[5] << endl;
                    }
                }
                case 6:
                {
                   if(i==pointer)
                    {
                        gotoxy(48,12);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[6] << endl;
                    }
                    else
                    {
                        gotoxy(48,12);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[6] << endl;
                    }
                }
                case 7:
                {
                   if(i==pointer)
                    {
                        gotoxy(52,12);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[7] << endl;
                    }
                    else
                    {
                        gotoxy(52,12);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[7] << endl;
                    }
                }
                case 8:
                {
                   if(i==pointer)
                    {
                        gotoxy(56,12);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[8] << endl;
                    }
                    else
                    {
                        gotoxy(56,12);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[8] << endl;
                    }
                }
                case 9:
                {
                   if(i==pointer)
                    {
                        gotoxy(44,14);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[9] << endl;
                    }
                    else
                    {
                        gotoxy(44,14);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[9] << endl;
                    }
                }
                case 10:
                {
                    if(i==pointer)
                    {
                        gotoxy(52,14);;
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[10] << endl;
                    }
                    else
                    {
                        gotoxy(52,14);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[10] << endl;
                    }
                }
              case 11:
                {
                  if(i==pointer)
                    {
                        gotoxy(58,14);;
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << options[11] << endl;
                    }
                    else
                    {
                        gotoxy(58,14);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << options[11] << endl;
                    }

                }
            }

             SetConsoleTextAttribute(hConsole, 15);
        }
        while(true)
            {
            if(GetAsyncKeyState(VK_UP) != 0){
                pointer-=3;
                if(pointer == -3){
                    pointer = 10;
                }
                if(pointer == -2){
                    pointer = 11;
                }
                if(pointer == -1){
                    pointer = 12;
                }

                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_DOWN) != 0){
                pointer+=3;
                if (pointer == 13)
                    pointer = 0;
                if (pointer == 14)
                    pointer = 1;
                if (pointer == 15)
                    pointer = 2;
                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_RIGHT) != 0){
                ++pointer;
                if(pointer == 13){
                    pointer = 0;
                }
                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_LEFT) != 0){
                --pointer;
                if(pointer == -1){
                    pointer = 12;
                }
                Sleep(100);
                break;
            }
            else if(ch=getch() == ENTER)
            {
                switch(pointer)
                {

                    case 0:
                    {
                        ShowConsoleCursor(true);
                        holder = holder + "1";
                        break;
                    }
                    case 1:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "2";
                        break;
                    }
                    case 2:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "3";
                        break;
                    }
                    case 3:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "4";
                        break;
                    }
                    case 4:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "5";
                        break;

                    }
                      case 5:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "6";
                        break;

                    }
                      case 6:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "7";
                        break;

                    }
                      case 7:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "8";
                        break;

                    }
                      case 8:
                    {
                        ShowConsoleCursor(true);
                        holder  = holder +  "9";
                        break;

                    }
                      case 9:
                    {
                        ShowConsoleCursor(true);
                        holder.pop_back();
                        break;

                    }
                    case 10:
                        {
                        ShowConsoleCursor(true);
                        holder  = holder +  "0";
                        break;
                        }
                    case 11:
                        {
                        return holder;
                        system("pause");
                        exit(0);
                        }
                }
                break;
            }
        }
    }
}

string project::pinCode (){
string holder = "";
string asterisk = "";

setFontStyle(40);
string menu[] =  {"1","2","3","4","5","6","7","8","9","<-","0","enter"};
char ch = ' ';
int pointer = 0;
const char ENTER = 13;

HANDLE  hConsole;
hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while(true){
        system("cls");
        ShowConsoleCursor(false);
        gotoxy(48,3);
        cout << "ENTER PIN\n\n";
        gotoxy(28, 16);
        cout<<"USE UP OR DOWN ARROW KEY TO NAVIGATE THROUGH MENU";
        gotoxy(5, 6);
        gotoxy(50, 6);cout<< setw(6) << asterisk << endl;


        for(int i=0; i<sizeof(menu)/sizeof(menu[0]) + 1; ++i){

            switch(i)
            {
                case 0:
                {
                    if(i==pointer)
                    {
                        gotoxy(48,8);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[0] << endl;
                    }
                    else
                    {
                        gotoxy(48,8);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[0] << endl;
                    }
                }
                case 1:
                {
                    if(i==pointer)
                    {
                        gotoxy(52,8);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[1] << endl;
                    }
                    else
                    {
                        gotoxy(52,8);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[1] << endl;
                    }
                }
                case 2:
                {
                    if(i==pointer)
                    {
                        gotoxy(56,8);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[2] << endl;
                    }
                    else
                    {
                        gotoxy(56,8);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[2] << endl;
                    }
                }
                case 3:
                {
                   if(i==pointer)
                    {
                        gotoxy(48,10);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[3] << endl;
                    }
                    else
                    {
                        gotoxy(48,10);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[3] << endl;
                    }
                }
                case 4:
                {
                   if(i==pointer)
                    {
                        gotoxy(52,10);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[4] << endl;
                    }
                    else
                    {
                        gotoxy(52,10);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[4] << endl;
                    }
                }
                case 5:
                {
                   if(i==pointer)
                    {
                        gotoxy(56,10);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[5] << endl;
                    }
                    else
                    {
                        gotoxy(56,10);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[5] << endl;
                    }
                }
                case 6:
                {
                   if(i==pointer)
                    {
                        gotoxy(48,12);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[6] << endl;
                    }
                    else
                    {
                        gotoxy(48,12);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[6] << endl;
                    }
                }
                case 7:
                {
                   if(i==pointer)
                    {
                        gotoxy(52,12);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[7] << endl;
                    }
                    else
                    {
                        gotoxy(52,12);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[7] << endl;
                    }
                }
                case 8:
                {
                   if(i==pointer)
                    {
                        gotoxy(56,12);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[8] << endl;
                    }
                    else
                    {
                        gotoxy(56,12);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[8] << endl;
                    }
                }
                case 9:
                {
                   if(i==pointer)
                    {
                        gotoxy(44,14);
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[9] << endl;
                    }
                    else
                    {
                        gotoxy(44,14);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[9] << endl;
                    }
                }
                case 10:
                {
                    if(i==pointer)
                    {
                        gotoxy(52,14);;
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[10] << endl;
                    }
                    else
                    {
                        gotoxy(52,14);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[10] << endl;
                    }
                }
              case 11:
                  if(i==pointer)
                    {
                        gotoxy(58,14);;
                        SetConsoleTextAttribute(hConsole, 1);
                        cout << menu[11] << endl;
                    }
                    else
                    {
                        gotoxy(58,14);
                        SetConsoleTextAttribute(hConsole, 15);
                        cout << menu[11] << endl;
                    }


            }

             SetConsoleTextAttribute(hConsole, 15);
        }
        while(true)
            {
            if(GetAsyncKeyState(VK_UP) != 0){
                pointer-=3;
                if(pointer == -3){
                    pointer = 9;
                }
                if(pointer == -2){
                    pointer = 10;
                }
                if(pointer == -1){
                    pointer = 11;
                }

                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_DOWN) != 0){
                pointer+=3;
                if (pointer == 12)
                    pointer = 0;
                if (pointer == 13)
                    pointer = 1;
                if (pointer == 14)
                    pointer = 2;
                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_RIGHT) != 0){
                ++pointer;
                if(pointer == 12){
                    pointer = 0;
                }
                Sleep(100);
                break;
            }
            else if(GetAsyncKeyState(VK_LEFT) != 0){
                --pointer;
                if(pointer == -1){
                    pointer = 11;
                }
                Sleep(100);
                break;
            }
            else if(ch=getch() == ENTER)
            {
                switch(pointer)
                {

                    case 0:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder = holder + "1";
                        break;
                    }
                    case 1:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "2";
                        break;
                    }
                    case 2:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "3";
                        break;
                    }
                    case 3:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "4";
                        break;
                    }
                    case 4:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "5";
                        break;

                    }
                      case 5:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "6";
                        break;

                    }
                      case 6:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "7";
                        break;

                    }
                      case 7:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "8";
                        break;

                    }
                      case 8:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "9";
                        break;

                    }
                      case 9:
                    {
                        ShowConsoleCursor(true);
                        asterisk.pop_back();
                        holder.pop_back();
                        break;

                    }
                    case 10:
                    {
                        ShowConsoleCursor(true);
                        asterisk = asterisk + "*";
                        holder  = holder +  "0";
                        break;
                    }
                    case 11:
                    {
                        if (holder.size() != 6){
                            asterisk.clear();
                            holder.clear();
                            gotoxy(47,6);
                            cout << "INVALID PIN" << endl;
                            Sleep(1000);
                            system("cls");
                        }
                        else{
                            return holder;
                            system("pause");
                            exit(0);
                        }
                    }
                }
                break;
            }
        }
    }

}

string project::insertCard(){
ShowConsoleCursor(false);
setFontStyle(40);
string p;
fstream fp;
    // CREATE FILE INCASE THERE IS NO FILE ON USER ATM
    fp.open("E:\\pin.code.txt",ios::out);
    fp.close();
    do{
    system("cls");
    Sleep(500);
    cout << "Please insert your card...";
    Sleep(500);
    fp.open("E:\\pin.code.txt",ios::in);
    }while(!fp);
    fp >> p >> p;
    //IF FILE IS EMPTY JUMP TO REGISTRATION
    if(p.size() == 0){
        system("cls");
        cout << "You do not have an account" << endl;
        cout << "Continuing to registration" << endl;
        Sleep(2000);
        rgstr();
    }

    fp.close();
    return p;
}


void project::rgstr(){
ShowConsoleCursor(true);
system("cls");
srand(time(0));
REC temp;
    // RANDOM ACCOUNT NUMBER GENERATOR
    temp.accNum = 10000 + rand()%90000;
    while(checkDupAccountNum(temp.accNum) == true){
        temp.accNum = 10000 + rand()%90000;
    }

    gotoxy(10,5);cout << "Surname, First name MI.: "; getline(cin, temp.name);
    gotoxy(10,6);cout << "Date of Birth: ";getline(cin, temp.dob);
    gotoxy(10,7);cout << "Contact Number: ";getline(cin, temp.contactNum);
    gotoxy(10,8);cout << "Account Number: " <<temp.accNum << endl;
    Sleep(2000);
    system("cls");
    gotoxy(10,6);cout << "Initial Deposit must be >= $5000.00" << endl;
    gotoxy(10,7);cout << "Deposit amount: ";cin >> temp.balance;
    // CHECK IF INITIAL DEPOSIT IS ABOVE 5000
    while(temp.balance < 5000){
        system("cls");
        gotoxy(15,6);cout << "Initial deposit must be >= $5000.00";
        Sleep(2000);
        system("cls");
        gotoxy(15,7);cout << "Deposit amount: ";cin >> temp.balance;
    }
    // SET PIN CODE
    system("cls");
    gotoxy(10,7);cout << "Set-up your 6 Digit PIN";
    gotoxy(10,8);cout << "Please wait...";
    Sleep(2000);
    temp.pin = pinCode();

    //ADD USER TO BANK DATA BASE
    add(temp);
    //SAVE USER ACCOUNT NUMBER AND PIN TO CARD
    save2card(temp.accNum, temp.pin);
    //DISPLAY USER ACCOUNT DETAILS
    displayUsrDetails(temp);
    //JUMP TO TRANSACTIONS
    transac(temp.accNum);
}

void project::displayUsrDetails(REC usr){
ShowConsoleCursor(false);
system("cls");
    gotoxy(10,5); cout << "ACCOUNT NUM. : ";
    gotoxy(30,5); cout << usr.accNum << endl;
    gotoxy(10,6); cout << "ACCOUNT NAME : ";
    gotoxy(30,6); cout << usr.name << endl;
    gotoxy(10,7); cout << "DATE OF BIRTH: ";
    gotoxy(30,7); cout << usr.dob << endl;
    gotoxy(10,8); cout << "CONTACT NUM. : ";
    gotoxy(30,8); cout << usr.contactNum << endl;
    gotoxy(10,9); cout << "ACCOUNT BAL. : ";
    gotoxy(30,9); cout << "$" << usr.balance << endl;
    gotoxy(10,10); cout << "PIN CODE     :";
    gotoxy(30,10); cout << "xxxx" << usr.pin.substr(4,2) << endl;
    gotoxy(10,12); cout << "Press any key to continue banking...";
    getch();
}

bool project::checkDupAccountNum(int usrAccountNum){
    REC* trverse = X;

    while (trverse != NULL){
        if (trverse->accNum == usrAccountNum)
            return true;
        else
            return false;

    }
    return false;
}

void project::transac(int accountNumber){
setFontStyle(40);
    string menu[7] =  {"BALANCE INQUIRY", "CASH WITHDRAWAL", "TRANSFER FUNDS" , "DEPOSIT FUNDS", "CHANGE PIN", "PAY BILLS", "EXIT"};
    int pointer = 0;
    const char ENTER = 13;
    char ch = ' ';
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    while(true){
        system("cls");
        ShowConsoleCursor(false);
        gotoxy(44,8);
        cout << "SELECT YOUR TRANSACTION\n\n";
        gotoxy(31, 21);
        cout<<"USE UP OR DOWN ARROW KEY TO NAVIGATE THROUGH MENU";


        for(int i=0; i<7; ++i){
            if(i==pointer){
                gotoxy(48,10+i);
                SetConsoleTextAttribute(hConsole, 1);
                cout << menu[i] << endl;
            }else{
                gotoxy(48,10+i);
                SetConsoleTextAttribute(hConsole, 15);
                cout << menu[i] << endl;
            }
             SetConsoleTextAttribute(hConsole, 15);
        }
        while(true){
            if(GetAsyncKeyState(VK_UP) != 0){
                --pointer;
                if(pointer == -1){
                    pointer = 6;
                }
                Sleep(100);
                break;
            }else if(GetAsyncKeyState(VK_DOWN) != 0){
                ++pointer;
                if(pointer == 7){
                    pointer = 0;
                }
                Sleep(100);
                break;
            }else if(ch=getch() == ENTER){
                switch(pointer){
                    case 0:
                    {
                        ShowConsoleCursor(false);
                        balanceInquiry(accountNumber);
                        break;
                    }
                    case 1:
                    {
                        ShowConsoleCursor(false);
                        withdraw(accountNumber);
                        break;
                    }
                    case 2:
                    {
                        ShowConsoleCursor(false);
                        transfer(accountNumber);
                        break;
                    }
                    case 3:
                    {
                        ShowConsoleCursor(false);
                        depositFunds(accountNumber);
                        break;

                    }
                    case 4:
                    {
                        ShowConsoleCursor(false);
                        changePin(accountNumber);
                        break;
                    }
                    case 5:
                    {
                        ShowConsoleCursor(false);
                        payBills(accountNumber);
                        break;
                    }
                    case 6:
                    {
                        ShowConsoleCursor(false);
                        save();
                        //save2card();
                        system("cls");
                        cout << "PROJECT BY: \nDURAN, DAVID JOHN ALEXANDER R.\nUGAY, CHRISTIAN O." << endl;
                        exit(0);
                    }
                }
                break;
            }
        }

    }
}

void project::save2card(int accountNumber, string pin){
fstream saveFile;

    saveFile.open("E:\\pin.code.txt", ios::out);
    if (!saveFile){
        cout << "File could not be located" << endl;
        system("pause");
    }
    else{
        saveFile << accountNumber << " " << pin;
    }
    saveFile.close();
}

void project::save(){
REC* tempNode = X;
fstream saveFile_1;

    saveFile_1.open("bankDataBase.txt", ios::out);
    if (!saveFile_1){
        cout << "File could not be located" << endl;
        system("pause");
    }
    else{
        while (tempNode!=NULL){
            saveFile_1 << tempNode->accNum << " " << tempNode->pin << endl;
            saveFile_1 << tempNode->balance << endl;
            saveFile_1 << tempNode->name << endl;
            saveFile_1 << tempNode->dob << endl;
            saveFile_1 << tempNode->contactNum << endl;
            tempNode = tempNode->next;
        }
    }
    saveFile_1.close();
}

void project::retrieve(){
REC temp;
fstream readFile;

    readFile.open("bankDataBase.txt", ios::in);
    if(!readFile){
        cout << "File could not be located" << endl;
        system("pause");
    }
    else{
        while(readFile >> temp.accNum >> temp.pin){
            readFile >> temp.balance;
            readFile.ignore();
            getline(readFile, temp.name);
            getline(readFile, temp.dob);
            getline(readFile, temp.contactNum);
            add(temp);
        }
    }
    readFile.close();
}

void project::add(REC info){
REC *p, *q, *newNode;
p = q = X;

    newNode = new REC;
    newNode->accNum = info.accNum;
    newNode->balance = info.balance;
    newNode->contactNum = info.contactNum;
    newNode->dob = info.dob;
    newNode->name = info.name;
    newNode->pin = info.pin;

    while (p!=NULL){
        q = p;
        p = p->next;
    }
    if (p == X){
        X = newNode;
    }
    else
        q->next = newNode;

    newNode->next = p;

}

void project::welcomeScr(){
ShowConsoleCursor(false);
setFontStyle(50);
system("cls");
    for (int i = 0; i <= 60; i++){
        gotoxy(10+i,2);
        cout << "\xb7";
        gotoxy(10+i,15);
        cout << "\xb7";
    }
    gotoxy(37,5);
    cout << "Welcome" << endl;
    gotoxy(39,6);
    cout << "to" << endl;
    gotoxy(34,7);
    cout << "Bankko Raput" << endl;
    gotoxy(25,12);
    cout << "The Cheapest Bank Of All Time" << endl;
    gotoxy(20,13);
    cout << "Press any key to continue banking .....";
    getchar();
}

int main(){
project duran;
duran.makeNull();
duran.retrieve();
string getpin;

    duran.welcomeScr();
    // RETURNS PINCODE FROM USER'S ATM CARD
    string p = duran.insertCard();
    // CHECK IF PINCODE FROM USER'S ATM CARD IS SAME FROM HIS INPUT
    while(p != getpin){
        getpin = duran.pinCode();
        duran.checker(getpin);
    }
}

void gotoxy(int x,int y){
    COORD coord = {0,0};
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void SetColor(int ForgC){
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;


    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

void setFontStyle(int FontSize){
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = FontSize;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas"); // font style
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}


