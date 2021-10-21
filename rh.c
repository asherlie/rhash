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
    /* n_bux will be set to -1 on the final layer */
    int n_bux;
    /* each entry either has n_bux more entries or a linked list at the final layer */
    union{
        struct rhash* buckets;
        struct ll_entry* list;
    };
};

void init_rhash(struct rhash* rh, int levels, int n_bux){
    struct rhash* rhp = rh;
    for(int i = 0; i < levels; ++i){
        rhp->n_bux = n_bux;
        rhp->buckets = calloc(sizeof(struct rhash), rhp->n_bux);
        rhp = rhp->buckets;
    }
    rhp->n_bux = -1;
    rhp->list = NULL;
}

int main(){
    return 0;
}
