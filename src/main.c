#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    unsigned     key;
    struct node *next;
} node;

typedef struct {
    unsigned color;
    unsigned dist;
    unsigned key;
    unsigned pred;
    node    *head;
} llist;

llist *
new_llist(unsigned key)
{
    llist *ll = malloc(sizeof (llist));
    ll->key  = key;
    ll->head = NULL;
    return ll;
}

void
llist_insert(llist *ll, unsigned key)
{
    node *vnode = malloc(sizeof (node));
    vnode->key  = key;
    vnode->next = ll->head;
    ll-> head   = vnode;
}

void
llist_fmap(llist *ll, void (*f)(node *))
{
    node *head = ll->head;
    while (head)
    {
        f(head);
        head = head->next;
    }
}

void
print_node(node *n)
{
    if (n->next)
    {
        printf("%u -> ", n->key);
    }
    else
    {
        printf("%u\n", n->key);
    }
}

void
llist_print(llist *ll)
{
    llist_fmap(ll, &print_node);
}

void
llist_free(llist *ll)
{
    node *n = ll->head;
    while (n)
    {
        node *tmp = n;
        n         = n->next;
        free(tmp);
    }
    free(ll);
}

typedef struct qnode{
    llist        *ll;
    struct qnode *next;
} qnode;

typedef struct {
    qnode *head;
    qnode *tail;
} queue;

queue *
new_queue()
{
    queue *q = malloc(sizeof (queue));
    q->head  = q->tail = NULL;
    return q;
}

llist *
dequeue(queue *q)
{
    llist *ll  = q->head->ll;
    qnode *tmp = q->head;
    q->head    = q->head->next;
    free(tmp);

    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    return ll;
}

void
enqueue(queue *q, llist *ll)
{
    qnode *qn = malloc(sizeof (qnode));
    qn->ll    = ll;
    qn->next  = NULL;

    if (q->head)
    {
        q->tail->next = qn;
        q->tail       = q->tail->next;
    }
    else
    {
        q->head = q->tail = qn;
    }
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
print_queue(queue *q)
{
    qnode *head = q->head;
    printf("[");
    while (head)
    {
        printf("%d,", head->ll->key);
        head = head->next;
    }
    printf("]\n");
}

#define emptyq(Q) !(Q->head)

#define WHITE 0
#define GRAY  1
#define BLACK 2
#define NIL   0

void
bfs(llist **adjls, size_t nvertices, int src_key)
{
    size_t n;
    llist *src = adjls[src_key - 1];
    queue *q   = NULL;

    for (n = 0; n < nvertices; n++)
    {
        adjls[n]->color = WHITE;
        adjls[n]->dist  = UINT_MAX;
        adjls[n]->pred  = NIL;
    }
    src->color = GRAY;
    src->dist  = 0;
    src->pred  = NIL;

    q = new_queue();
    enqueue(q, src);

    while (!emptyq(q))
    {
        llist *u;
        node  *v;

#define adjl(A) adjls[A->key - 1]

        u = dequeue(q);
        for (v = u->head; v != NULL; v = v->next)
        {
            if (adjl(v)->color == WHITE)
            {
                adjl(v)->color = GRAY;
                adjl(v)->dist  = adjl(u)->dist + 1;
                adjl(v)->pred  = u->key;
                enqueue(q, adjl(v));
            }
        }
        adjl(u)->color = BLACK;
    }
    free_queue(q);
}

int
main(void)
{
    int i, j, max_erdos_n;

    int nvertices, nedges, erdos;
    int *graph[2];
/* se calhar este dois nao devia estar hardcoded */

    llist **erdos_adjls  = NULL;
    int    *erdos_ncount = NULL;

    int *count = NULL;
    int *buf[2];

    scanf("%d", &nvertices);
    getchar();
    scanf("%d", &nedges);
    getchar();
    scanf("%d", &erdos);

    graph[0] = malloc(2 * nedges * sizeof (int));
    graph[1] = malloc(2 * nedges * sizeof (int));

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

    count  = malloc(nvertices  * sizeof (int));
    buf[0] = malloc(2 * nedges * sizeof (int));
    buf[1] = malloc(2 * nedges * sizeof (int));

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
     * Every list will become sorted by the keys of its nodes.
     */
    erdos_adjls = malloc(nvertices * sizeof (llist *));
    for (i = 0; i < nvertices; i++)
    {
        erdos_adjls[i] = new_llist(i + 1);
    }
    for (i = 2 * nedges - 1, j = nedges; i >= 0; i--)
    {
        int u = graph[0][i];
        int v = graph[1][i];

        llist_insert(erdos_adjls[u - 1], v);
    }
    free(graph[0]);
    free(graph[1]);

    bfs(erdos_adjls, nvertices, erdos);

    for (i = 0, max_erdos_n = 0; i < nvertices; i++)
    {
        if (erdos_adjls[i]->dist > max_erdos_n)
        {
            max_erdos_n = erdos_adjls[i]->dist;
        }
    }
    printf("%d\n", max_erdos_n);

    erdos_ncount = calloc(max_erdos_n + 1, sizeof (int));
    for (i = 0; i < nvertices; i++)
    {
        erdos_ncount[ erdos_adjls[i]->dist ]++;
    }

    for (i = 1; i < max_erdos_n + 1; i++)
    {
        printf("%d\n", erdos_ncount[i]);
    }
    free(erdos_ncount);

/*
for (i = 0; i < nvertices; i++) {
    printf("%d: ", i + 1);
    llist_print(erdos_adjls[i]);
}


for (i = 0; i < nvertices; i++) {
    printf("%d: dist = %u, pred = %u\n",
            i + 1,
            erdos_adjls[i]->dist,
            erdos_adjls[i]->pred);
}
*/

    for (i = 0; i < nvertices; i++)
    {
        llist_free(erdos_adjls[i]);
    }
    free(erdos_adjls);

    return 0;
}
