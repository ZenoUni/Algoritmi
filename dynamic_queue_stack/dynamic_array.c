#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * TODO: implementare le seguenti funzioni:
 * - darray_append(DArray* da, TInfo value)
 * - darray_insert(DArray* da, int insert_pos, TInfo value)
 * - darray_assert_equals(DArray* da, TInfo* expected, int expected_len)
 */

// NB: compile with gcc "-DDEBUG"
#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

static int GROWING_DELTA = 10;
static int SHRINKING_DELTA = 20;
static float GROWING_FACTOR = 2.5;
static float SHRINKING_FACTOR = 3.0;

typedef int TInfo;

struct SDArray {
    TInfo* item;
    int capacity;
    int size;
};
typedef struct SDArray DArray;

void darray_resize(DArray* da, int new_size);
DArray darray_create(int initial_size);
DArray darray_create_capac(int initial_size, int initial_capacity);
void darray_set(DArray* da, int pos, TInfo value);
void darray_print(DArray* da, char* eol);
void darray_destroy(DArray* da);
void darray_resize_linear(DArray* da, int new_size);
void darray_resize_geometric(DArray* da, int new_size);
void darray_resize(DArray* da, int new_size);
void darray_append(DArray* da, TInfo elem);
void darray_expand(DArray* da, TInfo* arr, int sz);
void darray_insert(DArray* da, int insert_pos, TInfo value);
void darray_assert_equals(DArray* da, TInfo* expected, int expected_len);

static void (*f_resize)(DArray*,int) = &darray_resize_linear;

DArray darray_create(int initial_size) {
    return darray_create_capac(initial_size, initial_size);
}

DArray darray_create_capac(int initial_size, int initial_capacity) {
    assert(initial_capacity >= initial_size);
    DArray a;
    a.item = (TInfo*) malloc(sizeof(TInfo) * initial_capacity);
    assert(initial_size==0 || a.item!=NULL);
    a.capacity = initial_capacity;
    a.size = initial_size;
    return a;
}

void darray_set(DArray* da, int pos, TInfo value) {
    if(da->size <= pos) {
        darray_resize(da, pos+1);
        // for(int i=curr_size; i<pos+1; i++) darray_set(da, i, 0);
    }
    (da->item)[pos] = value;
}

void darray_print(DArray* da, char* eol) {
    printf("[%d/%d]{", da->size, da->capacity);
    if(da->size>0){
        int i = 0;
        for(; i < da->size-1; i++) {
            printf("%d, ", da->item[i]);
        }
        printf("%d", da->item[i]);
    }
    printf("}%s", eol);
}

void darray_destroy(DArray* da) {
    free(da->item);
    da->item = NULL;
    da->capacity = 0;
    da->size = 0;
}

void darray_resize_linear(DArray* da, int new_size) {
    if(new_size < 0) return; 
    LOG("[LINEAR] Resize darray to new_size=%d.\n", new_size);
    if(new_size > da->capacity || (da->capacity - new_size) > SHRINKING_DELTA) {
        const int new_capacity = new_size + GROWING_DELTA;
        LOG("[LINEAR] Setting capacity to %d.\n", new_capacity);
        da->item = (TInfo*) realloc(da->item, sizeof(TInfo) * new_capacity);
        da->capacity = new_capacity;
    }
    da->size = new_size;
}

void darray_resize_geometric(DArray* da, int new_size) {
    if(new_size < 0) return; 
    LOG("[GEOMETRIC] Resize darray to new_size=%d.\n", new_size);
    if(new_size > da->capacity || (da->capacity / new_size) > SHRINKING_FACTOR) {
        const int new_capacity = new_size * GROWING_FACTOR;
        LOG("[GEOMETRIC] Setting capacity to %d.\n", new_capacity);
        da->item = (TInfo*) realloc(da->item, sizeof(TInfo) * new_capacity);
        da->capacity = new_capacity;
    }
    da->size = new_size;
}

void darray_resize(DArray* da, int new_size) {
    f_resize(da, new_size);
}

