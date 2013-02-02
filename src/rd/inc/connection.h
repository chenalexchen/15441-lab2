/** @file connection.h
 *  @brief define select-based connection 
 *
 *  @author Chen Chen
 *  @bug no bug known
 */

#ifndef __CONNECTION_H_
#define __CONNECTION_H_


#includ <sys/select.h>


#define CONN_BUFFER_SIZE     1024
#define CONN_HTABLE_SIZE     256

#define CONN_TYPE_SOCKET    0
#define CONN_TYPE_CONN      1




struct conn;

struct conn_ops{
        /* read the content in the conn buffer */
        int (*read)(struct conn *c, char *buf, int len);   
        /* write the content to the conn buffer */
        int (*write)(struct conn *c, char *buf, int len);                
        int (*close)(struct conn *c);

        /* when the connection finish reading from the fd,
         * this callback will be called */
        int (*read_callback)(struct conn *c);
        /* when the connection finish reading from the fd,
         * this callback will be called */
        int (*write_callback)(struct conn *c);

        
};

struct conn{
        int fd;
        int type;
        struct conn_ops ops;        
        
        /* define buffer used for connection */
        char buf_rd[CONN_BUFFER_SIZE];
        int buf_rd_len;
        char buf_wr[CONN_BUFFER_SIZE];
        int buf_wr_len;
        
        /* different link */
        struct list_head conn_link;  /* link for global conn htable */
        struct list_head type_link;  /* link for different type of 
                                      * connection */                
};


struct conn *get_conn(int fd);
void insert_conn(struct conn *);

int init_conn(struct conn *, int fd, int conn_type,
              int (*read)(struct conn *, char *, int),
              int (*write)(struct conn *, char *, int),
              int (*read_callback)(struct conn *),
              int (*write_callback)(struct conn *),
              int (*close)(struct conn *));

void prepare_read(struct conn* conn);
void prepare_write(struct conn *conn);
void stop_read(struct conn *conn);
void stop_write(struct conn *conn);


extern struct list_head *conn_htable;



#endif /*end of __CONNECTION_H_ */

