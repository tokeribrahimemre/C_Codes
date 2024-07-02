#include <stdio.h>
#include <stdlib.h>

// f fonksiyonu
int f(int x) {
    
    if(x%2==1){
        return x =((3*x)+1);
    }
    
    return x/2;
}

int main() {
    int n,i;
    int* yeniSepet;

    printf("Pozitif bir tamsayı girin: ");
    scanf("%d", &n);

    int* sepet = (int*)calloc(2, sizeof(int));
    if (sepet == NULL) {
        printf("Hafıza tahsisi başarısız oldu.\n");
        return 1;
    }
    sepet[0] = n;
    int sepetBoyutu = 2;
    int sepetElemanSayısı = 1;

    
    while (n>1) {
        n = f(n);
        if (sepetElemanSayısı == sepetBoyutu) {
            sepetBoyutu *= 2;
          yeniSepet = (int*)realloc(sepet, sepetBoyutu * sizeof(int));
            if (yeniSepet == NULL) {
                printf("Hafıza yeniden boyutlandırma başarısız oldu.\n");
                free(sepet);
                return 1;
            }
            sepet = yeniSepet;
        }
        sepet[sepetElemanSayısı] = n;
        sepetElemanSayısı++;
        
    }
    
    
    printf("Sepet Dizisi:\n");
    for (i = 0; i < sepetElemanSayısı; i++) {
        printf("%d ", sepet[i]);
    }
    printf("\n");
    printf("Hafızada Kaplanan Alan: %d \n", sepetBoyutu );

    
    free(sepet);

    return 0;
}

