#ifndef TOKO_TAS_H
#define TOKO_TAS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring> 

using namespace std;

// ============================================================================
// KONSTANTA
// ============================================================================
#define MAX_NAMA 100
#define MAX_SKU 20
#define MAX_KATEGORI 50
#define MAX_USERNAME 50
#define MAX_PASSWORD 50
#define MAX_ROLE 20

enum StatusTransaksi { PENDING = 0, SELESAI = 1, DIBATALKAN = 2 };

// ============================================================================
// STRUKTUR DATA UTAMA
// ============================================================================

struct User {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char email[MAX_NAMA];
    char role[MAX_ROLE]; 
    char nik[50];
    char tanggal_daftar[30];
    bool aktif;
    User* next;
};

struct DataKTP {
    char nik[50];
    char nama[100];
    char tempat_tgl_lahir[100];
    char jenis_kelamin[20];
    char alamat[200];
    char agama[20];
    char status_perkawinan[20];
    char pekerjaan[50];
    char kewarganegaraan[50];
    char berlaku_hingga[50];
};

struct NodeKTP {
    DataKTP data;
    NodeKTP* prev;
    NodeKTP* next;
};

// Data Tas
struct Tas {
    char sku[MAX_SKU];
    char nama[MAX_NAMA];
    double harga;
    int stok;
    char kategori[MAX_KATEGORI];
    Tas* next;
};

// Item dalam keranjang transaksi
struct ItemTransaksi {
    char sku[MAX_SKU];
    char nama_tas[MAX_NAMA];
    double harga;
    int jumlah;
    double subtotal;
    ItemTransaksi* next;
};

// Transaksi Penjualan
struct Transaksi {
    int id_transaksi;
    char username_pembeli[MAX_USERNAME];
    char tanggal[30];
    double total_harga;
    double diskon_persen;
    double total_akhir;
    StatusTransaksi status;
    ItemTransaksi* head_item;
    Transaksi* next;
};

// Log Aktivitas
struct LogAktivitas {
    char timestamp[30];
    char username[MAX_USERNAME];
    char aktivitas[200];
    LogAktivitas* next;
};

// --- STACK untuk Riwayat Operasi ---
struct StackNode {
    char deskripsi_operasi[100];
    StackNode* next;
};

// --- QUEUE untuk Permintaan Restock ---
struct QueueNode {
    char sku[MAX_SKU];
    char nama_tas[MAX_NAMA];
    int jumlah_restock;
    char alasan[100];
    QueueNode* next;
};

struct QueueRestock {
    QueueNode* front;
    QueueNode* rear;
};

// --- HUFFMAN untuk Kompresi Data ---
struct HuffNode {
    char data;
    unsigned freq;
    HuffNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    HuffNode** array;
};

// --- AVL TREE untuk Pencarian Cepat ---
struct AVLNode {
    char sku[MAX_SKU];
    char nama[MAX_NAMA];
    double harga;
    int height;
    AVLNode *left;
    AVLNode *right;
};

// ============================================================================
// PROTOTYPE FUNGSI
// ============================================================================

void getCopyTanggal(char* buffer, size_t size);

// KTP
void createNodeKTP(NodeKTP*& baru, const char* nik, const char* nama, const char* ttl, 
                   const char* jk, const char* alamat, const char* agama, 
                   const char* status, const char* pekerjaan, const char* wni, const char* berlaku);
void insertLastKTP(NodeKTP*& head, NodeKTP*& tail, const char* nik, const char* nama, 
                   const char* ttl, const char* jk, const char* alamat, const char* agama, 
                   const char* status, const char* pekerjaan, const char* wni, const char* berlaku);
void displayForwardKTP(NodeKTP* head);
void cariKTPByNIK(NodeKTP* head, const char* nik);
void hapusSemuaKTP(NodeKTP*& head, NodeKTP*& tail);

// User & Auth
User* loadUsersFromFile();
void saveUsersToFile(User* head);
User* loginSystem(User* head, const char* username, const char* password);
User* loginByNIK(User* head, const char* nik, const char* password);
void registerUser(User*& head);
void menuLogin(User*& head, User*& loggedInUser);

// Log
void tambahLog(LogAktivitas*& h, const char* u, const char* a);

// Tas (Inventaris)
void tambahTas(Tas*& head, const char* sku, const char* nama, double harga, int stok, const char* kategori);
void tampilkanInventaris(Tas* head);
void cariTas(Tas* head, const char* keyword);
void filterTasByHarga(Tas* head, double min, double max);
Tas* cariTasBySKU(Tas* head, const char* sku);
void updateStok(Tas* head, const char* sku, int jumlah);
void hapusSemuaTas(Tas*& head);

// Transaksi
void buatTransaksi(Transaksi*& head_trans, Tas*& head_tas, User* user, int& id_counter);
void tampilkanTransaksi(Transaksi* head);
void tampilkanTransaksiUser(Transaksi* head, const char* username);
void hapusSemuaTransaksi(Transaksi*& head);

// Stack Operasi
void pushStack(StackNode*& top, const char* deskripsi);
void popStack(StackNode*& top);
void printStack(StackNode* top);

// Queue Restock
void initQueue(QueueRestock& q);
void enqueueRestock(QueueRestock& q, const char* sku, const char* nama, int jumlah, const char* alasan);
void dequeueRestock(QueueRestock& q, Tas*& head_tas);
void printQueueRestock(QueueRestock q);

// Huffman
void demoHuffman(const char* text);

// AVL Tree
AVLNode* insertAVL(AVLNode* node, const char* sku, const char* nama, double harga);
AVLNode* searchAVL(AVLNode* root, const char* sku);
void inorderAVL(AVLNode* root);

// Menu
void menuAdmin(User* currentUser);
void menuUser(User* currentUser);
void menuUtama(const char* role);

#endif

