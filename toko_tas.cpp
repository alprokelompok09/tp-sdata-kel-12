#include "toko_tas.h"

// ============================================================================
// 1. UTILITY & KTP
// ============================================================================

void getCopyTanggal(char* buffer, size_t size) {
    time_t now = time(0);
    struct tm* t = localtime(&now);
    snprintf(buffer, size, "%02d/%02d/%04d %02d:%02d",
             t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
             t->tm_hour, t->tm_min);
}

void createNodeKTP(NodeKTP*& baru, const char* nik, const char* nama, const char* ttl,
                   const char* jk, const char* alamat, const char* agama,
                   const char* status, const char* pekerjaan, const char* wni, const char* berlaku) {
    baru = new NodeKTP;
    strncpy(baru->data.nik, nik, 50);
    strncpy(baru->data.nama, nama, 100);
    strncpy(baru->data.tempat_tgl_lahir, ttl, 100);
    strncpy(baru->data.jenis_kelamin, jk, 20);
    strncpy(baru->data.alamat, alamat, 200);
    strncpy(baru->data.agama, agama, 20);
    strncpy(baru->data.status_perkawinan, status, 20);
    strncpy(baru->data.pekerjaan, pekerjaan, 50);
    strncpy(baru->data.kewarganegaraan, wni, 50);
    strncpy(baru->data.berlaku_hingga, berlaku, 50);
    baru->prev = NULL;
    baru->next = NULL;
}

void insertLastKTP(NodeKTP*& head, NodeKTP*& tail, const char* nik, const char* nama,
                   const char* ttl, const char* jk, const char* alamat, const char* agama,
                   const char* status, const char* pekerjaan, const char* wni, const char* berlaku) {
    NodeKTP* baru = NULL;
    createNodeKTP(baru, nik, nama, ttl, jk, alamat, agama, status, pekerjaan, wni, berlaku);
    if (!head) {
        head = tail = baru;
    } else {
        tail->next = baru;
        baru->prev = tail;
        tail = baru;
    }
}

void displayForwardKTP(NodeKTP* head) {
    if (!head) {
        cout << "Data KTP kosong.\n";
        return;
    }
    cout << "\n=== DATA KTP (MAJU) ===\n";
    NodeKTP* temp = head;
    while (temp) {
        cout << "\n--- Data KTP ---\n";
        cout << "NIK                 : " << temp->data.nik << endl;
        cout << "Nama                : " << temp->data.nama << endl;
        cout << "Tempat/Tgl Lahir    : " << temp->data.tempat_tgl_lahir << endl;
        cout << "Jenis Kelamin       : " << temp->data.jenis_kelamin << endl;
        cout << "Alamat              : " << temp->data.alamat << endl;
        cout << "Agama               : " << temp->data.agama << endl;
        cout << "Status Perkawinan   : " << temp->data.status_perkawinan << endl;
        cout << "Pekerjaan           : " << temp->data.pekerjaan << endl;
        cout << "Kewarganegaraan     : " << temp->data.kewarganegaraan << endl;
        cout << "Berlaku Hingga      : " << temp->data.berlaku_hingga << endl;
        cout << "----------------\n";
        temp = temp->next;
    }
}

void cariKTPByNIK(NodeKTP* head, const char* nik) {
    NodeKTP* temp = head;
    while (temp) {
        if (strcmp(temp->data.nik, nik) == 0) {
            cout << "\n--- Data KTP Ditemukan ---\n";
            cout << "NIK                 : " << temp->data.nik << endl;
            cout << "Nama                : " << temp->data.nama << endl;
            cout << "Tempat/Tgl Lahir    : " << temp->data.tempat_tgl_lahir << endl;
            cout << "Jenis Kelamin       : " << temp->data.jenis_kelamin << endl;
            cout << "Alamat              : " << temp->data.alamat << endl;
            cout << "Agama               : " << temp->data.agama << endl;
            cout << "Status Perkawinan   : " << temp->data.status_perkawinan << endl;
            cout << "Pekerjaan           : " << temp->data.pekerjaan << endl;
            cout << "Kewarganegaraan     : " << temp->data.kewarganegaraan << endl;
            cout << "Berlaku Hingga      : " << temp->data.berlaku_hingga << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Data KTP dengan NIK " << nik << " tidak ditemukan.\n";
}

void hapusSemuaKTP(NodeKTP*& head, NodeKTP*& tail) {
    while (head) {
        NodeKTP* temp = head;
        head = head->next;
        delete temp;
    }
    tail = NULL;
}

// ============================================================================
// 2. USER & AUTHENTICATION
// ============================================================================

User* loadUsersFromFile() {
    ifstream file("users.txt");
    if (!file.is_open()) return NULL;

    User* head = NULL;
    User* tail = NULL;

    while (!file.eof()) {
        User* baru = new User;
        file.getline(baru->username, MAX_USERNAME, '|');
        if (file.eof()) { delete baru; break; }
        file.getline(baru->password, MAX_PASSWORD, '|');
        file.getline(baru->email, MAX_NAMA, '|');
        file.getline(baru->role, MAX_ROLE, '|');
        file.getline(baru->nik, 50, '|');
        file.getline(baru->tanggal_daftar, 30, '|');
        char aktifStr[10];
        file.getline(aktifStr, 10, '\n');
        baru->aktif = (strcmp(aktifStr, "1") == 0);
        baru->next = NULL;

        if (!head) {
            head = tail = baru;
        } else {
            tail->next = baru;
            tail = baru;
        }
    }
    file.close();
    return head;
}

void saveUsersToFile(User* head) {
    ofstream file("users.txt", ios::trunc);
    User* temp = head;
    while (temp) {
        file << temp->username << "|"
             << temp->password << "|"
             << temp->email << "|"
             << temp->role << "|"
             << temp->nik << "|"
             << temp->tanggal_daftar << "|"
             << (temp->aktif ? "1" : "0") << "\n";
        temp = temp->next;
    }
    file.close();
}

User* loginSystem(User* head, const char* username, const char* password) {
    User* temp = head;
    while (temp) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0) {
            if (temp->aktif) {
                return temp;
            } else {
                cout << "Akun tidak aktif.\n";
                return NULL;
            }
        }
        temp = temp->next;
    }
    cout << "Username atau password salah.\n";
    return NULL;
}

