#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "array.h"

// some associative functions on ints
static int
sum(int x, int y)
{
    return x+y;
}

static int
sum1(int x, int y)
{
    return x+y+1;
}

// unpack a function from n elements to n elements encoded as a non-negative int
static void
unpackFunction(int n, int x, int a[])
{
    assert(x >= 0);
    assert(n > 0);

    for(int i = 0; i < n; i++) {
        a[i] = x % n;
        x /= n;
    }
}

// inverse of unpackFunction
static int
packFunction(int n, int a[])
{
    assert(n > 0);

    int x = 0;

    for(int i = n-1; i >= 0; i--) {
        x = x * n + a[i];
    }

    return x;
}

#define DOMAIN_SIZE (5)

// compose two functions on DOMAIN_SIZE elements
static int
composeFunctions(int x, int y)
{
    int xx[DOMAIN_SIZE];
    int yy[DOMAIN_SIZE];
    int zz[DOMAIN_SIZE];

    unpackFunction(DOMAIN_SIZE, x, xx);
    unpackFunction(DOMAIN_SIZE, y, yy);

    for(int i = 0; i < DOMAIN_SIZE; i++) {
        zz[i] = yy[xx[i]];
    }

    return packFunction(DOMAIN_SIZE, zz);
}

static void
printArray(const Array *a)
{
    size_t n = arraySize(a);
    for(size_t i = 0; i < n; i++) {
        printf("%d ", arrayGet(a, i));
    }
    putchar('\n');
}

static void
printAggregates(const Array *a)
{
    size_t n = arraySize(a);
    for(size_t i = 0; i <= n; i++) {
        printf("%d ", arrayCombine(a, i));
    }
    putchar('\n');
}

static void
eatComment(void)
{
    int c;

    while((c = getchar()) != EOF && c != '\n');
}

#define ArgError() (fprintf(stderr, "%s: %c: missing argument(s)\n", argv[0], command), arrayDestroy(a), exit(1))

#define DEFAULT_N (1)

#define RANDOM_INCREMENTS_PER_LOCATION (8)

int
main(int argc, char **argv)
{
    char command;
    int arg1;
    int arg2;

    // start with a default array
    Array *a = arrayCreate(sum, DEFAULT_N);

    // make rand output consistent between runs
    srand(0);

    while((command = getchar()) != EOF) {
        switch(command) {
            case '+':
                if(scanf("%d", &arg1) != 1) { ArgError(); }
                arrayDestroy(a);
                a = arrayCreate(sum, arg1);
                break;
            case '1':
                if(scanf("%d", &arg1) != 1) { ArgError(); }
                arrayDestroy(a);
                a = arrayCreate(sum1, arg1);
                break;
            case '*':
                if(scanf("%d", &arg1) != 1) { ArgError(); }
                arrayDestroy(a);
                a = arrayCreate(composeFunctions, arg1);
                break;
            case 'z':
                printf("%zu\n", arraySize(a));
                break;
            case 's':
                if(scanf("%d %d", &arg1, &arg2) != 2) { ArgError(); }
                arraySet(a, arg1, arg2);
                break;
            case 'g':
                if(scanf("%d", &arg1) != 1) { ArgError(); }
                printf("%d\n", arrayGet(a, arg1));
                break;
            case 'c':
                if(scanf("%d", &arg1) != 1) { ArgError(); }
                printf("%d\n", arrayCombine(a, arg1));
                break;
            case 'i':
                // fill array with sequential values
                {
                    size_t n = arraySize(a);
                    for(size_t i = 0; i < n; i++) {
                        arraySet(a, i, i);
                    }
                }
                break;
            case 'p':
                printArray(a);
                break;
            case 'P':
                printAggregates(a);
                break;
            case 't':
                // stress test
                if(scanf("%d", &arg1) != 1) { ArgError(); }
                arrayDestroy(a);
                a = arrayCreate(sum, arg1);
                
                for(int i = 0; i < arg1; i++) {
                    arraySet(a, i, 1);
                    int total = arrayCombine(a, 0);
                    assert(total == i+1);
                    int half = arrayCombine(a, arg1 / 2);
                    assert(half == (i+1 < arg1 / 2 ? i+1 : arg1 / 2));
                }

                break;
            case 'r':
                {
                    // random increments stress test
                    if(scanf("%d", &arg1) != 1) { ArgError(); }
                    arrayDestroy(a);
                    a = arrayCreate(sum, arg1);

                    int total = 0;
                    size_t n = arraySize(a);

                    for(int i = 0; i < RANDOM_INCREMENTS_PER_LOCATION * n; i++) {
                        int location = rand() % n;
                        int oldValue = arrayGet(a, location);
                        int newValue = rand();
                        int delta = newValue - oldValue;

                        int oldBefore = arrayCombine(a, location);
                        int oldIncluding = arrayCombine(a, location+1);

                        arraySet(a, location, newValue);

                        int newBefore = arrayCombine(a, location);
                        int newIncluding = arrayCombine(a, location+1);

                        int newTotal = arrayCombine(a, 0);

                        if(location > 0) {
                            assert(newBefore == oldBefore);
                        }
                        assert(newIncluding == oldIncluding + delta);
                        assert(newTotal == total + delta);

                        total = newTotal;
                    }
                }
                break;
            case '\n':
            case ' ':
            case '\t':
                // ignore it
                break;
            case '#':
                // comment
                eatComment();
                break;
            default:
                fprintf(stderr, "%s: bad command %c\n", argv[0], command);
                break;
        }
    }

    arrayDestroy(a);

    return 0;
}