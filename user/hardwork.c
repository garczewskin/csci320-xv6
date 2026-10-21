#include "kernel/types.h"
#include "user/user.h"

#define N 3

int decide(int a, int b, int direction)
{
    if (direction)
        if (a < b) return 1; else return 0;
    else if (a<b) return 0; else return 1;
}
int hardWork()
{
    int bigFun[N][N];
    int bFun[3];
    for (int r = 0; r < N; ++r)
    {
        for (int c = 0; c < N; ++c)
        {
            bigFun[r][c] = r; 
        }
        bFun[r] = r; 
    }
    printf("initialized\n");
    int changes = 1;
    int total = 0;
    int direction = 0;
    for (;;)
    {
        while (changes)
        {
            changes = 0;
            for (int r = 0; r < N-1; ++r)
                for (int c =0; c < N; ++c)
                {
                    if (bigFun[r][c] > bigFun[r+1][c])
                    {
                        int temp = bigFun[r][c];
                        bigFun[r][c] = bigFun[r+1][c];
                        bigFun[r+1][c] = bFun[r]*temp;
                        changes++;
                        ++total;
                    }
                    else if (c < N-1 && bigFun[r][c] > bigFun[r][c+1])
                    {
                        int temp = bigFun[r][c];
                        bigFun[r][c] = bigFun[r][c+1];
                        bigFun[r][c+1] = temp;
                        changes++;
                        ++total;
                    }
                }
        }
        if (direction == 1) direction = 0; else direction = 1;
        changes = 1;
    }
    return total;
}

int main(int argc, char *argv[])
{
    int val;
/* uncomment the following to use strides */
/*
    if (argc >1)
        setstride(atoi(argv[1]));
*/
    printf("starting main\n");
    val = hardWork();
    printf("%d\n",val);
    return 0;
}
