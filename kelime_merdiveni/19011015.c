#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_DICTIONARY_SIZE 1000

typedef struct Node {
    char word[MAX_WORD_LENGTH];
    struct Node *next;
} Node;

typedef struct Stack {
    Node *top;
    struct Stack *next;
} Stack;

typedef struct Queue {
    Stack *front;
    Stack *rear;
    int count;
} Queue;


Stack *createStack();
void push(Stack *s, const char *word);
bool isEmptyQueue(Queue *q);
bool isEmptyStack(Stack *s);
int isOneLetterDifferent(char *word1, char *word2);
void printStack(Stack *stack) ;
Stack *copyStack(Stack *original);
void freeStack(Stack *stack) ;
void clearQueue(Queue *queue);
void findShortestLadder(char *startWord, char *endWord, char **dictionary,int wordcount);
void printQueue(Queue *queue);

int main() {
    
    char startWord[MAX_WORD_LENGTH], endWord[MAX_WORD_LENGTH];
    
   
    
    char **dictionary;
    int dictionarySize = INITIAL_DICTIONARY_SIZE;
    int wordCount = 0;
    FILE *file;

    printf("Başlangıç kelimesini girin : ");
    scanf("%s", startWord);
    printf("Hedef kelimesini girin : ");
    scanf("%s", endWord);
    
    // İlk olarak, belirli bir boyutta bellek ayır
    dictionary = (char **)malloc(dictionarySize * sizeof(char *));
    if (dictionary == NULL) {
        fprintf(stderr, "Bellek ayırma başarısız.\n");
        return 1;
    }

    file = fopen("dictionary.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Sözlük dosyası açılamadı.\n");
        return 1;
    }

    char buffer[MAX_WORD_LENGTH];
    while (fgets(buffer, MAX_WORD_LENGTH, file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';  // Satır sonundaki '\n' karakterini kaldır
        buffer[strcspn(buffer, "\r")] ='\0';
        // Daha fazla yer gerekiyorsa, belleği genişlet
        if (wordCount >= dictionarySize) {
            dictionarySize *= 2;  // Boyutu iki katına çıkar
            dictionary = (char **)realloc(dictionary, dictionarySize * sizeof(char *));
            if (dictionary == NULL) {
                fprintf(stderr, "Bellek genişletme başarısız.\n");
                return 1;
            }
        }

        // Okunan kelimeyi belleğe kopyala
        dictionary[wordCount] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        if (dictionary[wordCount] == NULL) {
            fprintf(stderr, "Bellek ayırma başarısız.\n");
            return 1;
        }
        strcpy(dictionary[wordCount], buffer);
        wordCount++;
    }

    fclose(file);
    findShortestLadder(startWord, endWord, dictionary,wordCount);
    
    free(dictionary);
    return 0;
}



Stack *createStack() {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

void push(Stack *s, const char *word) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->word, word);
    newNode->next = s->top;
    s->top = newNode;
}

char *pop(Stack *s) {
    if (isEmptyStack(s)) {
        return NULL;
    }
    Node *temp = s->top;
    s->top = s->top->next;
    char *word = strdup(temp->word);
    free(temp);
    return word;
}

bool isEmptyStack(Stack *s) {
    return s->top == NULL;
}

Queue *createQueue() {
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue *q, Stack *s) {
    
    if (isEmptyQueue(q)) {
        q->front = q->rear = s;
        q->rear->next =NULL;
    } else {
        q->rear->next = s;
        q->rear = s;
    }
    q->count++;
}
bool isEmptyQueue(Queue *q){
    return q->front==NULL;
}

Stack *dequeue(Queue *q) {
    if (isEmptyQueue(q)) {
        return NULL;
    }
    Stack *s_tmp = q->front;
    q->front = q->front->next;
    
    if (isEmptyQueue(q)) {
        q->rear = NULL;
    }
    q->count--;
    return s_tmp;
}

char *peekStack(Stack *s){
    return s->top->word;
}

int isOneLetterDifferent(char *word1, char *word2) {
    int length1 = strlen(word1);
    int length2 = strlen(word2);
    int diffCount = 0;
    // Eğer kelimelerin uzunlukları farklıysa, direkt olarak 0 döndür
    if (length1 != length2) {
        return 0;
    }
    // Kelimelerin her harfini karşılaştır ve farklı harf sayısını hesapla
    for (int i = 0; i < length1; i++) {
        if (word1[i] != word2[i]) {
            diffCount++;
        }
    }
    // Eğer sadece bir harf farkı varsa, 1 döndür; aksi takdirde 0 döndür
    return (diffCount == 1) ? 1 : 0;
}

