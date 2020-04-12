#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define M 5         /*进程数*/
#define N 3         /*资源数*/
#define True 1
#define False 0
/*M个进程对N类资源最大资源需求量*/
int Max[M][N] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
/*系统可用资源数*/
int Available[N] = {9, 5, 7};
/*M个进程已分配到的N类数量*/
int Allocation[M][N] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
/*M个进程已经得到N类资源的资源量*/
int Need[M][N] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
/*M个进程还需要N类资源的资源量*/
int Request[N] = {0, 0, 0};

void ShowData();
void ChangeData(int num);
int  SecurityCheck();

int main(){
    ShowData();
    while(1){
        int i,j;
        int flag = 1;
        printf("请输入需申请资源的进程号(从0到%d):",M-1);
        scanf("%d", &i);
        if (i<0||i>M-1)
            printf("输入的进程号不存在，重新输入!\n");
        else{
            printf("请输入进程%d申请的资源数，类别为ABC，例如'1 1 1'\n",i);
            for (int j=0;j<N;j++)
                scanf("%d", &Request[j]);
            for (j=0;j<N;j++){
                if (Request[j] > Available[j]){
                     printf("%d号进程申请的%c类资源数目 > 可获得的%c类资源数目。\n申请失败。\n",i,char(j+65),char(j+65));
                    flag = 0;//申请失败
                    break;
                }       
                if (Request[j] > Need[i][j]){
                    printf("%d号进程申请的%c类资源数目 > %d号进程还需要的%c类资源数目。\n申请失败。\n",i,char(j+65),i,char(j+65));
                    flag = 0;//申请失败
                    break;
                }
            }
            if (flag == 1)  ChangeData(i);
        }
    }
    return 0;
}

void ShowData(){
    int i, j;
    printf("系统可用资源向量:\n");
    printf("***Available***\n");
    printf("资源类别:ABC\n");
    printf("资源数目:");
    for (j = 0; j < N; j++)
        printf("%d", Available[j]);
    printf("\n\n");

    printf("各进程还需要的资源量:\n");
    printf("******Need******\n");
    printf("资源类别:ABC\n");
    for (i = 0; i < M; i++){
        printf("%d号进程:", i);
        for (j = 0; j < N; j++){
            printf("%d", Need[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("各进程已经得到的资源量:\n");
    printf("***Allocation***\n");
    printf("资源类别:ABC\n");
    for (i = 0; i < M; i++){
        printf("%d号进程:", i);
        for (j = 0; j < N; j++)
            printf("%d", Allocation[i][j]);
        printf("\n");
    }
    printf("\n");
} 

void ChangeData(int num){
    for (int i=0;i<N;i++){
        Available[i] = Available[i] - Request[i];
        Allocation[num][i] = Allocation[num][i] + Request[i];
        Need[num][i] = Need[num][i] - Request[i];
    }
    if (SecurityCheck() == 0){
        for (int i=0;i<N;i++){
            Available[i] = Available[i] + Request[i];
            Allocation[num][i] = Allocation[num][i] - Request[i];
            Need[num][i] = Need[num][i] + Request[i];
        }
    }
    ShowData();
}

int SecurityCheck(){
    int SecurityOrder[M];
    int k=0,flag;
    int Finish[M] = {False};
    int Work[N];
    for (int i=0;i<N;i++)
        Work[i] = Available[i];
    for (int i=0;i<M;i++){
        flag = 0;
        if (Finish[i] == False){
            for (int j=0;j<N;j++){
                if (Need[i][j] > Work[j]){
                    flag = 1;   //失败
                    break;
                }
            }
            if (flag == 0){
                Finish[i] = True;
                SecurityOrder[k++] = i;
                for (int j=0;j<N;j++)
                    Work[j] += Allocation[i][j]; 
                i = -1;
            }
        }
    }
    if (flag == 1){
        printf("系统为不安全状态！本次申请失败！\n");
        return 0;
    }
    printf("系统安全！安全序列为");
    for (int j=0;j<M;j++)
        printf(j!=M-1?"%d->":"%d\n",SecurityOrder[j]);
    return 1;
}

