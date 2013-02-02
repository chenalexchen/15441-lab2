/** @file node.h
 *  @brief define node
 *
 *  @author Chen Chen
 *  @bug no known bug
 */

#ifndef __RD_NODE_H_
#define __RD_NODE_H_

#include "list.h"


#define HOSTNAME_LEN 127

#define HASH_SIZE 255


struct node{
        int id;
        char hostname[HOSTNAME_LEN + 1];
        int rport;   /* routing port */
        int lport;   /* local port */
        int sport;   /* server port */        

        /* for shortest path algorithm (dijkstra) */
        int last_node_id;          /* node id of last hop on shortest path */
        int next_hop_node_id;      /* node id of the next hop on 
                                    * shortest path */
        int weight;                /* weight used by dijikstra algo. */

        struct list_head nlist;           /* list of the neighbor node */
        struct list_head glink;           /* list for node id */
};

struct nnode{                    /* neighbor node */
        int id;
        char hostname[HOSTNAME_LEN + 1];
        int rport;
        int lport;
        int sport;

        struct list_head nlink;
};


void add_neighbor(struct nnode *nn, struct node *n);
void insert_node(struct node *n);
void clear_neighbor_list(struct node *n);
struct node *get_node(int nid);



extern struct list_head *node_htable;
#endif /* end of __RD_NODE_H_ */
