#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
int nAccount1 = 0, nAccount2 = 0;
void transfer(int a);
DWORD WINAPI ThreadExecutiveZGS(LPVOID lpParameter);
int main() {
	 HANDLE hThread[2];
	int nPID0 = 0, nPID1 = 1;
	if ((hThread[0] = CreateThread(NULL, 0, ThreadExecutiveZGS, &nPID0, 0, NULL)) == NULL) {
		printf("线程 ThreadExecutiveZGS-0 创建失败！ 创建失败！ \n");
		exit(0);
	}
	if ((hThread[1] = CreateThread(NULL, 0, ThreadExecutiveZGS, &nPID1, 0, NULL)) == NULL) {
		printf("线程 ThreadExecutiveZGS-1 创建失败！ 创建失败！ \n");
		exit(0);
	}
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
        //printf("nRandom = %d\n", nRandom);
        //printf("nA1 = %d\nnA2 = %d\n", nAccount1, nAccount2);
    }while ((nAccount1 + nAccount2) == 0);
    printf("nLoop = %d\n",nLoop);
}

DWORD WINAPI ThreadExecutiveZGS(LPVOID lpParameter) {
	int *pID = (int*)lpParameter;
	transfer(*pID);
	return 0;
}