int isWordInStack(Stack *stack, char *word) {
    Node *current = stack->top;
    if (strcmp(current->word, word) == 0) {
        return 1; // Kelime bulundu
    }
    return 0; // Kelime stack'te bulunamadı
}
void findShortestLadder(char *startWord, char *endWord, char **dictionary,int wordcount) {
    Queue *queue;
    queue = (Queue *)malloc(sizeof(Queue));
    queue->front =queue->rear =NULL;
    Stack *stack, *newStack;
    char currentWord[MAX_WORD_LENGTH];
    int found = 0,deneme,counter=0;

    // İlk stack oluşturulur ve başlangıç kelimesi eklenir
    stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;
    push(stack, startWord);

    // Stack, queue'ya eklenir
    enqueue(queue, stack);

    while (!isEmptyQueue(queue) && !found) {
        // Queue'dan bir stack çıkarılır ve en üstteki kelime alınır
        
        stack = dequeue(queue);
        strcpy(currentWord, peekStack(stack));
        if (counter!=0) {
            printf("Kuyruga %d yeni stack eklendi\n",counter);
            counter=0;
        }
       
        if (strcmp(currentWord, endWord) == 0) {
            // Hedef kelime bulundu
            found = 1;
            printQueue(queue);
            printStack(stack); // Stack'i yazdır
        } else {
            // Hedef kelime bulunamadı, yeni kelimeler aranır
            for (int i = 0; i < wordcount; i++) {
            
                if (isOneLetterDifferent(currentWord, dictionary[i]) && !isWordInStack(stack, dictionary[i])) {
                    // Yeni stack oluşturulur ve kelime eklenir
                    newStack = copyStack(stack);
                    push(newStack, dictionary[i]);
                    enqueue(queue, newStack);
                    strcpy(dictionary[i], "\0");
                    counter++;
                }
                
            }
        }
        freeStack(stack); // Kullanılmayan stack bellekten serbest bırakılır
    }

    if (!found) {
        printf("Kelime merdiveni bulunamadı.\n");
    }

    // Queue ve kalan stack'ler temizlenir
    clearQueue(queue);
}

void printStack(Stack *stack) {
    Node *current = stack->top;
    int size = 0, i;

    // Stack'teki eleman sayısını bul
    while (current != NULL) {
        size++;
        current = current->next;
    }

    // Stack'teki kelimeleri bir diziye kopyala
    char words[size][MAX_WORD_LENGTH];
    current = stack->top;
    for (i = 0; i < size; i++) {
        strcpy(words[i], current->word);
        current = current->next;
    }

    // Diziyi ters sırada yazdır
    for (i = size - 1; i >= 0; i--) {
        printf("%s\n", words[i]);
    }
}

Stack *copyStack(Stack *original) {
    Stack *copy = (Stack *)malloc(sizeof(Stack));
    Node *current = original->top;
    Node *tempStackTop = NULL;

    // Orijinal stack'in elemanlarını geçici bir stack'e ters sırada kopyala
    while (current != NULL) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        strcpy(newNode->word, current->word);
        newNode->next = tempStackTop;
        tempStackTop = newNode;
        current = current->next;
    }

    // Geçici stack'in elemanlarını yeni stack'e kopyala
    copy->top = NULL;
    while (tempStackTop != NULL) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        strcpy(newNode->word, tempStackTop->word);
        newNode->next = copy->top;
        copy->top = newNode;
        Node *temp = tempStackTop;
        tempStackTop = tempStackTop->next;
        free(temp);
    }

    return copy;
}

void freeStack(Stack *stack) {
    Node *current = stack->top;
    Node *temp;

    // Stack'teki tüm düğümleri serbest bırak
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }

    // Stack'i serbest bırak
    free(stack);
}
void clearQueue(Queue *queue) {
    Stack *current = queue->front;
    Stack *temp;

    // Kuyruktaki tüm düğümleri serbest bırak
    while (current != NULL) {
        temp = current;
        current = current->next;
        freeStack(temp);  // Stack'i serbest bırak
    }
    // Kuyruğu başlangıç durumuna getir
    queue->front = NULL;
    queue->rear = NULL;
}
void printQueue(Queue *queue) {
    Stack *current = queue->front;
    int count = 1;

    // Kuyruktaki tüm stack'leri yazdır
    while (current != NULL) {
        printf("Stack %d:\n", count);
        printStack(current);
        printf("\n");
        current = current->next;
        count++;
    }

    if (count == 1) {
        printf("Kuyruk boş.\n");
    }
}
