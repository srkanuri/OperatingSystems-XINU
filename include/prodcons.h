/***************************************************************
 * prodcons.h - Header file for producer consumer
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangarish Wagle (mawagle)
 * Date Created: 09/20/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 09/20/2016
 * Assignment: 2
 ***************************************************************/
extern int n; //Global variable for producer consumer

/* Semaphore Declarations */
extern sid32 produced, consumed;

/*Function Prototypes*/
void consumer(int count);
void producer(int count);

