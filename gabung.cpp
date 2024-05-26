#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <limits> 
#include <atomic>
#include <condition_variable>
#include <iomanip>
#include <windows.h>
#include <conio.h>


std::atomic<bool> running(false);
std::condition_variable cv;
std::mutex mtx;

const int maks_user = 10;
std::string usernames[maks_user];
std::string passwords[maks_user];
int id[maks_user];
int userCount = 0;
std::fstream file;

int allindeks = 0;
struct settings {
    int pomodoro;
    int short_break;
    int long_break;
    int long_break_interval;
};

struct to_do{
    std::string kegiat;
    bool status;
};

to_do to[maks_user][20];
settings sett[maks_user];
int idx = 0;
std::string allid;

void jeda(int detik) {
    std::this_thread::sleep_for(std::chrono::seconds(detik));
}

bool inputSalah(const std::string& input) {
    return !input.empty();
}

void color(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
}

void gotoxy(int x, int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
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

void readCSV(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); 

    idx = 0;
    while (std::getline(file, line) && idx < maks_user) {
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

void readCSVtodo(const std::string &filename){
    std::ifstream file(filename);
    std::string line;
    int row = 0;

    if (file.is_open()) {
        while (std::getline(file, line) && row < 100) {
            std::stringstream ss(line);
            std::string value;
            int col = 0;

            while (std::getline(ss, value, ',') && col < 20) {
                to[row][col].kegiat = value;
                ++col;
            }
            ++row;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file";
    }
}

void writeCSVkeg(const std::string &filename){
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < 100; ++i) {
            if (!to[i][0].kegiat.empty()) {
                for (int j = 0; j < 20; ++j) {
                    if (!to[i][j].kegiat.empty()) {
                        file << to[i][j].kegiat;
                        if (j < 19) file << ",";
                        }
                    }
                file << "\n";

            }else{
                break;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file";
    }
}

void writeCSVstat(const std::string &filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < 100; ++i) {
            if (!to[i][0].kegiat.empty()) { 
                for (int j = 0; j < 20; ++j) {
                    if (!to[i][j].kegiat.empty()) { 
                        file << (to[i][j].status ? "true" : "false");
                        if (j < 19) file << ",";
                    }
                }
                file << "\n";
            } else {
                break; 
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file\n";
    }
}

void readCSVstat(const std::string &filename){
    std::ifstream file(filename);
    std::string line;
    int row = 0;
    if (file.is_open()) {
        // Membaca setiap baris
        while (std::getline(file, line) && row < maks_user) {
            std::stringstream ss(line);
            std::string cell;
            int col = 0;
            // Membaca setiap sel pada baris
            while (std::getline(ss, cell, ',') && col < 20) {
                if (cell == "true") {
                    to[row][col].status = true;
                } else if (cell == "false") {
                    to[row][col].status = false;
                } else {
                    // Penanganan kesalahan jika diperlukan
                    std::cerr << "Invalid value in CSV";
                }
                ++col;
            }
            ++row;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file";
    }
}

std::string trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

void bubbleSort(int length, int indeksk) {
    bool swapped;
    do {
        swapped = false;
        for (int i = 2; i < length; i++) {
            if (to[indeksk][i - 1].kegiat > to[indeksk][i].kegiat) {
                std::swap(to[indeksk][i - 1].kegiat, to[indeksk][i].kegiat);
                std::swap(to[indeksk][i - 1].status, to[indeksk][i].status);
                swapped = true;
            }
        }
        length--; 
    } while (swapped);
}

void registrasi() {
    if (userCount >= maks_user) {
        std::cout << "Tidak bisa mendaftar, kapasitas penuh!\n";
        return;
    }

    std::string username, password;
    std::cout << "Registrasi\n";
    
    do {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Masukkan username: ";
        std::getline(std::cin, username);
        if (trim(username).empty()) {
            std::cout<<"Tidak Bisa Kosong !!!\n";
            jeda(1);
        }
    } while (trim(username).empty());
    
    do {
        std::cout << "Masukkan password: ";
        std::getline(std::cin, password);
        if (trim(password).empty()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"Tidak Bisa Kosong !!!\n";
            jeda(1);
        }
    } while (trim(password).empty());

    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username) {
            std::cout << "Username sudah terdaftar, buat username lain!\n";
            jeda(1);
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

    readCSV("dataWaktu.csv");
    sett[userCount].pomodoro = 5;
    sett[userCount].short_break = 5;
    sett[userCount].long_break = 5;
    sett[userCount].long_break_interval = 5;

    file.open("dataWaktu.csv", std::ios::out | std::ios::app);
    if (file.is_open()) {
        // file << "Pomodoro,Short Break,Long Break,Long Break Interval\n";
        file << sett[userCount].pomodoro << ",";
        file << sett[userCount].short_break << ",";
        file << sett[userCount].long_break << ",";
        file << sett[userCount].long_break_interval << "\n";
        file.close();
    } else {
        std::cerr << "Gagal membuka file dataUser.csv\n";
        return;
    }

    readCSVtodo("kegiatan.csv");
    std::string ubah = std::to_string(userCount + 1);
    to[userCount][0].kegiat = ubah;

    file.open("kegiatan.csv", std::ios::out | std::ios::app);
    if (file.is_open()) {
        // file << "Pomodoro,Short Break,Long Break,Long Break Interval\n";
        file << to[userCount][0].kegiat << ",\n";
        file.close();
    } else {
        std::cerr << "Gagal membuka file dataUser.csv\n";
        return;
    }

    readCSVtodo("status.csv");
    to[userCount][0].status = false;

    file.open("status.csv", std::ios::out | std::ios::app);
    if (file.is_open()) {
        // file << "Pomodoro,Short Break,Long Break,Long Break Interval\n";
        file << (to[userCount][0].status ? "true" : "false") << ",\n";
        file.close();
    } else {
        std::cerr << "Gagal membuka file dataUser.csv\n";
        return;
    }
    
    userCount++;
    std::cout << "Registrasi berhasil!\n";
    jeda(1);
}

bool login() {
    std::string username, password;
    std::cout << "Login\n";
    
    do {
        std::cout << "Masukkan username: ";
        std::cin >> username;
        if (!inputSalah(username)) {
            std::cout << "Username tidak boleh kosong!\n";
            jeda(2);
        }
    } while (!inputSalah(username));
    
    do {
        std::cout << "Masukkan password: ";
        std::cin >> password;
        if (!inputSalah(password)) {
            std::cout << "Password tidak boleh kosong!\n";
            jeda(2);
        }
    } while (!inputSalah(password));

    for (int i = 0; i < userCount; ++i) {
        if (usernames[i] == username && passwords[i] == password) {
            allindeks = i;
            allid = std::to_string(id[i]);
            return true;
        }
    }
    return false;
}

// menulis setting timer
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
            std::cout << "Kembali ke Menu Utama..." << std::endl;
            return 0;
        } else {
            jeda(1);
            std::cout << "Pilihan Tidak Valid" << std::endl;
        }
    system("cls");
    }

}

void timer(int menit, int detik) {
    int detik_total = menit * 60 + detik;
    for (int i = 0; i < detik_total; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!running) return;
        int tersisa = detik_total - i - 1;
        std::cout << "\rWaktu Tersisa: " 
             << std::setw(2) << std::setfill('0') << tersisa / 60 << ":" 
             << std::setw(2) << std::setfill('0') << tersisa % 60 
             << "   " << std::flush; // Untuk Overwrite Character Lebih Yang Terprint
    }
    std::cout << "\nSesi Selesai!" << std::endl;
}

