#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

const int maks_user = 10;
std::string usernames[maks_user];
std::string passwords[maks_user];
int id[maks_user];
int userCount = 0;
std::fstream file;

bool inputSalah(const std::string& input) {
    return !input.empty();
}

void loadDataFromCSV() {
    file.open("dataUser.csv", std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Gagal membuka file dataUser.csv\n";
        return;
    }
    
    std::string line;
    while (std::getline(file, line) && userCount < maks_user) {
        std::stringstream ss(line);
        std::string username, password, idStr;
        
        if (std::getline(ss, username, ',') &&
            std::getline(ss, password, ',') &&
            std::getline(ss, idStr, ',')) {
            
            try {
                int userId = std::stoi(idStr);
                usernames[userCount] = username;
                passwords[userCount] = password;
                id[userCount] = userId;
                userCount++;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Gagal mengonversi id menjadi integer: " << idStr << "\n";
            }
        }
    }
    file.close();
}

void registrasi() {
    if (userCount >= maks_user) {
        std::cout << "Tidak bisa mendaftar, kapasitas penuh!\n";
        return;
    }

    std::string username, password;
    std::cout << "Registrasi\n";
    
    do {
        std::cout << "Masukkan username: ";
        std::cin >> username;
        if (!inputSalah(username)) {
            std::cout << "Username tidak boleh kosong!\n";
        }
    } while (!inputSalah(username));
    
    do {
        std::cout << "Masukkan password: ";
        std::cin >> password;
        if (!inputSalah(password)) {
            std::cout << "Password tidak boleh kosong!\n";
        }
    } while (!inputSalah(password));

    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username) {
            std::cout << "Username sudah terdaftar, buat username lain!\n";
            return;
        }
    }

    usernames[userCount] = username;
    passwords[userCount] = password;
    id[userCount] = userCount + 1; // ID unik untuk setiap pengguna

    file.open("dataUser.csv", std::ios::out | std::ios::app);
    if (file.is_open()) {
        file << usernames[userCount] << ",";
        file << passwords[userCount] << ",";
        file << id[userCount] << "\n";
        file.close();
    } else {
        std::cerr << "Gagal membuka file dataUser.csv\n";
        return;
    }
    
    userCount++;
    std::cout << "Registrasi berhasil!\n";
}

bool login() {
    std::string username, password;
    std::cout << "Login\n";
    
    do {
        std::cout << "Masukkan username: ";
        std::cin >> username;
        if (!inputSalah(username)) {
            std::cout << "Username tidak boleh kosong!\n";
        }
    } while (!inputSalah(username));
    
    do {
        std::cout << "Masukkan password: ";
        std::cin >> password;
        if (!inputSalah(password)) {
            std::cout << "Password tidak boleh kosong!\n";
        }
    } while (!inputSalah(password));

    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username && passwords[i] == password) {
            return true;
        }
    }
    return false;
}

// int main() {
//     loadDataFromCSV(); // Load data from CSV when the program starts

//     int pilihan;
//     while (true) {
//         std::cout << "1. Registrasi\n2. Login\n3. Keluar\nPilih opsi: ";
//         std::cin >> pilihan;
        
//         switch (pilihan) {
//             case 1:
//                 registrasi();
//                 break;
//             case 2:
//                 if (login()) {
//                     std::cout << "Login berhasil!\n";
//                     return 0;
//                 } else {
//                     std::cout << "Username atau password salah!\n";
//                 }
//                 break;
//             case 3:
//                 return 0;
//             default:
//                 std::cout << "Pilihan tidak valid!\n";
//         }
//     }
//     return 0;
// }
