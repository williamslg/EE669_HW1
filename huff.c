#include <stdio.h>
#include "stdlib.h"
#include "huff.h"
#include "count.h"

void ListInit(List *header)
{
	int i;
	Node *node;
	/* initialize the header of the node list */
	header->num_members = 0;
	header->anchor.next = &header->anchor;
	header->anchor.prev = &header->anchor;
	header->anchor.obj = NULL;

	/* find all symbols with non-zero weight and insert them into the list */
	for (i=NUM-1; i>0; i--) {
		if (symbol[i] != 0) {
			node = (Node *)malloc(sizeof(Node));
			if (node == NULL) {
				printf("Out of space!\n");
				exit(1);
			}
			node->symbol = i;
			node->weight = symbol[i];
			node->left_child = NULL;
			node->right_child = NULL;
			node->parent = NULL;
			
			ListAppend(header, (void *)node);
		}
	}
	/* sort the original list with all leaves */
	ListSort(header);
}

void ListAppend(List *header, void *obj)
{
	Link *link = (Link *)malloc(sizeof(Link));
	if (link == NULL) {
		printf("Out of space!\n");
		exit(1);
	}
	link->obj = obj;
	ListInsert(link, header->anchor.prev, &header->anchor);
	header->num_members++;
}

void ListInsert(Link *new, Link *prev, Link *next)
{
	new->next = next;
	new->prev = prev;
	prev->next = new;
	next->prev = new;
}


int ListSort(List *header)
{
	Link *tmp, *first, *last, *p1, *p2;
	first = header->anchor.next;
	last = header->anchor.prev;
	tmp = (Link *)malloc(sizeof(Link));

	if (tmp == NULL) {
		printf("Out of space!\n");
		exit(1);
	}

	if (header->anchor.next == &header->anchor) {
		printf("List is empty\n");
		return 0;
	}

	if (header->anchor.next == header->anchor.prev) {
		return 1;
	}

	for (p1=first; p1!=&header->anchor; p1=p1->next) {
		for (p2=first; p2!=last; p2=p2->next) {
			if (((Node *)(p2->next->obj))->weight < ((Node *)(p2->obj))->weight) {
				tmp->obj = p2->next->obj;
				p2->next->obj = p2->obj;
				p2->obj = tmp->obj;
			}
		}
		last = last->prev;
	}
	return 1;
}


void ListDisplay(List *header)
{
	printf("number of symbols: %d\n", header->num_members);
	Link *link;
	for (link = header->anchor.next; link != (&header->anchor); link = link->next) {
		printf("symbol: %x weight: %d\n", ((Node *)link->obj)->symbol, ((Node *)link->obj)->weight);
	}
}

void Unlink(List *header, Link *link)
{
	if ((link != &header->anchor) && header->num_members) {
		link->prev->next = link->next;
		link->next->prev = link->prev;
		--header->num_members;
		free(link);
	}
}

Node *TreeBuild(List *header)
{
	Link *l1, *l2;
	Node *n1, *n2;

	/* keep finding the minimum 2 nodes building their parent node and insert it back */
	while(header->num_members > 1) {
		l1 = header->anchor.next;
		l2 = l1->next;
		n1 = (Node *)(l1->obj);
		n2 = (Node *)(l2->obj);

		Link *new_link = (Link *)malloc(sizeof(Link));
		if (new_link == NULL) {
			printf("Out of space!\n");
			exit(1);
		}
		Node *new_node = (Node *)malloc(sizeof(Node));
		if (new_node == NULL) {
			printf("Out of space!\n");
			exit(1);
		}
		new_link->obj = (void *)new_node;
		
		/* use -1 to represent the internal node */
		new_node->symbol = -1;
		new_node->weight = n1->weight + n2->weight;
		new_node->parent = NULL;

		n1->parent = new_node;
		n2->parent = new_node;
		if (n1->weight <= n2->weight) {
			new_node->left_child = n1;
			new_node->right_child = n2;
		}
		else {
			new_node->left_child = n2;
			new_node->right_child = n1;
		}
		/* This is for my own check */

		Unlink(header, l1);
		Unlink(header, l2);

		printf("left: %d, right: %d, parent: %d\n", new_node->left_child->weight, new_node->right_child->weight, new_node->weight);
		ListInsert(new_link, &header->anchor, header->anchor.next);
		header->num_members++;
		ListSort(header);
	}
	if (header->num_members == 0) {
		printf("TreeBuild(): Wrong list number!\n");
		exit(1);
	}
	return (Node *)(header->anchor.next->obj);

}

void CodeCreate(Node *node, int bit, int code, int length, Table *table)
{
	code = code << 1;
	code = code + bit;
	if (node->left_child != NULL && node->right_child != NULL) {
		length++;
		CodeCreate(node->left_child, 0, code, length, table);
		CodeCreate(node->right_child, 1, code, length, table);
	}
	else {
		table[node->symbol].code = code;
		table[node->symbol].length = length;
		table[node->symbol].weight = node->weight;
	}
}
