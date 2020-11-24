#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}
void fireman (int);
int main(int argc, char *argv[])
{
    int sockfd,newsockfd, portno, clilen;
    char buffer[256];
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
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    signal(SIGCHLD, fireman);
    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,(socklen_t *)&clilen);
        if(fork() == 0)
        {
            if(newsockfd < 0)
                error("ERROR on accept");
            bzero(buffer, 256);
            n = read(newsockfd,buffer, 225);
            if(n < 0)
                error("Error reading from socket");
            printf(" here is the msg: %s\n", buffer);
            n = write(newsockfd, "i got the message", 18);
            if(n < 0) error("ERROR writing to socket");
            close(newsockfd);
            _exit(0);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}
void fireman (int x)
{
    while(waitpid(-1, NULL, WNOHANG) > 0)
        {
        }
}

