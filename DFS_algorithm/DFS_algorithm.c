//
//  main.c
//  veri_yapilari_proje
//
//  Created by İbrahim Emre Toker on 20.05.2024.
//

#include <stdio.h>
#include <stdlib.h>

#define MAKSIMUM 100
//kenarların agırlık ve next dugumlerini tutan struct yapısı
typedef struct Node{
    int dugum;
    int agirlik;
    struct Node* next;
}NODE;
//grafın baglantı listesini ve buyuklugunu tutan struct
typedef struct  Graph{
    int dugumsayisi;
    NODE** baglantiListesi;
}GRAPH;


NODE* createNode(int hedef,int agirlik);
GRAPH* createGraph(int dugumler);
void addKenar(GRAPH* graph,int kaynak,int hedef,int agirlik);
void DFS(GRAPH* graph, int dugum,int* gidildi, int* yol, int* yolIndex,int *toplamUzunluk,int start,int*cokgenler,int*cokgenSayisi,int *genler,int *uzunluklar,int *j);
void findCokgenler(GRAPH* graph,int *j);
void printEdges(GRAPH* graph);

int main(int argc, const char * argv[]) {
    int i,N,M,kaynak,hedef,agirlik, j =0; 
    
    printf("Dugum Sayısını Giriniz: ");
    scanf("%d",&N);
    printf("Kenar Sayısını Giriniz: ");
    scanf("%d",&M);
    GRAPH* graph = createGraph(N);
    
    printf("Kenar Bilgilerini Giriniz: (KAYNAK,HEDEF,AGIRLIK) \n");
    for (i=0; i<M; i++) {
        scanf("%d %d %d",&kaynak,&hedef,&agirlik);
        addKenar(graph, kaynak, hedef, agirlik);
    }
    printEdges(graph);
    findCokgenler(graph,&j);
    return 0;
}


//kenarlar için node
NODE* createNode(int hedef,int agirlik){
    NODE* yeniNode = malloc(sizeof(NODE));
    yeniNode->dugum =hedef;
    yeniNode->agirlik = agirlik;
    yeniNode->next = NULL;
    return yeniNode;
}
//graf yapının allokasyonunu yapmak için fonksiyon
GRAPH* createGraph(int dugumler){
    int i;
    GRAPH* yeniGraph =malloc(sizeof(GRAPH));
    yeniGraph->dugumsayisi =dugumler;
    yeniGraph->baglantiListesi =malloc(dugumler*sizeof(NODE*));
    
    for (i=0; i<dugumler; i++) {
        yeniGraph->baglantiListesi[i] =NULL;
    }
    return  yeniGraph;
}
//karşılıklı olarak kenar nodelarını birbirine baglayan fonksiyon
void addKenar(GRAPH* graph,int kaynak,int hedef,int agirlik){
    NODE* yeniNode = createNode(hedef, agirlik);
    yeniNode->next =graph->baglantiListesi[kaynak];
    graph->baglantiListesi[kaynak] =yeniNode;
    
    yeniNode =createNode(kaynak,agirlik);
    yeniNode->next =graph->baglantiListesi[hedef];
    graph->baglantiListesi[hedef] =yeniNode;
}

//dfs algoritması ile yolları  ve cokgenleri bulan fonksiyon
void DFS(GRAPH* graph, int dugum,int* gidildi, int* yol, int* yolIndex,int *toplamUzunluk,int start,int*
    cokgenler,int*cokgenSayisi, int *genler,int *uzunluklar,int *j){
    //control degeri aynı cokgenden tekrar gecmemesi icin kontrol
    int i,control = 0;
    gidildi[dugum] =1;
    yol[(*yolIndex)++] = dugum ;
    
    //kenarlistesinin baslangıcını almak icin
    NODE* tmp =graph->baglantiListesi[dugum];
    while (tmp){
       //dugumlere gidilip gidilmedigini kontrol etmek icin
        if (!gidildi[tmp->dugum]){
            *toplamUzunluk +=tmp->agirlik;
            //recursive fonksiyon kullanarak yolları gezmek icin
            DFS(graph, tmp->dugum, gidildi, yol, yolIndex, toplamUzunluk, start, cokgenler, cokgenSayisi,genler,uzunluklar,j);
            *toplamUzunluk -=tmp->agirlik;
            control=0;
            
            //yolIndex degeri 2 den buyuk ise cokgeni ekler
        }else if (tmp->dugum ==start && *yolIndex > 2){
            //aynı cokgenin daha önceden eklenip eklenmedigini kontrol etmek icin
            for (i=0; i<MAKSIMUM; i++) {
                if(genler[i] == (*yolIndex) && uzunluklar[i]== (*toplamUzunluk)+tmp->agirlik ){
                    control = 1;
                }
            }
            if (control == 0) {
                //cokgenin yolunu ve uzunlugunu bastıran fonk
                yol[*yolIndex] = start;
                    printf("%d'gen: ", *yolIndex);
                    for (i=0;i<= *yolIndex;i++) {
                        printf("%d ",yol[i]);
                    }
                    printf("Uzunluk: %d\n", *toplamUzunluk + tmp->agirlik);
                genler[(*j)] = (*yolIndex);
                //onceden basılan cokgenleri kontrol eden fonk
                uzunluklar[(*j)] = (*toplamUzunluk + tmp->agirlik);
                (*cokgenSayisi)++;
                (*j)++;
                    cokgenler[*yolIndex -2]++;
            }
                
        }
        tmp = tmp->next;
    }
    gidildi[dugum] =0;
    (*yolIndex)--;
}
//cokgenleri bulan fonksiyon
void findCokgenler(GRAPH* graph,int *j){
    int i;
    int gidildi[MAKSIMUM] = {0};
    int yol[MAKSIMUM];
    int yolIndex = 0;
    int toplamUzunluk = 0;
    int cokgenler[MAKSIMUM] ={0};
    int cokgenSayisi = 0;
    int uzunluklar[MAKSIMUM] ={0};
    int genler[MAKSIMUM] ={0};
    
    for (i = 0; i < graph->dugumsayisi ; i++) {
        
        DFS(graph, i, gidildi, yol, &yolIndex, &toplamUzunluk, i, cokgenler, &cokgenSayisi,genler,uzunluklar,j);
    }
    printf("Şekil sayisi: %d\n", cokgenSayisi);
    for (i = 1; i < MAKSIMUM;i++) {
        if(cokgenler[i] > 0){
            printf("%d gen sayisi: %d\n",i + 2,cokgenler[i]);
        }
    }
}

void printEdges(GRAPH* graph) {
    for (int v = 0; v < graph->dugumsayisi; v++) {
        NODE* temp = graph->baglantiListesi[v];
        while (temp) {
            if (v < temp->dugum) {  // Bu kontrol, çift yönlü kenarları yalnızca bir kez yazdırmak için
                printf("Kenar: %d - %d, Ağırlık: %d\n", v, temp->dugum, temp->agirlik);
            }
            temp = temp->next;
        }
    }
}