void darray_append(DArray* da, TInfo value) {
    // Verifica se c'è spazio sufficiente per aggiungere un nuovo elemento
    if (da->size >= da->capacity) {
        // Se non c'è spazio, espandiamo l'array
        darray_resize(da, da->size + 1);  // Incrementiamo di 1, poiché vogliamo aggiungere un elemento
    }
    
    // Aggiungiamo l'elemento alla fine
    da->item[da->size] = value;
    da->size++;
}


void darray_expand(DArray* da, TInfo* arr, int sz) {
    int curr_size = da->size; // NB: keep track of current size as resize will move it
    darray_resize(da, curr_size + sz);
    for(int i=0; i<sz; i++) {
        darray_set(da, curr_size + i, arr[i]);
    }
}

void darray_insert(DArray* da, int insert_pos, TInfo value) {
    // Verifica che la posizione di inserimento sia valida
    if (insert_pos < 0 || insert_pos > da->size) {
        fprintf(stderr, "Errore: posizione di inserimento non valida.\n");
        return;
    }
    
    // Se necessario, ridimensioniamo l'array per fare spazio al nuovo elemento
    if (da->size >= da->capacity) {
        darray_resize(da, da->size + 1);  // Espandiamo di 1 per aggiungere un nuovo elemento
    }
    
    // Spostiamo gli elementi da insert_pos in avanti per fare spazio all'elemento da inserire
    for (int i = da->size; i > insert_pos; i--) {
        da->item[i] = da->item[i - 1];
    }
    
    // Inseriamo il nuovo elemento
    da->item[insert_pos] = value;
    da->size++;
}

void darray_assert_equals(DArray* da, TInfo* expected, int expected_len) {
    // Variabile booleana per determinare se il test ha avuto successo
    bool test_passed = true;

    // Verifica che la dimensione dell'array coincida con la lunghezza attesa
    if (da->size != expected_len) {
        fprintf(stderr, "Errore: lunghezza non corrispondente. Atteso %d, trovato %d.\n", expected_len, da->size);
        test_passed = false;
    }
    
    // Verifica che gli elementi dell'array coincidano con quelli attesi
    for (int i = 0; i < da->size; i++) {
        if (da->item[i] != expected[i]) {
            fprintf(stderr, "Errore: elemento %d non corrisponde. Atteso %d, trovato %d.\n", i, expected[i], da->item[i]);
            test_passed = false;
        }
    }

    // Stampa il risultato del test
    if (test_passed) {
        printf("Test Passed!\n");
    } else {
        printf("Test Failed!\n");
    }
}

void test() {
    printf("*** TEST ***\n");
    DArray da = darray_create(5);
    for(int i=0; i<5; i++) {
        printf("setting d[%d] to %d\n", i, i+1);
        darray_set(&da, i, i+1);
    }
    darray_assert_equals(&da, (int[]){1,2,3,4,5}, 5);
    darray_print(&da,"\n");
    printf("appending 88\n");
    darray_append(&da, 88);
    darray_assert_equals(&da, (int[]){1,2,3,4,5,88}, 6);
    darray_print(&da,"\n");
    darray_resize(&da, 17);
    darray_resize(&da, 3);
    darray_assert_equals(&da, (int[]){1,2,3}, 3);
    darray_expand(&da, (TInfo[]){0,1,2,3,4,5,6,7,8,9,10}, 11);
    darray_print(&da,"\n");
    darray_assert_equals(&da, (int[]){1,2,3,0,1,2,3,4,5,6,7,8,9,10}, 14);
    darray_insert(&da, 2, 55);
    //darray_resize(&da, 100);
    darray_set(&da, 80, 888);
    darray_assert_equals(&da, (int[]){1,2,55,3,0,1,2,3,4,5,6,7,8,9,10}, 15);
    darray_print(&da,"\n\n");
}


int main() {
    f_resize = &darray_resize_linear;
    GROWING_DELTA = 10;
    SHRINKING_DELTA = 12;
    test();

    f_resize = &darray_resize_geometric;
    GROWING_FACTOR = 4.5;
    SHRINKING_FACTOR = 8.5;
    test();
}
