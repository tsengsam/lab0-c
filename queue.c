#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (!q)
        return;
    while (q->head) {
        list_ele_t *tmp = q->head;
        free(tmp->value);
        q->head = q->head->next;
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    /* Don't forget to allocate space for the string and copy it */
    char *tmp = malloc(sizeof(char) * strlen(s) + 1);
    if (!tmp) {
        free(newh);
        return false;
    }
    newh->value = tmp;
    strncpy(tmp, s, strlen(s) + 1);
    /* What if either call to malloc returns NULL? */
    newh->next = q->head;
    q->head = newh;
    q->size += 1;
    if (q->size == 1) {
        q->tail = newh;
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        return false;
    char *tmp = malloc(sizeof(char) * strlen(s) + 1);
    if (!tmp) {
        free(newh);
        return false;
    }
    newh->value = tmp;
    strncpy(tmp, s, strlen(s) + 1);
    newh->next = NULL;
    if (!q->tail) {
        q->tail = newh;
        q->head = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    if (sp) {
        strncpy(sp, tmp->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    free(tmp->value);
    free(tmp);
    q->size--;

    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size == 0)
        return;
    list_ele_t *current = q->head;
    list_ele_t *next = current->next;
    list_ele_t *tmp;
    q->tail = q->head;
    q->tail->next = NULL;
    while (next) {
        tmp = next->next;
        next->next = current;
        current = next;
        next = tmp;
    }
    q->head = current;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1)
        return;
    q->head = merge_list(q->head);
    q->tail = q->head;
    for (; q->tail->next; q->tail = q->tail->next) {
        /*nothing to do here*/
    }
}
/*
 * Apply divide and conquer strategy merge sort
 * Split into two lists recursivly then merge them
 */
list_ele_t *merge_list(list_ele_t *head)
{
    if (!head || !head->next)
        return head;
    list_ele_t *slow = head;
    list_ele_t *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    list_ele_t *ll = merge_list(head);
    list_ele_t *rl = merge_list(fast);

    return merge(ll, rl);
}

list_ele_t *merge(list_ele_t *ll, list_ele_t *rl)
{
    if (!ll)
        return rl;
    if (!rl)
        return ll;

    if (strcmp(ll->value, rl->value) > 0) {
        rl->next = merge(ll, rl->next);
        return rl;
    } else {
        ll->next = merge(ll->next, rl);
        return ll;
    }
}
