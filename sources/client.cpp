#define _POSIX_C_SOURCE 199309
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAXDATASIZE 100 // max number of bytes we can get at once

// TODO: add documentation

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

char *read_command(void)
{
    char *input = NULL;   // we set to NULL so getline will allocate a buffer for storing the line
    size_t buff_size = 0; // no need to allocate an actual number here, getline allocates a size as needed

    if (getline(&input, &buff_size, stdin) == -1)
    { // getline takes care of all allocations and reallocations while trying to read a string from stdin, for more :https://man7.org/linux/man-pages/man3/getline.3.html
        if (feof(stdin))
        {
            // we chillin.
        }
        else
        {
            perror("getline");
            exit(1);
        }
    }

    return input;
}

// recieiving data from server will be in a seperate thread
void *recv_thread_handler(void *arg)
{
    char buffer[2048];
    bzero(buffer, 2048);
    int n;
    int new_sock = *((int *)arg);
    while ((n = recv(new_sock, buffer, 2048, 0)) != -1)
    {
        if (n == -1)
        {
            perror("recv");
        }
        printf("%s\n", buffer);
        bzero(buffer, 2048);
    }
    pthread_exit(NULL);
}

// sending data to server wll be in a seperate thread
void *send_thread_handler(void *arg)
{
    char *commands;
    int n_sock = *((int *)arg);
    while (1)
    {
        commands = read_command();
        if (send(n_sock, commands, strlen(commands), 0) == -1)
        {
            perror("send");
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    pthread_t new_thread[2]; // array of two thread, [0] will be the recieving thread, [1] will be the sending thread

    // loop through all the results and connect to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure

    pthread_create(&new_thread[0], NULL, recv_thread_handler, &sockfd);
    pthread_create(&new_thread[1], NULL, send_thread_handler, &sockfd);

    pthread_join(new_thread[0], NULL);
    pthread_join(new_thread[1], NULL);

    close(sockfd);

    return 0;
}