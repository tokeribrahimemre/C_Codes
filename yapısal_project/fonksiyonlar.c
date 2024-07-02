//
//  fonksiyonlar.c
//  yapısal_project
//
//  Created by İbrahim Emre Toker on 31.05.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fonksiyonlar.h"
#define MAX_BOYUT 20
#define MAX_YIGIN 100

extern char taslar[]; // Taş türlerini tanımlayan dış değişken

// Oyun yığınına durum ekleme
void yigina_ekle(OyunYigini *yigin, OyunDurumu durum) {
    if (yigin->tepe < MAX_YIGIN - 1) {
        yigin->durumlar[++yigin->tepe] = durum;
    }
}

// Oyun yığınından durum çıkarma
OyunDurumu yigindan_cikar(OyunYigini *yigin) {
    if (yigin->tepe >= 0) {
        return yigin->durumlar[yigin->tepe--];
    }
    OyunDurumu bosDurum;
    memset(&bosDurum, 0, sizeof(OyunDurumu));
    return bosDurum;
}

// Tahtayı başlatma fonksiyonu
void tahta_baslat(OyunDurumu *oyun, int boyut) {
    oyun->tahta_boyutu = boyut;
    for (int i = 0; i < boyut; i++) {
        for (int j = 0; j < boyut; j++) {
            oyun->tahta.hucreler[i][j].tas = ' '; // Hücreleri boş olarak başlat
        }
    }
    srand(time(0)); // Rastgele taş yerleştirme için zaman tabanlı rastgele sayı üreticisini başlat
    for (int i = 0; i < boyut; i++) {
        for (int j = 0; j < boyut; j++) {
            if (!(i == boyut / 2 && j == boyut / 2) && !(i == boyut / 2 - 1 && j == boyut / 2) && !(i == boyut / 2 && j == boyut / 2 - 1) && !(i == boyut / 2 - 1 && j == boyut / 2 - 1)) {
                oyun->tahta.hucreler[i][j].tas = taslar[rand() % 6]; // Taşları rastgele yerleştir
            }
        }
    }
}

// Tahtayı ekrana yazdırma fonksiyonu
void tahta_yazdir(OyunDurumu *oyun) {
    for (int i = 0; i < oyun->tahta_boyutu; i++) {
        for (int j = 0; j < oyun->tahta_boyutu; j++) {
            printf("%c ", oyun->tahta.hucreler[i][j].tas); // Tahtadaki taşları yazdır
        }
        printf("\n");
    }
}

// Geçerli hamle kontrolü
int gecerli_hamle_mi(OyunDurumu *oyun, int x1, int y1, int x2, int y2) {
    // Hedef koordinatların tahtanın içinde olup olmadığını kontrol et
    if (x2 < 0 || x2 >= oyun->tahta_boyutu || y2 < 0 || y2 >= oyun->tahta_boyutu) {
        return 0;
    }
    // Hedef hücrenin boş olup olmadığını kontrol et
    if (oyun->tahta.hucreler[x2][y2].tas != ' ') {
        return 0;
    }
    // Hamlenin geçerli bir mesafede olup olmadığını kontrol et
    if (abs(x2 - x1) > 2 || abs(y2 - y1) > 2) {
        return 0;
    }
    return 1; // Geçerli hamle
}

// Hamleyi gerçekleştirir
void hamle_yap(OyunDurumu *oyun, int x1, int y1, int x2, int y2) {
    oyun->tahta.hucreler[x2][y2].tas = oyun->tahta.hucreler[x1][y1].tas; // Taşı yeni hücreye yerleştir
    oyun->tahta.hucreler[x1][y1].tas = ' '; // Eski hücreyi boşalt
}

