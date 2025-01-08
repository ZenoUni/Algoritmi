#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int TInfo;

typedef struct SStack {
    TInfo* item;     // Array per memorizzare gli elementi
    int size;        // Numero di elementi nello stack
    int capacity;    // Capacità massima dello stack
} Stack;

// Crea uno stack con una capacità specificata
Stack stack_create(int cap) {
    Stack s;
    s.item = (TInfo*) malloc(sizeof(TInfo) * cap);
    s.size = 0;       // Lo stack è inizialmente vuoto
    s.capacity = cap;
    return s;
}

// Distrugge lo stack, liberando la memoria allocata
void stack_destroy(Stack* s) {
    if (s->item != NULL) {
        free(s->item);
        s->item = NULL;
    }
    s->size = 0;
    s->capacity = 0;
}

// Aggiunge un elemento in cima allo stack
bool stack_push(Stack* s, TInfo val) {
    if (s->size < s->capacity) {
        s->item[s->size] = val; // Aggiunge il valore alla posizione `size`
        s->size++;              // Incrementa la dimensione dello stack
        return true;
    }
    return false; // Stack pieno
}

// Rimuove e restituisce l'elemento in cima allo stack
TInfo stack_pop(Stack* s) {
    if (s->size == 0) {
        fprintf(stderr, "Cannot pop from an empty stack. Exiting.\n");
        exit(-1); // Errore: stack vuoto
    }
    s->size--; // Decrementa la dimensione dello stack
    return s->item[s->size];
}

// Stampa lo stack (dal fondo alla cima)
void stack_print(Stack* s) {
    printf("STACK = [");
    for (int i = 0; i < s->size; i++) {
        printf("%d", s->item[i]);
        if (i < s->size - 1) {
            printf(", ");
        }
    }
    printf("] INTERNAL REPR = (capacity=%d, size=%d)[", s->capacity, s->size);
    for (int i = 0; i < s->capacity; i++) {
        printf("%d", s->item[i]);
        if (i < s->capacity - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Main per testare lo stack
int main() {
    Stack s = stack_create(4);
    stack_push(&s, 5);
    stack_push(&s, 7);
    stack_print(&s);
    printf("\nPopped from stack: %d\n", stack_pop(&s));
    stack_push(&s, 3);
    stack_push(&s, 1);
    stack_print(&s);
    puts("");
    stack_destroy(&s);
    return 0;
}