void mulai_timer(int menit, int detik) {
    running = true;
    std::thread(timer, menit, detik).detach();
}

void stop_timer() {
    running = false;
    cv.notify_all();
}

void tampilkan_menu() {
    std::cout << "\nPomodoro Timer\n";
    std::cout << "1. Mulai Sesi Pomodoro\n";
    std::cout << "2. Mulai Istirahat Pendek\n";
    std::cout << "3. Mulai Istirahat Panjang\n";
    std::cout << "4. Hentikan Timer\n";
    std::cout << "5. Keluar\n";
    std::cout << "Masukkan Pilihan Anda: ";
}

void menu_timer() {
    int pilihan;
    int menit_pomodoro = sett[allindeks].pomodoro;
    int menit_istirahat_pendek = sett[allindeks].short_break;
    int menit_istirahat_panjang = sett[allindeks].long_break;

    while(true) {
        tampilkan_menu();
        std::cin >> pilihan;

        switch (pilihan) {
            case 1:
                if (!running) {
                    std::cout << "Mulai Sesi Pomodoro: " << menit_pomodoro << " menit." << std::endl;
                    mulai_timer(menit_pomodoro, 0);
                } else {
                    std::cout << "Timer Sedang Berjalan." << std::endl;
                }
                break;
            case 2:
                if (!running) {
                    std::cout << "Mulai Sesi Istirahat Pendek: " << menit_istirahat_pendek << " menit." << std::endl;
                    mulai_timer(menit_istirahat_pendek, 0);
                } else {
                    std::cout << "Timer Sedang Berjalan." << std::endl;
                }
                break;
            case 3:
                if (!running) {
                    std::cout << "Mulai Sesi Istirahat Panjang: " << menit_istirahat_panjang << " menit." << std::endl;
                    mulai_timer(menit_istirahat_panjang, 0);
                } else {
                    std::cout << "Timer Sedang Berjalan." << std::endl;
                }
                break;
            case 4:
                if (running) {
                    std::cout << "Berhentikan Timer." << std::endl;
                    stop_timer();
                } else {
                    std::cout << "Tidak Ada Timer Yang Berjalan." << std::endl;
                }
                break;
            case 5:
                std::cout << "Kembali ke Menu Utama..." << std::endl;
                stop_timer();
                return;
            default:
                std::cout << "Pilihan Invalid. Tolong Coba Lagi." << std::endl;
                break;
        }
    }
}

