#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if 0
recursively defined hashing structure
can have O(1)...ish lookup time with minimal memory footprint

depth is configurable


hash of hashes of ... ints
#endif

int collisions;

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
    /* each entry either has n_bux more entries or a linked list at the final layer */
    union{
        struct rhash** buckets;
        struct ll* list;
    };
};

struct rhash_container{
    int n_bux, levels, data_sz;
    int (**hash_funcs) (int, void*);
    struct rhash rh;
};

void init_rhash(struct rhash_container* rhc, int levels, int n_bux, int (**hfuncs)(int, void*), int data_sz){
    struct rhash* rh = &rhc->rh;

    rhc->hash_funcs = hfuncs;
    rhc->levels = levels;
    rhc->n_bux = n_bux;
    rhc->data_sz = data_sz;
    
    rh->buckets = NULL;
}

int hash_exec(struct rhash_container* rhc, int idx, int val, void* data){
    return rhc->hash_funcs[idx](val, data) % rhc->n_bux;
}

void insert_rhash(struct rhash_container* rhc, int val, void* data){
    struct rhash* rh = &rhc->rh;
    int tmp_hash;

    printf("inserting in: ");
    for(int i = 0; i < rhc->levels; ++i){
        if(!rh->buckets)rh->buckets = calloc(sizeof(struct rhash*), rhc->n_bux);
        tmp_hash = hash_exec(rhc, i, val, data);
        printf("%.2i ", tmp_hash);

        /* filling in the actual rhash */
        if(!rh->buckets[tmp_hash]){
            rh->buckets[tmp_hash] = calloc(sizeof(struct rhash), 1);
        }
        rh = rh->buckets[tmp_hash];
    }
    puts("");

    if(!rh->list)rh->list = calloc(1, sizeof(struct ll));
    insert_ll(rh->list, val, data);
}

struct ll_entry* lookup_rhash(struct rhash_container* rhc, int val, void* data){
    struct rhash* rh = &rhc->rh;
    int tmp_hash;
    for(int i = 0; i < rhc->levels; ++i){
        tmp_hash = hash_exec(rhc, i, val, data);
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
        ++collisions;
    }
    return NULL;
}

int example_hash_0(int val, void* data){
    (void)data;
    return val % 100000;
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

int example_hash_3(int val, void* data){
    (void)data;
    return val % 17;
}

int example_hash_4(int val, void* data){
    (void)data;
    return val % 19;
}

int main(){
    struct rhash_container rhc;
    int depth = 5;
    int (*defined_funcs[5])(int, void*) = {example_hash_0, example_hash_1, example_hash_2, example_hash_3, example_hash_4};
    int (**hfuncs)(int, void*) = malloc(sizeof(hfuncs)*depth);

    for(int i = 0; i < depth; ++i){
        hfuncs[i] = defined_funcs[i%5];
        /* TODO: why does the below cause many collisions? */
        /*hfuncs[i] = defined_funcs[i%3];*/
    }

    collisions = 0;
    init_rhash(&rhc, depth, 100, hfuncs, -1);

    for(int i = 0; i < 100000; ++i){
        insert_rhash(&rhc, i, NULL);
    }

    for(int i = 0; i < 100000; ++i){
        lookup_rhash(&rhc, i, NULL);
    }

    printf("%i collisions!\n", collisions);


/*
 *     printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
 * 
 *     printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
 *     printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
 *     printf("exists: %p\n", (void*)lookup_rhash(&rhc, 32, NULL));
 *     printf("exists: %p\n", (void*)lookup_rhash(&rhc, 263, NULL));
*/

    return 0;
}
