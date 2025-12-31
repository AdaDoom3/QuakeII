#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char U;
typedef unsigned int I;
typedef float F;
typedef int J;

typedef struct{I o,l;}L;
typedef struct{F v[2][4];I f,val;char n[32];J nx;}TI;

int main() {
    FILE *f = fopen("baseq2/maps/demo1.bsp", "rb");
    fseek(f, 0, SEEK_END);
    I sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    U *d = malloc(sz);
    fread(d, 1, sz, f);
    fclose(f);

    if (memcmp(d, "IBSP", 4) || ((I*)d)[1] != 38) {
        printf("Not IBSP v38\n");
        return 1;
    }

    L *lp = (L*)(d+8);
    printf("Texinfo lump: offset=%d, length=%d\n", lp[5].o, lp[5].l);
    printf("sizeof(TI)=%lu\n", sizeof(TI));
    printf("Texinfo entries: %d\n", lp[5].l / sizeof(TI));

    TI *ti = (TI*)(d + lp[5].o);
    for (int i = 0; i < 10 && i < lp[5].l / sizeof(TI); i++) {
        printf("  [%d] name='%s', flags=%d, value=%d\n", i, ti[i].n, ti[i].f, ti[i].val);
    }

    free(d);
    return 0;
}
