#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <iomanip>
struct info
{
    int val1;
    int val2;
    float result;
};

void * addFunct(void *void_ptr);
void * subFunct(void *void_ptr);
void * multFunct(void *void_ptr);
void * divFunct(void *void_ptr);
int main()
{
    static std::vector <info> Vector;
    info temp;
    int in1, in2;
    while(std::cin>>in1)
    {
        std::cin>>in2;
        temp.val1 = in1;
        temp.val2 = in2;
        Vector.push_back(temp);
    }

    pthread_t tid[4];
    for ( int i = 0; i < 4; i++)
    {
        if(i == 0)
        {
            if(pthread_create(&tid[i], NULL, addFunct, &Vector[i]))
            {
                fprintf(stderr, "error\n");
                return 1;
            }
        }
        else if(i == 1)
        {
            if(pthread_create(&tid[i], NULL, subFunct, &Vector[i]))
            {
                fprintf(stderr, "error\n");
                return 1;
            }
        }

        else if(i == 2)
        {
            if(pthread_create(&tid[i], NULL, multFunct, &Vector[i]))
            {
                fprintf(stderr, "error\n");
                return 1;
            }
        }
        else if(i == 3)
        {
            if(pthread_create(&tid[i], NULL, divFunct, &Vector[i]))
            {
                fprintf(stderr, "error\n");
                return 1;
            }
        }

    }

    for (int i = 0; i < 4; i ++)
        pthread_join(tid[i], NULL);
    
    std::cout<< Vector[0].val1 <<" + " << Vector[0].val2<< " = " <<std::fixed << std::setprecision(2) << Vector[0].result<< '\n';
    std::cout<< Vector[1].val1 <<" - " << Vector[1].val2<< " = " <<std::fixed << std::setprecision(2) << Vector[1].result<< '\n';
    std::cout<< Vector[2].val1 <<" * " << Vector[2].val2<< " = " <<std::fixed << std::setprecision(2) << Vector[2].result<< '\n';
    std::cout<< Vector[3].val1 <<" / " << Vector[3].val2<< " = " <<std::fixed << std::setprecision(2) << Vector[3].result<< '\n';
    return 0;
}

void * addFunct(void *void_ptr)
{
    info *infoPtr = (info *) void_ptr;
    infoPtr->result = infoPtr->val1 + infoPtr->val2; 
    return NULL;
}

void * subFunct(void *void_ptr)
{
    info *infoPtr = (info *) void_ptr;
    infoPtr->result = infoPtr->val1 - infoPtr->val2;
    return NULL;
}

void * multFunct(void *void_ptr)
{
    info *infoPtr = (info *) void_ptr;
    infoPtr->result = infoPtr->val1 * infoPtr->val2;
    return NULL;
}

void * divFunct(void *void_ptr)
{
    info *infoPtr = (info *) void_ptr;
    if(infoPtr->val2 == 0)
        infoPtr->val2 = 0;
    else if( infoPtr->val2 != 0)
        infoPtr->result =(float)infoPtr->val1 / (float)infoPtr->val2;
    return NULL;
}
