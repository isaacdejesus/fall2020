#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>

static pthread_mutex_t semA;
static pthread_mutex_t semB;
static sem_t mysem;
struct info
{
    std::string ip;
    std::string mask;
    std::string ipCopy;
    std::string maskCopy;

      //ip ints
    int firstIpOctet;
    int secondIpOctet;
    int thirdIpOctet;
    int fourthIpOctet;
    //network
    int firstMaskOctet;
    int secondMaskOctet;
    int thirdMaskOctet;
    int fourthMaskOctet;
    //contain each individual octet of the resulting broadcast address
    int firstBroadcastOctet;
    int secondBroadcastOctet;
    int thirdBroadcastOctet;
    int fourthBroadcastOctet;
    //constain each individual octet of the resulting network address
    int firstNetworkOctet;
    int secondNetworkOctet;
    int thirdNetworkOctet;
    int FourthNetworkOctet;
    int minHost;
    int maxHost;
    int totalHosts;
};

void *createParentThreads(void *structVoidPtr);
void *netMinCalc(void *structVoidPtr);
void *broadMaxCacl(void *structVoidPtr);
void processStrings (info *, int&, int&, int&, int&, int&, int&, int&, int&);

int main()
{
    unsigned int initial_value = 0;
    int totalLines;
    std::string readIp;
    std::string readMask;
    info temp;
    std::cin>>totalLines;
    pthread_t tid[totalLines];
    pthread_mutex_init(&semA, NULL);
    pthread_mutex_init(&semB, NULL);
    sem_init(&mysem, O_CREAT | 0600, initial_value);
    for(int i = 0; i < totalLines; i++)
    {
        //lock?
        pthread_mutex_lock(&semA);
        std::cin>>readIp;
        std::cin>>readMask;
        temp.ip = readIp;
        temp.mask = readMask;
        temp.ipCopy = readIp;
        temp.maskCopy = readMask;

        if(pthread_create(&tid[i], NULL, createParentThreads, &temp))
        {
            fprintf(stderr, "error creating thread\n");
            return 1;
        }
    }

    for( int i = 0; i < totalLines; i++)
        pthread_join(tid[i], NULL);

    return 0;
}


void *createParentThreads(void *structVoidPtr)
{
    struct info *infoptr = (struct info *)structVoidPtr;
    info *infoPtr = new info;
    infoPtr->ip = infoptr->ip;
    infoPtr->mask = infoptr->mask;
    infoPtr->ipCopy = infoptr->ipCopy;
    infoPtr->maskCopy = infoptr->maskCopy;
    pthread_mutex_unlock(&semA);
    int arrayOfZeroes [] = { 255, 254, 252, 248, 240, 224, 192, 128};
    int ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4, zeroes;
    processStrings(infoPtr, ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4);

    //calc number of hosts
    zeroes = 0;
    if(mask1 == 0)
        zeroes = zeroes + 8;
    if(mask2 == 0)
        zeroes = zeroes + 8;
    if(mask3 == 0)
        zeroes = zeroes + 8;
    if(mask4 == 0)
        zeroes = zeroes + 8;

    for(int i = 0; i < 8; i++)
    {
        if(mask1 == arrayOfZeroes[i])
        {
            zeroes = zeroes + i;
            break;
        }
    }
    
    for(int i = 0; i < 8; i++)
    {
        if(mask2 == arrayOfZeroes[i])
        {
            zeroes = zeroes + i;
            break;
        }
    }
    
    for(int i = 0; i < 8; i++)
    {
        if(mask3 == arrayOfZeroes[i])
        {
            zeroes = zeroes + i;
            break;
        }
    }
    
    for(int i = 0; i < 8; i++)
    {
        if(mask4 == arrayOfZeroes[i])
        {
            zeroes = zeroes + i;
            break;
        }
    }

    infoPtr->totalHosts = pow(2, zeroes) - 2;
    //create grandbabies
    pthread_t tid;
    if(pthread_create(&tid, NULL, netMinCalc, infoPtr))
        {
            fprintf(stderr, "err\n");
            return NULL;
        }
    pthread_join(tid, NULL);
    pthread_t tid1;
    if(pthread_create(&tid1, NULL, broadMaxCacl, infoPtr))
        {
            fprintf(stderr, "err\n");
            return NULL;
        }
    pthread_join(tid1, NULL);
    //std::cout<<infoPtr->ip<<'\n';
    pthread_mutex_lock(&semB);
    std::cout<<"IP Address: "<<infoPtr->ipCopy<<'\n';
    std::cout<<"Subnet: "<<infoPtr->maskCopy<<'\n';
    std::cout<<"Network: "<<infoPtr->firstNetworkOctet<<'.'<<infoPtr->secondNetworkOctet<<'.'<<infoPtr->thirdNetworkOctet<<'.'<<infoPtr->FourthNetworkOctet<<'\n';
    std::cout<<"Broadcast: "<<infoPtr->firstBroadcastOctet<<'.'<<infoPtr->secondBroadcastOctet<<'.'<<infoPtr->thirdBroadcastOctet<<'.'<<infoPtr->fourthBroadcastOctet<<'\n';
    std::cout<<"HostMin: "<<infoPtr->firstNetworkOctet<<'.'<<infoPtr->secondNetworkOctet<<'.'<<infoPtr->thirdNetworkOctet<<'.'<<infoPtr->minHost<<'\n';
    std::cout<<"HostMax: "<<infoPtr->firstBroadcastOctet<<'.'<<infoPtr->secondBroadcastOctet<<'.'<<infoPtr->thirdBroadcastOctet<<'.'<<infoPtr->maxHost<<'\n';
    std::cout<<"# Hosts: "<<infoPtr->totalHosts<<'\n';
    std::cout<<'\n';
    pthread_mutex_unlock(&semB);
    return NULL;
}

