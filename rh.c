#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    /*int (*hash_func) (int, void*);*/
    /* n_bux will be set to -1 on the final layer */
    /*int n_bux;*/
    /* each entry either has n_bux more entries or a linked list at the final layer */

    union{
        struct rhash** buckets;
        /*struct ll_entry* list;*/
        struct ll* list;
    };
};

struct rhash_container{
    int n_bux, levels, data_sz;
    int (**hash_funcs) (int, void*);
    struct rhash rh;
};

/*
 * [a, b, c, d, e, f, g]
 * 
 * a = [x, y, z, xx, yy]
*/

void init_rhash(struct rhash_container* rhc, int levels, int n_bux, int (**hfuncs)(int, void*), int data_sz){
    struct rhash* rh = &rhc->rh;

    rhc->hash_funcs = hfuncs;
    rhc->levels = levels;
    rhc->n_bux = n_bux;
    rhc->data_sz = data_sz;
    
    /*rh->n_bux = n_bux;*/
    /*rh->buckets = calloc(sizeof(struct rhash*), rhc->n_bux);*/
    rh->buckets = NULL;

    #if 0
    for(int i = 0; i < levels; ++i){
        rhp->n_bux = n_bux;
        /* TODO: should buckets only be allocated when it's time for that space to be used? */
        rhp->buckets = calloc(sizeof(struct rhash*), rhp->n_bux);
        printf("created level %i bucket at %p\n", i, (void*)rhp->buckets);
        rhp->hash_func = hfuncs[i];
        rhp = rhp->buckets;
    }
    rhp->n_bux = -1;
    rhp->list = NULL;
    #endif
}

int hash_exec(struct rhash_container* rhc, int idx, int val, void* data){
    return rhc->hash_funcs[idx](val, data) % rhc->n_bux;
}

void insert_rhash(struct rhash_container* rhc, int val, void* data){
    struct rhash* rh = &rhc->rh, * tmp; 
    int tmp_hash;

    for(int i = 0; i < rhc->levels; ++i){
        if(!rh->buckets)rh->buckets = calloc(sizeof(struct rhash*), rhc->n_bux);
        tmp_hash = rhc->hash_funcs[i](val, data);
        /*printf("laid foundation at level %i: %p\n", i, (void*)rh->buckets);*/
        /*rh->n_bux = 20;*/
        /* filling in the actual rhash */
        if(!(tmp = rh->buckets[tmp_hash])){
            rh->buckets[tmp_hash] = calloc(sizeof(struct rhash), 1);
            /*printf("inserted an rhash at %p\n", (void*)rh->buckets[tmp_hash]);*/
        }
        rh = rh->buckets[tmp_hash];
        /*rh->n_bux = 20;*/
        /*
         * if(!(tmp = rh->buckets[tmp_hash])){
         *     rh->buckets[tmp_hash] = calloc(sizeof(struct rhash*), 20);
         *     rh->n_bux = 20;
         *     printf("inserted level %i at %p\n", i, (void*)rh);
         *     rh = rh->buckets[tmp_hash];
         * }
        */
    }

    if(!rh->list)rh->list = calloc(1, sizeof(struct ll));
    insert_ll(rh->list, val, data);
}

struct ll_entry* lookup_rhash(struct rhash_container* rhc, int val, void* data){
    struct rhash* rh = &rhc->rh;
    int tmp_hash;
    for(int i = 0; i < rhc->levels; ++i){
        tmp_hash = rhc->hash_funcs[i](val, data);
        if(!rh->buckets || !rh->buckets[tmp_hash])return NULL;
        rh = rh->buckets[tmp_hash];
    }

    for(struct ll_entry* l = rh->list->first; l; l = l->next){
        if(data){
            if(!memcmp(data, l->data, rhc->data_sz))return l;
        }
        else{
            if(l->val == val)return l;
        }
    }
    return NULL;
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
    struct rhash_container rhc;
    int (**hfuncs)(int, void*) = malloc(sizeof(hfuncs)*5);
    hfuncs[0] = example_hash_0;
    hfuncs[1] = example_hash_1;
    hfuncs[2] = example_hash_2;
    hfuncs[3] = example_hash_0;
    hfuncs[4] = example_hash_1;

    init_rhash(&rhc, 5, 20, hfuncs, -1);

    printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));

    insert_rhash(&rhc, 32, NULL);
    insert_rhash(&rhc, 32, NULL);

    /*
     * insert_rhash(&rhc, 263, NULL);
     * insert_rhash(&rhc, 494, NULL);
     * insert_rhash(&rhc, 725, NULL);
     * insert_rhash(&rhc, 956, NULL);
    */

    printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
    printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
    printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
    printf("exists: %p\n", (void*)lookup_rhash(&rhc, 263, NULL));

    return 0;
}
