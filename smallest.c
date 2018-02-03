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
};

struct list
{
	struct node *head;
	int level;
};

static int
random_level(void)
{
	int level = 1;
	int r = rand();
	while (r % 2 && level < MAX_SKIPLIST_HEIGHT)
	{
		level++;
		r = rand();
		printf("\nr = %d", r);
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
	printf("myvalue = %lf\n", myvalue);
	struct node *update[MAX_SKIPLIST_HEIGHT];

	struct node *curr = mylist.head;
	/* add 1st element to skip-list */
	if (curr == NULL)
	{
		struct node *new_node = create_node(value);
		mylist.level = 1;
		mylist.head = new_node;
		return;
	}

	/* find biggest element smaller than new value at every level */
	for (int i = mylist.level; i > 0; i--)
	{
		printf("i = %d\n", i);
		if (curr->next[i] != NULL)
		{
			printf("curr->next[i]->value = %lf", curr->next[i]->value);
			while (curr->next[i]->value < myvalue)
			{
				curr = curr->next[i];
			}
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

	/* add new node to skip-list */
	for (int i = 1; i <= new_level; i++)
	{
		new_node->next[i] = update[i]->next[i];
		update[i]->next[i] = new_node;
	}
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
}

static double
find_smallest95(void)
{
/* TODO lookup element at position count*0.95
 * return smallest int? double? such that it's bigger
 */


}

int main()
{
    int input_size = 3;
    /* keep track of iteration count */
    int cnt = 0;
    //int input_size = 128000000;
    double a;

	/* for random probability generation later on */
	srand(time(NULL));

    for (int i = 0; i < input_size; i++)
    {
        /* read double from stdin */
        scanf("%lf",&a);
        printf("Entered %lf\n", a);

		cnt++;

		/* TODO if magnitude of new double is smaller than epsilon's, update epsilon */

        /* add element to skip-list */
        insert_node(&a);

        /* find smallest double value bigger than ... */
        //find_smallest95(void)
    }
	print_list();


     /* TODO destroy list */
}
