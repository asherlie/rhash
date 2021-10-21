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
    struct ll* next;
    union{
        void* data;
        int val;
    };
};

struct ll{
    struct ll_entry* first;
};

struct rhash{
    /* it's left up to the user to define hashing functions that */
    int (*hash_func) (int, void*);
    /* n_bux will be set to -1 on the final layer */
    int n_bux;
    /* each entry either has n_bux more entries or a linked list at the final layer */

    union{
        struct rhash* buckets;
        struct ll_entry* list;
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

int main(){
    return 0;
}
