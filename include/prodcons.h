/***************************************************************
 * prodcons.h - Header file for producer consumer
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangarish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Mangarish Wagle
 * Date Last Modified: 09/27/2016
 * Assignment: 2 and 3
 ***************************************************************/
extern int n; //Global variable for producer consumer

/* Semaphore Declarations */
extern sid32 produced, consumed;

/*Function Prototypes*/
void consumer(int count);
void producer(int count);
