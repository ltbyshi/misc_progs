#include <stdio.h>

int main()
{
    int a = 1, b = 2, c = 2;

    b = __atomic_load_n(&a, __ATOMIC_SEQ_CST);
    __atomic_load(&a, &b, __ATOMIC_SEQ_CST);
    __atomic_store_n(&b, a, __ATOMIC_SEQ_CST);
    __atomic_store(&b, &a, __ATOMIC_SEQ_CST);
    c = __atomic_exchange_n(&b, &a, __ATOMIC_SEQ_CST);
    __atomic_exchange(&b, &a, &c, __ATOMIC_SEQ_CST);
    __atomic_compare_exchange_n(&b, &a, c, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    __atomic_compare_exchange(&b, &a, &c, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
    c = __atomic_add_fetch(&a, 1, __ATOMIC_SEQ_CST);
    c = __atomic_sub_fetch(&a, 1, __ATOMIC_SEQ_CST);
    c = __atomic_and_fetch(&a, 1, __ATOMIC_SEQ_CST);
    c = __atomic_xor_fetch(&a, 1, __ATOMIC_SEQ_CST);
    c = __atomic_or_fetch(&a, 1, __ATOMIC_SEQ_CST);
    c = __atomic_nand_fetch(&a, 1, __ATOMIC_SEQ_CST);

    return 0;
}
