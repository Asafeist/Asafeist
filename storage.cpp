#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <regex>
#include <cstdlib>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForUser() {
    cout << "Tekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

class Item {
private:
    int id;
    string nama;
    int jumlah;
    double harga;

public:
    Item(int id, const string& nama, int jumlah, double harga) 
        : id(id), nama(nama), jumlah(jumlah), harga(harga) {}

    int getId() const { return id; }
    string getNama() const { return nama; }
    int getJumlah() const { return jumlah; }
    double getHarga() const { return harga; }

    void setId(int newId) { id = newId; }
    void setNama(const string& newNama) { nama = newNama; }
    void setJumlah(int newJumlah) { jumlah = newJumlah; }
    void setHarga(double newHarga) { harga = newHarga; }

    void display() const {
        cout << setw(8) << id << setw(18) << nama << setw(9) << jumlah << setw(15) << fixed << setprecision(2) << harga << "\n";
    }
    static void subDisplay(string nama, int jumlah, double harga) {
        cout << "Nama barang: " << nama << endl;
        cout << "Jumlah barang: " << jumlah << endl;
        cout << "Harga barang: " << harga  << "\n" << endl;
    }
};

class Inventory {
private:
    vector<Item> items;
    int nextId;

public:
    Inventory() : nextId(1) {}

    bool itemExists(int id) const {
        for (const auto& item : items) {
            if (item.getId() == id) {
                return true;
            }
        }
        return false;
    }

    void createItem(const string& nama, int jumlah, double harga) {
        items.emplace_back(nextId++, nama, jumlah, harga);
        cout << "Barang berhasil ditambahkan.\n" << endl;
    }

    void readItems() const {
        if (items.empty()) {
            cout << "Tidak ada barang di dalam penyimpanan." << endl;
            return;
        }
        cout << left << setw(8) << "ID" << setw(18) << "Nama" << setw(9) << "Jumlah" << setw(15) << "Harga" << "\n";
        cout << "-------------------------------------------------------------\n";
        for (const auto& item : items) {
            item.display();
        }
        cout << "-------------------------------------------------------------\n\n";
    }

    void updateItem() {
        if (items.empty()) {
            cout << "Tidak ada barang yang dapat di update." << endl;
            waitForUser();
            return;
        }

        int id;
        string input;
        string nama;
        int jumlah;
        double harga;

        readItems();
        while (true) {
            cout << "Masukkan ID barang atau 'b' untuk kembali ke menu utama: ";
            getline(cin, input);

            if (input == "b" || input == "B") {
                break;
            }

            if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit)) {
                clearScreen();
                readItems();
                cout << "Input tidak valid. Masukkan ID yang benar\n";
                continue;
                }

            try {
                id = stoi(input);
            } catch (const invalid_argument&) {
                    clearScreen();
                    readItems();
                    cout << "Input tidak valid. Masukkan ID yang benar\n";
                    continue;
                }

