#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* ========================================================================== */
/* Structs                                                                    */
/* ========================================================================== */

/*
 * Single linked list construction for adjancency-list graph representation.
 *
 * A sllist has the properties color, distance (dist) and predecessor (pred),
 * used in the breadth-first search algorithm. It also has a key and link for
 * the head node.
 */

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
} sllist;


/*
 * Single linked list methods.
 */

sllist *
new_sllist(unsigned key)
{
    sllist *ll = malloc(sizeof (sllist));
    ll->key  = key;
    ll->head = NULL;
    return ll;
}

void
sllist_insert(sllist *ll, unsigned key)
{
    node *vnode = malloc(sizeof (node));
    vnode->key  = key;
    vnode->next = ll->head;
    ll-> head   = vnode;
}

void
sllist_free(sllist *ll)
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


/*
 * Queue construction.
 */

typedef struct qnode{
    sllist        *ll;
    struct qnode *next;
} qnode;

typedef struct {
    qnode *head;
    qnode *tail;
} queue;


/*
 * Queue methods.
 */

queue *
new_queue()
{
    queue *q = malloc(sizeof (queue));
    q->head  = q->tail = NULL;
    return q;
}

sllist *
dequeue(queue *q)
{
    sllist *ll  = q->head->ll;
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
enqueue(queue *q, sllist *ll)
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


/* ========================================================================== */
/* Functions                                                                  */
/* ========================================================================== */

#define emptyq(Q) !(Q->head)

#define WHITE 0
#define GRAY  1
#define BLACK 2
#define NIL   0

void
bfs(sllist **adjls, size_t nvertices, int src_key)
{
    size_t n;
    sllist *src = adjls[src_key - 1];
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
        sllist *u;
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
    char c;

    int nvertices, nedges, erdos;

     /*
      * The edges will be stored here and then inserted in the adj-list. This
      * way we will be able to sort the edges easily.
      */
    int *graph[2];

    sllist **erdos_adjls; /* Adjacency-lists. */
    int *erdos_ncount;

    int *count;
    int *buf[2];

    /*
     * Read input.
     */
    if ( scanf("%d%c%d%c%d", &nvertices, &c, &nedges, &c, &erdos) != 5 )
    {
        return -1;
    }

    graph[0] = malloc(2 * nedges * sizeof (int));
    graph[1] = malloc(2 * nedges * sizeof (int));

    for (i = 0; i < 2 * nedges - 1; i += 2)
    {
        int u, v;

        if ( scanf("%d%c%d", &u, &c, &v) != 3 )
        {
            return -1;
        }

        graph[0][i] = u;
        graph[1][i] = v;

        /* The graph will be undirected so we also insert the edge "reverted".*/
        graph[0][i + 1] = v;
        graph[1][i + 1] = u;
    }

    count  = malloc(nvertices  * sizeof (int));
    buf[0] = malloc(2 * nedges * sizeof (int));
    buf[1] = malloc(2 * nedges * sizeof (int));

#define nextj (j + 1) % 2

    /*
     * RadixLSD on graph.
     */
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
     * Every adj-list is sorted by the keys of its nodes.
     */
    erdos_adjls = malloc(nvertices * sizeof (sllist *));

    /* Initialization. */
    for (i = 0; i < nvertices; i++)
    {
        erdos_adjls[i] = new_sllist(i + 1);
    }

    /* Insertion. */
    for (i = 2 * nedges - 1, j = nedges; i >= 0; i--)
    {
        int u = graph[0][i];
        int v = graph[1][i];

        sllist_insert(erdos_adjls[u - 1], v);
    }

    free(graph[0]);
    free(graph[1]);

    /*
     * We do a breadth-first search to determine the Erdos number of each
     * scientist (distance to the source -- Paul Erdos).
     */
    bfs(erdos_adjls, nvertices, erdos);

    /* Determine the maximum Erdos number. */
    for (i = 0, max_erdos_n = 0; i < nvertices; i++)
    {
        if (erdos_adjls[i]->dist > max_erdos_n)
        {
            max_erdos_n = erdos_adjls[i]->dist;
        }
    }

    printf("%d\n", max_erdos_n);

    /*
     * The ith entry of erdos_ncount will be equal to number of scientists with
     * Erdos number equal to i.
     */
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

    for (i = 0; i < nvertices; i++)
    {
        sllist_free(erdos_adjls[i]);
    }
    free(erdos_adjls);

    return 0;
}
