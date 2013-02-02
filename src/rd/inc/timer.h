/** @file timer.h
 *  @brief define a simple timer
 *  
 *  @author Chen Chen
 *  @bug no known bugs
 */


#ifndef __TIMER_H_
#define __TIMER_H_

#include <sys/time.h>


#include "list.h"



struct timer_block{
        struct timeval next_time;
        int (*timeout_callback)(void *data);
        void *data;
        struct list_head tlink;
};

struct timer{
        struct timer_block *next;
        struct list_head timer_block_list;
};


struct timer_block *create_timer(struct timeval timeout, 
                                  int (*timeout_callback)(void *),
                                  void *data);

int cancel_timer(struct timer_block *timer_block);

int adjust_next_timer(struct timeval *left, int expired);

struct timeval get_next_timeout(void);

#endif /* end of __TIMER_H_ */