            if (itemExists(id)) {
                clearScreen();

                for (auto& item : items) {
                    if (item.getId() == id) {
                        nama = item.getNama();
                        jumlah = item.getJumlah();
                        harga = item.getHarga();
                    }
                }

                string choice;
                string input;
                regex valid_choice("[1-5]");

                do {

                    cout << "---------- Data barang terkini ----------\n";
                    Item::subDisplay(nama, jumlah, harga);

                    cout << "\n---------- Edit Barang ----------" << endl;
                    cout << "1. Ubah Nama Barang" << endl;
                    cout << "2. Ubah Jumlah Barang" << endl;
                    cout << "3. Ubah Harga Barang" << endl;
                    cout << "4. Pilih Barang Lain" << endl;
                    cout << "5. Kembali ke Menu Utama" << endl;
                    cout << "\nMasukkan Pilihan Anda: ";
                    getline(cin, choice);

                    if (!regex_match(choice, valid_choice)) {
                        cout << "Pilihan salah. Coba lagi." << endl;
                        waitForUser();
                        clearScreen();
                        continue;
                    }

                    if (choice == "1") {
                        clearScreen();
                        cout << "Nama Lama Barang: " << nama << "\n";

                        while (true) {
                            cout << "\nMasukkan Nama Baru: ";

                            getline(cin, input);
                            input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));

                            if (input.empty()) {
                                clearScreen();
                                cout << "Nama Lama Barang: " << nama << "\n";
                                cout << "Nama tidak boleh kosong." << endl;
                                continue;
                            } else if (input.length() > 18) {
                                clearScreen();
                                cout << "Nama Lama Barang: " << nama << "\n";
                                cout << "Panjang nama tidak boleh lebih dari 18 Karakter." << endl;
                                continue;
                            }

                            clearScreen();
                            nama = input;
                            for (auto& item : items) {
                                if (item.getId() == id) {
                                item.setJumlah(jumlah);
                                }
                            }

                            cout << "Nama Barang Diubah Menjadi " << nama << endl;
                            waitForUser();
                            clearScreen();
                            break;
                        } 
                    } else if (choice == "2") {
                        clearScreen();
                        cout << "Jumlah Lama Barang: " << jumlah << "\n";

                        while (true) {
                            cout << "\nMasukkan Jumlah Baru: ";
                            getline(cin, input);

                            if (input.empty()) {
                                clearScreen();
                                cout << "Jumlah Lama Barang: " << jumlah << "\n";
                                cout << "Jumlah tidak boleh kosong." << endl;
                                continue;
                            } else if (input.length() > 9) {
                                clearScreen();
                                cout << "Jumlah Lama Barang: " << jumlah << "\n";
                                cout << "Jumlah digit tidak boleh melebihi 9." << endl;
                                continue;
                            }

                            try {
                                jumlah = stoi(input);
                                break;
                            } catch (invalid_argument&) {
                                clearScreen();
                                cout << "Jumlah Lama Barang: " << jumlah << "\n";
                                cout << "Input salah. Masukkan harus diawali dengan angka." << endl;                                
                            }
                        }

                        clearScreen();
                        for (auto& item : items) {
                            if (item.getId() == id) {
                            item.setJumlah(jumlah);
                            }
                        }

                        cout << "Jumlah Barang Diubah Menjadi " << jumlah << endl;
                        waitForUser();
                        clearScreen();

                    } else if (choice == "3") {
                        clearScreen();
                        cout << "Harga Lama Barang: " << harga << "\n";
                            
                        while (true) {
                            cout << "\nMasukkan Harga Baru: ";
                            getline(cin, input);

                            if (input.empty()) {
                                clearScreen();
                                cout << "Harga Lama Barang: " << harga << "\n";
                                cout << "Harga tidak boleh kosong." << endl;
                                continue;
                            } else if (input.length() > 15) {
                                clearScreen();
                                cout << "Harga Lama Barang: " << harga << "\n";
                                cout << "Jumlah digit tidak boleh melebihi 15." << endl;
                                continue;
                            }

                            try {
                                harga = stod(input);
                                break;
                            } catch (invalid_argument&) {
                                clearScreen();
                                cout << "Harga Lama Barang: " << harga << "\n";
                                cout << "Input salah. Masukkan harus diawali dengan angka." << endl;
                            }
                        }                           

                        clearScreen();
                        for (auto& item : items) {
                            if (item.getId() == id) {
                                item.setHarga(harga);
                            }
                        }

                        cout << "Harga Barang Diubah Menjadi " << harga << endl;
                        waitForUser();
                        clearScreen();
                            
                    } else if (choice == "4"){
                        for (auto& item : items) {
                            if (item.getId() == id) {
                                item.setNama(nama);
                                item.setJumlah(jumlah);
                                item.setHarga(harga);
                            }
                        }

                        clearScreen();
                        readItems();
                        break;
                    }else if (choice == "5") {
                        for (auto& item : items) {
                            if (item.getId() == id) {
                            item.setNama(nama);
                            item.setJumlah(jumlah);
                            item.setHarga(harga);
                            return;
                            }
                        }
                    } 

                } while (true); 

            }else {
                clearScreen();
                readItems();
                cout << "Barang dengan ID " << id << " tidak ditemukan. Masukkan ID yang benar\n";
            }
        }
    }

    void deleteItem() {
        clearScreen();
        if (items.empty()) {
            cout << "Tidak ada barang di dalam penyimpanan." << endl;
            waitForUser();
            return;
        } else {
            readItems();
        }

        while (true) {
            int id;
            string input;
            cout << "Masukkan ID barang atau 'b' untuk kembali ke menu utama: ";
            getline(cin, input);

            if (input == "b" || input == "B") {
                break;
            }

            if (input.empty() || !all_of(input.begin(), input.end(), ::isdigit)) {
                clearScreen();
                readItems();
                cout << "Input tidak valid. Masukkan ID yang benar\n";
                continue;
                }

            try {
                id = stoi(input);
                auto it = find_if(items.begin(), items.end(), [id](const Item& item) {
                    return item.getId() == id;
                });

                if (it != items.end()) {
                    clearScreen();
                    items.erase(it);
                    cout << "Barang berhasil dihapus." << endl;
                    for (auto& item : items) {
                        if (item.getId() > id) {
                        item.setId(item.getId() - 1);
                        }
                    }
                    nextId--;
                    waitForUser();
                    break;
                } else {
                    clearScreen();
                    readItems();
                    cout << "Barang dengan ID " << id << " tidak ditemukan. Masukkan ID yang benar\n";
                }
            } catch (const invalid_argument&) {
                clearScreen();
                readItems();
                cout << "Input tidak valid. Masukkan ID yang benar atau ketik 'b' untuk kembali." << endl;
            }
        }
    }
};


