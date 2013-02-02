/** @file timer.c
 *  @brief define function used by timer
 *
 *  @author Chen Chen
 *  @bug no known bugs
 */

#include "timer.h"

struct timer global_timer;






static void is_timeval_smaller(struct timeval *t1, struct timeval *t2)
{
        struct timeval t3;
        sub_timeval(&t3, t1, t2);
        return (t3->tv_sec < 0);
}

static void sub_timeval(struct timeval *res, struct timeval *subee,
                   struct timeval *suber)
{
        res->tv_sec = subee->tv_sec - suber->tv_sec;
        res->tv_usec = subee->tv_usec - suber->tv_usec;
        if(res->tv_usec < 0){
                res->tv_sec -= 1;
                res->tv_usec += 1000000;
        }
        return;
}


static void load_next_timer(void)
{
        if(list_empty(&global_timer.timer_block_list)){
                global_timer.next = NULL;
        }else{
                /* re-elect new next timer_block */
                global_timer.next = 
                        list_first_entry(&global_timer.timer_block_list,
                                         struct timer_block, tlink);
        }
}


static void insert_timer(struct timer_block *timer_block)
{
        struct timer_block *timer_cur, *timer_next;
        
        if(list_empty(&global_timer.timer_block_list)){
                list_add(&timer_block->tlink,
                         &global_timer.timer_block_list);
                goto done;
        }
        
        list_for_each_entry_safe(timer_cur, timer_next,
                                 &global_timer.timer_block_list,
                                 tlink){
                if(is_timeval_smaller(&timer_block->next_time,
                                      &timer_cur->next_time)){
                        sub_timeval(&timer_cur->next_time,
                                    &timer_cur->next_time,
                                    &timer_block->next_time);
                        /* add before timer_cur */
                        list_add(&timer_block->tlink,
                                 &timer_cur->tlink.prev);
                        goto done;
                }else{
                        sub_timeval(&timer_block->next_time,
                                    &timer_block->next_time,
                                    &timer_cur->next_time);
                }
        }

        /* if the next_time value is too long */
        list_add_tail(&timer_block->tlink, 
                      &global_timer.timer_block_list);

 done:
        load_next_timer();
        return;                                     
}

struct timer_block * create_timer(struct timeval timeout, 
                                  int (*timeout_callback)(void *),
                                  void *data)
{
        struct timer_block * block = 
                (struct timer_block *)malloc(sizeof(struct timer_block));
        if(!block){
                return NULL;
        }
        block->next_time = timeout;
        block->timeout_callback = timeout_callback;
        block->data = data;

        insert_timer(timer_block);
}



int cancel_timer(struct timer_block *timer_block)
{
        list_del(&timer_block->tlink);
        free(timer_block);
        
        load_next_timer();
        return 0;
}



int adjust_next_timer(struct timeval *left, int expired)
{
        if(expired){
                if(global_timer.next->callback){
                        global_timer.next->callback(global_timer.
                                                    next->data);
                        list_del(&global_timer.next->tlink);
                        free(&global_timer.next);
                        load_next_timer();
                }
        }else{
                sub_timeval(&global_timer.next->next_time,
                            &global_timer.next->next_time,
                            left);
        }
        return expired;
}


struct timeval get_next_timeout(void)
{
        return global_timer.next->next_time;
}
