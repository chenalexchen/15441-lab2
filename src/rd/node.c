/** @file node.c
 *  @brief define function to manipulate node
 *
 *  @author Chen Chen
 *  @bug no known bug
 */

#include "node.h"



/* define the global node hash table */
list_head node_htable[HASH_SIZE];            





void add_neighbor(struct nnode *nn, struct node *n)
{
        list_add_tail(&nn->nlink, &n->nlist);
        return;
}

void insert_node(struct node *n)
{
        list_add_tail(&nn->nlink, &node_htable[n->id % HASH_SIZE]);
        return;
}

void clear_neighbor_list(struct node *n)
{
        struct nnode *n_curr, *n_next;
        list_for_each_entry_safe(n_curr, n_next, &n->nlist, nlink){
                list_del(&n_curr->nlink);
                free(n_curr);
                continue;
        }
        return;
}

struct node *get_node(int nid)
{
        struct node *nd;
        list_for_each_entry(nd, node_htable[nid % HASH_SIZE], glink){
                if(nd->id == nid){
                        return nd;
                }
        }
        return NULL;
}
