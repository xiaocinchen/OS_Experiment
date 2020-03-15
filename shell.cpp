#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <dirent.h>
#define SHELL_READIN 100
#define PATH_SIZE 50
#define MAX_HIS_INS 100
int cd(char func[]);     
char* pwd();            
char shellReadin[SHELL_READIN];
char dir[PATH_SIZE];
char historyIns[MAX_HIS_INS][SHELL_READIN];
void split(char *src,const char *separator,char **dest,int *num);
int main() {
    printf("*********************************************************\n");
    printf("*                    xxxxxxxx_Spade                    *\n");
    printf("*********************************************************\n");
    printf("实现了以下命令。\n");
    printf("1. pwd                               查看当前工作目录\n");
    printf("2. cd                                进入工作目录\n");
    printf("3. history                           查看输入历史\n");
    printf("4. echo                              输出字符串\n");
    printf("5. exit                              退出目前的shell\n");
    printf("6. ps                                显示当前进程状态\n");
    printf("7. ls                                查看目录下内容\n");
    printf("8. date                              显示或设定系统的日期与时间\n");
    printf("9. mkdir                             创建文件目录\n");
    printf("10. rm                               删除文件或者目录\n");
    memset(dir,0,sizeof(char));
	printf("root@hostname:~%s$ ",dir);
    int count = 0;
	while(gets(shellReadin)!= NULL){
        int i = 0;
        int j = 0;
        if (strcmp(shellReadin,"") != 0){
            strcpy(historyIns[count++],shellReadin);
        }
        else{
            printf("root@hostname:%s$ ",pwd());
            continue;
        }
        char *Instruction[10];
        int num = 0;
        split(shellReadin," ",Instruction,&num);
        Instruction[num++] = NULL;
        if (strcmp(Instruction[0],"cd") == 0){
            if (Instruction[1] == NULL){
                printf("root@hostname:%s$ ",pwd());
                continue;
            }
            int returnvalue = cd(Instruction[1]);
            if (returnvalue != 0)
                printf("-bash: cd: %s: No such file or directory\n",Instruction[0]);
        }
        else if (strcmp(Instruction[0],"pwd") == 0){
            printf("%s\n",pwd());
        }
        else if (strcmp(Instruction[0],"ls") == 0){
            pid_t pid;
            int status;
            pid = fork();
            if (pid<0)  
                printf("error in fork!");
            else if (pid == 0)
                execv("/usr/bin/ls",Instruction);
            else{
                wait(&status);
            }
        }
        else if (strcmp(Instruction[0],"date") == 0){
            pid_t pid;
            int status;
            pid = fork();
            if (pid<0)
                printf("error in fork!");
            else if (pid == 0)
                execv("/usr/bin/date",Instruction);
            else
                wait(&status);
        }
        else if (strcmp(Instruction[0],"mkdir") == 0){
            pid_t pid;
            int status;
            pid = fork();
            if (pid<0)
                printf("error in fork!");
            else if (pid == 0)
                execv("/usr/bin/mkdir",Instruction);
            else
                wait(&status);
        }
        else if (strcmp(Instruction[0],"rm") == 0){
            pid_t pid;
            int status;
            pid = fork();
            if (pid<0)
                printf("error in fork!");
            else if (pid == 0)
                execv("/usr/bin/rm",Instruction);
            else
                wait(&status);
        }
        else if (strcmp(Instruction[0],"echo") == 0){
            for (int i=1;i<num-1;i++)
                printf("%s ",Instruction[i]);
            printf("\n");
        }
        else if (strcmp(Instruction[0],"history") == 0){
            for (int i=1;i<=count;i++)
                printf("%d  %s\n",i,historyIns[i-1]);
        }
        else if (strcmp(Instruction[0],"exit") == 0){
            printf("logout\n");
            syscall(SYS_exit);
        }
        else if (strcmp(Instruction[0],"ps") == 0){
            pid_t pid;
            int status;
            pid = fork();
            if (pid<0)
                printf("error in fork!");
            else if (pid == 0)
                execv("/usr/bin/ps",Instruction);
            else
                wait(&status);
        }
        else {
            pid_t pid;
            int status;
            pid = fork();
            if (pid<0)
                printf("error in fork!");
            else if (pid == 0){
                if (execvp(Instruction[0],Instruction) == -1)
                    printf("bash: %s: command not found\n",Instruction[0]);
            }
            else
                wait(&status);
        }
        printf("root@hostname:%s$ ",pwd());
    }
	return 0;
}

int cd(char func[]){
    if (chdir(func) == 0)
        return 0;
    else
        return -1;
}

char* pwd(){
    return getcwd(dir,PATH_SIZE);
}

void split(char *src,const char *separator,char **dest,int *num) {
    char *pNext;
    int count = 0;
    if (src == NULL || strlen(src) == 0)
        return;
    if (separator == NULL || strlen(separator) == 0)
        return;    
    pNext = strtok(src,separator);
    while(pNext != NULL) {
        *dest++ = pNext;
        ++count;
        pNext = strtok(NULL,separator);  
    }
    *num = count;
}
