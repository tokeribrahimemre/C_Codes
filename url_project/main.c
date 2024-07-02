//
//  main.c
//  veri_yapilari1
//
//  Created by İbrahim Emre Toker on 17.03.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINES 100 // Maksimum satır sayısı
#define MAX_LINE_LENGTH 40// Maksimum satır uzunluğu
struct my_cache{
    char web_adress[MAX_LINE_LENGTH];
    unsigned int counter: 5;
    // pointer to the next element in the linked list:
    struct my_cache *next;
    struct my_cache *prev;
};
typedef struct my_cache My_Cache;

//FUNCTION PROTOYPES
My_Cache *create_chache_element();// Element olusturan fonk.
void add_My_Cache_element(My_Cache *e,My_Cache **head,My_Cache **prev);// Linked liste ekleme
void insert2_after(My_Cache *p, My_Cache **q);//istenilen degerden sonra ekleme
void My_Cache_veri_okuma(My_Cache *p);//verileri el ile okuma
My_Cache *find(My_Cache *head);// istenilen node u bulma
void delete_element(My_Cache **goner, My_Cache **head);//node silme
void print_My_Cache_fully(My_Cache *head,My_Cache *prev); // buffer elemanlarını yazdırma
void print_My_Cache(My_Cache *p);
My_Cache *find_with(My_Cache *head,char *name);// name adlı string ile verilen degerleri bulma
void My_Cache_veri_okuma_with(My_Cache *p,char *name);// name adlı string ile verilen degerleri okuma
void max_lim(My_Cache *p, My_Cache **q);// buffer limitini gectikten sonra yapılan islem
void deleteList(My_Cache **head_ref);//tüm listeyi silme


int main(int argc, const char * argv[]) {
    
    My_Cache *goner_My_Cache, *q_My_Cache, *p_My_Cache, *head_My_Cache = NULL,*prev =NULL,*p_control=NULL,*p_for=NULL,*temp=NULL;
    // goner : silinecek ,head : linked list baslangıc,p_control : find fonk ile bulunan control,p_for: donguler icin pointer
    int i,selection=20,size_counter=0,buffer_limit=10;
    //selection switch case icin,size_counter catch size count
    int line_count = 0; // Okunan satır sayısı
    
    char name_2[39];//el ile girilen deger
    FILE* file;
    
    
    while (selection!=0) {
        printf("Veri girmek icin '1'\nText dosyasindan veri okumak icin '2'\nCikis icin '0' : ");
        scanf("%d",&selection);
        switch (selection) {//selection islemi için tasarlanan menü
            case 1:
                if (head_My_Cache==NULL) {//ilk eleman yoksa ilk elemanı tanımlayan if
                    add_My_Cache_element(create_chache_element(),&head_My_Cache,&prev);
                    My_Cache_veri_okuma(head_My_Cache);
                }
                else{ //ilk eleman varsa linked liste'in devamına ekleyen liste
                    printf("Enter web_adress  :");
                    scanf("%s", name_2);
                    p_control = find_with(head_My_Cache,name_2);
                    if (p_control!=NULL) {// aynı adress'ten olup olmadığnı kontrol eden varsa 1 ekleyen yoksa yeni oluşturan sorgu
                        p_control->counter= p_control->counter +1;
                        if(p_control->counter >= 5){
                            max_lim(p_control, &head_My_Cache);
                        }
                    }
                    else{
                        p_control=create_chache_element();
                        add_My_Cache_element(p_control,&head_My_Cache,&prev);
                        My_Cache_veri_okuma_with(p_control,name_2);
                    }
                }
                break;
            case 2:
                
                file = fopen("/Users/ibrahimemretoker/Desktop/veri_yapilari1_deneme/veri_yapilari1_deneme/Bel2.txt", "r"); // Okunacak metin belgesi
                if (file == NULL) {
                    fprintf(stderr, "Dosya açma hatası!");
                    return EXIT_FAILURE;
                }

                char lines[MAX_LINES][MAX_LINE_LENGTH]; // Satırlar için dizi
                
                // Dosyadan satır satır oku ve diziye aktar
                while (fgets(lines[line_count], MAX_LINE_LENGTH, file) != NULL && line_count < MAX_LINES) {
                    // Satır sonundaki '\n' karakterini kaldır
                    size_t len = strlen(lines[line_count]);
                    if (len > 0 && lines[line_count][len - 1] == '\n')
                        lines[line_count][len - 1] = '\0';
                    
                    line_count++; // Okunan satır sayısını arttır
                }
                fclose(file); // Dosyayı kapat
                if (head_My_Cache==NULL) { // textten okunan stringlerin linked liste işlenmesi
                    p_control = create_chache_element();
                    add_My_Cache_element(p_control,&head_My_Cache,&prev);
                    My_Cache_veri_okuma_with(p_control,lines[0]);
                    for (i = 1; i < line_count; i++) {
                        p_control = find_with(head_My_Cache,lines[i]);
                        if (p_control!=NULL) {
                            p_control->counter= p_control->counter +1;
                                                    }
                        else{
                            p_control = create_chache_element();
                            add_My_Cache_element(p_control,&head_My_Cache,&prev);
                            My_Cache_veri_okuma_with(p_control,lines[i]);
                        }
                    }
                }
                else{
                    for (i = 0; i < line_count; i++) {
                        p_control = find_with(head_My_Cache, lines[i]);
                        if (p_control!=NULL) {
                            p_control->counter= p_control->counter +1;
                            
                        }
                        else{
                            add_My_Cache_element(create_chache_element(),&head_My_Cache,&prev);
                            My_Cache_veri_okuma_with(p_control,lines[i]);
                        }
                    }
                }
                break;
            case 0:
                selection = 0;
                break;
            default:
                printf("yanlis deger girdiniz\n");
                break;
        }
        for (p_for = head_My_Cache; p_for->next != NULL; p_for=p_for->next){//linked list size ini ögrenmek icin
            size_counter++;
        }
        while(size_counter>=buffer_limit) {// buffer limiti aşılmışsa sonraki degerleri silen dongu
            temp = p_for->prev;
            p_for->prev->next = NULL;
            free(p_for);
            p_for=temp;
            size_counter--;
        }
        printf("size : %d\n",size_counter+1);
        p_control=NULL;
        size_counter=0;
        print_My_Cache_fully(head_My_Cache, prev);
    }
    printf("\nlisteyi silmek icin 1:");// listeyi silmek için switch case
    scanf("%d",&selection);
    switch (selection) {
        case 1:
            deleteList(&head_My_Cache);
            break;
        default:
            printf("liste duruyor");
            break;
    }

    
    return 0;
}