int main() {
    Inventory inventory;
    string choice;
    regex valid_choice("[1-5]");

    do {
        clearScreen();
        cout << "\n---------- Inventaris ----------" << endl;
        cout << "1. Tambahkan Barang Baru" << endl;
        cout << "2. Lihat List Barang" << endl;
        cout << "3. Edit Data Barang" << endl;
        cout << "4. Hapus Barang" << endl;
        cout << "5. Keluar" << endl;
        cout << "\nMasukkan Pilihan Anda: ";
        getline(cin, choice);

        if (!regex_match(choice, valid_choice)) {
            cout << "Pilihan salah. Coba lagi." << endl;
            waitForUser();
            continue;
        }

        if (choice == "1") {
            clearScreen();
            string nama = "kosong";
            int jumlah = 0;
            double harga = 0.00;
            Item::subDisplay(nama, jumlah, harga);

            while (true) {
                cout << "Masukkan Nama Barang: ";
                string input;
                getline(cin, input);
                input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
                if (input.empty()) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Nama tidak boleh kosong." << endl;
                    continue;
                } else if (input.length() > 18) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Panjang nama tidak boleh lebih dari 18 Karakter." << endl;
                    continue;
                }
                clearScreen();
                nama = input;
                Item::subDisplay(nama, jumlah, harga);
                break;
            }
            
            while (true) {
                cout << "Masukkan Jumlah Barang: ";
                string input;
                getline(cin, input);
                if (input.empty()) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Jumlah tidak boleh kosong." << endl;
                    continue;
                } else if (input.length() > 9) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Jumlah digit tidak boleh melebihi 9." << endl;
                    continue;
                }
                try {
                    clearScreen();
                    jumlah = stoi(input);
                    Item::subDisplay(nama, jumlah, harga);
                    break;
                } catch (invalid_argument&) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Input salah. Masukkan harus diawali dengan angka." << endl;
                }
            }
            while (true) {
                cout << "Masukkan Harga Satuan Barang: ";
                string input;
                getline(cin, input);
                if (input.empty()) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Harga tidak boleh kosong." << endl;
                    continue;
                } else if (input.length() > 15) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Jumlah digit harga tidak boleh melebihi 15" << endl;
                    continue;
                }
                try {
                    clearScreen();
                    harga = stod(input);
                    Item::subDisplay(nama, jumlah, harga);
                    break;
                } catch (invalid_argument&) {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Input salah. Masukkan harus diawali dengan angka." << endl;
                }
            }
            while (true) {
                string input;
                cout << "Apakah anda yakin ingin menambahkan barang? (Y/N) ";
                getline(cin, input);

                if (input == "Y" || input == "y") {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    inventory.createItem(nama, jumlah, harga);
                    waitForUser();
                    break;
                } else if (input == "N" || input == "n") {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Barang tidak ditambahkan." << endl;
                    waitForUser();
                    break;
                } else {
                    clearScreen();
                    Item::subDisplay(nama, jumlah, harga);
                    cout << "Input tidak valid. Silakan masukkan Y atau N.\n";
                    continue;
                }
            }
            
        } else if (choice == "2") {
            clearScreen();
            inventory.readItems();
            waitForUser();
        } else if (choice == "3") {
            clearScreen();
            inventory.updateItem();
        } else if (choice == "4") {
            clearScreen();
            inventory.deleteItem();

        } else if (choice == "5") {
            cout << "Keluar Dari Program." << endl;
        }
    } while (choice != "5");

    return 0;
}
