#include <iostream>

int main()
{
    std::string x = "10101110";
    int zero = 0;
    for(int i = 0; i < 8; i++)
        if(x[i] == '0')
            zero++;
    std::cout<<zero;
    return 0;
}
