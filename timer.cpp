#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <iomanip>

std::atomic<bool> running(false);
std::condition_variable cv;
std::mutex mtx;

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
    int menit_pomodoro = 5;
    int menit_istirahat_pendek = 2;
    int menit_istirahat_panjang = 3;

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

// int main() {
//     int pilihan;
//     int menit_pomodoro = 5;
//     int menit_istirahat_pendek = 2;
//     int menit_istirahat_panjang = 3;

//     while (true) {
//         tampilkan_menu();
//         std::cin >> pilihan;

//         switch (pilihan) {
//             case 1:
//                 if (!running) {
//                     std::cout << "Mulai Sesi Pomodoro: " << menit_pomodoro << " menit." << std::endl;
//                     mulai_timer(menit_pomodoro, 0);
//                 } else {
//                     std::cout << "Timer Sedang Berjalan." << std::endl;
//                 }
//                 break;
//             case 2:
//                 if (!running) {
//                     std::cout << "Mulai Sesi Istirahat Pendek: " << menit_istirahat_pendek << " menit." << std::endl;
//                     mulai_timer(menit_istirahat_pendek, 0);
//                 } else {
//                     std::cout << "Timer Sedang Berjalan." << std::endl;
//                 }
//                 break;
//             case 3:
//                 if (!running) {
//                     std::cout << "Mulai Sesi Istirahat Panjang: " << menit_istirahat_panjang << " menit." << std::endl;
//                     mulai_timer(menit_istirahat_panjang, 0);
//                 } else {
//                     std::cout << "Timer Sedang Berjalan." << std::endl;
//                 }
//                 break;
//             case 4:
//                 if (running) {
//                     std::cout << "Berhentikan Timer." << std::endl;
//                     stop_timer();
//                 } else {
//                     std::cout << "Tidak Ada Timer Yang Berjalan." << std::endl;
//                 }
//                 break;
//             case 5:
//                 std::cout << "Keluar..." << std::endl;
//                 stop_timer();
//                 return 0;
//             default:
//                 std::cout << "Pilihan Invalid. Tolong Coba Lagi." << std::endl;
//                 break;
//         }
//     }

//     return 0;
// }