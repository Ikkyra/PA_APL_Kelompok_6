#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

// Fungsi untuk menambahkan kegiatan ke dalam file CSV
void tambahKegiatan(const std::string& namaFile) {
    std::ofstream file(namaFile, std::ios::app); // Membuka file dalam mode append

    if (!file.is_open()) {
        std::cerr << "Gagal membuka file untuk menulis!" << std::endl;
        return;
    }

    std::string namaKegiatan;
    
    std::cout << "Masukkan nama kegiatan: ";
    std::getline(std::cin, namaKegiatan);
    
    // Menulis data ke file CSV
    file << namaKegiatan << "\n";
    if (file.fail()) {
        std::cerr << "Gagal menulis ke file!" << std::endl;
    } else {
        std::cout << "Kegiatan berhasil ditambahkan!" << std::endl;
    }

    file.close();
}

// Fungsi untuk menampilkan semua kegiatan dari file CSV dalam bentuk tabel
void tampilkanKegiatan(const std::string& namaFile) {
    std::ifstream file(namaFile);

    if (!file.is_open()) {
        std::cerr << "Gagal membuka file untuk membaca!" << std::endl;
        return;
    }

    std::string namaKegiatan;
    int nomor = 1;

    std::cout << std::left << std::setw(5) << "No" << "Kegiatan" << std::endl;
    std::cout << "------------------------------" << std::endl;

    while (std::getline(file, namaKegiatan)) {
        std::cout << std::left << std::setw(5) << nomor << namaKegiatan << std::endl;
        nomor++;
    }
    
    if (file.bad()) {
        std::cerr << "Terjadi kesalahan saat membaca file!" << std::endl;
    }

    file.close();
}

int main() {
    std::string namaFile = "kegiatan.csv";

    try {
        tambahKegiatan(namaFile);
        tampilkanKegiatan(namaFile);
    } catch (const std::exception& e) {
        std::cerr << "Kesalahan: " << e.what() << std::endl;
    }

    return 0;
}
