#include <iostream>
#include <thread>
#include <chrono>

void jeda(int detik) {
    std::this_thread::sleep_for(std::chrono::seconds(detik));
}

int main() {
    int pilihan;

    do {
        system("clear");
        // system("cls");
        std::cout << "-------------------------" << std::endl;
        std::cout << "|       POMOFOCUS       |" << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "[1] Pomodoro             " << std::endl;
        std::cout << "[2] Login                " << std::endl;
        std::cout << "[3] Settings             " << std::endl;
        std::cout << "[4] Exit                 " << std::endl;
        std::cout << "=========================" << std::endl;
        std::cout << "Pilihan: ";
        std::cin >> pilihan;

        switch(pilihan) {
            case 1:
        }
        
    } while(pilihan != 4);
}
