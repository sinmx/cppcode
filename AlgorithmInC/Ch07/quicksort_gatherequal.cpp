#include "Item.h"
#define eq(A, B) (!less(A, B) && !less(B, A))
void quicksort(Item a[], int l, int r)
{
    int i, j, k, p, q;
    Item v;

    if (r <= l) return;

    v = a[r];
    i = l - 1;
    j = r;
    p = l;
    q = r-1;

    for (;;)
    {
        while (less(a[++i], v)) ;

        while (less(v, a[--j])) if (j == l) break;

        if (i >= j) break;

        exch(a[i], a[j]);

        if (eq(a[i], v))
        {
            exch(a[p], a[i]);
            p++;
        }

        if (eq(v, a[j]))
        {
            exch(a[q], a[j]);
            q--;
        }
    }

    exch(a[i], a[r]);
    j = i - 1;
    i = i + 1;

    for (k = l  ; k < p; k++, j--) exch(a[k], a[j]);

    for (k = r - 1; k > q; k--, i++) exch(a[k], a[i]);

    quicksort(a, l, j);
    quicksort(a, i, r);
}