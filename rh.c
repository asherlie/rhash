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
        struct ll* list;
    };
};

void init_rhash(struct rhash* rh, int levels){
    struct rhash* rhp = rh;
    for(int i = 0; i < levels; ++i){
        rhp->n_bux = n_bux;
        rhp->buckets = calloc(sizeof(struct rhash), rhp->n_bux);
        rhp->final_layer = 0;
        rhp = rhp->buckets;
    }
    rhp->final_layer = 1;
    rhp->n_bux = -1;
}

int main(){
    return 0;
}
