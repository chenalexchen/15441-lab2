/** @file flist.c
 *  @brief define flist related function
 * 
 *  @author Chen Chen
 *  @bug no known bugs
 */

#include "flist.h"




/* define the global variable for file map */
list_head file_list;





void insert_fmap(struct flist_map *fmap)
{
        list_add_tail(&fmap->flink, &file_list);
        return;
}
