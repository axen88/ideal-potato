
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  


#include "../os_adapter.h"
#include "../hashtab.h"

typedef struct test_hashtab_node
{
    u64_t key;       // 
    
    u64_t value;       // 
    
    hashtab_node_t hnode; // 在hashtab中登记
    
} test_hashtab_node_t;


uint32_t test_hash_func(hashtab_t *h, void *key)
{
    u64_t id = (u64_t)key;

    return id % h->slots_num;
}

int hash_compare_key(hashtab_t *h, void *key1, void *value)
{
    u64_t id1 = (u64_t)key1;
    test_hashtab_node_t *v = (test_hashtab_node_t *)value;

    if (id1 > v->key)
        return 1;
    else if (id1 == v->key)
        return 0;
    else 
        return -1;
}

void hash_print(void *value)
{
    test_hashtab_node_t *v = (test_hashtab_node_t *)value;
    
    printf("(%llu,%llu)", v->key, v->value);
}

int hash_print2(void *value, void *arg)
{
    test_hashtab_node_t *v = (test_hashtab_node_t *)value;

    printf("(%llu,%llu)", v->key, v->value);

    return 0;
}

void test_hashtab_case0(void)
{
    hashtab_t *h;
    hashtab_info_t stat;
    test_hashtab_node_t node[100];
    test_hashtab_node_t *hnode;

    h = hashtab_create(test_hash_func, hash_compare_key, 4, offsetof(test_hashtab_node_t, hnode));
    CU_ASSERT(h != NULL);

    hnode = node;
    
    hnode->key = 10; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 11; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 12; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 14; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 15; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 16; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 18; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;


    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)10))->value == 10 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)11))->value == 11 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)12))->value == 12 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)14))->value == 14 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)15))->value == 15 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)16))->value == 16 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)18))->value == 18 + 100);
    CU_ASSERT(hashtab_search(h, (void *)9) == NULL);
    CU_ASSERT(hashtab_search(h, (void *)17) == NULL);
    CU_ASSERT(hashtab_search(h, (void *)13) == NULL);

    hashtab_print(h, hash_print);

    // 
    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");

    hashtab_stat(h, &stat);
    CU_ASSERT(stat.max_chain_len == 3);
    CU_ASSERT(stat.slots_used == 3);


    CU_ASSERT(((test_hashtab_node_t *)hashtab_delete(h, (void *)11))->value == 11 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_delete(h, (void *)15))->value == 15 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_delete(h, (void *)10))->value == 10 + 100);
    CU_ASSERT(hashtab_delete(h, (void *)11) == NULL);
    CU_ASSERT(hashtab_delete(h, (void *)15) == NULL);
    CU_ASSERT(hashtab_delete(h, (void *)10) == NULL);

    printf("\n");
    hashtab_map(h, hash_print2, NULL);
    printf("\n");
    
    hashtab_stat(h, &stat);
    CU_ASSERT(stat.max_chain_len == 2);
    CU_ASSERT(stat.slots_used == 2);

    hnode->key = 10; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 11; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    hnode->key = 15; hnode->value = hnode->key+100;
    hashtab_insert(h, (void *)hnode->key, hnode); hnode++;
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)11))->value == 11 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)15))->value == 15 + 100);
    CU_ASSERT(((test_hashtab_node_t *)hashtab_search(h, (void *)10))->value == 10 + 100);

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