void processStrings (info *ptr, int &firstIp, int &secondIp, int &thirdIp, int &fourthIp, int &firstMask, int &secondMask, int &thirdMask, int &fourthMask )
{
    int loc;
    std::string temp;
    //first octet ip
    loc = ptr->ip.find_first_of('.');
    temp = ptr->ip.substr(0, loc);
    ptr->firstIpOctet = stoi(temp);
    firstIp = stoi(temp);
    ptr->ip.erase(0, loc + 1);

    //second octet ip
    loc = ptr->ip.find_first_of('.');
    temp = ptr->ip.substr(0,loc);
    ptr->secondIpOctet = stoi(temp);
    secondIp = stoi(temp);
    ptr->ip.erase(0, loc + 1);

    //third octet ip
    loc = ptr->ip.find_first_of('.');
    temp = ptr->ip.substr(0,loc);
    ptr->thirdIpOctet = stoi(temp);
    thirdIp = stoi(temp);
    ptr->ip.erase(0, loc + 1);

    //fourth octet ip
    ptr->fourthIpOctet = stoi(ptr->ip);
    fourthIp = stoi(ptr->ip);

    //first octet mask
    loc = ptr->mask.find_first_of('.');
    temp = ptr->mask.substr(0, loc);
    ptr->firstMaskOctet = stoi(temp);
    firstMask = stoi(temp);
    ptr->mask.erase(0, loc + 1);

    //second octet mask
    loc = ptr->mask.find_first_of('.');
    temp = ptr->mask.substr(0, loc);
    ptr->secondMaskOctet = stoi(temp);
    secondMask =stoi(temp);
    ptr->mask.erase(0, loc + 1);
    
    //third octet mask
    loc = ptr->mask.find_first_of('.');
    temp = ptr->mask.substr(0, loc);
    ptr->thirdMaskOctet = stoi(temp);
    thirdMask = stoi(temp);
    ptr->mask.erase(0, loc + 1);

    //fourth octet mask
    ptr->fourthMaskOctet = stoi(ptr->mask);
    fourthMask = stoi(ptr->mask);
    //std::cout<<firstMask<<secondMask<<thirdMask<<fourthMask<<'\n';
}

void *netMinCalc (void *structVoidPtr)
{
    struct info *infoPtr = (struct info*) structVoidPtr;
    infoPtr->firstNetworkOctet = infoPtr->firstIpOctet & infoPtr->firstMaskOctet;
    infoPtr->secondNetworkOctet = infoPtr->secondIpOctet & infoPtr->secondMaskOctet;
    infoPtr->thirdNetworkOctet = infoPtr->thirdIpOctet & infoPtr->thirdMaskOctet;
    infoPtr->FourthNetworkOctet = infoPtr->fourthIpOctet & infoPtr->fourthMaskOctet;
    sem_post(&mysem);
    //sem_destroy(&mysem);
    //calc hostmin
    //calculate hostmin
    if(infoPtr->FourthNetworkOctet == 255)
        infoPtr->minHost = 0;
    else if(infoPtr->FourthNetworkOctet != 255)
        infoPtr->minHost = infoPtr->FourthNetworkOctet + 1;

    return NULL;
}

void *broadMaxCacl ( void *structVoidPtr)
{
    sem_wait(&mysem);
    struct info *infoPtr = (struct info*) structVoidPtr;
    infoPtr->firstBroadcastOctet = (infoPtr->firstNetworkOctet | ~infoPtr->firstMaskOctet) + 256;
    infoPtr->secondBroadcastOctet = (infoPtr->secondNetworkOctet | ~infoPtr->secondMaskOctet) + 256;
    infoPtr->thirdBroadcastOctet = (infoPtr->thirdNetworkOctet | ~infoPtr->thirdMaskOctet)+ 256;
    infoPtr->fourthBroadcastOctet = (infoPtr->FourthNetworkOctet| ~infoPtr->fourthMaskOctet) + 256;

    //calc max
    if(infoPtr->fourthBroadcastOctet == 0)
        infoPtr->maxHost = 255;
    else if(infoPtr->fourthBroadcastOctet != 0)
        infoPtr->maxHost = infoPtr->fourthBroadcastOctet -1;
    return NULL;
}