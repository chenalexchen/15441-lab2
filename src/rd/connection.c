/** @file connection.c
 *  @brief define various function used by select-based connection
 *
 *  @author Chen Chen
 *  @bug no known bug
 */


#include <sys/select.h>
#include <string.h>


#include "list.h"
#include "connection.h"




struct list_head conn_htable[CONN_HTABLE_SIZE];



static fd_set rd_fd_set;
static fd_set wr_fd_set;
static int max_fd;      /* maximal fd + 1 */

struct conn *get_conn(int fd)
{
        struct conn *c;
        list_for_each_entry(c, &conn_htable[fd % CONN_HTABLE_SIZE], 
                            conn_link){
                if(c->fd == fd){
                        return c;
                }
        }
        return NULL;
}


void insert_conn(struct *conn)
{
        list_add_tail(&conn->conn_link, &conn_htable[conn->fd % CONN_HTABLE_SIZE]);
        return;
}


static void init_conn_buf(struct conn *conn)
{
       memset(conn->buf_rd, CONN_BUFFER_SIZE);
       conn->buf_rd_len = 0;
       memset(conn->buf_wr, CONN_BUFFER_SIZE);
       conn->buf_wr_len = 0;
 
}

int init_conn(struct *conn conn, int fd, int conn_type,
              int (*read)(struct conn *, char *, int),
              int (*write)(struct conn *, char *, int),
              int (*read_callback)(struct conn *),
              int (*write_callback)(struct conn *),
              int (*close)(struct conn *))

{
        conn->fd = fd;
        /* add fd into rd_fd_set, won't add fd to wr_fd_set,
           until fd is explicited written */

        conn->type = conn_type;
        /* set conn ops */
        conn->ops.read = read;
        conn->ops.write = write;
        conn->ops.read_callback = read_callback;
        conn->ops.write_callback = write_callback;
        conn->ops.close = close;
        

        init_conn_buf(conn);
        
        insert_conn(conn);
        

        /* TODO: for different type, should we 
         * add conn to different list */

}



static void select_max_fd(void)
{
        max_fd --;
        while(!FD_ISSET(max_fd, &rd_fd_set) &&
              !FD_ISSET(max_fd, &wr_fd_set) && max_fd > 0)
                max_fd --;
        max_fd ++;
        return;
}

void prepare_read(struct conn* conn)
{
        FD_SET(conn->fd, &rd_fd_set);
        select_max_fd();
}


void prepare_write(struct conn *conn)
{
        FD_SET(conn->fd, &wr_fd_set);
        select_max_fd();
}

void stop_read(struct conn *conn)
{
        FD_CLR(conn->fd, &rd_fd_set);
        select_max_fd();
}

void stop_write(struct conn *conn)
{
        FD_CLR(conn->fd, &wr_fd_set);
        select_max_fd();
}


int process_connection(void)
{
        fd_set rd_fd_set_ready = rd_fd_set;
        fd_set wr_fd_set_ready = wr_fd_set;
        
        int ret;
        struct timeval left;

        while(1){
                left = get_next_timeout();
                ret = select(max_fd, &rd_fd_set_ready,
                             &wr_fd_set_ready, NULL,
                             &left);
                if(ret > 0){
                }else if(ret == 0){
                }else{
                }
                
        }
}

