#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <stdint.h>
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

void *createParentThreads(void *structVoidPtr);
void processStrings (info *, int&, int&, int&, int&, int&, int&, int&, int&);
int main()
{
    //to be used to calculate number of zeroes on each octet of the mask
    int arrayOfZeroes [] = { 255, 254, 252, 248, 240, 224, 192, 128};
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
   /* 
    std::cout<<readVector[4].firstIpOctet;
    std::cout<<readVector[4].secondIpOctet;
    std::cout<<readVector[4].thirdIpOctet;
    std::cout<<readVector[4].fourthIpOctet;
    */
    
    for( int i = 0; i < size; i ++)
    {
        std::cout<<'\n';
        std::cout<<readVector[i].firstIpOctet;
        std::cout<<readVector[i].secondIpOctet;
        std::cout<<readVector[i].thirdIpOctet;
        std::cout<<readVector[i].fourthIpOctet;
        std::cout<<'\n';
    }


    return 0;
}

void *createParentThreads(void *structVoidPtr)
{
    int ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4;
    struct info *infoPtr = (struct info *)structVoidPtr;
    processStrings(infoPtr, ip1, ip2, ip3, ip4, mask1, mask2, mask3, mask4);    //turns string into ints and saves info to struct ptr
    //now create 4 threads to perform the operations and 
    /*
    static std::vector<octetInfo> octetVector;
    octetInfo temp;
    temp.ip = infoPtr->firstIpOctet; 
    temp.mask = infoPtr->firstMaskOctet;
    octetVector.push_back(temp);

    temp.ip = infoPtr->secondIpOctet;
    temp.mask = infoPtr->secondMaskOctet;
    octetVector.push_back(temp);

    temp.ip = infoPtr->thirdIpOctet;
    temp.mask = infoPtr->thirdMaskOctet;
    octetVector.push_back(temp);

    temp.ip = infoPtr->fourthIpOctet;
    temp.mask = infoPtr->fourthMaskOctet;
    octetVector.push_back(temp);
    */ 
    /*
    for ( int i = 0; i < 4; i ++)
    {       
        std::cout<<octetVector[i].ip<<' ';
        std::cout<<'\n';
    }*/
    /*
    std::cout<< infoPtr->firstIpOctet<<' ';
    std::cout<< infoPtr->secondIpOctet<< ' ';
    std::cout<< infoPtr->thirdIpOctet<<' ';
    std::cout<< infoPtr->fourthIpOctet;
    std::cout<<'\n';
    */
    /*
    std::cout<<ip1<<ip2<<ip3<<ip4<<'\n';
    std::cout<<mask1<<mask2<<mask3<<mask4<<'\n';
    */
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
    fourthIp = stoi(temp);

    //first octet mask
    loc = ptr->mask.find_first_of(',');
    temp = ptr->mask.substr(0, loc);
    ptr->firstMaskOctet = stoi(temp);
    firstMask = stoi(temp);
    ptr->mask.erase(0, loc + 1);

    //second octet mask
    loc = ptr->mask.find_first_of(',');
    temp = ptr->mask.substr(0, loc);
    ptr->secondMaskOctet = stoi(temp);
    secondMask =stoi(temp);
    ptr->mask.erase(0, loc + 1);
    
    //third octet mask
    loc = ptr->mask.find_first_of(',');
    temp = ptr->mask.substr(0, loc);
    ptr->thirdMaskOctet = stoi(temp);
    thirdMask = stoi(temp);
    ptr->mask.erase(0, loc + 1);

    //fourth octet mask
    ptr->fourthMaskOctet = stoi(ptr->mask);
    fourthMask = stoi(temp);

    //std::cout<<firstIp<<secondIp<<thirdIp<<fourthIp<<'\n';
    std::cout<<ptr->firstIpOctet;
    std::cout<<ptr->secondIpOctet;
    std::cout<<ptr->thirdIpOctet;
    std::cout<<ptr->fourthIpOctet<<'\n';
}
