#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <stdint.h>
#include <math.h>
//contains value of each octet of the broadcast address
//contains value of each octet of the network address
struct info
{
    //initial string containing read ip
    std::string ip;
    //initial string containing read mask
    std::string mask;
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

struct octetInfo
{
    int ip;
    int mask;
    int net;
    int broadcast;

};

int netcalc (int, int);
int broadcastCalc(int , int);
void *createParentThreads(void *structVoidPtr);
void *performCalcs(void *anotherVoidPtr);
void processStrings (info *, int&, int&, int&, int&, int&, int&, int&, int&);
int main()
{
    //to be used to calculate number of zeroes on each octet of the mask
    static std::vector<info> readVector;       //need to make this static
    std::string readIp;
    std::string readMask;  
    while (std::cin>>readIp)
    {
        std::cin>>readMask;
        info temp;
        temp.ip = readIp;
        temp.mask = readMask;
        readVector.push_back(temp);
    }
    int size = readVector.size();
    //std::cout<<size;
    pthread_t tid[size];
   //create parent threads and pass struct
   for ( int i = 0; i < size; i++)
   {
       if (pthread_create(&tid[i], NULL, createParentThreads, &readVector[i]))
        {
            fprintf(stderr, "error creating thread\n");
            return 1;
        }
    } 

    for ( int i = 0; i < size; i++)
        pthread_join(tid[i], NULL);
    
    for( int i = 0; i < size; i ++)
    {
        /*
        std::cout<<'\n';
        std::cout<<readVector[i].firstNetworkOctet<<' ';
        std::cout<<readVector[i].secondNetworkOctet<<' ';
        std::cout<<readVector[i].thirdNetworkOctet<<' ';
        std::cout<<readVector[i].FourthNetworkOctet;
        std::cout<<'\n';
        std::cout<<readVector[i].firstBroadcastOctet<<' ';
        std::cout<<readVector[i].secondBroadcastOctet<<' ';
        std::cout<<readVector[i].thirdBroadcastOctet<<' ';
        std::cout<<readVector[i].fourthBroadcastOctet<<'\n';
        std::cout<<'\n';
        std::cout<<readVector[i].minHost;
        std::cout<<'\n';
        std::cout<<readVector[i].maxHost;
        std::cout<<'\n';
        */
        //std::cout<<readVector[i].totalHosts<<'\n';
        //prints results out
        std::cout<<"IP Address: "<<readVector[i].firstIpOctet<<'.'<<readVector[i].secondIpOctet<<'.'<<readVector[i].thirdIpOctet<<'.'<<readVector[i].fourthIpOctet<<'\n';
        std::cout<<"Subnet: "<<readVector[i].firstMaskOctet<<'.'<<readVector[i].secondMaskOctet<<'.'<<readVector[i].thirdMaskOctet<<'.'<<readVector[i].fourthMaskOctet<<'\n';
        std::cout<<"Network: "<<readVector[i].firstNetworkOctet<<'.'<<readVector[i].secondNetworkOctet<<'.'<<readVector[i].thirdNetworkOctet<<'.'<<readVector[i].FourthNetworkOctet<<'\n';
        std::cout<<"Broadcast: "<<readVector[i].firstBroadcastOctet<<'.'<<readVector[i].secondBroadcastOctet<<'.'<<readVector[i].thirdBroadcastOctet<<'.'<<readVector[i].fourthBroadcastOctet<<'\n';
        std::cout<<"HostMin: "<<readVector[i].firstNetworkOctet<<'.'<<readVector[i].secondNetworkOctet<<'.'<<readVector[i].thirdNetworkOctet<<'.'<<readVector[i].minHost<<'\n';
        std::cout<<"HostMax: "<<readVector[i].firstBroadcastOctet<<'.'<<readVector[i].secondBroadcastOctet<<'.'<<readVector[i].thirdBroadcastOctet<<'.'<<readVector[i].maxHost<<'\n';
        std::cout<<"# Hosts: "<<readVector[i].totalHosts<<'\n';
        std::cout<<'\n';
    }
    
   
    return 0;
}

void *createParentThreads(void *structVoidPtr)
{
    int arrayOfZeroes [] = { 255, 254, 252, 248, 240, 224, 192, 128};
    int ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4, zeroes;
    struct info *infoPtr = (struct info *)structVoidPtr;
    processStrings(infoPtr, ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4);    //turns string into ints and saves info to struct ptr

    std::vector<octetInfo> octetVector;
    octetInfo temp;
    temp.ip = ip1;
    temp.mask = mask1;
    octetVector.push_back(temp);

    temp.ip = ip2;
    temp.mask = mask2;
    octetVector.push_back(temp);

    temp.ip=ip3;
    temp.mask = mask3;
    octetVector.push_back(temp);

    temp.ip = ip4;
    temp.mask = mask4;
    octetVector.push_back(temp);
    
    pthread_t tid[4];
    for (int i = 0; i < 4; i++)
    {
        if(pthread_create(&tid[i], NULL, performCalcs, &octetVector[i]))
        {
            fprintf(stderr, "error\n");
                    return NULL;
        }
    }
    for (int i = 0; i < 4; i++)
       pthread_join(tid[i], NULL);

   infoPtr->firstNetworkOctet = octetVector[0].net;     
   infoPtr->secondNetworkOctet = octetVector[1].net;
   infoPtr->thirdNetworkOctet = octetVector[2].net;
   infoPtr->FourthNetworkOctet = octetVector[3].net;

   infoPtr->firstBroadcastOctet = octetVector[0].broadcast;
   infoPtr->secondBroadcastOctet = octetVector[1].broadcast;
   infoPtr->thirdBroadcastOctet = octetVector[2].broadcast;
   infoPtr->fourthBroadcastOctet = octetVector[3].broadcast;
 
    //calculate hostmin
    if(infoPtr->FourthNetworkOctet == 255)
        infoPtr->minHost = 0;
    else if(infoPtr->FourthNetworkOctet != 255)
        infoPtr->minHost = infoPtr->FourthNetworkOctet + 1;
    //calculate hostmax 
    if(infoPtr->fourthBroadcastOctet == 0)
        infoPtr->maxHost = 255;
    else if(infoPtr->fourthBroadcastOctet != 0)
        infoPtr->maxHost = infoPtr->fourthBroadcastOctet -1;

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

void *performCalcs (void *anotherVoidPtr)
{
    struct octetInfo *octetInfoPtr = (struct octetInfo *) anotherVoidPtr;
    octetInfoPtr->net = netcalc(octetInfoPtr->ip, octetInfoPtr->mask);
    octetInfoPtr->broadcast = broadcastCalc(octetInfoPtr->net, octetInfoPtr->mask);
    return NULL;
}


int netcalc (int a, int b)
{
    return (a & b); 
}

int broadcastCalc ( int a, int b)
{
    return ( a | ~b) + 256;
}
