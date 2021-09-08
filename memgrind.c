
#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "mymalloc.h"

int main()
{
     double testA, testB, testC, testD, testE = 0;
    printf("Start testcase A:\n");
    for (int i = 0; i <50; i++)
    {
        struct timeval current;
        int start, end;
        gettimeofday(&current, NULL);
        start = current.tv_usec;
        //test case A, malloc and immediately free a pointer
        for (int i = 0; i<119; i++)
        {
            void *q = malloc(1);
            free(q);
        }
        gettimeofday(&current, NULL);
        end = current.tv_usec;
        testA += (end-start);
    }
    testA = testA/50;
    printf("mean time: %f micro seconds\n", testA);
    printf("\nStart testcase B: \n");

    for (int i = 0; i < 50; i++)
    {
        struct timeval current;
        int start, end;
        gettimeofday(&current, NULL);
        start = current.tv_usec;
        //test case B, malloc 120 addresses, then free those addresses
        void *ptrs[120];

        for (int i = 0; i<119; i++)
        {
            ptrs[i] = malloc(1);
        }
        for (int i = 0; i<119; i++)
        {
            free(ptrs[i]);

        }
        gettimeofday(&current, NULL);
        end =  current.tv_usec;
        testB+= (end-start);

    }
    testB = testB/50;
    printf("mean time: %f micro seconds\n", testB);
    printf("\nStart testcase C: \n");


    for(int i =0; i < 50; i++)
    {
        //test case C, randomly malloc and free 120 addresses
        int m = 0; //count for mallocs
        int f = 0; //count for frees
        void *mallocs[120]; //array to save our malloced addresses
        int start,end;
        struct timeval current;
        gettimeofday(&current,NULL);
        start = current.tv_usec;
        for(int i = 0; i < 240; i++)
        {
            int random;
            random = rand() % 10; //random returns a random number. If we modulo that number with 10, we are guranteed a number between 0 and 9

            if(random < 5 && m < 120 ) //if the random number is less than 5 and we haven't malloc'd 120 addresses yet
            {
                mallocs[m] = malloc(1); //malloc a new 1 byte address
                m++; //increment


            }
            else if (f < m && m > 0) //if our frees are less than our mallocs and we have at least 1 malloc, free and increment f
            {
                free(mallocs[f]);
                f++;

            }
            else //otherwise malloc and increment m
            {
              mallocs[m] = malloc(1);
              m++;
            }


        }
        gettimeofday(&current,NULL);
        end = current.tv_usec;
        testC += (end-start);

    }
    testC = testC / 50;
    printf("mean time: %f micro seconds\n", testC);
    printf("\nStart testcase D: \n");


    for(int i = 0; i<50; i++)
    {

        //test case D: malloc varrying sizes and then free them
        void * mallocs[50];
        int start,end;
        struct timeval current;
        gettimeofday(&current,NULL);
        start = current.tv_usec;
        for(int j = 0; j<50; j++)
        {
            mallocs[j] = malloc(j);
        }
        for(int k = 0; k <50; k++)
        {
            free(mallocs[k]);
        }
        gettimeofday(&current, NULL);
        end = current.tv_usec;
        testD +=(end-start);
    }
    testD = testD / 50;
    printf("mean time: %f micro seconds\n", testD);
    printf("\nStart testcase E: \n");



    for(int i = 0; i < 50; i++)
    {
        //test case E: malloc larger sizes and then free
        void * mallocs2[10];
        int start,end;
        struct timeval current;
        gettimeofday(&current,NULL);
        start = current.tv_usec;
        for(int j = 0; j <10; j++)
        {
            mallocs2[j] = malloc(350);


        }
        for(int j = 0; j <= 10; j++)
        {
            free(mallocs2[j]);
        }
        gettimeofday(&current, NULL);
        end = current.tv_usec;
        testE += (end-start);
    }
    testE = testE / 50;
    printf("mean time: %f micro seconds\n", testD);
}
