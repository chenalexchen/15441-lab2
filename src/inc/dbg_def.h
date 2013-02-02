/** @file debug_define.h
 *  @brief define some primitives for debuging
 *
 *  @author Chen Chen (chenche1)
 *  @bug no bug report 
 */

#ifndef __DBG_DEF_H_
#define __DBG_DEF_H_


#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define dbg_printf(fmt, args...) do{fprintf(stdout, "(%s@line%d)dbg:"fmt"\n", \
                                            __func__,__LINE__,          \
                                            ##args); fflush(stdout);}while(0)
#define err_printf(fmt, args...) do{fprintf(stderr, "(%s@line%d)err:"fmt"\n", \
                                            __func__,__LINE__,          \
                                            ##args); fflush(stderr);}while(0)
#else
#define dbg_printf(fmt, args...)
#define err_printf(fmt, args...) 
#endif


#endif /* end of __DBG_DEF_H_ */
