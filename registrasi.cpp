#include <iostream>
#include <string>
using namespace std;

const int maks_user = 100;
string usernames[maks_user];
string passwords[maks_user];
int userCount = 0;

bool inputSalah(const string& input) {
    return !input.empty();
}

void registrasi() {
    if (userCount >= maks_user) {
        cout << "Tidak bisa mendaftar, kapasitas penuh!\n";
        return;
    }

    string username, password;
    cout << "Registrasi\n";
    
    do {
        cout << "Masukkan username: ";
        cin >> username;
        if (!inputSalah(username)) {
            cout << "Username tidak boleh kosong!\n";
        }
    } while (!inputSalah(username));
    
    do {
        cout << "Masukkan password: ";
        cin >> password;
        if (!inputSalah(password)) {
            cout << "Password tidak boleh kosong!\n";
        }
    } while (!inputSalah(password));

    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username) {
            cout << "Username sudah terdaftar, buat username lain!\n";
            return;
        }
    }

    usernames[userCount] = username;
    passwords[userCount] = password;
    userCount++;
    
    cout << "Registrasi berhasil!\n";
}

bool login() {
    string username, password;
    cout << "Login\n";
    
    do {
        cout << "Masukkan username: ";
        cin >> username;
        if (!inputSalah(username)) {
            cout << "Username tidak boleh kosong!\n";
        }
    } while (!inputSalah(username));
    
    do {
        cout << "Masukkan password: ";
        cin >> password;
        if (!inputSalah(password)) {
            cout << "Password tidak boleh kosong!\n";
        }
    } while (!inputSalah(password));

    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username && passwords[i] == password) {
            return true ;
        }
    }
    return false;
}

int main() {
    int pilihan;
    while (true) {
        cout << "1. Registrasi\n2. Login\n3. Keluar\nPilih opsi: ";
        cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                registrasi();
                break;
            case 2:
                if (login()) {
                    cout << "Login berhasil!\n";
                    int main();
                } else {
                    cout << "Username atau password salah!\n";
                }
                break;
            case 3:
                return 0;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    }
    return 0;
}