//#include <unistd.h>
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

    int port;
    const char *host;
};

struct message2
{
    int returnResult;
};

void *createThreads(void *structPtr);
int main(int argc, char *argv[])
{
     if(argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    static std::vector<message> readVector;
    message one, two, three;
    strcpy(one.first ,  "hello");
    //one.second = "There";
    one.port =atoi(argv[2]);
    one.host = argv[1];
    strcpy(two.first , "why");
    //two.second = "are";
    two.port =  atoi(argv[2]);
    two.host = argv[1];
    strcpy(three.first , "you");
    //three.second = "not working";
    three.host = argv[1];
    three.port = atoi(argv[2]);
    readVector.push_back(one);
    readVector.push_back(two);
    readVector.push_back(three);
    pthread_t tid[3];
    //create parent threads
    for(int i = 0; i < 3; i++)
        if(pthread_create(&tid[i], NULL, createThreads, &readVector[i]))
        {
            fprintf(stderr, "error, creating thread\n");
            return 1;
        }
    for(int i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);

    for(int i = 0; i < 3; i ++)
        {
            std::cout<<readVector[i].result<<'\n';
        }
                    
    return 0;
}
void *createThreads(void *structPtr)
{
    struct message *messagePtr = (struct message *) structPtr;
    int sockfd, portno, n, m;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    portno = messagePtr->port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(messagePtr->host);
    if(server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    n = write(sockfd, messagePtr,sizeof( struct message));
    if (n < 0 )
        error("ERROR writing to socket");
    struct message2 returnedMsg;
    n = read(sockfd, &returnedMsg, sizeof(struct message2));
    if(n < 0)
        error("Error reading from socket");
    messagePtr->result = returnedMsg.returnResult;
    return NULL;
}
