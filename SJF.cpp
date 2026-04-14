//短作业优先
#include<stdio.h>
#include<iostream>
#include<windows.h>
#include<vector>
#include<string>
using namespace std;
typedef char ProcessNameType;
typedef struct SJF
{
    ProcessNameType ProcessName[20];
    int ComeTime;
    int ServeTime;
    int RotateTime;//周转时间=完成时间-到达时间
    int WaitTime;//等待时间=周转时间-服务时间
    float WeightRotateTime;//带权周转时间=周转时间/服务时间
    int RemainderTime;//剩余服务时间用于抢占式调度算法
    SJF *next;
}SJF;
//添加进程
void Add_process(SJF* process,SJF* Node){
    if(Node->ComeTime<0||Node->ServeTime<0){
        cout<<"不合法输入!"<<endl;
        return;
    }
    process->next=Node;
    Node->next=NULL;
}
//非抢占式短作业优先调度
void NoGrab_mean(SJF* process){
    int SysTime=0;//模拟总时间
    vector<SJF>Waiting;//已经到达了但是在等待中的进程
    //先将第一个程序加入等待队列
    Waiting.push_back(*process);
    process=process->next;
    SysTime=Waiting[0].ComeTime;
    vector<SJF>Result;//结果数组
    while(!Waiting.empty()){
        //找等待队列中服务时间最短的进程
        int Max=0x10000;
        int Pos=-1;
        for(int i=0;i<Waiting.size();i++){
            if(Waiting[i].ServeTime<Max){
                Max=Waiting[i].ServeTime;
                Pos=i;
            }
        }
        SysTime+=Waiting[Pos].ServeTime;
        Waiting[Pos].RotateTime=SysTime-Waiting[Pos].ComeTime;
        Waiting[Pos].WaitTime=Waiting[Pos].RotateTime-Waiting[Pos].ServeTime;
        Waiting[Pos].WeightRotateTime=(float)Waiting[Pos].RotateTime/(float)Waiting[Pos].ServeTime;
        Result.push_back(Waiting[Pos]);
        Waiting.erase(Waiting.begin()+Pos);
        //取进程加入等待队列
        while(process!=NULL&&process->ComeTime<=SysTime){
            Waiting.push_back(*process);
            process=process->next;
        }
        if(Waiting.empty()&&process!=NULL){//等待队列里的进程全部完成后下一个进程还未到达
            SysTime=process->ComeTime;
            Waiting.push_back(*process);
            process=process->next;
        }
    }   
    //输出结果
    int process_count=Result.size();//总进程数
    cout<<"进程顺序为：";
    cout<<Result[0].ProcessName;
    for(int i=1;i<Result.size();i++){
        cout<<" -> "<<Result[i].ProcessName;
    }
    cout<<endl;
    int rotatetime_sum=0;//总周转时间
    float weightrotatetime_sum=0.0;//带权周转时间
    for(int i=0;i<Result.size();i++){
        cout<<Result[i].ProcessName<<"     周转时间:"<<Result[i].RotateTime<<"      等待时间:"<<Result[i].WaitTime<<"       带权周转时间："<<Result[i].WeightRotateTime<<endl;
        rotatetime_sum+=Result[i].RotateTime;
        weightrotatetime_sum+=Result[i].WeightRotateTime;
    }
    float Averotatetime=(float)rotatetime_sum/process_count;
    float Aveweightrotatetime=weightrotatetime_sum/process_count;
    cout<<"平均周转时间为："<<Averotatetime<<endl;
    cout<<"平均带权周转时间为："<<Aveweightrotatetime;
}

//抢占式进程调度
void Grab_mean(SJF* process){
     int SysTime=0;//模拟总时间
    vector<SJF>Waiting;//已经到达了但是在等待中的进程
    //先将第一个程序加入等待队列
    Waiting.push_back(*process);
    process=process->next;
    SysTime=Waiting[0].ComeTime;
    vector<SJF>Result;//结果数组
    vector<SJF>Order;//保存程序调度顺序的数组
    while(!Waiting.empty()){
        int Max=0x10000;
        int Pos=-1;
        for(int i=0;i<Waiting.size();i++){
            if(Waiting[i].ComeTime<=SysTime&&Waiting[i].RemainderTime<Max){
                Max=Waiting[i].RemainderTime;
                Pos=i;
            }
        }
        while(process!=NULL&&SysTime+Waiting[Pos].RemainderTime>=process->ComeTime){//在执行程序完成之前后新程序到来
            Waiting.push_back(*process);
            if(process->RemainderTime<Waiting[Pos].RemainderTime){
                Waiting[Pos].RemainderTime-=(process->ComeTime-SysTime);
                SysTime=process->ComeTime;
                Order.push_back(Waiting[Pos]);
                Pos=Waiting.size()-1;
            }
            process=process->next;
        }
        SysTime+=Waiting[Pos].RemainderTime;
        Waiting[Pos].RotateTime=SysTime-Waiting[Pos].ComeTime;
        Waiting[Pos].WaitTime=Waiting[Pos].RotateTime-Waiting[Pos].ServeTime;
        Waiting[Pos].WeightRotateTime=(float)Waiting[Pos].RotateTime/(float)Waiting[Pos].ServeTime;
        Result.push_back(Waiting[Pos]);
        Order.push_back(Waiting[Pos]);
        Waiting.erase(Waiting.begin()+Pos);
        if(Waiting.empty()&&process!=NULL){//等待队列里的进程全部完成后下一个进程还未到达
            SysTime=process->ComeTime;
            Waiting.push_back(*process);
            process=process->next;
        }
    }
    //输出结果
    int process_count=Result.size();//总进程数
    cout<<"进程顺序为：";
    cout<<Order[0].ProcessName;
    for(int i=1;i<Order.size();i++){
        cout<<" -> "<<Order[i].ProcessName;
    }
    cout<<endl;
    int rotatetime_sum=0;//总周转时间
    float weightrotatetime_sum=0.0;//带权周转时间
    for(int i=0;i<Result.size();i++){
        cout<<Result[i].ProcessName<<"     周转时间:"<<Result[i].RotateTime<<"      等待时间:"<<Result[i].WaitTime<<"       带权周转时间："<<Result[i].WeightRotateTime<<endl;
        rotatetime_sum+=Result[i].RotateTime;
        weightrotatetime_sum+=Result[i].WeightRotateTime;
    }
    float Averotatetime=(float)rotatetime_sum/process_count;
    float Aveweightrotatetime=weightrotatetime_sum/process_count;
    cout<<"平均周转时间为："<<Averotatetime<<endl;
    cout<<"平均带权周转时间为："<<Aveweightrotatetime;
}

int main(int argc,char const* argv[]){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    SJF* process=(SJF*)malloc(sizeof(SJF));
    process->next=NULL;
    cout<<"Please input process infomations:"<<endl;
    cout<<"ProcessName      ComeTime        ServeTime"<<endl;
    SJF* curr=process;
    while(1){
        SJF* Node=(SJF*)malloc(sizeof(SJF));
        cin>>Node->ProcessName;
        if(!strcmp(Node->ProcessName,"end")) break;
        cin>>Node->ComeTime>>Node->ServeTime;
        Node->RemainderTime=Node->ServeTime;
        Add_process(curr,Node);
        curr=curr->next;
    }
    curr=process->next;
    if(curr==NULL){
        cout<<"——————空进程——————";
    }else{
    cout<<"——————非抢占式调度算法——————"<<endl;
    NoGrab_mean(curr);
    curr=process->next;
    cout<<"\n——————抢占式调度算法———————"<<endl;
    Grab_mean(curr);
    }
    return 0;
}