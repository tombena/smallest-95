#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* TODO describe a skip-list */

/* TODO replace all ints by doubles */

/* TODO create indexable skip-list */
/* log2(128000000) + 1 */
#define MAX_SKIPLIST_HEIGHT 29

/* TODO declare skip list globally */
struct list mylist;
double epsilon = 1;

struct node
{
    double value;
    int level;	//highest level node is on
    struct node *next[MAX_SKIPLIST_HEIGHT]; // pointer to the next node depending on the level
											// NULL if no neighbor on this level
    int distance[MAX_SKIPLIST_HEIGHT]; // distance to the next element
};

struct list
{
	struct node *head;
	int level;
    int size;
};

static int
random_level(void)
{
	int level = 1;
	int r = rand();
	while ((r%2 == 0) && (level < MAX_SKIPLIST_HEIGHT))
	{
		level++;
		r = rand();
	}
	return level;
}

static struct node*
create_node(double *value)
{
    struct node *new_node = NULL;
    new_node = (struct node*) malloc(sizeof(struct node));

    if (new_node == NULL)
        printf("Unable to create node\n");

	/* doesn't have a right neighbor yet */
	for (int i = 0; i < MAX_SKIPLIST_HEIGHT; i++)
			new_node->next[i] = NULL;

	new_node->value = *value;
	return new_node;
}

/* TODO replace all 1s by 0s
 */
static void
insert_node(double *value)
{
	double myvalue = *value;
	struct node *update[MAX_SKIPLIST_HEIGHT];

	struct node *curr = mylist.head;
	/* add 1st element to skip-list */
	if (curr == NULL)
	{
		struct node *new_node = create_node(value);

        /* level 1 distances are always 1 */
        new_node->distance[1] = 1;

        /* update list */
		mylist.level = 1;
		mylist.head = new_node;
        mylist.size++;
		return;
	}

	/* find biggest element smaller than new value at every level */
	for (int i = mylist.level; i > 0; i--)
	{
        while (curr->next[i] != NULL)
        {
            if (curr->next[i]->value < myvalue)
                curr = curr->next[i];
            else
                break;
        }
        update[i] = curr;
	}

	/* element just bigger than new value */
	curr = curr->next[1];

	int new_level = random_level();
	/* if new level added to skip-list */
	if (new_level > mylist.level)
	{
		for (int i = mylist.level + 1; i <= new_level; i++)
			update[i] = mylist.head;

		/* increase skip-mylist max level */
		mylist.level = new_level;
	}

	/* create new node */
	struct node *new_node = create_node(value);
    int d_before = 0;
    int d_after = 0;
    struct node *tmp_before, *tmp_after;
	/* add new node to skip-list */
	for (int i = 1; i <= new_level; i++)
	{

        /* example of skip-list:
         * 4: 3 ---------------------------------------------------> NULL
         * 3: 3 ---------> 5 -----------> (8) ---------------> 11 -> NULL
         * 2: 3 ---------> 5 ------> 7--> (8) --------> 10 --> 11 -> NULL
         * 1: 3 --> 4 ---> 5 -> 6 -> 7 -> (8) --> 9 --> 10 --> 11 -> NULL
         */

        /* update distance from prev element to new element */
        if (i == 1)
            /* distance to next element on level 1 is always 1 */
            d_before = 1;
        else if (tmp_before != update[i])
            /* If (8) is added to the skip-list in the following way,
             * distance to (8) can be computed on each level:
             * 1: distance(7, 8) = 1 (direct neighbors)
             * 2: distance(7, 8) = 1 (already known)
             * 3: distance(5, 8) = distance(5, 7) + distance(7, 8)
             */
            d_before += update[i]->distance[i - 1];
        tmp_before = update[i];

        /* update distance from new element to next element */
        if (update[i]->next[i] != NULL)
        {
            if (i == 1)
                d_after = 1;
            else
             /* If (8) is added to the skip-list in the following way,
             * distance from (8) can be computed on each level:
             * 1: distance(8, 9)  = 1 (direct neighbors)
             * 2: distance(8, 10) = distance(8, 9) + distance (9, 10)
             * 3: distance(8, 11) = distance(8, 10) + distance (10, 11)
             */
                d_after += tmp_after->distance[i - 1];
        }
        else
            d_after = 0;
        tmp_after = update[i]->next[i];

		new_node->next[i] = update[i]->next[i];
        new_node->distance[i] = d_after;
		update[i]->next[i] = new_node;
        update[i]->distance[i] = d_before;
	}

    mylist.size++;
}

/* TODO need to update this function */
static double
search_by_position(int n)
{
    if (n >= mylist.size)
    {
        printf("Wrong index = %d\n", n);
        /* TODO limits */
        return 0.0;
    }
    struct node *curr = mylist.head;
    int pos = 0;

    /* note: curr stays at position pos */
    /* TODO need to figure this out */
    for (int i = mylist.level; i > 0; i--)
    {
        printf("curr->distance[i] = %d\n", curr->distance[i]);
        while (pos + curr->distance[i] <= n)
        {
            pos += curr->distance[i];
            curr = curr->next[i];
        }
    }
    return curr->value;
}

static void
print_list()
{
	struct node *curr;
	printf("list level = %d\n", mylist.level);
	for (int i = mylist.level; i > 0; i--)
	{
		curr = mylist.head;
		printf("\n");
		while (curr != NULL)
		{
            printf("%lf -", curr->value);
			curr = curr->next[i];
		}
	}


    printf("\ndistances");
    for (int i = mylist.level; i > 0; i--)
	{
        printf("level = %d\n", i);
		curr = mylist.head;
		printf("\n");
		while (curr->next[i] != NULL)
		{
            printf("%lf-%lf d= %d\n", curr->value, curr->next[i]->value, curr->distance[i]);
			curr = curr->next[i];
		}
	}



}

int main()
{
    int input_size = 7;
    /* iteration counter */
    int cnt = 0;
    //int input_size = 128000000;
    double a;
    int index = 0;

	/* for random probability generation later on */
	srand(time(NULL));

    for (int i = 0; i < input_size; i++)
    {
        /* read double from stdin */
        scanf("%lf\n", &a);

        /* add element to skip-list */
        /* TODO why is last node not added? */
        insert_node(&a);

        index = cnt*0.95;
        //printf("95 percent is index = %d", index);

        /* find index-th element in list */
        //a = search_by_position(index);

        /*update iteration counter */
		cnt++;
    }
	print_list();
    printf("list size = %d\n", mylist.size);

     /* TODO destroy list */
}
