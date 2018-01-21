#include <stdio.h>
#include <stdlib.h>

#include <cRand64.h>
#include <string.h>

int main()
{
    char str[1024];
        strcpy(str,"Hello World How are you doing i am doing great wow!!!");

    printf("Hello world!\n");

    pRand64 rand = New_Rand64_Seed(5);

    while (1) {
        //Shuffle_String(rand,str,strlen(str));
        printf("%s\n",str);
    }
}
