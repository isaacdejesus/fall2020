#include <iostream>
#include <unistd.h>
#include <stdlib.h>

int firstOctetIp;
int secondOctetIp;
int thirdOctetIp;
int fourthOctetIp;

int firstOctetMask;
int secondOctetMask;
int thirdOctetMask;
int fourthOctetMask;

std::string networkAddress;
std::string broadcastAddress;
std::string minHost;
std::string maxHost;
int numberOfHosts;

int arrayOfZeroes [] = {255, 254, 252, 248, 240, 224, 192, 128};
void containsInfo(std::string, std::string);
void processStrings(int &,std::string , std::string );
void calculate( std::string, int, int);
void calculateNetworkAddress(std::string &, int, int);
void calculateBroadcastAddress(int &, int, int);
void calculateMinHost(int);
void calculateMaxHost(int);
//void calculateNumberOfHosts();
//std::string octetToBinary();

int main()
{
    std::string ip;
    std::string mask;
    int processId;
    while(std::cin>>ip)
    {
        std::cin>>mask;
        processId = fork();
        if(processId == 0)
        {
            containsInfo(ip, mask);
            close(0);
        }
        else
            std::cout<< networkAddress;
    }

    return 0;
}

//takes ip & mask and splits string into octets
void containsInfo(std::string IP, std::string Mask)
{
    //call processStrings to fill in values of octets
    processStrings(firstOctetIp,IP, Mask);
    //fork for first octet
    if(fork() == 0)
    {
        calculate(networkAddress, firstOctetIp, firstOctetMask); 
        //calculateBroadcastAddress();
        close(0);
    }
    if(fork() == 0)
    {
        calculate(networkAddress, secondOctetIp, secondOctetMask);
        //calculateBroadcastAddress();
    }
    if(fork() == 0)
    {
        calculate(networkAddress, thirdOctetIp, thirdOctetMask);
        //calculateBroadcastAddress();
    }
    if(fork() == 0)
    {
        calculate(networkAddress, thirdOctetIp, thirdOctetMask);
        //calculateBroadcastAddress();
    }
    //back to parent octet to put together the network/broadcast addresses
    //find mix/max
    //number of hosts
}

void processStrings(int &f, std::string a, std::string b)
{ 
    int loc; 
    std::string temp;
    //first octet IP
    loc = a.find_first_of('.');
    temp = a.substr(0,loc);
    //firstOctetIp = stoi(temp);
    f = stoi(temp);
    a.erase(0,loc + 1);
    //std::cout<<a<<'\n';
    
    //second octet IP
    loc = a.find_first_of('.');
    temp = a.substr(0,loc);
    secondOctetIp = stoi(temp);
    a.erase(0, loc + 1);
    //std::cout<<a<<'\n';

    //third octet IP
    loc = a.find_first_of('.');
    temp = a.substr(0,loc);
    thirdOctetIp = stoi(temp);
    a.erase(0, loc + 1);
    //std::cout<<a<<'\n';
    
    //fourth octet IP
    fourthOctetIp = stoi(a);
    
    //first octet mask
    loc = b.find_first_of('.');
    temp = b.substr(0,loc);
    firstOctetMask = stoi(temp);
    b.erase(0, loc + 1);

    //second octet mask
    loc = b.find_first_of('.');
    temp = b.substr(0,loc);
    secondOctetMask = stoi(temp);
    b.erase(0, loc + 1);

    //third octet mask
    loc = b.find_first_of('.');
    temp = b.substr(0,loc);
    thirdOctetMask = stoi(temp);
    b.erase(0, loc + 1);

    //fourth octet
    fourthOctetMask = stoi(b);
}

void calculate(std::string aString, int ip, int mask)
{
    calculateNetworkAddress(aString, ip, mask);
    //calculateBroadcastAddress(ip, mask);
    //calculateMinHost(ip);
    //calculateMaxHost(ip);
    //calculateNumberOfHosts();
}

void calculateNetworkAddress(std::string &netAddr, int ip, int mask)
{   
    int val = ip & mask;
    //clear string 
    //netAddr.clear();
    netAddr.append(std::to_string(val));
    netAddr.append(".");
    std::cout<<netAddr;

}

void calculateBroadcastAddress(int &oct, int ip, int mask)
{
   oct = ip | !mask; 
}


