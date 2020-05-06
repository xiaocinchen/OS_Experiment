#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define N 10   //逻辑地址空间页面总数
#define S 3   //物理块数
#define w 6   //工作集包含的页数
//#define s 1   //工作集起始页号
#define v 10   //工作集移动速率
#define t 0.4 //用于比较
#define R 0   //只支持读
#define RW 1  //支持读写
#define INFINITE 100 //定义最大距离
#define LACK 0
#define UNLACK 1

typedef struct
{
    int BlockNumber;  //所使用的物理块序号
    int PageNumber;   //所引入的页号
    int RorRW = -1;        //读还是读写
    int IfLack;       //是否缺页。0为缺页，1为不缺页
}Orders;  //访问顺序

typedef struct 
{
    int PageNumber;  //当前数据块存储的页号
    int NextUse = INFINITE;    //下次使用的距离
    int status = 0;  //状态位
    int change = 0;  //修改位
}Blocks;   //数据块

Orders Order[50];
Blocks Block[S];
int s = 1;
int Page[7] = {R,RW,RW,RW,R,RW,R};
int Sequence[50];
int num = 0;
int min(int a,int b);
int min3(int a,int b,int c);
int max(int a,int b,int c);
void initSequence();
void optAlgo();  //最佳淘汰
void fifoAlgo(); //先进先出
void LruAlgo();  //最近最久未使用
void SimpleClockAlgo(); //简单Clock
void ImprovedClockAlgo(); //改进Clock
clock_t start, stop;
double PageMissingRate1,PageMissingRate2,PageMissingRate3,PageMissingRate4,PageMissingRate5;

int main(){
    srand((int)time(0));
    initSequence();
    initSequence();
    initSequence();
    //initSequence();
    printf("num = %d\n",num);
    for (int i=0;i<num;i++)
        printf("%d ",Sequence[i]);
    printf("\n");

    memset(Order,0,sizeof(Order));
    memset(Block,0,sizeof(Block));
    start = clock();
    optAlgo();
    stop = clock();
    double consume1 = (double)(stop-start)/CLK_TCK;

    memset(Order,0,sizeof(Order));
    memset(Block,0,sizeof(Block));
    start = clock();
    fifoAlgo();
    stop = clock();
    double consume2 = (double)(stop-start)/CLK_TCK;

    memset(Order,0,sizeof(Order));
    memset(Block,0,sizeof(Block));
    start = clock();
    LruAlgo();
    stop = clock();
    double consume3 = (double)(stop-start)/CLK_TCK;

    memset(Order,0,sizeof(Order));
    memset(Block,0,sizeof(Block));
    start = clock();
    SimpleClockAlgo();
    stop = clock();
    double consume4 = (double)(stop-start)/CLK_TCK;

    memset(Order,0,sizeof(Order));
    memset(Block,0,sizeof(Block));
    start = clock();
    ImprovedClockAlgo();
    stop = clock();
    double consume5 = (double)(stop-start)/CLK_TCK;

    printf("OptAlgo缺页率为%lf,耗时%lf\n",PageMissingRate1,consume1);
    printf("FifoAlgo缺页率为%lf,耗时%lf\n",PageMissingRate2,consume2);
    printf("LruAlgo缺页率为%lf,耗时%lf\n",PageMissingRate3,consume3);
    printf("SimpleClockAlgo缺页率为%lf,耗时%lf\n",PageMissingRate4,consume4);
    printf("ImprovedClockAlgo缺页率为%lf,耗时%lf\n",PageMissingRate5,consume5);

    return 0;
}

int min(int a,int b){
    return a<b?a:b;
}

int min3(int a,int b,int c){
    if (a<b)
        if (a<=c) return 0;
        else return 2;
    else
        if (b<=c) return 1;
        else return 2;
}

int max(int a,int b,int c){
    if (a>b)
        if (a>=c) return 0;
        else return 2;
    else
        if (b>=c) return 1;
        else return 2;
}

void initSequence(){
    for (int i=0;i<v;i++)
        Sequence[num++] = rand()%(min(s+w, N-1)-s+1)+s;
    if (rand()%10 < t*10)
        s = rand()%N;
    else
        s = (s+1)%N;
}

