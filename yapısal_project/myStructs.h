
//
//  myStructs.h
//  yapısal_project
//
//  Created by İbrahim Emre Toker on 31.05.2024.
//

#ifndef myStructs_h
#define myStructs_h

#define MAX_BOYUT 20  // Tahta boyutu için maksimum değer
#define MAX_YIGIN 100 // Yığın boyutu için maksimum değer

// Oyun tahtası hücresi
typedef struct {
    char tas; // Hücredeki taşın türü
} Hucre;

// Oyun tahtası
typedef struct {
    Hucre hucreler[MAX_BOYUT][MAX_BOYUT]; // Tahtadaki hücreler
} Tahta;

// Oyuncu tipleri
typedef enum {
    INSAN,      // İnsan oyuncu
    BILGISAYAR  // Bilgisayar oyuncu
} OyuncuTipi;

// Oyuncu bilgileri
typedef struct {
    char isim[50];   // Oyuncunun ismi
    int puan;        // Oyuncunun puanı
    OyuncuTipi tipi; // Oyuncunun tipi (insan veya bilgisayar)
} Oyuncu;

// Oyun durumu
typedef struct {
    Tahta tahta;             // Oyun tahtası
    Oyuncu oyuncular[2];     // İki oyuncu
    int mevcutOyuncu;        // Mevcut oyuncu
    int tahta_boyutu;        // Tahta boyutu
} OyunDurumu;

// Oyun durumu yığını (geri alma/yeniden yapma için)
typedef struct {
    OyunDurumu durumlar[MAX_YIGIN]; // Yığındaki oyun durumları
    int tepe;                       // Yığının tepe noktası
} OyunYigini;

// Hamle bilgileri
typedef struct {
    int baslangic_x, baslangic_y; // Hamlenin başlangıç koordinatları
    int hedef_x, hedef_y;         // Hamlenin hedef koordinatları
    int puan;                     // Hamle sonucu kazanılan puan
} Hamle;

#endif /* myStructs_h */