// Taşın yakalanabilir olup olmadığını kontrol eder
int tas_yakalanabilir_mi(OyunDurumu *oyun, int x, int y) {
    // Basit bir kontrol, daha gelişmiş kurallar eklenebilir
    if (x > 0 && oyun->tahta.hucreler[x-1][y].tas != ' ' && oyun->tahta.hucreler[x-1][y].tas != oyun->tahta.hucreler[x][y].tas) {
        return 1;
    }
    if (x < oyun->tahta_boyutu - 1 && oyun->tahta.hucreler[x+1][y].tas != ' ' && oyun->tahta.hucreler[x+1][y].tas != oyun->tahta.hucreler[x][y].tas) {
        return 1;
    }
    if (y > 0 && oyun->tahta.hucreler[x][y-1].tas != ' ' && oyun->tahta.hucreler[x][y-1].tas != oyun->tahta.hucreler[x][y].tas) {
        return 1;
    }
    if (y < oyun->tahta_boyutu - 1 && oyun->tahta.hucreler[x][y+1].tas != ' ' && oyun->tahta.hucreler[x][y+1].tas != oyun->tahta.hucreler[x][y].tas) {
        return 1;
    }
    return 0;
}

// Taşı yakalar
void tas_yakala(OyunDurumu *oyun, int x1, int y1, int x2, int y2) {
    oyun->tahta.hucreler[x2][y2].tas = ' '; // Hedef hücreyi boşalt
    oyun->oyuncular[oyun->mevcutOyuncu].puan++; // Geçerli oyuncunun puanını artır
}

// Oyuncuların puanlarını yazdırır
void puanlari_yazdir(OyunDurumu *oyun) {
    printf("Puanlar: %s: %d, %s: %d\n", oyun->oyuncular[0].isim, oyun->oyuncular[0].puan, oyun->oyuncular[1].isim, oyun->oyuncular[1].puan);
}

// Oyun durumunu kopyalar
OyunDurumu oyun_durumunu_kopyala(OyunDurumu *oyun) {
    OyunDurumu kopya;
    memcpy(&kopya, oyun, sizeof(OyunDurumu));
    return kopya;
}

// Oyunu verilen dosya adına kaydeder
void oyun_kaydet(OyunDurumu *oyun, const char *dosya_adi) {
    FILE *dosya = fopen(dosya_adi, "wb");
    if (dosya) {
        fwrite(oyun, sizeof(OyunDurumu), 1, dosya);
        fclose(dosya);
        printf("Oyun kaydedildi.\n");
    } else {
        printf("Oyun kaydedilemedi.\n");
    }
}

// Oyunu verilen dosya adından yükler
void oyun_yukle(OyunDurumu *oyun, const char *dosya_adi) {
    FILE *dosya = fopen(dosya_adi, "rb");
    if (dosya) {
        fread(oyun, sizeof(OyunDurumu), 1, dosya);
        fclose(dosya);
        printf("Oyun yüklendi.\n");
    } else {
        printf("Oyun yüklenemedi.\n");
    }
}

// Oyun modunu seçer (insan veya bilgisayar)
void oyun_modu_sec(OyunDurumu *oyun) {
    char cevap;
    printf("Bilgisayara karşı mı oynamak istersiniz? (e/h): ");
    scanf(" %c", &cevap);
    if (cevap == 'e' || cevap == 'E') {
        oyun->oyuncular[1].tipi = BILGISAYAR;
    } else {
        oyun->oyuncular[1].tipi = INSAN;
    }
}

// Geçerli hamleleri bulur
void gecerli_hamleleri_bul(OyunDurumu *oyun, Hamle gecerli_hamleler[], int *hamle_sayisi) {
    *hamle_sayisi = 0;
    for (int i = 0; i < oyun->tahta_boyutu; i++) {
        for (int j = 0; j < oyun->tahta_boyutu; j++) {
            if (oyun->tahta.hucreler[i][j].tas != ' ') {
                for (int dx = -2; dx <= 2; dx++) {
                    for (int dy = -2; dy <= 2; dy++) {
                        if (gecerli_hamle_mi(oyun, i, j, i + dx, j + dy)) {
                            Hamle h;
                            h.baslangic_x = i;
                            h.baslangic_y = j;
                            h.hedef_x = i + dx;
                            h.hedef_y = j + dy;
                            gecerli_hamleler[*hamle_sayisi] = h;
                            (*hamle_sayisi)++;
                        }
                    }
                }
            }
        }
    }
}