void optAlgo(){
    int block = 0;
    for (int i=0;i<num;i++){
        int flag = 0; 
        if (block < S){
            for (int k=0;k<block;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k;
                    Block[k].status = 1;
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;  //不缺页
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){    //缺页
                Order[i].BlockNumber = block;
                Order[i].PageNumber = Sequence[i]; 
                Order[i].IfLack = LACK; //缺页
                Block[block].status = 1;
                Block[block++].PageNumber = Sequence[i];
            }
        }
        else{
            for (int k=0;k<S;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k; 
                    Block[k].status = 1;
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){ //缺页
                for (int k=0;k<S;k++){
                    for (int j = i;j<num;j++){
                        if (Block[k].PageNumber == Sequence[j])
                            Block[k].NextUse = j-i;
                    }
                }
                int EliminateNum = max(Block[0].NextUse,Block[1].NextUse,Block[2].NextUse);
                Order[i].BlockNumber = EliminateNum;
                Block[EliminateNum].status = 1;
                Order[i].PageNumber = Sequence[i];
                Order[i].IfLack = LACK;
                Block[EliminateNum].PageNumber = Sequence[i];
            }
            for (int k=0;k<S;k++){
                Block[k].NextUse = INFINITE;
            }
        }
        if (Page[Sequence[i]] == R)
                Order[i].RorRW = R;
        else{
            if (rand()%10 > 7) 
                Order[i].RorRW = RW;
            else
                Order[i].RorRW = R;
        }
        printf("页号为%d，状态位为%d，访问方式为%d (0为读，1为读写)\n",Order[i].PageNumber,Block[Order[i].BlockNumber].status,Order[i].RorRW);
        if (Order[i].IfLack == LACK)    printf("此时触发缺页中断。页面存入%d号物理块中。\n",Order[i].BlockNumber);
        if (Order[i].IfLack == UNLACK)  printf("页面已在内存%d号物理块中。\n",Order[i].BlockNumber);
        printf("此时物理块内情况为:\n");
        if (block<S)
            for (int k=0;k<block;k++)
            printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        else
            for (int k=0;k<S;k++)
                printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        printf("\n");
    }
    int count = 0;
    for (int i=0;i<num;i++){
        if (Order[i].IfLack == LACK)
            count++;
    }
    PageMissingRate1 = (double(count)-3)/double(num) * 100;
    printf("OptAlgo缺页率为%%%lf\n\n",PageMissingRate1);
}

void fifoAlgo(){
    int block = 0;
    for (int i=0;i<num;i++){
        int flag = 0; 
        if (block < S){
            for (int k=0;k<block;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k;
                    Block[k].status = 1;
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;  //不缺页
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){    //缺页
                Order[i].BlockNumber = block;
                Order[i].PageNumber = Sequence[i]; 
                Order[i].IfLack = LACK; //缺页
                Block[block].status = 1;
                Block[block++].PageNumber = Sequence[i];
            }
        }
        else{
            for (int k=0;k<S;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k; 
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;
                    Block[k].status = 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){ //缺页
                Block[block%S].PageNumber = Sequence[i];
                Order[i].BlockNumber = block%S;
                Order[i].PageNumber = Sequence[i];
                Order[i].IfLack = LACK;
                Block[block%S].status = 1;
                block++;
            }
        }
        if (Page[Sequence[i]] == R)
                Order[i].RorRW = R;
        else{
            if (rand()%10 > 7) 
                Order[i].RorRW = RW;
            else
                Order[i].RorRW = R;
        }
        printf("页号为%d，状态位为%d，访问方式为%d (0为读，1为读写)\n",Order[i].PageNumber,Block[Order[i].BlockNumber].status,Order[i].RorRW);
        if (Order[i].IfLack == LACK)    printf("此时触发缺页中断。页面存入%d号物理块中。\n",Order[i].BlockNumber);
        if (Order[i].IfLack == UNLACK)  printf("页面已在内存%d号物理块中。\n",Order[i].BlockNumber);
        printf("此时物理块内情况为:\n");
        if (block<S)
            for (int k=0;k<block;k++)
            printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        else
            for (int k=0;k<S;k++)
                printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        printf("\n");
    }
    int count = 0;
    for (int i=0;i<num;i++){
        if (Order[i].IfLack == LACK)
            count++;
    }
    PageMissingRate2 = (double(count)-3)/double(num) * 100;
    printf("FifoAlgo缺页率为%%%lf\n\n",PageMissingRate2);
}

