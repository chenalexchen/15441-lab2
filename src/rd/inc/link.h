/** @file link.h
 *  @brief define link struct
 *
 *  @author Chen Chen
 *  @bug no known bug
 */

#ifndef __RD_LINK_H_
#define __RD_LINK_H_


#include "node.h"
#include "list.h"

struct link{
        struct node[2];
        int weight;
        struct list_head glink;
};





#endif /* end of __RD_LINK_H_ */
