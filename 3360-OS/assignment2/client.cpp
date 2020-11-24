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
    char ip[100];
    std::string originalIp;
    char mask[100];
    std::string originalMask;

    int firstIpOctet;
    int secondIpOctet;
    int thirdIpOctet;
    int fourthIpOctet;

    int firstMaskOctet;
    int secondMaskOctet;
    int thirdMaskOctet;
    int fourthMaskOctet;

    int firstBroadcastOctet;
    int secondBroadcastOctet;
    int thirdBroadcastOctet;
    int fourthBroadcastOctet;

    int firstNetworkOctet;
    int secondNetworkOctet;
    int thirdNetworkOctet;
    int fourthNetworkOctet;

    int minHost;
    int maxHost;
    int totalHosts;
    
    const char  *host;
    int port;
};

struct message2
{
    int returnedFirstBroadcastOctet;
    int returnedSecondBroadcastOctet;
    int returnedThirdBroadcastOctet;
    int returnedFourthBroadcastOctet;

    int returnedFirstNetworkOctet;
    int returnedSecondNetworkOctet;
    int returnedThirdNetworkOctet;
    int returnedFourthNetworkOctet;

    int returnedMinHost;
    int returnedMaxHost;
    int returnedTotalHosts;
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
    std::string readIp;
    std::string readMask;
    while(std::cin>>readIp)
    {
        std::cin>>readMask;
        message temp;
        strcpy(temp.ip , readIp.c_str());
        temp.originalIp = readIp;
        strcpy(temp.mask , readMask.c_str());
        temp.originalMask = readMask;
        temp.host = argv[1];
        temp.port = atoi(argv[2]);
        readVector.push_back(temp);
    }
    int size = readVector.size();
    pthread_t tid[size];
    //create parent threads
    for(int i = 0; i < size; i++)
        if(pthread_create(&tid[i], NULL, createThreads, &readVector[i]))
        {
            fprintf(stderr, "error, creating thread\n");
            return 1;
        }
    for(int i = 0; i < size; i++)
        pthread_join(tid[i], NULL);

    for(int i = 0; i < size; i ++)
        {
            std::cout<<"IP Address: "<<readVector[i].originalIp<<'\n';
            std::cout<<"Subnet: "<<readVector[i].originalMask<<'\n';;
            std::cout<<"Network: "<<readVector[i].firstNetworkOctet<<'.'<<readVector[i].secondNetworkOctet<<'.'<<readVector[i].thirdNetworkOctet<<'.'<<readVector[i].fourthNetworkOctet<<'\n';
            std::cout<<"Broadcast: "<<readVector[i].firstBroadcastOctet<<'.'<<readVector[i].secondBroadcastOctet<<'.'<<readVector[i].thirdBroadcastOctet<<'.'<<readVector[i].fourthBroadcastOctet<<'\n';
            std::cout<<"HostMin: "<<readVector[i].firstNetworkOctet<<'.'<<readVector[i].secondNetworkOctet<<'.'<<readVector[i].thirdNetworkOctet<<'.'<<readVector[i].minHost<<'\n';
            std::cout<<"HostMax: "<<readVector[i].firstBroadcastOctet<<'.'<<readVector[i].secondBroadcastOctet<<'.'<<readVector[i].thirdBroadcastOctet<<'.'<<readVector[i].maxHost<<'\n';
            std::cout<<"# Hosts: "<<readVector[i].totalHosts<<'\n';
            std::cout<<'\n';
        }
                    
    return 0;
}
void *createThreads(void *structPtr)
{
    struct message *messagePtr = (struct message *) structPtr;
    int sockfd, portno, n;
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
    n = write(sockfd, messagePtr, sizeof(struct message));
    if (n < 0)
        error("ERROR writing to socket");
    struct message2 returnedMsg;
    n = read(sockfd, &returnedMsg, sizeof(struct message2));
    if(n < 0)
        error("Error reading from socket");
    messagePtr->firstBroadcastOctet = returnedMsg.returnedFirstBroadcastOctet; 
    messagePtr->secondBroadcastOctet = returnedMsg.returnedSecondBroadcastOctet;
    messagePtr->thirdBroadcastOctet = returnedMsg.returnedThirdBroadcastOctet;
    messagePtr->fourthBroadcastOctet = returnedMsg.returnedFourthBroadcastOctet;

    messagePtr->firstNetworkOctet = returnedMsg.returnedFirstNetworkOctet;
    messagePtr->secondNetworkOctet = returnedMsg.returnedSecondNetworkOctet;
    messagePtr->thirdNetworkOctet = returnedMsg.returnedThirdNetworkOctet;
    messagePtr->fourthNetworkOctet = returnedMsg.returnedFourthNetworkOctet;

    messagePtr->minHost = returnedMsg.returnedMinHost;
    messagePtr->maxHost = returnedMsg.returnedMaxHost;
    messagePtr->totalHosts = returnedMsg.returnedTotalHosts;
    return NULL;
}
