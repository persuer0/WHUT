#include<iostream>
#include<stdio.h>
#include<cmath> 
using namespace std;
int open_cnt=0; //记录open表中每一个扩展的节点 
int open_node_cnt;//open表节点个数 
int num=0;//记录移动步数 

struct Node{
    int a[3][3];
    int x,y;
    int f,g,h;	//启发式搜索的函数 
    int flag; //上一次移动方向 
    Node *father;
}start,end;


struct Open_Close{
    int f;
    Node *np;
}open[10000],close[10000];


bool isable(){/*判断是否有解，逆序数之和奇偶性相同，有解  
				用F(X)表示数字X前面比它小的数的个数，全部数字的F(X)之和为Y=∑(F(X))，如果Y为奇数则称原数字的排列是奇排列，如果Y为偶数则称原数字的排列是偶排列。
				*/ 

    int s[9],e[9];
    int tf=0,ef=0;
    int k=0;
    for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
            s[k]=start.a[i][j];	//将初始状态存入s[]数组中 
            e[k]=end.a[i][j];	//将目标状态存入e[]数组中  
            k++;
        }
    }

    for(int i=0;i<9;i++){
        for(int j=0;j<i;j++){
            if(s[i]>s[j]&&s[j]!=0) tf+=1;	//记录对应位置上前面比他小 的数的个数
            if(e[i]>e[j]&&e[j]!=0) ef+=1;
        }
    }
    
    if((tf%2==1&&ef%2==1)||(tf%2==0&&ef%2==0)) return true;	//奇偶相同，则有解 
    else return false;

}


int a_start_h(Node *node){  //求 h（） 计算每个数字当前状态与最终状态的曼哈顿距离 之和作为代价 
    int old_x,old_y,end_x,end_y;
    int h=0;
    for(int k=1;k<9;k++){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(node->a[i][j]==k){	//找到每个数在对数组中的对应位置 
                    old_x=i;
                    old_y=j;
                }
                if(end.a[i][j]==k){	//目标状态的数在数组中的对应位置 
                    end_x=i;
                    end_y=j;
                }
            }
        }

        h+=abs(old_x-end_x)+abs(old_y-end_y);	//计算和目标状态偏移量之和 
    }   
    return h;
}


void input(){ 
	printf("请输入初始状态:\n");              //输入 
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cin>>start.a[i][j];
            if(start.a[i][j]==0){	//记录0位置，即空位置 
                start.x=i;
                start.y=j;
            }
        }
    }
	printf("请输入目标状态:\n"); 
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cin>>end.a[i][j];
            if(end.a[i][j]==0){	//记录0位置，即空位置 
                end.x=i;
                end.y=j;
            }
        }
    }
    start.g=0;	//函数g 
    start.h=a_start_h(&start);	//函数h 
    start.f=start.g+start.h;	//函数f
}


int show(Node *node){    //显示 
    Node *p = node;num++;
    if(p==&start) return 1; 
    else show(p->father);
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cout<<p->a[i][j]<<" ";
        }
        printf("\n");
    }
    cout<<"====================================\n";
}


bool isend(Node *node){         //判断是否为目标节点 
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(node->a[i][j]!=end.a[i][j])	//对应位置元素不等于目标状态的数，返回false 
                return false;
        }
    }
    return true;
} 


void sort(Open_Close *open){      //open表排序 
    int min=99999,min_flag=0; 
    Open_Close temp;
    for(int i=0;i<=open_cnt;i++){
        if(min>open[i].f&&open[i].f>0){	//找出最小节点 
            min=open[i].f;
            min_flag=i;	
        }
    }
    
    temp=open[min_flag];	//调整亲子关系 
    open[min_flag]=open[0];
    open[0]=temp;   
}

void move(int flag,Node *node){   //向四个方向扩展 
    int temp;
    if(flag==1&&node->x>0){	//靠右边，向左移动 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];	//n接收node矩阵 
            } 
        }
        
        n->a[node->x][node->y]=node->a[node->x-1][node->y];	//左移 
        n->a[node->x-1][node->y]=0; //对应位置置0，即置空 
        n->x=node->x-1;	//改变x的值 
        n->y=node->y;	//改变y的值 
        n->flag=3; 	//左边
        n->father=node;
        n->g=node->g+1;             //  求 g（） 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;	//扩展的节点 
        open_node_cnt++;
        open[open_cnt].np=n;     //添加到open表
        open[open_cnt].f=n->f;  //  求 f（） 
        
    }else if(flag==2&&node->y<2){	//靠下，上移 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];
            }
        }
        
        n->a[node->x][node->y]=node->a[node->x][node->y+1];	//下移
        n->a[node->x][node->y+1]=0;
        n->x=node->x;
        n->y=node->y+1;
        n->flag=4; 
        n->father=node;
        n->g=node->g+1;             //  求 g（） 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;
        open_node_cnt++;
        open[open_cnt].np=n;        //添加到open表
        open[open_cnt].f=n->f;  //  求 f（）
        
    }else if(flag==3&&node->x<2){	//靠左边，向右移 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];
            }
        }
        
        n->a[node->x][node->y]=node->a[node->x+1][node->y];	//右移
        n->a[node->x+1][node->y]=0; 
        n->x=node->x+1;
        n->y=node->y;   
        n->flag=1; 
        n->father=node;
        n->g=node->g+1;             //  求 g（） 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;
        open_node_cnt++;
        open[open_cnt].np=n;        //添加到open表
        open[open_cnt].f=n->f;  //  求 f（） 
        
    }else if(flag==4&&node->y>0){	//靠上，下移 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];
            }
        }
        
        n->a[node->x][node->y]=node->a[node->x][node->y-1];
        n->a[node->x][node->y-1]=0;
        n->x=node->x;
        n->y=node->y-1;     
        n->flag=2; 
        n->father=node;
        n->g=node->g+1;             //  求 g（） 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;
        open_node_cnt++;
        open[open_cnt].np=n;        //添加到open表
        open[open_cnt].f=n->f;  //  求 f（） 
    }    
} 

void expand(Node *node){    //节点扩展    
    for(int i=1;i<5;i++){	//向4个方向扩展 
        if(i!=node->flag) move(i,node);
    }
}

int main(){

    input();
    open[0].np = &start;	//start放入open表 
    open_node_cnt=1;	//从开始状态 

    if(isable()){
        while(true){	//open表不为空 
            if(isend(open[0].np)){
            	printf("\n路径：\n");
                show(open[0].np);
                printf("Step=%d",num-1);
                break;
            }

            expand(open[0].np);//扩展最优节点的子节点 
            open[0].np=NULL;
            open[0].f=-1;
            open_node_cnt--; //open表数量-1 
            sort(open);   //open表排序
        }
    }else cout<<"目标状态不可达";
    system("pause");
}