void LruAlgo(){
    int block = 0;
    for (int i=0;i<num;i++){
        int flag = 0; 
        if (block < S){
            for (int k=0;k<block;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k;
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;  //不缺页
                    Block[k].NextUse += 128;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){    //缺页
                Order[i].BlockNumber = block;
                Order[i].PageNumber = Sequence[i]; 
                Order[i].IfLack = LACK; //缺页
                Block[block].NextUse += 128;
                Block[block++].PageNumber = Sequence[i];
            }
        }
        else{
            for (int k=0;k<S;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k; 
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;
                    Block[k].NextUse += 128;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){ //缺页,开始淘汰
                int EliminateNum = min3(Block[0].NextUse,Block[1].NextUse,Block[2].NextUse);
                Order[i].BlockNumber = EliminateNum;
                Order[i].PageNumber = Sequence[i];
                Order[i].IfLack = LACK;
                Block[EliminateNum].PageNumber = Sequence[i];
                Block[EliminateNum].NextUse += 128;
            }
        }
        if (Page[Sequence[i]] == R)
                Order[i].RorRW = R;
        else{
            if (rand()%10 > 7) 
                Order[i].RorRW = RW;
            else
                Order[i].RorRW = R;
        }
        if (block < S)
            for (int k=0;k<block;k++){
                Block[k].NextUse = Block[k].NextUse >> 1;
                //printf("=%d ",Block[k].NextUse);
            }
        else
            for (int k=0;k<S;k++){
                Block[k].NextUse = Block[k].NextUse >> 1;
                //printf("=%d ",Block[k].NextUse);
            }
    printf("页号为%d，状态位为%d，访问方式为%d (0为读，1为读写)\n",Order[i].PageNumber,Block[Order[i].BlockNumber].status,Order[i].RorRW);
        if (Order[i].IfLack == LACK)    printf("此时触发缺页中断。页面存入%d号物理块中。\n",Order[i].BlockNumber);
        if (Order[i].IfLack == UNLACK)  printf("页面已在内存%d号物理块中。\n",Order[i].BlockNumber);
        printf("此时物理块内情况为:\n");
        if (block<S)
            for (int k=0;k<block;k++)
            printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        else
            for (int k=0;k<S;k++)
                printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        printf("\n");
    }
    int count = 0;
    for (int i=0;i<num;i++){
        if (Order[i].IfLack == LACK)
            count++;
    }
    PageMissingRate3 = (double(count)-3)/double(num) * 100;
    printf("LruAlgo缺页率为%%%lf\n\n",PageMissingRate3);
}

void SimpleClockAlgo(){
    int index = 0;
    int block = 0;
    for (int i=0;i<num;i++){
        int flag = 0; 
        if (block < S){
            for (int k=0;k<block;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k;
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;  //不缺页
                    Block[k].status = 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){    //缺页
                Order[i].BlockNumber = block;
                Order[i].PageNumber = Sequence[i]; 
                Order[i].IfLack = LACK; //缺页
                Block[block].status = 1;
                Block[block++].PageNumber = Sequence[i];
            }
        }
        else{
            for (int k=0;k<S;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k; 
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;
                    Block[k].status = 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){ //缺页,开始淘汰
                while(1){
                    if (Block[index].status == 0){
                        Order[i].BlockNumber = index;
                        Order[i].PageNumber = Sequence[i];
                        Order[i].IfLack = LACK;
                        Block[index].PageNumber = Sequence[i];
                        Block[index].status = 1;
                        index = (index+1)%S;
                        break;
                    }  
                    else{
                        Block[index].status = 0;
                        index = (index+1)%S;
                    }
                }
            }
        }
        if (Page[Sequence[i]] == R)
                Order[i].RorRW = R;
        else{
            if (rand()%10 > 7) 
                Order[i].RorRW = RW;
            else
                Order[i].RorRW = R;
        }
        printf("页号为%d，状态位为%d，访问方式为%d (0为读，1为读写)\n",Order[i].PageNumber,Block[Order[i].BlockNumber].status,Order[i].RorRW);
        if (Order[i].IfLack == LACK)    printf("此时触发缺页中断。页面存入%d号物理块中。\n",Order[i].BlockNumber);
        if (Order[i].IfLack == UNLACK)  printf("页面已在内存%d号物理块中。\n",Order[i].BlockNumber);
        printf("此时物理块内情况为:\n");
        if (block<S)
            for (int k=0;k<block;k++)
            printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        else
            for (int k=0;k<S;k++)
                printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        printf("\n");
    }
    int count = 0;
    for (int i=0;i<num;i++){
        if (Order[i].IfLack == LACK)
            count++;
    }
    PageMissingRate4 = (double(count)-3)/double(num) * 100;
    printf("SimpleClockAlgo缺页率为%%%lf\n\n",PageMissingRate4); 
}

