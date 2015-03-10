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

#define ADJL(A) adjls[A->key - 1]
#define EMPTYQ(Q) !(Q->head)

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

    while (!EMPTYQ(q))
    {
        sllist *u;
        node  *v;

        u = dequeue(q);
        for (v = u->head; v != NULL; v = v->next)
        {
            if (ADJL(v)->color == WHITE)
            {
                ADJL(v)->color = GRAY;
                ADJL(v)->dist  = ADJL(u)->dist + 1;
                ADJL(v)->pred  = u->key;
                enqueue(q, ADJL(v));
            }
        }
        ADJL(u)->color = BLACK;
    }
    free_queue(q);
}

int
main(void)
{
    int i, max_erdos_n;

    int nvertices, nedges, erdos;

    sllist **erdos_adjls; /* Adjacency-lists. */
    int *erdos_ncount;


    if ( scanf("%d %d\n%d", &nvertices, &nedges, &erdos) != 3 )
    {
        return -1;
    }

    /*
     * Create Erdos colaboration graph.
     */
    erdos_adjls = malloc(nvertices * sizeof (sllist *));

    /* Initialization. */
    for (i = 0; i < nvertices; i++)
    {
        erdos_adjls[i] = new_sllist(i + 1);
    }

    /* Insertion. */
    for (i = 0; i < nedges; i++)
    {
        int u, v;

        if ( scanf("%d %d", &u, &v) != 2 )
        {
            return -1;
        }

        sllist_insert(erdos_adjls[u - 1], v);

        /*
         * Because the graph will be undirected, we also insert the edge
         * "reversed".
         */
        sllist_insert(erdos_adjls[v - 1], u);
    }

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

    /*
     * The ith entry of erdos_ncount will be equal to the number of scientists
     * with Erdos number equal to i.
     */
    erdos_ncount = calloc(max_erdos_n + 1, sizeof (int));
    for (i = 0; i < nvertices; i++)
    {
        erdos_ncount[ erdos_adjls[i]->dist ]++;
    }

    /* Print output. */
    printf("%d\n", max_erdos_n);
    for (i = 1; i < max_erdos_n + 1; i++)
    {
        printf("%d\n", erdos_ncount[i]);
    }

    /* Free what's left to be freed. */
    free(erdos_ncount);
    for (i = 0; i < nvertices; i++)
    {
        sllist_free(erdos_adjls[i]);
    }
    free(erdos_adjls);

    return 0;
}
