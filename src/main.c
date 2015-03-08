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

int
main(void)
{
    int i, j;

    int nvertices, nedges, erdos;
    int *graph[2]; /* se calhar este dois nao devia estar hardcoded */
    node **erdos_adj = NULL;

    int *count  = NULL;
    int *buf[2];

    scanf("%d", &nvertices);
    getchar();
    scanf("%d", &nedges);
    getchar();
    scanf("%d", &erdos);

    graph[0]  = malloc(nedges * sizeof (int));
    graph[1]  = malloc(nedges * sizeof (int));
    erdos_adj = calloc(nvertices, sizeof (node *));

    count  = malloc(nvertices * sizeof (int));
    buf[0] = malloc(nedges    * sizeof (int));
    buf[1] = malloc(nedges    * sizeof (int));


/* printf("vs = %d; es = %d; erdos = %d\n", nvertices, nedges, erdos); */
printf("%d %d\n%d\n", nvertices, nedges, erdos);

    for (i = 0; i < nedges; i++)
    {
        scanf("%d", &graph[0][i]);
        getchar();
        scanf("%d", &graph[1][i]);

        /* printf("%d %d\n", graph[0][i], graph[1][i]); */
        /* erdos_adj[v - 1] = insert_node(erdos_adj[v - 1], u); */
        /* erdos_adj[u - 1] = insert_node(erdos_adj[u - 1], v); */
    }

#define nextj (j + 1) % 2

    /* RadixLSD on graph. */
    for (j = 1; j >= 0; j--) {
        /* Counting sort on graph[j]. */

        for (i = 0; i < nvertices; i++)
        {
            count[i] = 0;
        }

        for (i = 0; i < nedges; i++)
        {
            count[ graph[j][i] - 1]++;
        }

        for (i = 1; i < nvertices; i++)
        {
            count[i] += count[i - 1];
        }

        for (i = nedges - 1; i >= 0; i--)
        {
            int u = graph[j]    [i];
            int v = graph[nextj][i];

            buf[j]    [ count[u - 1] - 1 ] = u;
            buf[nextj][ count[u - 1] - 1 ] = v;
            --count[u - 1];
        }

        for (i = nedges - 1; i >= 0; i--)
        {
            graph[j]    [i] = buf[j]    [i];
            graph[nextj][i] = buf[nextj][i];
        }
    }
    free(count);
    free(buf[0]);
    free(buf[1]);

    /* Create Erdos colaboration graph. */
    for (j = 0; j < 2; j++)
    {
        for (i = nedges - 1; i >= 0; i--)
        {
            int u = graph[j]    [i];
            int v = graph[nextj][i];

            erdos_adj[u - 1] = insert_node( erdos_adj[u - 1], v);
        }
    }

for (i = 0; i < nedges; i++) {
    printf("%d %d\n", graph[0][i], graph[1][i]);
}

puts("");
for (i = 0; i < nvertices; i++) {
    printf("%d: ", i + 1);
    print_list(erdos_adj[i]);
}


    /* fazer free as listas todas (e a tudo o resto) */
    free(erdos_adj);
    return 0;
}