void ImprovedClockAlgo(){
    int index = 0;
    int block = 0;
    for (int i=0;i<num;i++){
        if (Order[i].RorRW == -1){
            if (Page[Sequence[i]] == R)
                    Order[i].RorRW = R;
            else{
                if (rand()%10 > 7) 
                    Order[i].RorRW = RW;
                else
                    Order[i].RorRW = R;
            }
        }
        int flag = 0; 
        if (block < S){
            for (int k=0;k<block;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k;
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;  //不缺页
                    Block[k].status = 1;
                    if (Order[i].RorRW == R)
                        Block[k].change = 0;
                    else
                        Block[k].change = 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){    //缺页
                Order[i].BlockNumber = block;
                Order[i].PageNumber = Sequence[i]; 
                Order[i].IfLack = LACK; //缺页
                Block[block].status = 1;
                if (Order[i].RorRW == R)
                    Block[block].change = 0;
                else
                    Block[block].change = 1;
                Block[block++].PageNumber = Sequence[i];
            }
        }
        else{
            for (int k=0;k<S;k++){
                if (Block[k].PageNumber == Sequence[i]){
                    Order[i].BlockNumber = k; 
                    Order[i].PageNumber = Sequence[i];
                    Order[i].IfLack = UNLACK;
                    if (Order[i].RorRW == R)
                        Block[k].change = 0;
                    else
                        Block[k].change = 1;
                    Block[k].status = 1;
                    flag = 1;
                    break;
                }
            }
            if (flag == 0){ //缺页,开始淘汰
                int count = 0;
                for (;;index=(index+1)%S){
                    //printf("index = %d\n",index);
                    if (Block[index].status == 0 && Block[index].change == 0){
                        Order[i].BlockNumber = index;
                        Order[i].PageNumber = Sequence[i];
                        Order[i].IfLack = LACK;
                        Block[index].PageNumber = Sequence[i];
                        Block[index].status = 1;
                        if (Order[i].RorRW == R)
                            Block[index].change = 0;
                        else
                            Block[index].change = 1;
                        index = (index+1)%S;
                        flag = 1; //找到了
                        break;
                    }  
                    if (++count == S){
                        index=(index+1)%S;
                        break;
                    }
                       
                }
                //printf("flag == %d\n",flag);
                if (flag == 0){   //没找到
                    count = 0;
                    for (;;index=(index+1)%S){
                        if (Block[index].status == 0 && Block[index].change == 1){
                            Order[i].BlockNumber = index;
                            Order[i].PageNumber = Sequence[i];
                            Order[i].IfLack = LACK;
                            Block[index].PageNumber = Sequence[i];
                            Block[index].status = 1;
                            if (Order[i].RorRW == R)
                                Block[index].change = 0;
                            else
                                Block[index].change = 1;
                            index = (index+1)%S;
                            flag = 1;
                            break;
                        }
                        else{
                            Block[index].status = 0;
                        }
                        if(++count == S){
                            index=(index+1)%S;
                            break;
                        }
                            
                    }
                    if (flag == 0){
                        i--;
                        continue;
                    }
                }
            }
        }
    printf("页号为%d，状态位为%d，访问方式为%d (0为读，1为读写)\n",Order[i].PageNumber,Block[Order[i].BlockNumber].status,Order[i].RorRW);
        if (Order[i].IfLack == LACK)    printf("此时触发缺页中断。页面存入%d号物理块中。\n",Order[i].BlockNumber);
        if (Order[i].IfLack == UNLACK)  printf("页面已在内存%d号物理块中。\n",Order[i].BlockNumber);
        printf("此时物理块内情况为:\n");
        if (block<S)
            for (int k=0;k<block;k++)
            printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        else
            for (int k=0;k<S;k++)
                printf("%d号 物理块内页面为 %d号\n",k,Block[k].PageNumber);
        printf("\n");
    }
    int count = 0;
    for (int i=0;i<num;i++){
        if (Order[i].IfLack == LACK)
            count++;
    }
    PageMissingRate5 = (double(count)-3)/double(num) * 100;
    printf("SimpleClockAlgo缺页率为%%%lf\n\n",PageMissingRate5); 
}