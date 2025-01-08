#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Esercizio: implementare le seguenti funzioni per una coda (queue) implementata su array statico: 
 *   - queue_add
 *   - queue_remove
 */

typedef int TInfo;

/* Queue implementation */

struct SQueue {
    TInfo *item;
    int from;
    int size;
    int capacity;
};
typedef struct SQueue Queue;

Queue queue_create(int capacity) {
    Queue q;
    q.item = (TInfo*) malloc(sizeof(TInfo) * capacity);
    q.capacity = capacity;
    q.size = 0;
    q.from = 0;
    return q;
}

void queue_destroy(Queue *q) {
    if(q->item != NULL) {
        free(q->item);
        q->item = NULL;
    }
    q->size = 0;
    q->capacity = 0;
    q->from = 0;
}

bool queue_add(Queue *q, TInfo value) {
    if(q->size < q->capacity) {
        printf("Adding %d to queue.\n", value);
        q->item[(q->from + q->size) % q->capacity] = value; // Calcolo corretto della posizione
        q->size++;
        return true;
    }
    return false; // Coda piena
}

TInfo queue_remove(Queue *q) {
    if(q->size == 0) {
        fprintf(stderr, "%s", "Cannot remove from an empty queue. Exiting.\n");
        exit(-1);
    }
    TInfo result = q->item[q->from];
    q->from = (q->from + 1) % q->capacity; // Uso delle parentesi per priorità corretta
    q->size--;
    return result;
}

void queue_print(Queue *q) {
    printf("QUEUE = [");
    if(q->size > 0) {
        int i = q->from;
        int to = (q->from + q->size - 1) % q->capacity; // Priorità corretta
        for (; i != to; i = (i + 1) % q->capacity) { // Iterazione circolare
            printf("%d, ", q->item[i]);
        }
        printf("%d", q->item[i]);
    }
    printf("] INTERNAL REPR = (capacity=%d; from=%d; to=%d)[", q->capacity, q->from, (q->from + q->size) % q->capacity);
    for(int i = 0; i < q->capacity; i++) {
        printf("%d,", q->item[i]);
    }
    printf("]\n");
}

int main() {
    Queue q = queue_create(4);
    queue_add(&q, 5);
    queue_add(&q, 7);
    queue_print(&q);
    printf("\nRemoved from queue: %d\n", queue_remove(&q));
    queue_add(&q, 3);
    queue_add(&q, 1);
    queue_print(&q);
    puts("");
    queue_destroy(&q);
    return 0;
}
