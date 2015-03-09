#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY  1
#define BLACK 2

typedef struct node {
    unsigned key;
    unsigned color;
    unsigned dist;
    struct node *pred;
    struct node *next;
} node;

node *
llist_insert(node *head, unsigned v)
{
    node *vnode = malloc(sizeof (node));
    vnode->key  = v;
    vnode->next = head;
    return vnode;
}

void
llist_fmap(node *head, void (*f)(node *))
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
llist_print(node *head)
{
    llist_fmap(head, &print_node_key);
}

void
llist_free(node *head)
{
    if (head)
    {
        node *next = head->next;
        free(head);
        llist_free(next);
    }
}

typedef struct qnode {
    node *node;
    struct qnode *next;
} qnode;

typedef struct {
    qnode *head;
} queue;

queue *
new_queue()
{
    queue *q = malloc(sizeof (queue));
    q->head  = NULL;
    return q;
}

node *
dequeue(queue *q)
{
    node *headn = q->head->node;
    qnode *tmp  = q->head;
    q->head     = q->head->next;
    free(tmp);
    return headn;
}

void
enqueue(queue *q, node *n)
{
    qnode *qn = malloc(sizeof (qnode));
    qn->node  = n;
    qn->next  = q->head;
    q->head   = qn;
}

int
free_queue(queue *q)
{
    if (q->head)
    {
        return EXIT_FAILURE;
    }
    else
    {
        free(q);
        return EXIT_SUCCESS;
    }
}

void
bfs(node **adjs, size_t nnodes, node *src)
{
    size_t n;
    node **q = NULL;

    for (n = 0; n < nnodes; nnodes++)
    {
        if (adjs[n] != src)
        {
            adjs[n]->color = WHITE;
            adjs[n]->dist  = UINT_MAX;
            adjs[n]->pred  = NULL;
        }
    }

    src->color = GRAY;
    src->dist  = 0;
    src->pred  = NULL;

    /* ... */

}

int
main(void)
{
    int i, j;

    int nnodes, nedges, erdos;
    int *graph[2];
/* se calhar este dois nao devia estar hardcoded */
    node **erdos_adjs = NULL;

    int *count = NULL;
    int *buf[2];

    scanf("%d", &nnodes);
    getchar();
    scanf("%d", &nedges);
    getchar();
    scanf("%d", &erdos);

    graph[0]   = malloc(2 * nedges * sizeof (int));
    graph[1]   = malloc(2 * nedges * sizeof (int));
    erdos_adjs = calloc(nnodes, sizeof (node *));

    count  = malloc(nnodes  * sizeof (int));
    buf[0] = malloc(2 * nedges * sizeof (int));
    buf[1] = malloc(2 * nedges * sizeof (int));

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
    }

#define nextj (j + 1) % 2

    /* RadixLSD on graph. */
    for (j = 1; j >= 0; j--) {
        /* Counting sort on graph[j]. */

        for (i = 0; i < nnodes; i++)
        {
            count[i] = 0;
        }

        for (i = 0; i < 2 * nedges; i++)
        {
            count[ graph[j][i] - 1]++;
        }

        for (i = 1; i < nnodes; i++)
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

        erdos_adjs[u - 1] = llist_insert(erdos_adjs[u - 1], v);
    }
    free(graph[0]);
    free(graph[1]);

/*
for (i = 0; i < 2 * nedges; i++) {
    printf("%d %d\n", graph[0][i], graph[1][i]);
}
puts("");
*/
for (i = 0; i < nnodes; i++) {
    printf("%d: ", i + 1);
    llist_print(erdos_adjs[i]);
}


    /* fazer free as listas todas (e a tudo o resto) */

    for (i = 0; i < nnodes; i++)
    {
        llist_free(erdos_adjs[i]);
    }
    free(erdos_adjs);

    return 0;
}
