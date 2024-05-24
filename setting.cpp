#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits> 
#include "registrasi.cpp"
#include "timer.cpp"

struct settings {
    int pomodoro;
    int short_break;
    int long_break;
    int long_break_interval;
};

const int maks = 100; 
settings sett[maks];
int idx = 0;

int countLines(const std::string& filename) {
    std::ifstream file(filename);
    int lines = 0;
    std::string line;
    while (std::getline(file, line)) {
        ++lines;
    }
    file.close();
    return lines - 1; 
}

void readCSV(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); 

    idx = 0;
    while (std::getline(file, line) && idx < maks) {
        std::istringstream ss(line);
        std::string token;
        int col = 0;

        while (std::getline(ss, token, ',')) {
            switch (col) {
                case 0: sett[idx].pomodoro = std::stoi(token); break;
                case 1: sett[idx].short_break = std::stoi(token); break;
                case 2: sett[idx].long_break = std::stoi(token); break;
                case 3: sett[idx].long_break_interval = std::stoi(token); break;
                default: break;
            }
            ++col;
        }
        ++idx;
    }
    file.close();
}


void writeCSV(const std::string &filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    file << "Pomodoro,Short Break,Long Break,Long Break Interval\n";
    for (int i = 0; i < idx; ++i) {
        file << sett[i].pomodoro << ","
             << sett[i].short_break << ","
             << sett[i].long_break << ","
             << sett[i].long_break_interval << "\n";
    }
    file.close();
}

int menu_setting() {
    readCSV("dataWaktu.csv");

    std::string pili;
    int y = 1;  
    while (true) {
        std::cout << "-------------------------------\n";    
        std::cout << "|          SETTINGS           |\n";    
        std::cout << "===============================\n";    
        std::cout << "[1] Setting Pomodoro\n";    
        std::cout << "[2] Setting Short Break\n";    
        std::cout << "[3] Setting Long Break\n";    
        std::cout << "[4] Setting Long Break Interval\n";
        std::cout << "[5] Kembali\n";
        std::cout << "===============================\n";    
        std::cout << "Masukkan Pilihan Anda: "; std::cin >> pili;

        int newValue;
        if (pili >= "1" && pili <= "4") {
            while (true) {
                system("cls");
                std::cout << "\nMasukkan Waktu Baru: ";
                if (!(std::cin >> newValue)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Error: Harap Masukkan Dalam Bentuk Angka!!" << std::endl;
                    jeda(1);
                    break;
                }

                if (newValue < 1) {
                    jeda(1);
                    std::cout << "Error: Waktu Tidak Boleh Kurang dari 1" << std::endl;
                    break;
                } else {
                    switch(pili[0]) {
                        case '1':
                            sett[allindeks].pomodoro = newValue;
                            break;
                        case '2':
                            sett[allindeks].short_break = newValue;
                            break;
                        case '3':
                            sett[allindeks].long_break = newValue;
                            break;
                        case '4':
                            sett[allindeks].long_break_interval = newValue;
                            break;
                    }
                    jeda(1);
                    std::cout << "Waktu berhasil diperbarui menjadi: " << newValue << std::endl;
                    break;
                }
            }
        
        } else if (pili == "5") {
            writeCSV("dataWaktu.csv");
            break;
        } else {
            jeda(1);
            std::cout << "Pilihan Tidak Valid" << std::endl;
        }
    system("cls");
    }

}
// int menu_setting() {
//     readCSV("dataWaktu.csv");

//     std::string pili;
//     int y = 1;  
//     while (true) {
//         std::cout << "-------------------------------\n";    
//         std::cout << "|          SETTINGS           |\n";    
//         std::cout << "===============================\n";    
//         std::cout << "[1] Setting Pomodoro\n";    
//         std::cout << "[2] Setting Short Break\n";    
//         std::cout << "[3] Setting Long Break\n";    
//         std::cout << "[4] Setting Long Break Interval\n";
//         std::cout << "[5] Kembali\n";
//         std::cout << "===============================\n";    
//         std::cout << "Masukkan Pilihan Anda: "; std::cin >> pili;

//         int newValue;
//         if (pili >= "1" && pili <= "4") {
//             while (true) {
//                 system("cls");
//                 std::cout << "\nMasukkan Waktu Baru: ";
//                 if (!(std::cin >> newValue)) {
//                     std::cin.clear();
//                     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//                     std::cout << "Error: Harap Masukkan Dalam Bentuk Angka!!" << std::endl;
//                     jeda(1);
//                     break;
//                 }

//                 if (newValue < 1) {
//                     jeda(1);
//                     std::cout << "Error: Waktu Tidak Boleh Kurang dari 1" << std::endl;
//                     break;
//                 } else {
//                     switch(pili[0]) {
//                         case '1':
//                             sett[y].pomodoro = newValue;
//                             break;
//                         case '2':
//                             sett[y].short_break = newValue;
//                             break;
//                         case '3':
//                             sett[y].long_break = newValue;
//                             break;
//                         case '4':
//                             sett[y].long_break_interval = newValue;
//                             break;
//                     }
//                     jeda(1);
//                     std::cout << "Waktu berhasil diperbarui menjadi: " << newValue << std::endl;
//                     break;
//                 }
//             }
        
//         } else if (pili == "5") {
//             writeCSV("dataWaktu.csv");
//             break;
//         } else {
//             jeda(1);
//             std::cout << "Pilihan Tidak Valid" << std::endl;
//         }
//     system("cls");
//     }

//     return 0;
// }
