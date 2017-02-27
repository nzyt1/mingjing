/*
 * =====================================================================================
 *
 *       Filename:  debug.h
 *
 *    Description:  copy from rofl0r
 *
 *        Version:  1.0
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  rofl0r
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#include <unistd.h>
#include <stdio.h>
#define PSTDERR(fmt, args...) do { dprintf(2, fmt, ## args);} while(0)
#define PDEBUG(fmt, args...) PSTDERR("[%d]: DEBUG: " fmt, getpid(), ##args)
#define DEBUGDECL(args...) args
#else
#define PDEBUG(fmt, args...) do {} while(0)
#define DEBUGDECL(args...)
#endif

#define PFUNC() do {PDEBUG("pid[%d]:%s\n", getpid(), __FUNCTION__); } while(0)

#endif
