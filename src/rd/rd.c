/** @file rd.c 
 *  @brief define the main function of routing daemon 
 *  
 *  @author Chen Chen
 *  @bug no known bug
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "dbg_def.h"
#include "list.h"
#include "node.h"
#include "err_code.h"



static int self_nid = 1;
static char *config_filename = "../starter/node1.conf";
static char *file_list_fileanme = "../starter/node1.files";
static int ad_cycle_time = 1;                 /* in sec */
static int neighbor_timeout = 10;
static int retran_timeout = 5;
static int lsa_timeout = 2;




static list_head file_list;


static int read_config(int conf_fd)
{
        int nid;
        char host_name[HOSTNAME_LEN+1];
        int rport;
        int lport;
        int sport;
        struct node *n == NULL;
        struct nnode *nn;
        while(fscanf(conf_fd, "%d %s %d %d %d", &nid, host_name,
                     &rport, &lport, &sport)!= EOF){
                dbg_printf("nid(%d), host_name(%s), rport(%d), "
                           "lport(%d), sport(%d)", nid, host_name,
                           rport, lport, sport);
                if(nid == self_nid){
                        n = (struct node *)malloc(sizeof(struct node));
                        if(!n){
                                return ERR_NO_MEM;
                        }
                        
                        n->id = nid;
                        strncpy(n->host_name, host_name, HOSTNAME_LEN);
                        n->rport = rport;
                        n->lport = lport;
                        n->sport = sport;
                        
                        insert_node(n);
                }else{
                        n = get_node(self_nid);
                        if(!n){
                                err_printf("curr node must come "
                                           "first of conf file");
                                return ERR_NO_SELF_NODE;
                        }
                        nn = (struct nnode *)malloc(sizeof(struct nnode));
                        if(!nn){
                                return ERR_NO_MEM;
                        }
                        nn->id = nid;
                        strncpy(nn->host_name, host_name, HOSTNAME_LEN);
                        nn->rport = rport;
                        nn->lport = lport;
                        nn->sport = sport;
                        add_neighbor(nn, n);
                }                
        }
        return 0;        
}


static int read_file_list(int fmap)
{
        char obj_name[OBJ_NAME_LEN];
        char path_name[PATH_NAME_LEN];
        struct flist_map *fmap;
        int ret;
        while(fscanf(fmap, "%s %s", obj_name, path_name) != EOF){
                dbg_printf("fmap(obj_name=%s, path_name=%s)",obj_name, path_name);
                if(!(fmap = (struct flist_map *)
                     malloc(sizeof(struct flist_map)))){
                        ret = ERR_NO_MEM;
                }
                strcpy(fmap->obj_name, obj_name);
                strcpy(fmap->path_name, path_name);
                insert_fmap(fmap);
        }
        return 0;
}

int main(int argc, char *argv[])
{
        int ret;
        int conf_fd;
        int fmap_fd;
        /* daemonize the program */

        /* check arguments and fill in config static values */

        /* read in the config and file list */
        if(!(conf_fd = open(config_filename, O_RDONLY))){
                err_printf("open config file %s failed", config_filename);
                return EXIT_FAILURE;
        }
        
        if((ret = read_config(conf_fd)) < 0){
                ret = EXIT_FAILURE;
                goto out1;
        }

        if(!(fmap_fd = open(file_list_filename, O_RDONLY))){
                err_printf("open fmap file %s failed", file_list_filename);
                ret = EXIT_FAILURE;
                goto out1;
        }

        if((ret = read_file_list(fmap_fd)) < 0){
                ret = EXIT_FAILURE;
                goto out2;
        }

        /* setting up connections */

        /* listen on the tcp port for incoming flask request */
        
        /* listen on the routing port */
        
        /* connect to routing port */

        /* process request and send back response */


        /* end */
        close(fmap_fd);
        close(conf_fd);

        return 0;
 out2:
        close(fmap_fd);
 out1:
        close(conf_fd);
        return ret;
}


