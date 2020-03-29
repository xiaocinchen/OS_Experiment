#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

int nAccount1 = 0, nAccount2 = 0;
int flag[2] = {FALSE};
int turn = -1;
void transfer(int a);
DWORD WINAPI ThreadExecutiveZGS(LPVOID lpParameter);
int main()
{
    HANDLE hThread[2];
    DWORD start, end;
    int nPID0 = 0, nPID1 = 1;
    start = GetTickCount();
    if ((hThread[0] = CreateThread(NULL, 0, ThreadExecutiveZGS, &nPID0, 0, NULL)) == NULL){
        printf("线程 ThreadExecutiveZGS-0 创建失败！ 创建失败！ \n");
        exit(0);
    }
    if ((hThread[1] = CreateThread(NULL, 0, ThreadExecutiveZGS, &nPID1, 0, NULL)) == NULL){
        printf("线程 ThreadExecutiveZGS-1 创建失败！ 创建失败！ \n");
        exit(0);
    }
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    end = GetTickCount();
    printf("ac1 = %d\tac2 = %d\n", nAccount1, nAccount2);
    printf("Total Time Consuming is %dms\n", end - start);
    return 0;
}

void transfer(int a)
{
    int nTemp1, nTemp2, nRandom;
    srand((int)time(0) * a %100 );
    nRandom = rand();
    nTemp1 = nAccount1;
    nTemp2 = nAccount2;
    nAccount1 = nTemp1 + nRandom;
    nAccount2 = nTemp2 - nRandom;
}

DWORD WINAPI ThreadExecutiveZGS(LPVOID lpParameter)
{
    int *pID = (int *)lpParameter;
    int nLoop1 = 0, nLoop2 = 0;
    if (*pID == 0){
        do{
            flag[0] = TRUE;
            turn = 1;
            while (flag[1] && turn == 1);
            transfer(*pID);
            if (nAccount1 + nAccount2 != 0)
                break;
            nLoop1++;
            flag[0] = FALSE;
        } while (nLoop1 < 1000000);
    }
    if (*pID == 1){
        do{
            flag[1] = TRUE;
            turn = 0;
            while (flag[0] && turn == 0);
            transfer(*pID);
            if (nAccount1 + nAccount2 != 0)
                break;
            nLoop2++;
            flag[1] = FALSE;
        } while (nLoop2 < 1000000);
    }
    
    return 0;
}