int cekindeks(){
    for (int i = 0; i < 100; ++i ){
        if (to[i][0].kegiat == allid){
            return i;
            break;
        }
    }
}

int readindex(int indeksk){
    for (int i = 0; i<20;++i){
        if(to[indeksk][i].kegiat.empty()){
            return i;
            break;
        }
    }
}

// Fungsi untuk menambahkan kegiatan ke dalam file CSV
void tambahKegiatan() {
    int indeksk = cekindeks();
    int indekss = readindex(indeksk);

    std::string namaKegiatan;
    while(true){
        std::cout<<"Masukkan Kegiatan Baru:  ";
        std::getline(std::cin, namaKegiatan);
        if(trim(namaKegiatan).empty()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout<<"Tidak Bisa Kosong !!!";
        }
        else{
            to[indeksk][indekss].kegiat = namaKegiatan;
            to[indeksk][indekss].status = false;
            std::cout << "Kegiatan berhasil ditambahkan!\n";
            break;
        }

    }
}

// Fungsi untuk menampilkan semua kegiatan dari file CSV dalam bentuk tabel
void tampilkanKegiatan() {
    int indeksk = cekindeks();
    int indekss = readindex(indeksk);
    int nomor = 1;
    if(indekss < 2){
        std::cout<<"Belum Ada Kegiatan\n";
    }else{
        bubbleSort(indekss, indeksk);
        std::cout << std::left << std::setw(5) << "\nNo" << "Kegiatan" << std::endl;
        std::cout << "------------------------------" << std::endl;

        for (int i = 1; i < 20; ++i) {
            if (!to[indeksk][i].kegiat.empty()) {
                if(to[indeksk][i].status == true){
                std::cout << "\033[1;32m" << std::left << std::setw(5) << i << to[indeksk][i].kegiat << "\033[0m" << std::endl;
                }else{
                    std::cout << std::left << std::setw(5) << i << to[indeksk][i].kegiat << std::endl;

                }
            }
        }

    }
}

