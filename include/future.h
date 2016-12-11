#ifndef _FUTURE_H_
#define _FUTURE_H_
/***************************************************************
 * future.h - Header file for Futures
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 10/06/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 12/10/2016
 * Assignment: 8,4
 ***************************************************************/
 
/* define states */
#define FUTURE_EMPTY	  0
#define FUTURE_WAITING 	  1         
#define FUTURE_VALID 	  2         

/* modes of operation for future*/
#define FUTURE_EXCLUSIVE  1	

typedef struct futent
{
  int *value;		//Produced Value
  int flag;		//FUTURE_EXCLUSIVE only
  int state;         	//FUTURE_EMPTY, FUTURE_WAITING, FUTURE_VALID
  pid32 pid;            //Process ID of the waiting process
} future;

/* Interface for system call */
future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);

/* Interface for app function call */
uint future_prod(future *fut);
uint future_cons(future *fut);
 
#endif /* _FUTURE_H_ */
