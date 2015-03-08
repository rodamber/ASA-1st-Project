#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int vertex;
    struct node *next;
} node;

node *
insert_node(node *head, int v)
{
    node *vnode   = malloc(sizeof (node));
    vnode->vertex = v;
    vnode->next   = head;
    return vnode;
}

void
node_fmap(node *head, void (*f)(node *))
{
    while (head) {
        f(head);
        head = head->next;
    }
}

void
print_int(node *n)
{
    if (n->next) {
        printf("%d -> ", n->vertex);
    } else {
        printf("%d\n", n->vertex);
    }
}

void
print_list(node *head)
{
    node_fmap(head, &print_int);
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

    int vertices;
    int edges;
    int erdos;

    node **erdos_adj = NULL;

    scanf("%d", &vertices);
    getchar();
    scanf("%d", &edges);
    getchar();
    scanf("%d", &erdos);

    erdos_adj = calloc(vertices, sizeof (node *));

printf("vs = %d; es = %d; erdos = %d\n", vertices, edges, erdos);

    for (i = 0; i < edges; i++) {
        int u, v;

        scanf("%d", &u);
        getchar();
        scanf("%d", &v);

        erdos_adj[v - 1] = insert_node(erdos_adj[v - 1], u);
        erdos_adj[u - 1] = insert_node(erdos_adj[u - 1], v);
    }

for (i = 0; i < vertices; i++) {
    printf("%d: ", i + 1);
    print_list(erdos_adj[i]);
}

    /* fazer free as listas todas (e a tudo o resto) */
    free(erdos_adj);
    return 0;
}
