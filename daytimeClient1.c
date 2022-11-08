#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {
    char recvbuffer[BUFSIZE];
    int numBytes = 0;

    if(argc < 3)
    DieWithUserMessage("Parameter (s)",
        "<Server Address> <Server Port>");
    
    char *servIP =argv[1];

    in_port_t servPort = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock < 0)
        DieWithSystemMessage("socket() failed");

    // Construct the server address structure
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;

    int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);

    if(rtnVal == 0)
        DieWithUserMessage("inet_pton() failed", "invalid address string");
    else if(rtnVal < 0)
        DieWithSystemMessage("inet_pton() failed");
    servAddr.sin_port = htons(servPort);

    if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("connect() failed");
    
    while ((numBytes = recv(sock, recvbuffer, BUFSIZE - 1, 0)) > 0) {
        recvbuffer[numBytes] = '\0';
        fputs(recvbuffer, stdout);
        } //end while
    
    if(numBytes < 0)
        DieWithSystemMessage("recv() failed");
	else if (numBytes == 0)
		DieWithUserMessage("recv()", "connection closed prematurely");

    fputc('\n', stdout);

    close(sock);
    exit(0);
    }
