#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>

using namespace std;

// Mutex dan condition variable untuk sinkronisasi
pthread_mutex_t mutex;
pthread_cond_t kondisi;

// Menentukan thread yang boleh mencetak
int giliran = 1;

// ===============================
// THREAD 1: FAKTORIAL
// ===============================
void* hitungFaktorial(void* arg) {

    int n = 5;
    long long hasil = 1;

    // Menghitung faktorial
    for (int i = 1; i <= n; i++) {
        hasil *= i;
    }

    // Menunggu giliran Thread 1
    pthread_mutex_lock(&mutex);

    while (giliran != 1) {
        pthread_cond_wait(&kondisi, &mutex);
    }

    cout << "\n[Thread 1 - Faktorial]" << endl;
    cout << "Faktorial dari " << n << " = " << hasil << endl;

    // Memberikan giliran ke Thread 2
    giliran = 2;

    pthread_cond_broadcast(&kondisi);
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

// ===============================
// THREAD 2: FIBONACCI
// ===============================
void* fibonacci(void* arg) {

    int jumlah = 10;
    int a = 0;
    int b = 1;

    // Menunggu giliran Thread 2
    pthread_mutex_lock(&mutex);

    while (giliran != 2) {
        pthread_cond_wait(&kondisi, &mutex);
    }

    cout << "\n[Thread 2 - Fibonacci]" << endl;
    cout << "Deret Fibonacci: ";

    for (int i = 0; i < jumlah; i++) {
        cout << a << " ";

        int berikutnya = a + b;
        a = b;
        b = berikutnya;
    }

    cout << endl;

    // Memberikan giliran ke Thread 3
    giliran = 3;

    pthread_cond_broadcast(&kondisi);
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

// ===============================
// THREAD 3: MEMBACA FILE
// ===============================
void* bacaFile(void* arg) {

    ifstream file("data.txt");

    // Menunggu giliran Thread 3
    pthread_mutex_lock(&mutex);

    while (giliran != 3) {
        pthread_cond_wait(&kondisi, &mutex);
    }

    cout << "\n[Thread 3 - Membaca File]" << endl;

    if (file.is_open()) {

        string baris;

        while (getline(file, baris)) {
            cout << baris << endl;
        }

        file.close();

    } else {
        cout << "File data.txt tidak ditemukan!" << endl;
    }

    // Semua thread sudah selesai
    giliran = 4;

    pthread_cond_broadcast(&kondisi);
    pthread_mutex_unlock(&mutex);

    return nullptr;
}

// ===============================
// PROGRAM UTAMA
// ===============================
int main() {

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    // Inisialisasi mutex dan condition variable
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&kondisi, nullptr);

    cout << "====================================" << endl;
    cout << "       PROGRAM MULTITHREADING C++" << endl;
    cout << "====================================" << endl;

    // Membuat 3 thread
    pthread_create(&thread1, nullptr, hitungFaktorial, nullptr);
    pthread_create(&thread2, nullptr, fibonacci, nullptr);
    pthread_create(&thread3, nullptr, bacaFile, nullptr);

    // Menunggu semua thread selesai
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    pthread_join(thread3, nullptr);

    // Menghapus mutex dan condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&kondisi);

    cout << "\n====================================" << endl;
    cout << "Semua thread telah selesai." << endl;
    cout << "====================================" << endl;

    return 0;
}