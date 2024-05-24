#include <iostream>

#include "registrasi.cpp"
#include "timer.cpp"

void menu_utama() {
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
                    jeda(2);
                    menu_utama();
                } else {
                    std::cout << "Username atau password salah!\n";
                    jeda(2);
                }
                break;
            case 3:
                return 0;
            default:
                std::cout << "Pilihan tidak valid!\n";
                jeda(2);
        }
    }
    return 0;
}