#include <stdlib.h>
#if 0
recursively defined hashing structure
can have O(1)...ish lookup time with minimal memory footprint

depth is configurable


hash of hashes of ... ints

rhash[outer_hash(el)][mid_hash(el)][inner_hash(el)]

outside is an array

/*
 * array of linked lists
 * [, , , ,]
 *  |
 *  |
 *  |
 *  []
*/
#endif

struct ll_entry{
    struct ll_entry* next;
    union{
        void* data;
        int val;
    };
};

 struct ll{
     struct ll_entry* first, * last;
 };

void insert_ll(struct ll* l, int val, void* data){
    struct ll_entry* ins_e = malloc(sizeof(struct ll_entry));
    if(!l->last){
        l->first = l->last = ins_e;
    }
    else{
        l->last->next = ins_e;
        l->last = ins_e;
    }
    if(data)ins_e->data = data;
    else ins_e->val = val;
}

struct rhash{
    /* it's left up to the user to define hashing functions that */
    int (*hash_func) (int, void*);
    /* n_bux will be set to -1 on the final layer */
    int n_bux;
    /* each entry either has n_bux more entries or a linked list at the final layer */

    union{
        struct rhash* buckets;
        /*struct ll_entry* list;*/
        struct ll* list;
    };
};

void init_rhash(struct rhash* rh, int levels, int n_bux, int (**hfuncs)(int, void*)){
    struct rhash* rhp = rh;
    for(int i = 0; i < levels; ++i){
        rhp->n_bux = n_bux;
        /* TODO: should buckets only be allocated when it's time for that space to be used? */
        rhp->buckets = calloc(sizeof(struct rhash), rhp->n_bux);
        rhp->hash_func = hfuncs[i];
        rhp = rhp->buckets;
    }
    rhp->n_bux = -1;
    rhp->list = NULL;
}

void insert_rhash(struct rhash* rh, int val, void* data){
    struct rhash* rhp = rh;
    while(rhp->n_bux != -1){
        rhp = &rhp->buckets[rhp->hash_func(val, data)];
    }
    /*
     * if(!data){
     * }
    */
}

int example_hash_0(int val, void* data){
    (void)data;
    return val % 7;
}

int example_hash_1(int val, void* data){
    (void)data;
    return val % 3;
}

int example_hash_2(int val, void* data){
    (void)data;
    return val % 11;
}

int main(){
    struct rhash rh;
    int (**hfuncs)(int, void*) = malloc(sizeof(hfuncs)*5);
    hfuncs[0] = example_hash_0;
    hfuncs[1] = example_hash_1;
    hfuncs[2] = example_hash_2;
    hfuncs[3] = example_hash_0;
    hfuncs[4] = example_hash_1;

    init_rhash(&rh, 5, 20, hfuncs);

    return 0;
}
