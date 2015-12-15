#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#define OFFSETOF(type, i) printf(#type".m%d: offsetof=%lu\n", i, offsetof(type, m##i));
#define SIZEOF(obj) printf("sizeof("#obj") = %lu\n", sizeof(obj));

struct Garbage {
    char m1;
    long m2;
    char* m3;
    int m4;
    double m5;
    float m6;
    short m7;
    void* m8;
};

void PrintOffset()
{
    OFFSETOF(struct Garbage, 1);
    OFFSETOF(struct Garbage, 2);
    OFFSETOF(struct Garbage, 3);
    OFFSETOF(struct Garbage, 4);
    OFFSETOF(struct Garbage, 5);
    OFFSETOF(struct Garbage, 6);
    OFFSETOF(struct Garbage, 7);
    OFFSETOF(struct Garbage, 8);
}

void PrintSize()
{
    struct Garbage g;
    SIZEOF(g.m1);
    SIZEOF(g.m2);
    SIZEOF(g.m3);
    SIZEOF(g.m4);
    SIZEOF(g.m5);
    SIZEOF(g.m6);
    SIZEOF(g.m7);
    SIZEOF(g.m8);
}

void PrintData()
{
    struct Garbage g;
    g.m1 = 1;
    g.m2 = 1;
    g.m3 = 0;
    g.m4 = 1;
    g.m5 = 1;
    g.m6 = 1;
    g.m7 = 0;
    g.m8 = 0;

    fwrite(&g, sizeof(g), 1, stdout);
}

int main()
{
    PrintOffset();
    PrintSize();
    return 0;
}
