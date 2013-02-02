/** @file flist.h
 *  @brief file list the node serves
 *
 *  @author Chen Chen
 *  @bug no known bug
 */

#ifndef __FLIST_H_
#define __FLIST_H_

#include "list.h"

#define OBJ_NAME_LEN        128
#define PATH_NAME_LEN       256
struct flist_map{
        char obj_name[OBJ_NAME_LEN];
        char path_name[PATH_NAME_LEN];
        struct list_head flink;
        
};

void insert_fmap(struct flist_map *fmap);

#endif /*end of __FLIST_H_ */
