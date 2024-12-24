#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef int TInfo;

typedef struct list{
    TInfo val;
    struct list* next;
} list;

list* list_create (TInfo val, list* t){
    list* r = (list*) malloc(sizeof(list));
    if(r == NULL) return NULL;
    r->val= val;
    r->next=t;
    return r;
}

void list_destroy(list *L) {
    if (L != NULL) {
        list_destroy(L->next);
        L->next = NULL; /* non necessario... */
        free(L);
    }
}

void list_printr(list *L) {
    if(L == NULL) {
        printf("()");
        return;
    }
    printf("(");
    printf("%d,", L->val);
    list_printr(L->next);
    printf(")");
} // time complexity: Θ(n)

int list_equal(list *L1, list *L2) {
    if (L1 == NULL || L2 == NULL) {
        return L1 == NULL && L2 == NULL;
    } else {
        return L1->val == L2->val && list_equal(L1->next, L2->next);
    }
}

int list_length(list *L) {
    if (NULL == L) {
        return 0;
    } else {
        return 1 + list_length(L->next);
    }
}

list *list_insert(list *L, int pos, TInfo newvalue) {
    if(pos == 0) { return list_create(newvalue, L); }
    list *curr = L;
    int i = 0;
    for(; curr != NULL && i < pos - 1; i++, curr = curr->next);
    if(i == pos - 1 && curr != NULL) {
        curr->next = list_create(newvalue, curr->next);
    }
    return L;
}

int main(){
    struct list* L = list_create(2017, NULL); //[2017]
    L = list_create(5, L); //[5, 2017]
    L = list_create(15, L); //[15,5,2017]
    L->next->next->val=11; //[15,5,11]
    L->next=list_create(25,L->next);
    

    return 1;
}