
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  


#include "../os_adapter.h"
#include "../hashtab.h"

uint32_t hash_cache_value(hashtab_t *h, void *key)
{
    u64_t id = (u64_t)key;

    return id % h->slot_num;
}

int hash_compare_key(hashtab_t *h, void *key1, void *key2)
{
    u64_t id1 = (u64_t)key1;
    u64_t id2 = (u64_t)key2;

    if (id1 > id2)
        return 1;
    else if (id1 == id2)
        return 0;
    else 
        return -1;
}

void hash_print(void *key, void *dat)
{
    printf("(%llu,%llu)", (u64_t)key, (u64_t)dat);
}

int hash_print2(void *key, void *dat, void *arg)
{
    printf("(%llu,%llu)", (u64_t)key, (u64_t)dat);

    return 0;
}

void test_hashtab_case0(void)
{
    hashtab_t *h;
    #define VALUE(key)  (key+12)
    hashtab_info_t stat;

    h = hashtab_create(hash_cache_value, hash_compare_key, 4, 100);
    CU_ASSERT(h != NULL);

    hashtab_insert(h, (void *)0, (void *)VALUE(0));
    hashtab_insert(h, (void *)10, (void *)VALUE(10));
    hashtab_insert(h, (void *)12, (void *)VALUE(12));
    hashtab_insert(h, (void *)20, (void *)VALUE(20));
    hashtab_insert(h, (void *)13, (void *)VALUE(13));

    CU_ASSERT(hashtab_search(h, (void *)0) == (void *)VALUE(0));
    CU_ASSERT(hashtab_search(h, (void *)10) == (void *)VALUE(10));
    CU_ASSERT(hashtab_search(h, (void *)12) == (void *)VALUE(12));
    CU_ASSERT(hashtab_search(h, (void *)20) == (void *)VALUE(20));
    CU_ASSERT(hashtab_search(h, (void *)13) == (void *)VALUE(13));
    CU_ASSERT(hashtab_search(h, (void *)9) == NULL);

    hashtab_print(h, hash_print);

    // 
    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");

    hashtab_stat(h, &stat);
    CU_ASSERT(stat.max_chain_len == 3);
    CU_ASSERT(stat.slots_used == 3);


    CU_ASSERT(hashtab_delete(h, (void *)13) == (void *)VALUE(13));
    CU_ASSERT(hashtab_delete(h, (void *)20) == (void *)VALUE(20));
    CU_ASSERT(hashtab_delete(h, (void *)20) == NULL);
    CU_ASSERT(hashtab_delete(h, (void *)13) == NULL);

    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");
    
    hashtab_stat(h, &stat);
    CU_ASSERT(stat.max_chain_len == 2);
    CU_ASSERT(stat.slots_used == 2);

    hashtab_insert(h, (void *)13, (void *)VALUE(13));
    hashtab_insert(h, (void *)20, (void *)VALUE(20));
    CU_ASSERT(hashtab_search(h, (void *)13) == (void *)VALUE(13));
    CU_ASSERT(hashtab_search(h, (void *)20) == (void *)VALUE(20));

    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");

    hashtab_stat(h, &stat);
    CU_ASSERT(stat.max_chain_len == 3);
    CU_ASSERT(stat.slots_used == 3);

    hashtab_destroy(h);

}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_hashtab_cases[]
= {  
    {to_str(test_hashtab_case0), test_hashtab_case0},  
    CU_TEST_INFO_NULL  
};  


