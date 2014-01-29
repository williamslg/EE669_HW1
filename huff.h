#ifndef _HUFF_H_
#define _HUFF_H_

/* node struture to store huffman tree nodes */
typedef struct node {
	int symbol;	
	int weight;
	struct node *left_child;
	struct node *right_child;
	struct node *parent;
} Node;

/* link strture stores nodes to build the huffman tree */
typedef struct link {
	void *obj;
	struct link *next;
	struct link *prev;
} Link;

/* nodes list */
typedef struct list {
	int num_members;
	Link anchor; 
} List;

typedef struct table {
	int code;
	int length;
	int weight;
} Table;

/* Initialize the nodes list.
   Add all symbols whose weight is non-zero into this list, then sort the list.
*/
void ListInit(List *header);

/* append a link with a node */
void ListAppend(List *header, void *obj);

/* Insert a new node link after specified link*/
void ListInsert(Link *new, Link *prev, Link *next);

/* use bubblesort to sort the list by its node weight */
int ListSort(List *header);

/* display the list*/
void ListDisplay(List *header);

/* unlink one link in the list */
void Unlink(List *header, Link *link);

/* build Huffman Tree by the leaves list and return its root node */
Node *TreeBuild(List *header);

/* create the Huffman talbe */
void CodeCreate(Node *node, int bit, int code, int length, Table *table);
#endif /* _HUFF_H_ */
