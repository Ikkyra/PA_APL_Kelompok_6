#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

struct account {
    int id;
    std::string username;
    std::string password;
};

struct settings {
    int pomodoro;
    int short_break;
    int long_break;
    int long_break_interval;
};

account akun[100];
settings sett[100];

std::fstream file;

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

void readCSV(const std::string& filename, settings* sett, int size) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Gagal Membuka File" << std::endl;
        return;
    }

    std::getline(file, line);

    int index = 0;
    while (std::getline(file, line) && index < size) {
        std::stringstream ss(line);
        std::string value;

        try {
            std::getline(ss, value, ',');
            sett[index].pomodoro = std::stoi(value);

            std::getline(ss, value, ',');
            sett[index].short_break = std::stoi(value);

            std::getline(ss, value, ',');
            sett[index].long_break = std::stoi(value);

            std::getline(ss, value, ',');
            sett[index].long_break_interval = std::stoi(value);

            ++index;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data format : " << line << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Number out of range : " << line << std::endl;
        }
    }

    file.close();
}

void tuliscsv(const std::string& filename, const settings* sett, int size) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Gagal Membuka File" << std::endl;
        return;
    }

    file << "Pomodoro, Short Break, Long Break, Long Break Interval\n"; // Menulis header

    for (int i = 0; i < size; ++i) {
        file << sett[i].pomodoro << ", " << sett[i].short_break << ", "
             << sett[i].long_break << ", " << sett[i].long_break_interval << "\n";
    }

    file.close();
}

int main() {
    std::string filename = "dataWaktu.csv";
    int lineCount = countLines(filename);
    
    if (lineCount <= 0) {
        std::cerr << "File Kosong" << std::endl;
        return 1;
    }

    settings* settingsArray = new (std::nothrow) settings[lineCount];
    if (!settingsArray) {
        std::cerr << "Gagal Mengalokasikan Memory" << std::endl;
        return 1;
    }
    readCSV(filename, settingsArray, lineCount);

    std::string pili;
    int y = 0;  
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
            try {
                while (true) {
                    std::cout << "\nMasukkan Nilai Baru: ";
                    if (!(std::cin >> newValue)) {
                        std::cout << "Error: Harap Masukkan Dalam Bentuk Angka!!" << std::endl;
                        std::cin.clear();
                        std::cin.ignore();
                        continue;
                    }

                    if (newValue < 1) {
                        std::cout << "Error: Nilai Tidak Boleh Kurang dari 1" << std::endl;
                        continue;
                    } else {
                        switch(pili[0]) {
                            case '1':
                                sett[y].pomodoro = newValue;
                                break;
                            case '2':
                                sett[y].short_break = newValue;
                                break;
                            case '3':
                                sett[y].long_break = newValue;
                                break;
                            case '4':
                                sett[y].long_break_interval = newValue;
                                break;
                        }
                        throw newValue;
                    }
                }
            } catch (int a) {
                std::cout << "Nilai berhasil diperbarui menjadi: " << a << std::endl;
            }
        } else if (pili == "5") {
            tuliscsv(filename, sett, lineCount);
            break;
        } else {
            std::cout << "Pilihan Tidak Valid" << std::endl;
        }
    }

    delete[] settingsArray;

    return 0;
}
