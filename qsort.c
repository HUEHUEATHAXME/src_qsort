#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

#define DEBUG_PARTITION         0
#define DEBUG_QSORT             0

typedef struct qsort qsort_t;
struct qsort {
    void *tmp;
    int (*cmpfunc)(void *data, int a, int b);
    int (*cpyfunc)(void *from, int a, void *to, int b);
    int datasize;
};


static int partition(qsort_t *q, void *d, int left, int right) 
{
    int i, j, pidx;

    if (DEBUG_PARTITION)
        printf("PARTITION: left=%d, right=%d\n", left, right);
    
    /* NOTE: Using the rightmost element as a partitioning element */
    pidx = right;
    i = left-1;
    j = right;
    while (1) {
        do { i++; } while (q->cmpfunc(d, i, pidx) > 0) ;
        do { j--; } while (q->cmpfunc(d, pidx, j) > 0 &&
                           j > left);
        if (i >= j)
            break;
        q->cpyfunc(d, i, q->tmp, 0);
        q->cpyfunc(d, j, d, i);
        q->cpyfunc(q->tmp, 0, d, j);
    }

    q->cpyfunc(d, i, q->tmp, 0);
    q->cpyfunc(d, pidx, d, i);
    q->cpyfunc(q->tmp, 0, d, pidx);
    return i;
}


static void _qsort(qsort_t *q, void *d, int left, int right)
{
    int i;

    if (right <= left)
        return;

    if (DEBUG_QSORT)
        printf("RECURSE: left=%d, right=%d\n", left, right);

    i = partition(q, d, left, right);
    _qsort(q, d, left, i-1);
    _qsort(q, d, i+1, right);
}


int sort(void *data, 
           int left, int right,
           cmpfunc_t cmpfunc,
           cpyfunc_t cpyfunc,
           int datasize)
{
    qsort_t *q;

    q = (qsort_t*)malloc(sizeof(qsort_t));
    if (q == NULL)
        goto error;
    q->cmpfunc = cmpfunc;
    q->cpyfunc = cpyfunc;
    q->datasize = datasize;
    q->tmp = malloc(datasize);
    if (q->tmp == NULL)
        goto error;

    _qsort(q, data, left, right);

    free(q->tmp);
    free(q);

    return 0;
 error:
    if (q != NULL) {
        if (q->tmp != NULL)
            free(q->tmp);
        free(q);
    }
    return -1;
}
