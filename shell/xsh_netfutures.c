#include <xinu.h>
#include <stdio.h>
#include <future.h>
/***************************************************************
 * xsh_netfutures.c - Custom implementation of network futures
 *
 * Authors: Srikanth Kanuri (srkanuri)
 *          Mangirish Wagle (mawagle)
 * Date Created: 12/10/2016
 * Last Modified by: Srikanth Kanuri
 * Date Last Modified: 12/10/2016
 * Assignment: 8
 ***************************************************************/

/*Global variable n will be on heap and accessible in consume and produce*/

shellcmd xsh_netfutures(int nargs, char *args[])
{
  //Argument verifications and validations
  int32	retval,rval;			/* return value from sys calls	*/
  uint32 localip;		/* local IP address		*/
  uint32 remip;			/* remote sender's IP address	*/
  uint16 remport;		/* remote sender's UDP port	*/
  char	buff[1500];		/* buffer for incoming reply	*/
  char	obuff[1500];
  int32	msglen;			/* length of outgoing message	*/
  int32	slot;			/* slot in UDP table 		*/
  int cons_ready;
  uint16 echoserverport= 7;	/* port number for UDP echo	*/
  uint32 t1,t2;
  
  /* Output info for '--help' argument */
  if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
    printf("Usage: %s [<count> | -f]\n\n", args[0]);
    printf("Description:\n");
    printf("\tNetwork Futures Implementation in XINU\n");
    printf("\tThe command accepts only 1 optional argument 'UDP Port' (7 is default)\n");
    return 0;
  }
   
  if (nargs == 1 || nargs == 2) {
    //printf("nargs is 1 or 2\n");
    if(nargs == 2){
      int input_port;
      input_port = atoi(args[1]);
      if(input_port == 0 && args[1] != 0){
	fprintf(stderr, "%s: wrong argument enterted\n", args[0]);
	fprintf(stderr, "%s accepts only an integer argument for port\n", args[0]);
	return 1;
      }
      else
	echoserverport = input_port;
    }
    // printf("Local IP %u\n",localip);
    localip = getlocalip();
    if (localip == SYSERR) {
      fprintf(stderr, "%s: could not obtain a local IP address\n", args[0]);
      return 1;
    }
    
    /* register local UDP port */
    
    slot = udp_register(0, 0, echoserverport);
    if (slot == SYSERR) {
      fprintf(stderr, "%s: could not reserve UDP port %d\n", args[0], echoserverport);
      return 1;
    }
    
    future *f1;
    //printf("Starting Futures\n");
    while (TRUE) {
      if(!cons_ready){
	printf("-------------------------------------------------\n");
	f1 = future_alloc(FUTURE_EXCLUSIVE);
	resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
	//printf("Consumer Ready\n");
	rval = gettime(&t1);
	//printf("Initial %u, %d\n",t1,retval);
	cons_ready = 1;
	memset(obuff, 0, sizeof obuff);
	memset(buff, 0, sizeof buff);
      }
      retval = udp_recvaddr(slot, &remip, &remport, buff,sizeof(buff), 600000);
      //printf("Receiving %d, %d, %d\n",retval, TIMEOUT, SYSERR);
      if (retval == TIMEOUT) {
	continue;
      } else if (retval == SYSERR) {
	fprintf(stderr, "%s: error receiving UDP\n",args[0]);
	return 1;
      }
      else{
	printf("Request Message\t: %s\n",buff);
	//f1->value = &buff;
	resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
	sleep(1);
	rval = gettime(&t2);
	//printf("Final %u, %d, %d\n",t2,retval, f1->value);
	sprintf(obuff,"Input Request\t: %s\nFutures Output\t: %d\nWait Time\t: %u\n",buff,(f1->value),(t2-t1));
	msglen = strlen(obuff);
	//printf("%d,%d\n",retval,msglen);
	//printf("%s\n",obuff);
	retval = udp_sendto(slot, remip, remport, obuff, msglen);
	//printf("%d\n",retval);
	if (retval == SYSERR) {
	  fprintf(stderr, "%s: udp_sendto failed\n",args[0]);
	  return 1;
	}
	cons_ready = 0;
      }
    }
  }
  /* Wrong argument count */
  else {
    fprintf(stderr, "%s: too many arguments\n", args[0]);
    fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
    return 1;
  }
}
