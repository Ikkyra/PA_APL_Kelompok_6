#include <iostream>
#include "main.cpp"
using namespace std;

struct account {
    int id;
    string username;
    string password;
};

struct settings{
    int pomodoro;
    int short_break;
    int long_break;
    int long_break_interval;
};



int main(){
    string pili;
    account akun[100];
    settings sett[100];
    int y;
    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username && passwords[i] == password) {
            y = i ;
        }
    }
    cout<<"[1] Setting Pomodoro\n";    
    cout<<"[2] Setting Short Break\n";    
    cout<<"[3] Setting Long Break\n";    
    cout<<"[4] Setting Long Break Interval\n";
    cout<<"[5] Kembali\n";
    cout<<"Masukkan Pilihan Anda: ";cin>>pili;
    if (pili=="1"){
        int baru1;
        int n = akun[y].id ;
        cout<<"Masukkan Waktu Pomodoro Baru: ";cin>>baru1;
        sett[y].pomodoro = baru1;
    }   
    else if (pili=="2"){
        int baru2;
        int n = akun[y].id ;
        cout<<"Masukkan Waktu Short Break Baru: ";cin>>baru2;
        sett[y].short_break = baru2;

    }    
    else if (pili=="3"){
        int baru3;
        int n = akun[y].id ;
        cout<<"Masukkan Waktu Long Break Baru: ";cin>>baru3;
        sett[y].long_break = baru3;

    }    
    else if (pili=="4"){
        int baru4;
        int n = akun[y].id ;
        cout<<"Masukkan Interval Long Break Baru: ";cin>>baru4;
        sett[y].long_break_interval = baru4;

    }     
    else if (pili=="5"){
        return ;
    }    
    else{
        cout<<"Pilihan Tidak Valid";
    }    
}