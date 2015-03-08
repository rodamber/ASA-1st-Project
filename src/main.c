#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int vertex;
    struct node *next;
} node;

node *
insert_node(node *head, int v)
{
    node *j       = head;
    node *vnode   = malloc(sizeof (node));
    vnode->vertex = v;

    if (head == NULL || v <= head->vertex) {
        vnode->next = head;
        return vnode;
    }

    while (j->next && v > j->next->vertex) {
        j = j->next;
    }

    vnode->next = j->next;
    j->next     = vnode;

    return head;
}

void
node_map(node *head, void (*f)(node *))
{
    while (head) {
        f(head);
        head = head->next;
    }
}

void
print_int(node *n)
{
    printf("%d -> ", n->vertex);
}

void
print_list(node *head)
{
    node_map(head, &print_int);
}

void
csort(int *a, const int min, const int max, const int l, const int r)
{
    int i;

    const int csize = max - min + 1;
    const int asize = r - l + 1;

    int *count = calloc(csize, sizeof (int));
    int *buf   = malloc(asize * sizeof (int));

    for (i = l; i <= r; i++) {
        count[a[i] - min]++;
    }

    for (i = 1; i < csize; i++) {
        count[i] += count[i - 1];
    }

    for (i = r; i >= l; i--) {
        buf[count[a[i] - min] - 1] = a[i];
        --count[a[i] - min];
    }
    free(count);

    for (i = r; i >= l; i--) {
        a[i] = buf[i - l];
    }
    free(buf);
}

int
main(void)
{
    int i;

    node **erdos_adj = calloc(1, sizeof (node *));

    for (i = 0; i < 1; i++) {
        erdos_adj[0] = insert_node(erdos_adj[0], i);
    }
    print_list(erdos_adj[0]);

    /* fazer free as listas todas (e a tudo o resto) */
    free(erdos_adj);
    return 0;
}
