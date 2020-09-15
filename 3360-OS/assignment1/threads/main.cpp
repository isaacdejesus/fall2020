#include <iostream>
#include <string>


int firstBroadcastOctet;
int secondBroadcastOctet;
int thirdBroadcastOctet;
int fourthBroadcastOctet;

int firstNetworkOctet;
int secondNetworkOctet;
int thirdNetworkOctet;
int FourthNetworkOctet;


void processString(std::string);
int main()
{
    //to be used to calculate number of zeroes on each octet of the mask
    int arrayOfZeroes [] = { 255, 254, 252, 248, 240, 224, 192, 128};

    int firstIpOctet;
    int secondIpOctet;
    int thirdIpOctet;
    int fourthIpOctet;

    int firstMaskOctet;
    int secondMaskOctet;
    int thirdMaskOctet;
    int fourthMaskOctet;


    std::string ip;
    std::string mask;
    while (std::cin>>ip)
    {
        std::cin>>mask;
    }

    return 0;
}

void processString(std::string)
{
    //will process process string and create a thread for each entry
    //then pass each octet of the ip to a function to create a new thread.
    
}

