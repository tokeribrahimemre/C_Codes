//
//  fonksiyonlar.h
//  yapısal_project
//
//  Created by İbrahim Emre Toker on 31.05.2024.
//

#ifndef fonksiyonlar_h
#define fonksiyonlar_h

#include <stdio.h>
#include "myStructs.h"

// Yığına yeni bir oyun durumu ekler
void yigina_ekle(OyunYigini *yigin, OyunDurumu durum);

// Yığından en üstteki oyun durumunu çıkarır
OyunDurumu yigindan_cikar(OyunYigini *yigin);

// Oyunu başlatır ve tahta boyutunu ayarlar
void tahta_baslat(OyunDurumu *oyun, int boyut);

// Oyun tahtasını yazdırır
void tahta_yazdir(OyunDurumu *oyun);

// Geçerli bir hamle olup olmadığını kontrol eder
int gecerli_hamle_mi(OyunDurumu *oyun, int x1, int y1, int x2, int y2);

// Hamleyi gerçekleştirir
void hamle_yap(OyunDurumu *oyun, int x1, int y1, int x2, int y2);

// Taşın yakalanabilir olup olmadığını kontrol eder
int tas_yakalanabilir_mi(OyunDurumu *oyun, int x, int y);

// Taşı yakalar
void tas_yakala(OyunDurumu *oyun, int x1, int y1, int x2, int y2);

// Oyuncuların puanlarını yazdırır
void puanlari_yazdir(OyunDurumu *oyun);

// Oyun durumunu kopyalar
OyunDurumu oyun_durumunu_kopyala(OyunDurumu *oyun);

// Oyunu verilen dosya adına kaydeder
void oyun_kaydet(OyunDurumu *oyun, const char *dosya_adi);

// Oyunu verilen dosya adından yükler
void oyun_yukle(OyunDurumu *oyun, const char *dosya_adi);

// Oyun modunu seçer (insan veya bilgisayar)
void oyun_modu_sec(OyunDurumu *oyun);

// Geçerli hamleleri bulur
void gecerli_hamleleri_bul(OyunDurumu *oyun, Hamle gecerli_hamleler[], int *hamle_sayisi);

// En iyi hamleyi bulur
Hamle en_iyi_hamleyi_bul(OyunDurumu *oyun);

// Bilgisayar hamlesini gerçekleştirir
void bilgisayar_hamlesi(OyunDurumu *oyun);

// Oyunun bitip bitmediğini kontrol eder
int oyun_bitti_mi(OyunDurumu *oyun);

// Kazananı yazdırır
void kazananı_yazdir(OyunDurumu *oyun);

#endif /* fonksiyonlar_h */

