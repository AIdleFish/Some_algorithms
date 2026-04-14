//实现FCFS(先到先服务)进程功能
#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<string>
#include<cstring>//用于字符数组对比
using namespace std;
typedef struct FCFS
{
	char ProcessName[20];
    int ComeTime;
    int ServeTime;
	struct FCFS * next;
}FCFS;
//输入进程
void Process_Input(FCFS* p, FCFS *Node){
    if(Node->ComeTime<0||Node->ServeTime<0){
        cout<<"不合法输入！";
        return;
    }
    p->next=Node;
    Node->next=NULL;
}
//输出进程
void Process_Output(FCFS* p){
    if(p->next==NULL){
        cout<<"——————空进程——————";
        return;
    }
    FCFS* follow=p->next;//因为p是带头结点的链表
    int i=1;
    int TimeSum=follow->ComeTime;//总时间,下一进程开始时间
    //输出进程顺序
    while(follow!=NULL){
        if(follow->ComeTime>TimeSum){
            TimeSum=follow->ComeTime;
        }
        cout<<follow->ProcessName<<"    到达时间："<<follow->ComeTime<<"    完成时间："<<TimeSum+follow->ServeTime<<endl;
        TimeSum+=follow->ServeTime;
        follow=follow->next;
    }

}
//链表初始化
int main(int argc,char const *argv[]){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    //初始化链表
    FCFS* process=(FCFS*)malloc(sizeof(FCFS));
    process->next=NULL;
    FCFS* cur=process;//作为输入进程函数的指针，保证链表结构不变
    cout<<"Please input proscess name:"<<endl;
    cout<<"Precess name     ComeTime       Servetime"<<endl;
    while(1){
        FCFS *NewNode=(FCFS*)malloc(sizeof(FCFS));
        cin>>NewNode->ProcessName;
        if(!strcmp(NewNode->ProcessName,"end")) break;
        cin>>NewNode->ComeTime>>NewNode->ServeTime;
        Process_Input(cur,NewNode);
        cur=cur->next;
    }
    cur->next=NULL;
    cur=process->next;//初始化cur指针,用于输出进程就绪队列
    cout<<"进程就绪队列为：";
    while(cur!=NULL){
        cout<<cur->ProcessName<<"    ";
        cur=cur->next;
    }
    cout<<"\n进程调度顺序为：\n";
    Process_Output(process);
    return 0;
}