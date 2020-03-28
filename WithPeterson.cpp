#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

int nAccount1 = 0, nAccount2 = 0;
int flag[2] = {FALSE};
int turn = -1;
void transfer(int a);
DWORD WINAPI ThreadExecutiveZGS(LPVOID lpParameter);
int main() {
    DWORD start,end;
	HANDLE hThread[2];
	int nPID0 = 0, nPID1 = 1;
    start = GetTickCount();
	if ((hThread[0] = CreateThread(NULL, 0, ThreadExecutiveZGS, &nPID0, 0, NULL)) == NULL) {
		printf("线程 ThreadExecutiveZGS-0 创建失败！ 创建失败！ \n");
		exit(0);
	}
	if ((hThread[1] = CreateThread(NULL, 0, ThreadExecutiveZGS, &nPID1, 0, NULL)) == NULL) {
		printf("线程 ThreadExecutiveZGS-1 创建失败！ 创建失败！ \n");
		exit(0);
	}
    end = GetTickCount();
    printf("Total time consuming is %lfs\n",(end-start));
	system("pause");
	return 0;
}

void transfer(int a) {
    int nLoop = 0;
	int nTemp1, nTemp2, nRandom;
    do {
        srand((int)time(0)*a);
        nRandom = rand();
        nTemp1 = nAccount1;
        nTemp2 = nAccount2;
        nAccount1 = nTemp1 + nRandom;
        nAccount2 = nTemp2 - nRandom;
        nLoop++;
        // printf("nRandom = %d\n", nRandom);
        // printf("nA1 = %d\nnA2 = %d\n", nAccount1, nAccount2);
    }while (nLoop < 5000000);
}

DWORD WINAPI ThreadExecutiveZGS(LPVOID lpParameter) {
	int *pID = (int*)lpParameter;
    if (*pID == 0){
        flag[0] = TRUE;
	    turn = 1;
	    while(flag[1] && turn == 1);
        transfer(*pID);
	    flag[0] = FALSE;
    }
    if (*pID == 1){
        flag[1] = TRUE;
        turn = 0;
        while(flag[0] && turn == 0);
        transfer(*pID);
        flag[1] = FALSE;
    } 
	return 0;
}