//FUNCTIONS
My_Cache *create_chache_element(){
    My_Cache *p=NULL;
    p=(My_Cache*) malloc (sizeof (My_Cache));
    if(p == NULL) {
        printf("create_chace_element (): malloc failed. \n");
        exit(1);
        }
    p->next=NULL;
    p->prev=NULL;
    return p;
}
void add_My_Cache_element(My_Cache *e,My_Cache **head,My_Cache **prev){
    My_Cache *p=NULL;
    if(*head==NULL){
    *head=e;
    return;
    }
    for (p = *head; p->next != NULL; p=p->next);
    p->next=e;
    e->prev=p;
    *prev=e;
}
void max_lim(My_Cache *p, My_Cache **head){
    My_Cache *temp=NULL;
    My_Cache *func_head = (*head);
    if(p ==NULL || func_head==NULL){
    printf("insert_after(): Bad arguments \n");
        temp = func_head->next;
        func_head = p;
        p->next =temp;
        p->prev = func_head;
        return;
    }
    
}
void insert_after(My_Cache *p, My_Cache **q){
    My_Cache *temp=NULL;
    My_Cache *func_q = (*q);
    if(p ==NULL || func_q==NULL || p==func_q || func_q->next == p){
    printf("insert_after(): Bad arguments \n");
    return;
    }
    temp = func_q->next;
    p->prev=func_q;
    temp->prev=p;
    p->next=temp;
    func_q->next=p;
}

void My_Cache_veri_okuma(My_Cache *p){
    
    printf("Enter web_adress1  :");
    scanf("%s", p->web_adress);
    p->counter = 0;
}

void My_Cache_veri_okuma_with(My_Cache *p,char *name){
    strncpy(p->web_adress, name, 20);
    p->counter = 1;
}

My_Cache *find(My_Cache *head){
   My_Cache *p=NULL;
    char name[9];
    printf(" aramak icin web adress giriniz : ");
    scanf("%s",name);
   
    for(p=head; p!= NULL; p=p->next){
        if(strcmp(p->web_adress, name) == 0) // returns 0, if 2 strings are same
            return p;
    }
    return NULL;
}

My_Cache *find_with(My_Cache *head,char *name){
   My_Cache *p=NULL;
    
    for(p=head; p!= NULL; p=p->next){
        if(strcmp(p->web_adress, name) == 0) // returns 0, if 2 strings are same
            return p;
    }
    return NULL;
}

void delete_element(My_Cache **goner, My_Cache **head){
    My_Cache *p=NULL;
    My_Cache *temp=(*goner)->next;
    if(*goner == *head){
        *head=(*goner)->next;
        temp=NULL;
    }
    else{
        
        for(p=*head; (p!=NULL) && (p->next != *goner); p=p->next); // null statement
    }
    if(p == NULL){
        printf("delete_element(): could not find the element \n");
        return;
    }
    temp->prev=p;
    p->next=p->next->next;
    free(*goner);
}
void print_My_Cache_fully(My_Cache *head,My_Cache *prev){
    int j;
    char sw_val;
    My_Cache *p=NULL;
    printf("ileri icin \"1\" geri icin \"2\" \n ");
    do{
        
        scanf(" %c",&sw_val);
        
        switch(sw_val){
            case '1':
                printf("\n");
                for(j=0, p=head; p != NULL; p=p->next, j++){
                    printf("%d-th adress: %s : %d\n", j+1, p->web_adress, p->counter);
                }
                break;
            case '2':
                printf("\n");
                for(j=0, p=prev; p != NULL; p=p->prev, j++){
                    printf("%d-th adress: %s : %d\n", j+1, p->web_adress, p->counter);
                }
            default:
                printf("yanlis deger girdiniz yeniden giriniz\n");
                break;
        }
    }while(sw_val != '1' && sw_val != '2');
}

void print_My_Cache(My_Cache *p){
    printf(" %s\t %d", p->web_adress, p->counter);
}
void deleteList(My_Cache **head_ref) {
    My_Cache *current = *head_ref;
    My_Cache *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL; // Bağlı liste başı NULL olarak ayarlanır
    printf("liste silindi");
}
