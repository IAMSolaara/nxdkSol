#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

node *addnode(node *head, int sign, int num) {
    node *new = (node *) malloc(sizeof(node));
    new->sign = sign;
    new->num = num;
    new->next = NULL;

    if (head == NULL)
        head = new;
    else {
        node *it;

        for (it = head; it->next != NULL; it = it->next)
            ;

        it->next = new;
    }

    return head;
}

node *find(node *head, int sign, int num) {
    node *it;

    for (it = head; it != NULL; it = it->next)
        if (it->sign == sign && it->num == num)
            break;

    return it;
}

node *delnode(node *head, node *ep) {
    if (head == ep) {
        node *new_head = head->next;
        free(head);
        return new_head;
    }

    node *it;

    for (it = head; it->next != ep; it = it->next)
        ;

    it->next = ep->next;
    free(ep);

    return head;
}

void freelist(node *head) {
    node *it = head, *tmp;

    while (it != NULL) {
        tmp = it;
        it = it->next;
        free(tmp);
    }
}
