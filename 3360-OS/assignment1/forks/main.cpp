#include <iostream>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <sys/wait.h>

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
void calculate( std::string &, int, int);
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
    pid_t processId;
    std::ifstream readFile;
    std::string var;
    while(std::cin>>ip)
    {
        std::cin>>mask;
        processId = fork();
        if(processId == 0)
        {
            containsInfo(ip, mask);
            _exit(0);
        }
        wait(0);
        readFile.open("firstOctetNet.txt");
        if(readFile.is_open())
        {
            while(!readFile.eof())
            {
                readFile>>var;
                std::cout<<var;
            }
        }
        readFile.close();
    }

    return 0;
}

//takes ip & mask and splits string into octets
void containsInfo(std::string IP, std::string Mask)
{
    std::string c_octet;
    //call processStrings to fill in values of octets
    processStrings(firstOctetIp,IP, Mask);
    pid_t pid;
    //fork for first octet
    for  (int i = 0; i < 3; i++)
    {
        pid = fork();
        if( pid == 0)
        {
            if(i == 0)
            {
            calculate(networkAddress, firstOctetIp, firstOctetMask); 
           // std::cout<< networkAddress;
            //calculateBroadcastAddress();
            ////write to file
            std::ofstream writeTo("firstOctetNet.txt");
            if(writeTo.is_open())
            {
                writeTo<<networkAddress;
                writeTo.close();
            }
            

            _exit(0);
            }
        }
        for (int i = 0; i < 3; i++)
            wait(0);
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

void calculate(std::string &aString, int ip, int mask)
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


