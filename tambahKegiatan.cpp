#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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

// Fungsi untuk membaca semua kegiatan dari file CSV
std::vector<std::string> bacaKegiatan(const std::string& namaFile) {
    std::ifstream file(namaFile);
    std::vector<std::string> kegiatan;

    if (!file.is_open()) {
        std::cerr << "Gagal membuka file untuk membaca!" << std::endl;
        return kegiatan;
    }

    std::string namaKegiatan;
    while (std::getline(file, namaKegiatan)) {
        kegiatan.push_back(namaKegiatan);
    }

    if (file.bad()) {
        std::cerr << "Terjadi kesalahan saat membaca file!" << std::endl;
    }

    file.close();
    return kegiatan;
}

// Fungsi untuk menampilkan semua kegiatan dalam bentuk tabel
void tampilkanKegiatan(const std::vector<std::string>& kegiatan) {
    std::cout << std::left << std::setw(5) << "No" << "Kegiatan" << std::endl;
    std::cout << "------------------------------" << std::endl;

    int nomor = 1;
    for (const auto& namaKegiatan : kegiatan) {
        std::cout << std::left << std::setw(5) << nomor << namaKegiatan << std::endl;
        nomor++;
    }
}

int main() {
    std::string namaFile = "kegiatan.csv";

    tambahKegiatan(namaFile);

    std::vector<std::string> kegiatan = bacaKegiatan(namaFile);
    std::sort(kegiatan.begin(), kegiatan.end());

    tampilkanKegiatan(kegiatan);

    return 0;
}
