#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "myStructs.h"
#include "fonksiyonlar.h"

#define MAX_BOYUT 20  // Tahta boyutu için maksimum değer
#define MAX_YIGIN 100 // Yığın boyutu için maksimum değer

// Taş türleri
char taslar[] = {'A', 'B', 'C', 'D', 'E', 'F'};

int main() {
    OyunDurumu oyun; // Oyun durumu değişkeni
    OyunYigini geriAlYigini = { .tepe = -1 }; // Geri alma yığını, başlangıçta boş
    OyunYigini yenidenYapYigini = { .tepe = -1 }; // Yeniden yapma yığını, başlangıçta boş
    int boyut; // Tahta boyutu

    // Kullanıcıdan tahta boyutunu al
    printf("Tahta boyutunu giriniz (max 20): ");
    scanf("%d", &boyut);
    if (boyut > MAX_BOYUT) {
        printf("Boyut 20'den büyük olamaz.\n");
        return 1;
    }
    tahta_baslat(&oyun, boyut); // Tahtayı başlat
    oyun_modu_sec(&oyun); // Oyun modunu seç
    oyun.mevcutOyuncu = 0; // Başlangıçta geçerli oyuncu olarak ilk oyuncuyu seçiyoruz
    tahta_yazdir(&oyun); // Tahtayı yazdır

    int x1, y1, x2, y2; // Hamle koordinatları
    while (1) {
        if (oyun.oyuncular[oyun.mevcutOyuncu].tipi == INSAN) { // Geçerli oyuncu insan mı?
            // Hamle veya komut için kullanıcıdan giriş al
            printf("Hamle yapmak için taşın ve hedef hücrenin koordinatlarını giriniz (x1 y1 x2 y2) veya undo/redo/save/load (u/r/s/l): ");
            char komut[5];
            scanf("%s", komut);

            // Komutlara göre işlemleri gerçekleştir
            if (komut[0] == 'u') { // Undo işlemi
                if (geriAlYigini.tepe >= 0) {
                    yigina_ekle(&yenidenYapYigini, oyun_durumunu_kopyala(&oyun)); // Durumu yeniden yap yığınına ekle
                    oyun = yigindan_cikar(&geriAlYigini); // Geri al yığınından çıkar
                    tahta_yazdir(&oyun); // Tahtayı yazdır
                } else {
                    printf("Geri alma yapılamaz.\n");
                }
            } else if (komut[0] == 'r') { // Redo işlemi
                if (yenidenYapYigini.tepe >= 0) {
                    yigina_ekle(&geriAlYigini, oyun_durumunu_kopyala(&oyun)); // Durumu geri al yığınına ekle
                    oyun = yigindan_cikar(&yenidenYapYigini); // Yeniden yap yığınından çıkar
                    tahta_yazdir(&oyun); // Tahtayı yazdır
                } else {
                    printf("Yeniden yapma yapılamaz.\n");
                }
            } else if (komut[0] == 's') { // Save işlemi
                oyun_kaydet(&oyun, "oyun_kaydi.dat"); // Oyunu kaydet
            } else if (komut[0] == 'l') { // Load işlemi
                oyun_yukle(&oyun, "oyun_kaydi.dat"); // Oyunu yükle
                tahta_yazdir(&oyun); // Tahtayı yazdır
            } else {
                // Hamle koordinatlarını al
                sscanf(komut, "%d %d %d %d", &x1, &y1, &x2, &y2);
                if (gecerli_hamle_mi(&oyun, x1, y1, x2, y2)) { // Geçerli hamle mi kontrol et
                    yigina_ekle(&geriAlYigini, oyun_durumunu_kopyala(&oyun)); // Durumu geri al yığınına ekle
                    hamle_yap(&oyun, x1, y1, x2, y2); // Hamleyi yap
                    tahta_yazdir(&oyun); // Tahtayı yazdır
                    
                    // Aynı taş ile mümkün olduğunca hamle yapmaya devam et
                    while (tas_yakalanabilir_mi(&oyun, x2, y2)) {
                        printf("Ek hamle yapmak için hedef hücrenin koordinatlarını giriniz (x2 y2): ");
                        scanf("%d %d", &x1, &y1);
                        if (gecerli_hamle_mi(&oyun, x2, y2, x1, y1)) {
                            hamle_yap(&oyun, x2, y2, x1, y1); // Hamleyi yap
                            tahta_yazdir(&oyun); // Tahtayı yazdır
                            x2 = x1;
                            y2 = y1;
                        } else {
                            printf("Geçersiz hamle. Tekrar deneyiniz.\n");
                        }
                    }

                    while (yenidenYapYigini.tepe >= 0) yigindan_cikar(&yenidenYapYigini); // Redo yığınını temizle
                } else {
                    printf("Geçersiz hamle. Tekrar deneyiniz.\n");
                }
            }
        } else if (oyun.oyuncular[oyun.mevcutOyuncu].tipi == BILGISAYAR) { // Geçerli oyuncu bilgisayar mı?
            bilgisayar_hamlesi(&oyun); // Bilgisayar hamlesi yap
            tahta_yazdir(&oyun); // Tahtayı yazdır
        }

        puanlari_yazdir(&oyun); // Puanları yazdır

        // Oyun sonunu kontrol et
        if (oyun_bitti_mi(&oyun)) {
            kazananı_yazdir(&oyun); // Kazananı yazdır
            break;
        }

        // Sıradaki oyuncuya geç
        oyun.mevcutOyuncu = (oyun.mevcutOyuncu + 1) % 2;
    }

    return 0;
}

