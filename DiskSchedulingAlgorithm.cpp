/*
 * @Descripttion: 
 * @version: 
 * @Author: Spade Xiao
 * @Date: 2020-05-21 14:46:49
 * @LastEditors: Spade Xiao
 * @LastEditTime: 2020-05-21 20:40:12
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SEQUENCE_LENGTH 100  //请求序列长度
#define MAX_TRACK_NUMBER 199  //最大磁道口位置
#define TRACK_INIT_INDEX 100  //定义磁道初始位置为100#
#define TRACK_RIGHT 1   //磁道向右
#define TRACK_LEFT  0   //磁道向左
#define TRACK_INIT_DIRECTION TRACK_RIGHT //定义磁道初始方向，默认向右
#define INFINITE 300  //无穷大

typedef struct
{
    int order;
    int distance;
}Accesses;

Accesses Access[SEQUENCE_LENGTH]; //输出序列
int Sequence[SEQUENCE_LENGTH];    //输入序列
int flag = 0;   //序列标记位

void InitSequence(); //初始化请求队列
void FindFlag(int a[]);  //寻找标记位
void Clear();  //初始化结构体
void FCFSAlg();   //先来先服务调度算法
void SSTFAlg();   //最短寻道时间优先算法
void SCANAlg();   //电梯调度算法
void CSCANAlg();  //循环式单向电梯调度算法
void FSCANAlg();  //双队列单向电梯调度算法

int cmp_int(const void *_a, const void *_b){
    return *(int *)_a - *(int *)_b;
}

int main(){
    srand((int)time(0));
    InitSequence();
    double SumOfDistance;
    double AvgOfDistance;

    Clear();
    FCFSAlg();
    SumOfDistance = 0,AvgOfDistance = 0;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SumOfDistance += Access[i].distance;
    AvgOfDistance = SumOfDistance/SEQUENCE_LENGTH;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        printf("%d ",Access[i].order);
    printf("\n");
    printf("FCFS的平均寻道数为:%lf\n",AvgOfDistance);

    Clear();
    SSTFAlg();
    SumOfDistance = 0,AvgOfDistance = 0;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SumOfDistance += Access[i].distance;
    AvgOfDistance = SumOfDistance/SEQUENCE_LENGTH;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        printf("%d ",Access[i].order);
    printf("\n");
    printf("SSFT的平均寻道数为:%lf\n",AvgOfDistance);

    Clear();
    SCANAlg();
    SumOfDistance = 0,AvgOfDistance = 0;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SumOfDistance += Access[i].distance;
    AvgOfDistance = SumOfDistance/SEQUENCE_LENGTH;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        printf("%d ",Access[i].order);
    printf("\n");
    printf("SCAN的平均寻道数为:%lf\n",AvgOfDistance);

    Clear();
    CSCANAlg();
    SumOfDistance = 0,AvgOfDistance = 0;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SumOfDistance += Access[i].distance;
    AvgOfDistance = SumOfDistance/SEQUENCE_LENGTH;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        printf("%d ",Access[i].order);
    printf("\n");
    printf("CSCAN的平均寻道数为:%lf\n",AvgOfDistance);

    Clear();
    FSCANAlg();
    SumOfDistance = 0,AvgOfDistance = 0;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SumOfDistance += Access[i].distance;
    AvgOfDistance = SumOfDistance/SEQUENCE_LENGTH;
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        printf("%d ",Access[i].order);
    printf("\n");
    printf("FSCAN的平均寻道数为:%lf\n",AvgOfDistance);
        
    return 0;
}

void InitSequence(){
    for (int i = 0; i < SEQUENCE_LENGTH; i++)
        Sequence[i] = rand() % 200;
}

void FindFlag(int a[]){
    int min = abs(TRACK_INIT_INDEX - a[0]);
    for (int i=1;i<SEQUENCE_LENGTH;i++){
        int difference = abs(TRACK_INIT_INDEX-a[i]);
        if (min > difference){
            min = difference;
            flag = i;
        }
        if (min < difference)
            return;
    }
}

void Clear(){
    memset(Access,0,sizeof(Access));
    flag = 0;
}

void FCFSAlg(){
    for (int i=0;i<SEQUENCE_LENGTH;i++){
        Access[i].order = Sequence[i];
        if (i == 0)
            Access[i].distance = TRACK_INIT_INDEX - Sequence[i];
        else
            Access[i].distance = abs(Sequence[i]-Sequence[i-1]);
    }
}

void SSTFAlg(){
    int SpareSequence[SEQUENCE_LENGTH];
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SpareSequence[i] = Sequence[i];
    qsort(SpareSequence,SEQUENCE_LENGTH,sizeof(SpareSequence[0]),cmp_int);
    FindFlag(SpareSequence);
    int length = SEQUENCE_LENGTH;
    Access[0].order = SpareSequence[flag];
    Access[0].distance = abs(TRACK_INIT_INDEX - Access[0].order);
    for (int i= 1;i<SEQUENCE_LENGTH;i++){
        int a,b;
        if (flag != 0 && flag != length-1){
            a = abs(SpareSequence[flag-1] - SpareSequence[flag]);
            b = abs(SpareSequence[flag+1] - SpareSequence[flag]);
        }
        else if (flag == 0) {
            a = INFINITE;
            b = abs(SpareSequence[flag+1] - SpareSequence[flag]);
        }
        else{
            a = abs(SpareSequence[flag-1] - SpareSequence[flag]);
            b = INFINITE;
        }
        Access[i].distance = a<b ? a : b;
        Access[i]. order = a<b ? SpareSequence[flag-1] : SpareSequence[flag+1];
        for (int j=flag;j<length-1;j++)
            SpareSequence[j] = SpareSequence[j+1];
        length --;
        flag = a<b ? flag-1 : flag;
    }
}

void SCANAlg(){
    int SpareSequence[SEQUENCE_LENGTH];
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SpareSequence[i] = Sequence[i];
    qsort(SpareSequence,SEQUENCE_LENGTH,sizeof(SpareSequence[0]),cmp_int);
    for (int i=0;i<SEQUENCE_LENGTH;i++){
        if (SpareSequence[i] - TRACK_INIT_INDEX >= 0){
            flag = i;
            break;
        }
    }
    Access[0].order = SpareSequence[flag];
    Access[0].distance = SpareSequence[flag] - TRACK_INIT_INDEX;
    int j = 1;
    for (int i=flag+1;i<SEQUENCE_LENGTH;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }

    for (int i=flag-1;i>=0;i--){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }
}

void CSCANAlg(){
    int SpareSequence[SEQUENCE_LENGTH];
    for (int i=0;i<SEQUENCE_LENGTH;i++)
        SpareSequence[i] = Sequence[i];
    qsort(SpareSequence,SEQUENCE_LENGTH,sizeof(SpareSequence[0]),cmp_int);
    for (int i=0;i<SEQUENCE_LENGTH;i++){
        if (SpareSequence[i] - TRACK_INIT_INDEX >= 0){
            flag = i;
            break;
        }
    }
    Access[0].order = SpareSequence[flag];
    Access[0].distance = SpareSequence[flag] - TRACK_INIT_INDEX;
    int j = 1;
    for (int i=flag+1;i<SEQUENCE_LENGTH;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }

    for (int i=0;i<flag;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }
}

void FSCANAlg(){
    /*将请求队列分成两部分，模拟先后的请求*/
    int Sequence1[SEQUENCE_LENGTH/2];
    int Sequence2[SEQUENCE_LENGTH/2];
    for (int i=0;i<SEQUENCE_LENGTH/2;i++){
        Sequence1[i] = Sequence[i];
        Sequence2[i] = Sequence[SEQUENCE_LENGTH/2+i];
    }
    int SpareSequence[SEQUENCE_LENGTH/2];
    
    /*第一队列*/
    for (int i=0;i<SEQUENCE_LENGTH/2;i++)
        SpareSequence[i] = Sequence1[i];
    qsort(SpareSequence,SEQUENCE_LENGTH/2,sizeof(SpareSequence[0]),cmp_int);
    /*依然假设第一次处理前磁盘头是在#100，方向向右*/
    for (int i=0;i<SEQUENCE_LENGTH/2;i++){
        if (SpareSequence[i] - TRACK_INIT_INDEX >= 0){
            flag = i;
            break;
        }
    }
    Access[0].order = SpareSequence[flag];
    Access[0].distance = abs(SpareSequence[flag] - TRACK_INIT_INDEX);
    int j = 1;
    for (int i=flag+1;i<SEQUENCE_LENGTH/2;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }
    for (int i=0;i<flag;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }

    /*第二队列*/
    memset(SpareSequence,0,sizeof(SpareSequence[0]));
    for (int i=0;i<SEQUENCE_LENGTH/2;i++)
        SpareSequence[i] = Sequence2[i];
    qsort(SpareSequence,SEQUENCE_LENGTH/2,sizeof(SpareSequence[0]),cmp_int);
    for (int i=0;i<SEQUENCE_LENGTH/2;i++){
        if (SpareSequence[i] - TRACK_INIT_INDEX >= 0){
            flag = i;
            break;
        }
    }

    Access[j].order = SpareSequence[flag];
    Access[j++].distance = abs(SpareSequence[flag] - Access[j-1].order);
    for (int i=flag+1;i<SEQUENCE_LENGTH/2;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }
    for (int i=0;i<flag;i++){
        Access[j].order = SpareSequence[i];
        Access[j++].distance = abs(SpareSequence[i] - Access[j-1].order);
    }
}