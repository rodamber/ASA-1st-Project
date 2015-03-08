#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    struct node *next;
} node;

node *
insert_node(node *head, int v)
{
    node *vnode   = malloc(sizeof (node));
    vnode->key = v;
    vnode->next   = head;
    return vnode;
}

void
node_fmap(node *head, void (*f)(node *))
{
    while (head)
    {
        f(head);
        head = head->next;
    }
}

void
print_node_key(node *n)
{
    if (n->next)
    {
        printf("%d -> ", n->key);
    }
    else
    {
        printf("%d\n", n->key);
    }
}

void
print_list(node *head)
{
    node_fmap(head, &print_node_key);
}

void
free_list(node *head)
{
    if (head)
    {
        node *next = head->next;
        free(head);
        free_list(next);
    }
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

    graph[0]  = malloc(2 * nedges * sizeof (int));
    graph[1]  = malloc(2 * nedges * sizeof (int));
    erdos_adj = malloc(nvertices  * sizeof (node *));

    count  = malloc(nvertices  * sizeof (int));
    buf[0] = malloc(2 * nedges * sizeof (int));
    buf[1] = malloc(2 * nedges * sizeof (int));

/*printf("%d %d\n%d\n", nvertices, nedges, erdos);*/

    for (i = 0, j = 1; i < 2 * nedges - 1; i += 2, j += 2)
    {
        int u, v;

        scanf("%d", &u);
        getchar();
        scanf("%d", &v);

        graph[0][i] = u;
        graph[1][i] = v;

        graph[0][j] = v;
        graph[1][j] = u;

/*printf("%d %d\n", graph[0][i], graph[1][i]);*/
/*printf("%d %d\n", graph[0][j], graph[1][j]);*/
    }

#define nextj (j + 1) % 2

    /* RadixLSD on graph. */
    for (j = 1; j >= 0; j--) {
        /* Counting sort on graph[j]. */

        for (i = 0; i < nvertices; i++)
        {
            count[i] = 0;
        }

        for (i = 0; i < 2 * nedges; i++)
        {
            count[ graph[j][i] - 1]++;
        }

        for (i = 1; i < nvertices; i++)
        {
            count[i] += count[i - 1];
        }

        for (i = 2 * nedges - 1; i >= 0; i--)
        {
            int u = graph[j]    [i];
            int v = graph[nextj][i];

            buf[j]    [ count[u - 1] - 1 ] = u;
            buf[nextj][ count[u - 1] - 1 ] = v;
                      --count[u - 1];
        }

        for (i = 2 * nedges - 1; i >= 0; i--)
        {
            graph[j]    [i] = buf[j]    [i];
            graph[nextj][i] = buf[nextj][i];
        }
    }
    free(count);
    free(buf[0]);
    free(buf[1]);

    /*
     * Create Erdos colaboration graph.
     * Every linked list will become sorted by node->key.
     */
    for (i = 2 * nedges - 1; i >= 0; i--)
    {
        int u = graph[0][i];
        int v = graph[1][i];

        erdos_adj[u - 1] = insert_node( erdos_adj[u - 1], v);
    }
    free(graph[0]);
    free(graph[1]);

/*
for (i = 0; i < 2 * nedges; i++) {
    printf("%d %d\n", graph[0][i], graph[1][i]);
}
puts("");
*/

for (i = 0; i < nvertices; i++) {
    printf("%d: ", i + 1);
    print_list(erdos_adj[i]);
}


    /* fazer free as listas todas (e a tudo o resto) */

    for (i = 0; i < nvertices; i++)
    {
        free_list(erdos_adj[i]);
    }
    free(erdos_adj);

    return 0;
}