void ubahkegiatan(){
    int indeksk = cekindeks();
    int indekss = readindex(indeksk);
    if(indekss < 2){
        std::cout<<"Belum Ada Kegiatan\n";
    }else{
        std::cout << std::left << std::setw(5) << "\nNo" << "Kegiatan" << std::endl;
        std::cout << "------------------------------" << std::endl;
        for (int i = 1; i < 20; ++i) {
            if (!to[indeksk][i].kegiat.empty()) {
                std::cout << std::left << std::setw(5) << i << to[indeksk][i].kegiat << std::endl;
            }
        }
        int nomor;
        std::cout << "Pilih Nomor Yang Ingin Diubah: ";
        while (!(std::cin >> nomor) || nomor < 1 || nomor >= 20 || to[indeksk][nomor].kegiat.empty()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nomor tidak valid. Silakan masukkan nomor yang benar: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string ubah;
        while(true){
            std::cout<<"Masukkan Kegiatan Baru:  ";
            std::getline(std::cin, ubah);
            if(trim(ubah).empty()){
                std::cin.clear();
                std::cout<<"Tidak Bisa Kosong !!!";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else{
                to[indeksk][nomor].kegiat = ubah;
                to[indeksk][nomor].status = false;
                std::cout << "Kegiatan berhasil diubah!\n";
                break;
            }

        }
    }
}

void delkegiatan(){
    int indeksk = cekindeks();
    int indekss = readindex(indeksk);
        if(indekss < 2){
        std::cout<<"Belum Ada Kegiatan\n";
    }else{

        std::cout << std::left << std::setw(5) << "\nNo" << "Kegiatan" << std::endl;
        std::cout << "------------------------------" << std::endl;
        for (int i = 1; i < 20; ++i) {
            if (!to[indeksk][i].kegiat.empty()) {
                std::cout << std::left << std::setw(5) << i << to[indeksk][i].kegiat << std::endl;
            }
        }
        int nomor;
        std::cout << "Pilih Nomor Yang Ingin Dihapus: ";
        while (!(std::cin >> nomor) || nomor < 1 || nomor >= 20 || to[indeksk][nomor].kegiat.empty()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nomor tidak valid. Silakan masukkan nomor yang benar: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int j = nomor; j < indekss ; j++) {
            to[indeksk][j].kegiat = to[indeksk][j + 1].kegiat;
            to[indeksk][j].status = to[indeksk][j + 1].status;
    }
    }
}

void ceklis(){
    int indeksk = cekindeks();
    int indekss = readindex(indeksk);
        if(indekss < 2){
        std::cout<<"Belum Ada Kegiatan\n";
    }else{
        std::cout << std::left << std::setw(5) << "\nNo" << "Kegiatan" << std::endl;
        std::cout << "------------------------------" << std::endl;
        for (int i = 1; i < 20; ++i) {
            if (!to[indeksk][i].kegiat.empty()) {
                std::cout << std::left << std::setw(5) << i << to[indeksk][i].kegiat << std::endl;
            }
        }
        int nomor;
        std::cout << "Pilih Nomor Yang Ingin Dichecklist: ";
        while (!(std::cin >> nomor) || nomor < 1 || nomor >= 20 || to[indeksk][nomor].kegiat.empty()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Nomor tidak valid. Silakan masukkan nomor yang benar: ";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        to[indeksk][nomor].status = true;
        std::cout << "Kegiatan berhasil dichecklist!\n";
    }
    
}

void menu_todo(){

    int Set[] = {7,7,7,7,7,7}; // Warna default
    int counter = 3;
    char key;

    for(int i=0;;) 
    {

        system("cls");
        gotoxy(10,5);
        color(Set[0]);
        std::cout << "1. Lihat To-Do";

        gotoxy(10,6);
        color(Set[1]);
        std::cout << "2. Tulis To-Do";

        gotoxy(10,7);
        color(Set[2]);
        std::cout << "3. Checked To-Do";

        gotoxy(10,8);
        color(Set[3]);
        std::cout << "4. Ubah To-Do";

        gotoxy(10,9);
        color(Set[4]);
        std::cout << "5. Hapus To-Do";

        gotoxy(10,10);
        color(Set[5]);
        std::cout << "6. Kembali";

        key = _getch();

        if(key == 72 && (counter > 1)) // 72 itu value ASCII dari up arrow
        {
            counter--;
        }
        if(key == 80 && (counter < 6)) // 80 itu value ASCII dari down arrow
        {
            counter++;
        }
        if(key == '\r') // '\r' itu value ASCII dari enter 
        { 
            system("cls");
            switch(counter){
                case 1: 
                    // jika diklik enter dan highlight menu di nomor 1 maka program mulai dari sini
                    tampilkanKegiatan();
                    writeCSVkeg("kegiatan.csv");
                    writeCSVstat("status.csv");
                    break;
                case 2: 
                    tambahKegiatan();
                    writeCSVkeg("kegiatan.csv");
                    writeCSVstat("status.csv");
                    break;
                case 3: 
                    ceklis();
                    writeCSVstat("status.csv");
                    break;
                case 4: 
                    ubahkegiatan();
                    writeCSVkeg("kegiatan.csv");
                    writeCSVkeg("status.csv");
                    break;
                case 5: 
                    delkegiatan();
                    writeCSVkeg("kegiatan.csv");
                    writeCSVstat("status.csv");
                    jeda(1);
                    break;
                case 6: 
                    std::cout << "Anda Memilih Kembali";
                    jeda(1);
                    return;
            }
            system("pause");
        } 

        Set[0] = 7;
        Set[1] = 7;
        Set[2] = 7;
        Set[3] = 7;
        Set[4] = 7;
        Set[5] = 7;
        
        if(counter == 1) 
        {
            Set[0] = 12;
        }
        if(counter == 2) 
        {
            Set[1] = 12;
        }
        if(counter == 3) 
        {
            Set[2] = 12;
        }
        if(counter == 4) 
        {
            Set[3] = 12;
        }
        if(counter == 5) 
        {
            Set[4] = 12;
        }
        if(counter == 6) 
        {
            Set[5] = 12;
        }


    }


}

void loadalldata(){
    loadDataFromCSV();
    readCSV("dataWaktu.csv");
    readCSVstat("status.csv");
    readCSVtodo("kegiatan.csv");
}

void menu_utama() {
    loadalldata();
    int pilihan;

    do {
        // system("clear");
        system("cls");
        std::cout << "-------------------------" << std::endl;
        std::cout << "|       POMOFOCUS       |" << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "[1] Pomodoro             " << std::endl;
        std::cout << "[2] To-Do                " << std::endl;
        std::cout << "[3] Settings             " << std::endl;
        std::cout << "[4] Exit                 " << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "Pilihan: ";
        std::cin >> pilihan;

        switch(pilihan) {
            case 1:
                // system("clear");
                system("cls");
                menu_timer();
                break;
            case 2:
                // system("clear");   
                system("cls"); 
                menu_todo();
                break;
            case 3:
                // system("clear");   
                system("cls"); 
                menu_setting();
                break;
            case 4:  
                // system("clear");   
                system("cls");
                break;
            default:
                std::cout<<"Pilihan Tidak Valid !!!\n";
                jeda(1);
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        } 
    } while(pilihan != 4);
}

int main() {
    loadDataFromCSV();

    int pilihan;
    while (true) {
        // system("clear");
        system("cls");
        std::cout << "-------------------------" << std::endl;
        std::cout << "|     SELAMAT DATANG    |" << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "[1] Registrasi           " << std::endl;
        std::cout << "[2] Login                " << std::endl;
        std::cout << "[3] Keluar               " << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "Pilihan Opsi: ";
        std::cin >> pilihan;
        
        switch (pilihan) {
            case 1:
                registrasi();
                break;
            case 2:
                if (login()) {
                    // system("clear");
                    system("cls");
                    std::cout << "Login berhasil!\n";
                    jeda(1);
                    menu_utama();
                } else {
                    std::cout << "Username atau password salah!\n";
                    jeda(1);
                }
                break;
            case 3:
                return 0;
            default:
                std::cout << "Pilihan tidak valid!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                jeda(1);
        }
    }
    return 0;
}