// En iyi hamleyi bulur
Hamle en_iyi_hamleyi_bul(OyunDurumu *oyun) {
    Hamle gecerli_hamleler[MAX_YIGIN];
    int hamle_sayisi;
    gecerli_hamleleri_bul(oyun, gecerli_hamleler, &hamle_sayisi);
    
    int en_iyi_puan = -1;
    Hamle en_iyi_hamle;
    
    for (int i = 0; i < hamle_sayisi; i++) {
        OyunDurumu gecici_oyun = oyun_durumunu_kopyala(oyun);
        hamle_yap(&gecici_oyun, gecerli_hamleler[i].baslangic_x, gecerli_hamleler[i].baslangic_y, gecerli_hamleler[i].hedef_x, gecerli_hamleler[i].hedef_y);
        
        // Basit puanlama
        int puan = gecici_oyun.oyuncular[gecici_oyun.mevcutOyuncu].puan;
        
        if (puan > en_iyi_puan) {
            en_iyi_puan = puan;
            en_iyi_hamle = gecerli_hamleler[i];
        }
    }
    
    return en_iyi_hamle;
}

// Bilgisayar hamlesi
void bilgisayar_hamlesi(OyunDurumu *oyun) {
    Hamle en_iyi_hamle = en_iyi_hamleyi_bul(oyun);
    
    printf("Bilgisayar hamlesi: (%d, %d) -> (%d, %d)\n", en_iyi_hamle.baslangic_x, en_iyi_hamle.baslangic_y, en_iyi_hamle.hedef_x, en_iyi_hamle.hedef_y);
    hamle_yap(oyun, en_iyi_hamle.baslangic_x, en_iyi_hamle.baslangic_y, en_iyi_hamle.hedef_x, en_iyi_hamle.hedef_y);
    
    // Bilgisayar aynı taş ile mümkün olduğunca hamle yapmaya devam eder
    while (tas_yakalanabilir_mi(oyun, en_iyi_hamle.hedef_x, en_iyi_hamle.hedef_y)) {
        en_iyi_hamle = en_iyi_hamleyi_bul(oyun);
        printf("Bilgisayar hamlesi: (%d, %d) -> (%d, %d)\n", en_iyi_hamle.baslangic_x, en_iyi_hamle.baslangic_y, en_iyi_hamle.hedef_x, en_iyi_hamle.hedef_y);
        hamle_yap(oyun, en_iyi_hamle.baslangic_x, en_iyi_hamle.baslangic_y, en_iyi_hamle.hedef_x, en_iyi_hamle.hedef_y);
    }
}

// Oyunun bitip bitmediğini kontrol eder
int oyun_bitti_mi(OyunDurumu *oyun) {
    // Oyunda geçerli hamle kalmadığında oyun sona erer
    // Burada basit bir kontrol yapılmıştır, daha gelişmiş kontroller eklenebilir
    for (int i = 0; i < oyun->tahta_boyutu; i++) {
        for (int j = 0; j < oyun->tahta_boyutu; j++) {
            if (oyun->tahta.hucreler[i][j].tas != ' ') {
                return 0;
            }
        }
    }
    return 1;
}

// Kazananı yazdırır
void kazananı_yazdir(OyunDurumu *oyun) {
    printf("Oyun Bitti!\n");
    puanlari_yazdir(oyun);
    
    if (oyun->oyuncular[0].puan > oyun->oyuncular[1].puan) {
        printf("Kazanan: %s\n", oyun->oyuncular[0].isim);
    } else if (oyun->oyuncular[1].puan > oyun->oyuncular[0].puan) {
        printf("Kazanan: %s\n", oyun->oyuncular[1].isim);
    } else {
        printf("Oyun Berabere!\n");
    }
}

