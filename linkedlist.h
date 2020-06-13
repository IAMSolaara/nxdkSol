#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct node {
    int sign;
    int num;
    struct node *next;
} node;

node *addnode(node *head, int sign, int num); /* adds element with specified data to the end of the list and returns new head node. */
node *find(node *head, int sign, int num); /* returns pointer of element with specified data in list. */
node *delnode(node *head, node *ep); /* removes element from the list and returns new head node. */
void freelist(node *head); /* deletes all elements of the list. */

#endif
