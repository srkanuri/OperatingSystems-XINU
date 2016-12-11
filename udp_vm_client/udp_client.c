// UDP Client sending and receiving data over UDP
// Authors: Mangirish Wagle, Srikanth Kanuri

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFERSIZE 1024*1024*2   //Setting sufficient buffer size to handle large files

struct timeval start_timev, end_timev;  //Structs to use for gettimeoftheday

/*
 * Error reporting function. Closes the socket file descriptors before exiting
 */
void report_error(const char *message, int *server_sfd, int *client_sfd)
{
  if(*server_sfd)
  {
    close(*server_sfd);
  }
  if(*client_sfd)
  {
    close(*client_sfd);
  }
  perror(message);
  exit(1);
}

/*
 * Core client function that sends and receives data to server on udp
 * Accepts hostname, portnumber and filename as params
 */
void run_client(char *host, char *portnum, char *dataStr)
{
    //printf("Inside run client");
    int server_sfd, client_sfd, port, n, lnt;
    socklen_t clen;
    struct sockaddr_in client_addr, server_addr;
    struct hostent *server;

    int timeout = 5; //Timeout value for response in seconds.

    char *buffer = (char *)malloc(BUFFERSIZE*sizeof(char));

    port = atoi(portnum);

    fd_set fds;                // fd set declare
    struct timeval time_out; // Struct for Timeout value

    client_sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sfd < 0)
    {
        report_error("ERROR opening socket", &server_sfd, &client_sfd);
    }
    //printf("Client sfd defined");
    server = gethostbyname(host);

    if (server == NULL)
    {
        fprintf(stderr,"ERROR, Host not found\n");
        exit(0);
    }

    bzero((char *) &server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
         (char *)&server_addr.sin_addr.s_addr,
         server->h_length);

    server_addr.sin_port = htons(port);

    char *requestData = (char *)malloc(BUFFERSIZE*sizeof(char));
    bzero(requestData, BUFFERSIZE);
    clen = sizeof(server_addr);

    //construct_request(request, filename);

    strcat(requestData, dataStr);
    printf("\nRequest Data: %s\n", requestData);

    gettimeofday(&start_timev,NULL);

    n = sendto(client_sfd, requestData, strlen(requestData), 0, (struct sockaddr *) &server_addr, clen);
    if (n < 0)
    {
      report_error("ERROR writing to socket", &server_sfd, &client_sfd);
    }

    FD_ZERO(&fds);             // Clear the fd set
    FD_SET(client_sfd, &fds);  // Set the client fd for select

    time_out.tv_sec = 5;
    time_out.tv_usec = 0;

    // Waiting until timeout for SYNACK
    if ( select(32, &fds, NULL, NULL, &time_out) == 0 )
    {
        printf("Response receiving timed out...\n");
    }
    else
    {
        bzero(buffer, BUFFERSIZE);

        n = recvfrom(client_sfd, buffer, BUFFERSIZE, 0, (struct sockaddr *) &server_addr, &clen);
        if (n < 0)
        {
          report_error("ERROR reading from socket", &server_sfd, &client_sfd);
        }
        gettimeofday(&end_timev,NULL);

        printf("\nNumber of bytes received: %d\n", n);

        printf("Response:-\n%s\n",buffer);
        //printf("return code:-\n%s\n",get_status_code(buffer));

        printf("\nRequest sent at: %ld seconds and %ld microseconds\n", start_timev.tv_sec, start_timev.tv_usec);
        printf("\nLast response received at: %ld seconds and %ld microseconds\n", end_timev.tv_sec, end_timev.tv_usec);
    }

    free(requestData);

    close(client_sfd);
    close(server_sfd);
}

/*
 * Main function
 */
void main(int argc, char *argv[])
{
  if (argc < 4)
  {
     fprintf(stderr,"usage %s <HOST> <PORT> <MESSAGE>\n", argv[0]);
     exit(0);
   }

  run_client(argv[1], argv[2], argv[3]);
}
