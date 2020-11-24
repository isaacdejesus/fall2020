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

void fireman(int);
void processStrings(std::string&, std::string& , int&, int&, int&, int&, int&, int&, int&, int&);
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
            int arrayOfZeroes [] = {255, 254, 252, 248, 240, 224, 192, 128};
            int ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4, zeroes;
            if(newsockfd < 0)
                error("ERROR on accept");
            n = read(newsockfd,&msg1,sizeof(struct message));
            if(n < 0)
                error("Error reading from socket");
            std::string ip(msg1.ip);
            std::string mask(msg1.mask);
            processStrings(ip, mask, ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4);
            
            msg2.returnedFirstNetworkOctet = netcalc(ip1, mask1);
            msg2.returnedSecondNetworkOctet = netcalc(ip2, mask2);
            msg2.returnedThirdNetworkOctet = netcalc(ip3, mask3);
            msg2.returnedFourthNetworkOctet = netcalc(ip4, mask4);
            msg2.returnedFirstBroadcastOctet = broadcastCalc(ip1, mask1);
            msg2.returnedSecondBroadcastOctet= broadcastCalc(ip2, mask2);
            msg2.returnedThirdBroadcastOctet = broadcastCalc(ip3, mask3);
            msg2.returnedFourthBroadcastOctet = broadcastCalc( ip4, mask4);
           
            //calc max host
            if(broadcastCalc(ip4, mask4) == 0)
               msg2.returnedMaxHost = 255;
            else if (broadcastCalc( ip4, mask4) != 0)
                msg2.returnedMaxHost = broadcastCalc(ip4, mask4) - 1;

            //calc min host
            if(netcalc(ip4, mask4) == 255)
                msg2.returnedMinHost = 0;
            else if (netcalc(ip4, mask4) != 255)
                msg2.returnedMinHost = netcalc(ip4, mask4) + 1;
               
            //calc hosts 
            zeroes = 0; 
            if(mask1 == 0)
                zeroes = zeroes + 8;
            if(mask2 == 0)
                zeroes = zeroes + 8;
            if(mask3 == 0)
                zeroes = zeroes + 8;
            if(mask4 == 0)
                zeroes = zeroes + 8;
            
            for( int i = 0; i < 8; i++)
            {
                if(mask1 == arrayOfZeroes[i])
                {
                    zeroes = zeroes + i;
                    break;
                }
            }
            
            for( int i = 0; i < 8; i++)
            {
                if(mask2 == arrayOfZeroes[i])
                {
                    zeroes = zeroes + i;
                    break;
                }
            }

            for( int i = 0; i < 8; i++)
            {
                if(mask3 == arrayOfZeroes[i])
                {
                    zeroes = zeroes + i;
                    break;
                }
            }
            
            for( int i = 0; i < 8; i++)
            {
                if(mask4 == arrayOfZeroes[i])
                {
                    zeroes = zeroes + i;
                    break;
                }
            }
            msg2.returnedTotalHosts = pow(2,zeroes) - 2;
           
            //Write results to socket here
            //strcpy(msg2.buffer, "Messagew from server\0");
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
void processStrings(std::string &strIp, std::string &strMask, int &firstIp, int &secondIp, int &thirdIp, int &fourthIp, int &firstMask, int &secondMask, int &thirdMask, int &fourthMask)
{
    int loc;
    std::string temp;
    loc = strIp.find_first_of('.');
    temp = strIp.substr(0, loc);
    firstIp = stoi(temp);
    strIp.erase(0, loc + 1);

    loc = strIp.find_first_of('.');
    temp = strIp.substr(0, loc);
    secondIp = stoi(temp);
    strIp.erase(0, loc + 1);
    
    loc = strIp.find_first_of('.');
    temp = strIp.substr(0, loc);
    thirdIp = stoi(temp);
    strIp.erase(0, loc + 1);

    fourthIp = stoi(strIp);

    loc = strMask.find_first_of('.');
    temp = strMask.substr(0, loc );
    firstMask = stoi(temp);
    strMask.erase(0, loc + 1);

    loc = strMask.find_first_of('.');
    temp = strMask.substr(0, loc);
    secondMask = stoi(temp);
    strMask.erase(0, loc + 1); 
    
    loc = strMask.find_first_of('.');
    temp = strMask.substr(0, loc);
    thirdMask = stoi(temp);
    strMask.erase(0, loc + 1);

    fourthMask = stoi( strMask);
}

int netcalc( int a, int b)
{
    return (a & b);
}

int broadcastCalc(int a, int b)
{
    return (a | ~b) + 256;
} 