User* loginByNIK(User* head, const char* nik, const char* password) {
    User* temp = head;
    while (temp) {
        if (strcmp(temp->nik, nik) == 0 && strcmp(temp->password, password) == 0) {
            if (temp->aktif) {
                return temp;
            } else {
                cout << "Akun tidak aktif.\n";
                return NULL;
            }
        }
        temp = temp->next;
    }
    cout << "NIK atau password salah.\n";
    return NULL;
}

void registerUser(User*& head) {
    User* baru = new User;
    cout << "\n=== REGISTER USER BARU ===\n";
    cout << "Username: "; cin.getline(baru->username, MAX_USERNAME);
    cout << "Password: "; cin.getline(baru->password, MAX_PASSWORD);
    cout << "Email: "; cin.getline(baru->email, MAX_NAMA);
    cout << "NIK: "; cin.getline(baru->nik, 50);
    
    strcpy(baru->role, "user");
    getCopyTanggal(baru->tanggal_daftar, 30);
    baru->aktif = true;
    baru->next = NULL;

    if (!head) {
        head = baru;
    } else {
        User* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }

    saveUsersToFile(head);
    cout << "\nRegistrasi berhasil! Role: " << baru->role << "\n";
}

void menuLogin(User*& head, User*& loggedInUser) {
    int pil = 0;
    while (loggedInUser == NULL) {
        cout << "\n================================\n";
        cout << "   SISTEM INVENTARIS TOKO TAS  LES CATINO\n";
        cout << "================================\n";
        cout << "1. Login\n";
        cout << "2. Register (Daftar Baru)\n";
        cout << "3. Cek Data KTP (by NIK)\n";
        cout << "4. Tambah Data KTP Baru\n";
        cout << "5. Keluar Aplikasi\n";
        cout << "Pilihan: ";
        cin >> pil; cin.ignore();

        if (pil == 1) {
            cout << "\n=== LOGIN SYSTEM ===\n";
            cout << "Login menggunakan:\n";
            cout << "1. Username\n";
            cout << "2. NIK\n";
            cout << "Pilihan: ";
            int loginType;
            cin >> loginType; cin.ignore();

            if (loginType == 1) {
                char username[MAX_USERNAME], password[MAX_PASSWORD];
                cout << "Username: "; cin.getline(username, MAX_USERNAME);
                cout << "Password: "; cin.getline(password, MAX_PASSWORD);
                loggedInUser = loginSystem(head, username, password);
            } else if (loginType == 2) {
                char nik[50], password[MAX_PASSWORD];
                cout << "NIK: "; cin.getline(nik, 50);
                cout << "Password: "; cin.getline(password, MAX_PASSWORD);
                loggedInUser = loginByNIK(head, nik, password);
            }

            if (loggedInUser) {
                cout << "\n? Login berhasil!\n";
                cout << "Selamat datang, " << loggedInUser->username << "!\n";
                cout << "NIK: " << loggedInUser->nik << "\n";
            }
        } else if (pil == 2) {
            registerUser(head);
        } else if (pil == 3) {
            NodeKTP* head_ktp = NULL;
            NodeKTP* tail_ktp = NULL;
            
            // Load data KTP dummy
            insertLastKTP(head_ktp, tail_ktp, "3201010101010001", "Admin User", "Jakarta, 01/01/1990",
                         "Laki-laki", "Jl. Sudirman No. 1", "Islam", "Kawin", "Karyawan Swasta", "WNI", "Seumur Hidup");
            insertLastKTP(head_ktp, tail_ktp, "3201010101010002", "User Biasa", "Bandung, 15/05/1995",
                         "Perempuan", "Jl. Asia Afrika No. 50", "Kristen", "Belum Kawin", "Mahasiswa", "WNI", "Seumur Hidup");
            
            char nik[50];
            cout << "Masukkan NIK: "; cin.getline(nik, 50);
            cariKTPByNIK(head_ktp, nik);
            
            hapusSemuaKTP(head_ktp, tail_ktp);
        } else if (pil == 4) {
            // Implementasi tambah KTP baru
            cout << "\n[Fitur Tambah KTP Baru - Dalam Pengembangan]\n";
        } else if (pil == 5) {
            cout << "Terima kasih.\n";
            exit(0);
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }
}

// ============================================================================
// 3. LOG AKTIVITAS
// ============================================================================

void tambahLog(LogAktivitas*& h, const char* u, const char* a) {
    LogAktivitas* baru = new LogAktivitas;
    getCopyTanggal(baru->timestamp, 30);
    strncpy(baru->username, u, MAX_USERNAME);
    strncpy(baru->aktivitas, a, 200);
    baru->next = h;
    h = baru;
}

// ============================================================================
// 4. TAS (INVENTARIS)
// ============================================================================

void tambahTas(Tas*& head, const char* sku, const char* nama, double harga, int stok, const char* kategori) {
    Tas* baru = new Tas;
    strncpy(baru->sku, sku, MAX_SKU);
    strncpy(baru->nama, nama, MAX_NAMA);
    baru->harga = harga;
    baru->stok = stok;
    strncpy(baru->kategori, kategori, MAX_KATEGORI);
    baru->next = head;
    head = baru;
}

void tampilkanInventaris(Tas* head) {
    if (!head) {
        cout << "Inventaris kosong.\n";
        return;
    }

    cout << "\n=== DAFTAR INVENTARIS TAS ===\n";
    cout << left << setw(10) << "SKU"
         << setw(30) << "Nama"
         << setw(15) << "Harga"
         << setw(10) << "Stok"
         << setw(15) << "Kategori" << endl;
    cout << string(80, '=') << endl;

    Tas* temp = head;
    while (temp) {
        cout << left << setw(10) << temp->sku
             << setw(30) << temp->nama
             << setw(15) << fixed << setprecision(2) << temp->harga
             << setw(10) << temp->stok
             << setw(15) << temp->kategori << endl;
        temp = temp->next;
    }
}

void cariTas(Tas* head, const char* keyword) {
    bool found = false;
    cout << "\n=== CARI TAS ===\n";
    cout << "Kata kunci: " << keyword << "\n\n";
    cout << "Hasil:\n";
    cout << left << setw(10) << "SKU"
         << setw(30) << "Nama"
         << setw(15) << "Harga"
         << setw(10) << "Stok"
         << setw(15) << "Kategori" << endl;
    cout << string(80, '=') << endl;

    Tas* temp = head;
    while (temp) {
        if (strstr(temp->sku, keyword) || strstr(temp->nama, keyword) || strstr(temp->kategori, keyword)) {
            cout << left << setw(10) << temp->sku
                 << setw(30) << temp->nama
                 << setw(15) << fixed << setprecision(2) << temp->harga
                 << setw(10) << temp->stok
                 << setw(15) << temp->kategori << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "Tidak ada hasil.\n";
    }
}

void filterTasByHarga(Tas* head, double min, double max) {
    bool found = false;
    cout << "\n=== FILTER HARGA ===\n";
    cout << "Min: " << min << "\nMax: " << max << "\n\n";
    cout << "Hasil:\n";
    cout << left << setw(10) << "SKU"
         << setw(30) << "Nama"
         << setw(15) << "Harga"
         << setw(10) << "Stok"
         << setw(15) << "Kategori" << endl;
    cout << string(80, '=') << endl;

    Tas* temp = head;
    while (temp) {
        if (temp->harga >= min && temp->harga <= max) {
            cout << left << setw(10) << temp->sku
                 << setw(30) << temp->nama
                 << setw(15) << fixed << setprecision(2) << temp->harga
                 << setw(10) << temp->stok
                 << setw(15) << temp->kategori << endl;
            found = true;
        }
        temp = temp->next;
    }

    if (!found) {
        cout << "Tidak ada hasil.\n";
    }
}

Tas* cariTasBySKU(Tas* head, const char* sku) {
    Tas* temp = head;
    while (temp) {
        if (strcmp(temp->sku, sku) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void updateStok(Tas* head, const char* sku, int jumlah) {
    Tas* tas = cariTasBySKU(head, sku);
    if (tas) {
        tas->stok += jumlah;
        cout << "Stok " << tas->nama << " diupdate. Stok baru: " << tas->stok << "\n";
    } else {
        cout << "Tas dengan SKU " << sku << " tidak ditemukan.\n";
    }
}

void hapusSemuaTas(Tas*& head) {
    while (head) {
        Tas* temp = head;
        head = head->next;
        delete temp;
    }
}

// ============================================================================
// 5. TRANSAKSI
// ============================================================================

void buatTransaksi(Transaksi*& head_trans, Tas*& head_tas, User* user, int& id_counter) {
    cout << "\n=== BUAT TRANSAKSI BARU ===\n";
    
    Transaksi* trans = new Transaksi;
    trans->id_transaksi = id_counter++;
    strncpy(trans->username_pembeli, user->username, MAX_USERNAME);
    getCopyTanggal(trans->tanggal, 30);
    trans->total_harga = 0;
    trans->diskon_persen = 0;
    trans->status = PENDING;
    trans->head_item = NULL;
    trans->next = NULL;

    cout << "Berapa jenis tas yang ingin dibeli? ";
    int jumlah_jenis;
    cin >> jumlah_jenis; cin.ignore();

    for (int i = 0; i < jumlah_jenis; i++) {
        char sku[MAX_SKU];
        int jumlah;

        cout << "\nItem ke-" << (i+1) << ":\n";
        cout << "SKU Tas: "; cin.getline(sku, MAX_SKU);
        cout << "Jumlah: "; cin >> jumlah; cin.ignore();

        Tas* tas = cariTasBySKU(head_tas, sku);
        if (tas) {
            if (tas->stok >= jumlah) {
                ItemTransaksi* item = new ItemTransaksi;
                strncpy(item->sku, tas->sku, MAX_SKU);
                strncpy(item->nama_tas, tas->nama, MAX_NAMA);
                item->harga = tas->harga;
                item->jumlah = jumlah;
                item->subtotal = tas->harga * jumlah;
                item->next = trans->head_item;
                trans->head_item = item;

                trans->total_harga += item->subtotal;
                tas->stok -= jumlah;

                cout << "Item ditambahkan: " << tas->nama << " x" << jumlah << "\n";
            } else {
                cout << "Stok tidak cukup! Stok tersedia: " << tas->stok << "\n";
            }
        } else {
            cout << "Tas dengan SKU " << sku << " tidak ditemukan.\n";
        }
    }

    cout << "\nApakah ada diskon (%)? ";
    cin >> trans->diskon_persen; cin.ignore();

    double diskon_nominal = trans->total_harga * (trans->diskon_persen / 100.0);
    trans->total_akhir = trans->total_harga - diskon_nominal;
    trans->status = SELESAI;

    // Tambahkan ke linked list transaksi
    trans->next = head_trans;
    head_trans = trans;

    cout << "\n=== DETAIL TRANSAKSI ===\n";
    cout << "ID Transaksi: " << trans->id_transaksi << "\n";
    cout << "Pembeli: " << trans->username_pembeli << "\n";
    cout << "Tanggal: " << trans->tanggal << "\n";
    cout << "Total Harga: Rp" << fixed << setprecision(2) << trans->total_harga << "\n";
    cout << "Diskon: " << trans->diskon_persen << "% (Rp" << diskon_nominal << ")\n";
    cout << "Total Akhir: Rp" << trans->total_akhir << "\n";
    cout << "Status: SELESAI\n";
}

void tampilkanTransaksi(Transaksi* head) {
    if (!head) {
        cout << "Tidak ada transaksi.\n";
        return;
    }

    cout << "\n=== RIWAYAT TRANSAKSI ===\n";
    Transaksi* temp = head;
    while (temp) {
        cout << "\nID: " << temp->id_transaksi << " | Pembeli: " << temp->username_pembeli 
             << " | Tanggal: " << temp->tanggal << "\n";
        cout << "Total: Rp" << fixed << setprecision(2) << temp->total_akhir 
             << " | Status: " << (temp->status == SELESAI ? "SELESAI" : "PENDING") << "\n";
        
        ItemTransaksi* item = temp->head_item;
        while (item) {
            cout << "  - " << item->nama_tas << " (x" << item->jumlah 
                 << ") = Rp" << item->subtotal << "\n";
            item = item->next;
        }
        cout << "---\n";
        temp = temp->next;
    }
}

void tampilkanTransaksiUser(Transaksi* head, const char* username) {
    cout << "\n================================\n";
    cout << "   RIWAYAT TRANSAKSI SAYA\n";
    cout << "================================\n";
    
    Transaksi* temp = head;
    bool ada = false;
    int no = 1;
    
    while (temp) {
        if (strcmp(temp->username_pembeli, username) == 0) {
            ada = true;
            cout << "\n[" << no++ << "] ID Transaksi: " << temp->id_transaksi << "\n";
            cout << "Tanggal: " << temp->tanggal << "\n";
            cout << "Total Harga: Rp" << fixed << setprecision(2) << temp->total_harga << "\n";
            cout << "Diskon: " << temp->diskon_persen << "%\n";
            cout << "Total Akhir: Rp" << temp->total_akhir << "\n";
            cout << "Status: ";
            if (temp->status == SELESAI) cout << "SELESAI\n";
            else if (temp->status == DIBATALKAN) cout << "DIBATALKAN\n";
            else cout << "PENDING\n";
            
            cout << "Item:\n";
            ItemTransaksi* item = temp->head_item;
            while (item) {
                cout << "  - " << item->nama_tas << " (SKU: " << item->sku << ")\n";
                cout << "    Harga: Rp" << item->harga << " x " << item->jumlah 
                     << " = Rp" << item->subtotal << "\n";
                item = item->next;
            }
        }
        temp = temp->next;
    }
    
    if (!ada) {
        cout << "\nAnda belum memiliki transaksi.\n";
    }
}


void laporanPenjualan(Transaksi* head, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Gagal membuat file laporan.\n";
        return;
    }
    
    file << "========================================\n";
    file << "   LAPORAN PENJUALAN TOKO TAS LES CATINO\n";
    file << "========================================\n\n";
    
    time_t now = time(0);
    char* dt = ctime(&now);
    file << "Tanggal Cetak: " << dt << "\n";
    
    Transaksi* temp = head;
    int total_transaksi = 0;
    double total_pendapatan = 0;
    
    while (temp) {
        file << "\n--- Transaksi #" << temp->id_transaksi << " ---\n";
        file << "Pembeli: " << temp->username_pembeli << "\n";
        file << "Tanggal: " << temp->tanggal << "\n";
        file << "Total Harga: Rp" << fixed << setprecision(2) << temp->total_harga << "\n";
        file << "Diskon: " << temp->diskon_persen << "%\n";
        file << "Total Akhir: Rp" << temp->total_akhir << "\n";
        file << "Status: ";
        if (temp->status == SELESAI) file << "SELESAI\n";
        else if (temp->status == DIBATALKAN) file << "DIBATALKAN\n";
        else file << "PENDING\n";
        
        file << "Item:\n";
        ItemTransaksi* item = temp->head_item;
        while (item) {
            file << "  - " << item->nama_tas << " (SKU: " << item->sku << ")\n";
            file << "    Harga: Rp" << item->harga << " x " << item->jumlah 
                 << " = Rp" << item->subtotal << "\n";
            item = item->next;
        }
        
        if (temp->status == SELESAI) {
            total_transaksi++;
            total_pendapatan += temp->total_akhir;
        }
        temp = temp->next;
    }
    
    file << "\n========================================\n";
    file << "RINGKASAN:\n";
    file << "Total Transaksi Selesai: " << total_transaksi << "\n";
    file << "Total Pendapatan: Rp" << fixed << setprecision(2) << total_pendapatan << "\n";
    file << "========================================\n";
    
    file.close();
    cout << "\nLaporan penjualan berhasil diekspor ke '" << filename << "'!\n";
}

void laporanStok(Tas* head, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Gagal membuat file laporan.\n";
        return;
    }
    
    file << "========================================\n";
    file << "   LAPORAN STOK TOKO TAS LES CATINO\n";
    file << "========================================\n\n";
    
    time_t now = time(0);
    char* dt = ctime(&now);
    file << "Tanggal Cetak: " << dt << "\n";
    
    file << left << setw(15) << "SKU" 
         << setw(30) << "Nama Tas" 
         << setw(15) << "Harga" 
         << setw(10) << "Stok" 
         << setw(20) << "Kategori" << "\n";
    file << string(90, '-') << "\n";
    
    Tas* temp = head;
    int total_item = 0;
    int total_stok = 0;
    double total_nilai = 0;
    
    while (temp) {
        file << left << setw(15) << temp->sku
             << setw(30) << temp->nama
             << "Rp" << setw(12) << fixed << setprecision(0) << temp->harga
             << setw(10) << temp->stok
             << setw(20) << temp->kategori << "\n";
        
        total_item++;
        total_stok += temp->stok;
        total_nilai += (temp->harga * temp->stok);
        
        if (temp->stok < 5) {
            file << "    ** STOK RENDAH! **\n";
        }
        
        temp = temp->next;
    }
    
    file << string(90, '-') << "\n";
    file << "\nRINGKASAN:\n";
    file << "Total Jenis Produk: " << total_item << "\n";
    file << "Total Unit Stok: " << total_stok << "\n";
    file << "Total Nilai Stok: Rp" << fixed << setprecision(2) << total_nilai << "\n";
    file << "========================================\n";
    
    file.close();
    cout << "\nLaporan stok berhasil diekspor ke '" << filename << "'!\n";
}


void hapusSemuaTransaksi(Transaksi*& head) {
    while (head) {
        ItemTransaksi* item = head->head_item;
        while (item) {
            ItemTransaksi* temp_item = item;
            item = item->next;
            delete temp_item;
        }
        Transaksi* temp = head;
        head = head->next;
        delete temp;
    }
}

// ============================================================================
// 6. STACK (Riwayat Operasi)
// ============================================================================

void pushStack(StackNode*& top, const char* deskripsi) {
    StackNode* baru = new StackNode;
    strncpy(baru->deskripsi_operasi, deskripsi, 100);
    baru->next = top;
    top = baru;
    cout << "[STACK] Push: " << deskripsi << "\n";
}

void popStack(StackNode*& top) {
    if (!top) {
        cout << "[STACK] Stack kosong.\n";
        return;
    }
    StackNode* temp = top;
    cout << "[STACK] Pop: " << temp->deskripsi_operasi << "\n";
    top = top->next;
    delete temp;
}

void printStack(StackNode* top) {
    if (!top) {
        cout << "[STACK] Stack kosong.\n";
        return;
    }
    cout << "\n=== RIWAYAT OPERASI (STACK) ===\n";
    StackNode* temp = top;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->deskripsi_operasi << "\n";
        temp = temp->next;
    }
}

// ============================================================================
// 7. QUEUE (Permintaan Restock)
// ============================================================================

void initQueue(QueueRestock& q) {
    q.front = NULL;
    q.rear = NULL;
}

void enqueueRestock(QueueRestock& q, const char* sku, const char* nama, int jumlah, const char* alasan) {
    QueueNode* baru = new QueueNode;
    strncpy(baru->sku, sku, MAX_SKU);
    strncpy(baru->nama_tas, nama, MAX_NAMA);
    baru->jumlah_restock = jumlah;
    strncpy(baru->alasan, alasan, 100);
    baru->next = NULL;

    if (!q.rear) {
        q.front = q.rear = baru;
    } else {
        q.rear->next = baru;
        q.rear = baru;
    }
    cout << "[QUEUE] Permintaan restock ditambahkan: " << nama << " (" << jumlah << " unit)\n";
}

void dequeueRestock(QueueRestock& q, Tas*& head_tas) {
    if (!q.front) {
        cout << "[QUEUE] Antrian kosong.\n";
        return;
    }
    QueueNode* temp = q.front;
    cout << "[QUEUE] Proses restock: " << temp->nama_tas << " (" << temp->jumlah_restock << " unit)\n";
    
    updateStok(head_tas, temp->sku, temp->jumlah_restock);

    q.front = q.front->next;
    if (!q.front) q.rear = NULL;
    delete temp;
}

void printQueueRestock(QueueRestock q) {
    if (!q.front) {
        cout << "[QUEUE] Antrian restock kosong.\n";
        return;
    }
    cout << "\n=== ANTRIAN RESTOCK (QUEUE) ===\n";
    QueueNode* temp = q.front;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->nama_tas << " (" << temp->sku << ") - " 
             << temp->jumlah_restock << " unit\n";
        cout << "   Alasan: " << temp->alasan << "\n";
        temp = temp->next;
    }
}

// ============================================================================
// 8. HUFFMAN (Kompresi)
// ============================================================================

HuffNode* newNode(char data, unsigned freq) {
    HuffNode* temp = new HuffNode;
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap* createMinHeap(unsigned capacity) {
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = new HuffNode*[capacity];
    return minHeap;
}

void swapHuffNode(HuffNode** a, HuffNode** b) {
    HuffNode* t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < (int)minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < (int)minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapHuffNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

HuffNode* extractMin(MinHeap* minHeap) {
    HuffNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap* minHeap, HuffNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

MinHeap* createAndBuildMinHeap(char data[], int freq[], int size) {
    MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

HuffNode* buildHuffmanTree(char data[], int freq[], int size) {
    HuffNode *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (minHeap->size != 1) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

void printHuffmanCodes(HuffNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printHuffmanCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printHuffmanCodes(root->right, arr, top + 1);
    }
    if (!root->left && !root->right) {
        cout << root->data << ": ";
        for (int i = 0; i < top; ++i)
            cout << arr[i];
        cout << "\n";
    }
}

void demoHuffman(const char* text) {
    int freq[256] = {0};
    for (int i = 0; text[i]; i++)
        freq[(unsigned char)text[i]]++;

    char data[256];
    int frequencies[256];
    int size = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            data[size] = (char)i;
            frequencies[size] = freq[i];
            size++;
        }
    }

    cout << "\n=== DEMO HUFFMAN CODING ===\n";
    cout << "Teks: " << text << "\n\n";
    cout << "Huffman Codes:\n";

    HuffNode* root = buildHuffmanTree(data, frequencies, size);
    int arr[100], top = 0;
    printHuffmanCodes(root, arr, top);
}

// ============================================================================
// 9. AVL TREE (Pencarian Cepat)
// ============================================================================

int height(AVLNode* N) {
    if (N == NULL) return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

AVLNode* newAVLNode(const char* sku, const char* nama, double harga) {
    AVLNode* node = new AVLNode;
    strncpy(node->sku, sku, MAX_SKU);
    strncpy(node->nama, nama, MAX_NAMA);
    node->harga = harga;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(AVLNode* N) {
    if (N == NULL) return 0;
    return height(N->left) - height(N->right);
}

AVLNode* insertAVL(AVLNode* node, const char* sku, const char* nama, double harga) {
    if (node == NULL)
        return newAVLNode(sku, nama, harga);

    int cmp = strcmp(sku, node->sku);
    if (cmp < 0)
        node->left = insertAVL(node->left, sku, nama, harga);
    else if (cmp > 0)
        node->right = insertAVL(node->right, sku, nama, harga);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(sku, node->left->sku) < 0)
        return rightRotate(node);

    if (balance < -1 && strcmp(sku, node->right->sku) > 0)
        return leftRotate(node);

    if (balance > 1 && strcmp(sku, node->left->sku) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(sku, node->right->sku) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

AVLNode* searchAVL(AVLNode* root, const char* sku) {
    if (root == NULL) return NULL;
    
    int cmp = strcmp(sku, root->sku);
    if (cmp == 0) return root;
    if (cmp < 0) return searchAVL(root->left, sku);
    return searchAVL(root->right, sku);
}

void inorderAVL(AVLNode* root) {
    if (root != NULL) {
        inorderAVL(root->left);
        cout << "SKU: " << root->sku << " | " << root->nama 
             << " | Rp" << fixed << setprecision(2) << root->harga << "\n";
        inorderAVL(root->right);
    }
}

// ============================================================================
// 10. MENU & MAIN
// ============================================================================

void menuAdmin(User* currentUser) {
    cout << "\n================================================\n";
    cout << "           MENU ADMIN - " << currentUser->username << "\n";
    cout << "================================================\n";
    cout << "1. Tampilkan Inventaris\n";
    cout << "2. Tambah Tas\n";
    cout << "3. Cari Tas\n";
    cout << "4. Jual Tas\n";
    cout << "5. Edit Tas\n";
    cout << "6. Hapus Tas\n";
    cout << "7. Urutkan Inventaris\n";
    cout << "8. Tampilkan Riwayat Penjualan\n";
    cout << "9. Tampilkan Statistik\n";
    cout << "10. [Stack] Undo Edit\n";
    cout << "11. [Stack] Redo Edit\n";
    cout << "12. [Stack] Histori Edit\n";
    cout << "13. [Queue] Tambah ke Antrian\n";
    cout << "14. [Queue] Proses Antrian\n";
    cout << "15. [Queue] Lihat Antrian\n";
    cout << "16. [AVL] Cari Data KTP\n";
    cout << "17. [AVL] Tambah Data KTP\n";
    cout << "18. [AVL] Lihat Semua Data KTP\n";
    cout << "19. [AVL] Build AVL Tree\n";
    cout << "20. [Huffman] Demo Kompresi\n";
    cout << "21. Export Laporan Penjualan (TXT)\n";
    cout << "22. Export Laporan Stok (TXT)\n";
    cout << "0. Logout\n";
    cout << "================================================\n";
    cout << "Pilihan: ";
}

void menuUser(User* currentUser) {
    cout << "\n================================\n";
    cout << "  MENU CUSTOMER - " << currentUser->username << "\n";
    cout << "================================\n";
    cout << "BELANJA:\n";
    cout << "1. Lihat Katalog Tas\n";
    cout << "2. Cari Tas\n";
    cout << "3. Filter Berdasarkan Harga\n";
    cout << "4. Buat Pesanan (Transaksi)\n";
    cout << "\nRIWAYAT:\n";
    cout << "5. Lihat Riwayat Transaksi Saya\n";
    cout << "\nSTRUKTUR DATA:\n";
    cout << "19. [AVL] Build AVL Tree\n";
    cout << "16. [AVL] Cari Data KTP\n";
    cout << "17. [AVL] Tambah Data KTP\n";
    cout << "18. [AVL] Lihat Semua Data KTP\n";
    cout << "20. [Huffman] Demo Kompresi\n";
    cout << "\n0. Logout\n";
    cout << "Pilihan: ";
}


int main() {
    // Inisialisasi Data
    Tas* head_tas = NULL;
    Transaksi* head_transaksi = NULL;
    LogAktivitas* head_log = NULL;
    StackNode* operasi_stack = NULL;
    QueueRestock restock_queue;
    initQueue(restock_queue);
    AVLNode* avl_root = NULL;

    User* head_users = loadUsersFromFile();
    
    // Jika tidak ada user, buat default
    if (!head_users) {
        User* admin = new User;
        strcpy(admin->username, "admin");
        strcpy(admin->password, "admin123");
        strcpy(admin->email, "admin@lescatino.com");
        strcpy(admin->role, "admin");
        strcpy(admin->nik, "3201010101010001");
        getCopyTanggal(admin->tanggal_daftar, 30);
        admin->aktif = true;
        admin->next = NULL;

        User* user = new User;
        strcpy(user->username, "user");
        strcpy(user->password, "user123");
        strcpy(user->email, "user@lescatino.com");
        strcpy(user->role, "user");
        strcpy(user->nik, "3201010101010002");
        getCopyTanggal(user->tanggal_daftar, 30);
        user->aktif = true;
        user->next = NULL;

        admin->next = user;
        head_users = admin;
        saveUsersToFile(head_users);
    }

    // Data Tas Dummy
    tambahTas(head_tas, "TAS020", "Tas Outdoor Hiking", 720000, 3, "Outdoor");
    tambahTas(head_tas, "TAS019", "Tas Anak Bergambar", 160000, 30, "Sekolah");
    tambahTas(head_tas, "TAS018", "Tas Pria Kulit Asli", 650000, 5, "Formal");
    tambahTas(head_tas, "TAS017", "Tas Wanita Mini", 175000, 22, "Fashion");
    tambahTas(head_tas, "TAS016", "Tas Travel Kabin", 480000, 8, "Travel");
    tambahTas(head_tas, "TAS015", "Tas Kerja Eksekutif", 500000, 6, "Kerja");
    tambahTas(head_tas, "TAS014", "Tas Selempang Kanvas", 200000, 16, "Selempang");
    tambahTas(head_tas, "TAS013", "Tas Ransel Anti Air", 420000, 7, "Ransel");
    tambahTas(head_tas, "TAS012", "Tas Laptop Slim", 380000, 11, "Laptop");
    tambahTas(head_tas, "TAS011", "Tas Pinggang Trendy", 150000, 25, "Pinggang");
    tambahTas(head_tas, "TAS010", "Tas Olahraga Gym", 220000, 18, "Olahraga");
    tambahTas(head_tas, "TAS009", "Tas Kamera Profesional", 700000, 4, "Kamera");
    tambahTas(head_tas, "TAS008", "Tas Gunung Outdoor", 600000, 6, "Outdoor");
    tambahTas(head_tas, "TAS007", "Tas Pria Formal", 300000, 9, "Formal");
    tambahTas(head_tas, "TAS006", "Tas Wanita Elegan", 320000, 12, "Fashion");
    tambahTas(head_tas, "TAS005", "Tas Sekolah Anak", 180000, 20, "Sekolah");
    tambahTas(head_tas, "TAS004", "Tas Travel Besar", 550000, 5, "Travel");
    tambahTas(head_tas, "TAS003", "Tas Laptop Premium", 450000, 8, "Laptop");
    tambahTas(head_tas, "TAS002", "Tas Selempang Kulit", 350000, 10, "Selempang");
    tambahTas(head_tas, "TAS001", "Tas Ransel Casual", 250000, 15, "Ransel");

    User* currentUser = NULL;
    int id_transaksi_counter = 1;

    while (true) {
        currentUser = NULL;
        menuLogin(head_users, currentUser);
        if (!currentUser) break;

        tambahLog(head_log, currentUser->username, "Login");
        pushStack(operasi_stack, "User login");

        bool logout = false;
        while (!logout) {
            char admin[] = "admin";
            bool isAdmin = (strcmp(currentUser->role, admin) == 0);
            
            if (isAdmin) {
                menuAdmin(currentUser);
            } else {
                menuUser(currentUser);
            }
            
            int pil;
            cin >> pil; cin.ignore();

            if (isAdmin) {
                if (pil == 1) { // Tampilkan Inventaris
                    tampilkanInventaris(head_tas);
                } else if (pil == 2) { // Tambah Tas
                    char sku[MAX_SKU], nama[MAX_NAMA], kategori[MAX_KATEGORI];
                    double harga;
                    int stok;
                    cout << "\n=== TAMBAH TAS BARU ===\n";
                    cout << "SKU: "; cin.getline(sku, MAX_SKU);
                    cout << "Nama: "; cin.getline(nama, MAX_NAMA);
                    cout << "Harga: "; cin >> harga; cin.ignore();
                    cout << "Stok: "; cin >> stok; cin.ignore();
                    cout << "Kategori: "; cin.getline(kategori, MAX_KATEGORI);
                    tambahTas(head_tas, sku, nama, harga, stok, kategori);
                    pushStack(operasi_stack, "Tambah tas baru");
                    cout << "Tas berhasil ditambahkan!\n";
                } else if (pil == 3) { // Cari Tas
                    char keyword[MAX_NAMA];
                    cout << "\n=== CARI TAS ===\n";
                    cout << "Kata kunci: "; cin.getline(keyword, MAX_NAMA);
                    cariTas(head_tas, keyword);
                } else if (pil == 4) { // Jual Tas
                    tampilkanInventaris(head_tas);
                    buatTransaksi(head_transaksi, head_tas, currentUser, id_transaksi_counter);
                    pushStack(operasi_stack, "Buat transaksi");
                } else if (pil == 5) { // Edit Tas
                    // Implementasi Edit Tas
                    cout << "\n[Fitur Edit Tas - Dalam Pengembangan]\n";
                } else if (pil == 6) { // Hapus Tas
                    // Implementasi Hapus Tas
                    cout << "\n[Fitur Hapus Tas - Dalam Pengembangan]\n";
                } else if (pil == 7) { // Urutkan Inventaris
                    // Implementasi Urutkan Inventaris (misal: by Harga, Nama, Stok)
                    cout << "\n[Fitur Urutkan Inventaris - Dalam Pengembangan]\n";
                } else if (pil == 8) { // Tampilkan Riwayat Penjualan
                    tampilkanTransaksi(head_transaksi);
                } else if (pil == 9) { // Tampilkan Statistik
                    // Implementasi Statistik (misal: total penjualan, produk terlaris)
                    cout << "\n[Fitur Statistik - Dalam Pengembangan]\n";
                } else if (pil == 10) { // [Stack] Undo Edit
                    popStack(operasi_stack);
                } else if (pil == 11) { // [Stack] Redo Edit
                    // Implementasi Redo
                    cout << "\n[Fitur Redo - Dalam Pengembangan]\n";
                } else if (pil == 12) { // [Stack] Histori Edit
                    printStack(operasi_stack);
                } else if (pil == 13) { // [Queue] Tambah ke Antrian
                    char sku[MAX_SKU], nama[MAX_NAMA], alasan[100];
                    int jumlah;
                    cout << "\n=== PERMINTAAN RESTOCK ===\n";
                    cout << "SKU: "; cin.getline(sku, MAX_SKU);
                    cout << "Nama Tas: "; cin.getline(nama, MAX_NAMA);
                    cout << "Jumlah: "; cin >> jumlah; cin.ignore();
                    cout << "Alasan: "; cin.getline(alasan, 100);
                    enqueueRestock(restock_queue, sku, nama, jumlah, alasan);
                } else if (pil == 14) { // [Queue] Proses Antrian
                    dequeueRestock(restock_queue, head_tas);
                } else if (pil == 15) { // [Queue] Lihat Antrian
                    printQueueRestock(restock_queue);
                } else if (pil == 16) { // [AVL] Cari Data KTP
                    char nik[50];
                    cout << "Masukkan NIK untuk dicari: "; cin.getline(nik, 50);
                    // Asumsi ada fungsi cariKTPByNIK yang terintegrasi dengan AVL jika KTP disimpan di sana
                    // Untuk saat ini, menggunakan fungsi KTP yang ada
                    NodeKTP* dummy_head = NULL; // Perlu implementasi AVL untuk KTP
                    cariKTPByNIK(dummy_head, nik); 
                    cout << "[Fitur Cari KTP di AVL - Dalam Pengembangan]\n";
                } else if (pil == 17) { // [AVL] Tambah Data KTP
                    char nik[50], nama[100], ttl[100], jk[20], alamat[200], agama[20], status[20], pekerjaan[50], wni[50], berlaku[50];
                    cout << "\n=== TAMBAH DATA KTP BARU ===\n";
                    cout << "NIK: "; cin.getline(nik, 50);
                    cout << "Nama: "; cin.getline(nama, 100);
                    cout << "Tempat/Tgl Lahir: "; cin.getline(ttl, 100);
                    cout << "Jenis Kelamin: "; cin.getline(jk, 20);
                    cout << "Alamat: "; cin.getline(alamat, 200);
                    cout << "Agama: "; cin.getline(agama, 20);
                    cout << "Status Perkawinan: "; cin.getline(status, 20);
                    cout << "Pekerjaan: "; cin.getline(pekerjaan, 50);
                    cout << "Kewarganegaraan: "; cin.getline(wni, 50);
                    cout << "Berlaku Hingga: "; cin.getline(berlaku, 50);
                    // Asumsi ada fungsi insertAVL untuk KTP
                    cout << "[Fitur Tambah KTP ke AVL - Dalam Pengembangan]\n";
                } else if (pil == 18) { // [AVL] Lihat Semua Data KTP
                    // Asumsi ada fungsi inorderAVL untuk KTP
                     cout << "[Fitur Lihat Semua KTP di AVL - Dalam Pengembangan]\n";
                } else if (pil == 19) { // [AVL] Build AVL Tree
                    cout << "\n=== BUILD AVL TREE ===\n";
                    Tas* temp = head_tas;
                    while (temp) {
                        avl_root = insertAVL(avl_root, temp->sku, temp->nama, temp->harga);
                        temp = temp->next;
                    }
                    cout << "AVL Tree berhasil dibuat!\n\n";
                    cout << "Data dalam AVL (Inorder):\n";
                    inorderAVL(avl_root);
                } else if (pil == 20) { // [Huffman] Demo Kompresi
                    char text[200];
                    cout << "\n=== DEMO HUFFMAN ===\n";
                    cout << "Masukkan teks untuk dikompresi: "; cin.getline(text, 200);
                    demoHuffman(text);
                } else if (pil == 21) { // Export Laporan Penjualan (TXT)
                    char filename[100];
                    cout << "\n=== EXPORT LAPORAN PENJUALAN ===\n";
                    cout << "Nama file (ex: laporan_penjualan.txt): ";
                    cin.getline(filename, 100);
                    laporanPenjualan(head_transaksi, filename);
                } else if (pil == 22) { // Export Laporan Stok (TXT)
                    char filename[100];
                    cout << "\n=== EXPORT LAPORAN STOK ===\n";
                    cout << "Nama file (ex: laporan_stok.txt): ";
                    cin.getline(filename, 100);
                    laporanStok(head_tas, filename);
                } else if (pil == 0) { // Logout
                    logout = true;
                    tambahLog(head_log, currentUser->username, "Logout");
                    cout << "\nLogout berhasil.\n";
                }
            } else { // Menu User
                if (pil == 1) { // Lihat Katalog Tas
                    tampilkanInventaris(head_tas);
                } else if (pil == 2) { // Cari Tas
                    char keyword[MAX_NAMA];
                    cout << "\n=== CARI TAS ===\n";
                    cout << "Kata kunci: "; cin.getline(keyword, MAX_NAMA);
                    cariTas(head_tas, keyword);
                } else if (pil == 3) { // Filter Berdasarkan Harga
                    double min, max;
                    cout << "\n=== FILTER HARGA ===\n";
                    cout << "Min: "; cin >> min;
                    cout << "Max: "; cin >> max; cin.ignore();
                    filterTasByHarga(head_tas, min, max);
                } else if (pil == 4) { // Buat Pesanan (Transaksi)
                    tampilkanInventaris(head_tas);
                    buatTransaksi(head_transaksi, head_tas, currentUser, id_transaksi_counter);
                    pushStack(operasi_stack, "Transaksi user");
                } else if (pil == 5) { // Lihat Riwayat Transaksi Saya
                    tampilkanTransaksiUser(head_transaksi, currentUser->username);
                } else if (pil == 16) { // [AVL] Cari Data KTP (User view)
                    char nik[50];
                    cout << "Masukkan NIK untuk dicari: "; cin.getline(nik, 50);
                    NodeKTP* dummy_head = NULL; // Perlu implementasi AVL untuk KTP
                    cariKTPByNIK(dummy_head, nik);
                    cout << "[Fitur Cari KTP di AVL - Dalam Pengembangan]\n";
                } else if (pil == 17) { // [AVL] Tambah Data KTP (User view)
                     // User tidak bisa tambah KTP sembarangan, mungkin hanya bisa cek KTP sendiri?
                    cout << "[Fitur Tambah KTP di AVL - Hanya untuk Admin]\n";
                } else if (pil == 18) { // [AVL] Lihat Semua Data KTP (User view)
                    cout << "[Fitur Lihat Semua KTP di AVL - Hanya untuk Admin]\n";
                } else if (pil == 19) { // [AVL] Build AVL Tree
                    cout << "\n=== BUILD AVL TREE ===\n";
                    Tas* temp = head_tas;
                    while (temp) {
                        avl_root = insertAVL(avl_root, temp->sku, temp->nama, temp->harga);
                        temp = temp->next;
                    }
                    cout << "AVL Tree berhasil dibuat!\n\n";
                    cout << "Data dalam AVL (Inorder):\n";
                    inorderAVL(avl_root);
                } else if (pil == 20) { // [Huffman] Demo Kompresi
                    char text[200];
                    cout << "\n=== DEMO HUFFMAN ===\n";
                    cout << "Masukkan teks untuk dikompresi: "; cin.getline(text, 200);
                    demoHuffman(text);
                } else if (pil == 0) { // Logout
                    logout = true;
                    tambahLog(head_log, currentUser->username, "Logout");
                    cout << "\nLogout berhasil.\n";
                }
            }
        }
    }

    // Cleanup
    hapusSemuaTas(head_tas);
    hapusSemuaTransaksi(head_transaksi);
    saveUsersToFile(head_users);

    cout << "\nTerima kasih telah menggunakan Sistem Inventaris Toko Tas Les Catino!\n";
    return 0;
}

