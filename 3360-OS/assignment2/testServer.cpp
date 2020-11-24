#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct message
{
    char first[16];
    char second[16];

    int result;
    const char  *host;
    int port;
};

struct message2
{
    int returnedResult;
};

void fireman(int);
//void processStrings(message* , int&, int&, int&, int&, int&, int&, int&, int&);
int netcalc(int, int);
int broadcastCalc(int, int);

int main(int argc, char *argv[])
{
    int sockfd,newsockfd, portno, clilen;
    char buffer[256];
    struct message msg1;
    struct message2 msg2;        
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if(argc < 2)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,90);
    clilen = sizeof(cli_addr);
    //Create child processes
    signal(SIGCHLD, fireman);
    while(1)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,(socklen_t *)&clilen);
        if(fork() == 0)
        {
            if(newsockfd < 0)
                error("ERROR on accept");
            n = read(newsockfd,&msg1, sizeof(struct message));
            if(n < 0)
                error("Error reading from socket");
            std::string ip(msg1.first);
            msg2.returnedResult = ip.size(); //msg1.first.size() + msg1.second.size();
            //Write results to socket here
            n = write(newsockfd, &msg2,sizeof(struct message2));
            if(n < 0) error("ERROR writing to socket");
            close(newsockfd);
            _exit(0);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}

void fireman(int)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
    {
    }
}
/*
void processStrings(message *ptr, int &firstIp, int &secondIp, int &thirdIp, int &fourthIp, int &firstMask, int &secondMask, int &thirdMask, int &fourthMask)
{
    
    int loc;
    std::string temp;
    loc = ptr->ip.find_first_of('.');
    temp = ptr->ip.substr(0, loc);
    firstIp = stoi(temp);
    ptr->ip.erase(0, loc + 1);

    loc = ptr->ip.find_first_of('.');
    temp = ptr->ip.substr(0, loc);
    secondIp = stoi(temp);
    ptr->ip.erase(0, loc + 1);
    
    loc = ptr->ip.find_first_of('.');
    temp = ptr->ip.substr(0, loc);
    thirdIp = stoi(temp);
    ptr->ip.erase(0, loc + 1);

    fourthIp = stoi(ptr->ip);

    loc = ptr->mask.find_first_of('.');
    temp = ptr->mask.substr(0, loc );
    firstMask = stoi(temp);
    ptr->mask.erase(0, loc + 1);

    loc = ptr->mask.find_first_of('.');
    temp = ptr->mask.substr(0, loc);
    secondMask = stoi(temp);
    ptr->mask.erase(0, loc + 1); 
    
    loc = ptr->mask.find_first_of('.');
    temp = ptr->mask.substr(0, loc);
    thirdMask = stoi(temp);
    ptr->mask.erase(0, loc + 1);

    fourthMask = stoi( ptr->mask);
}
*/
int netcalc( int a, int b)
{
    return (a & b);
}

int broadcastCalc(int a, int b)
{
    return (a | ~b) + 256;
} 
