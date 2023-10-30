#include<iostream>
#include<stdio.h>
#include<cmath> 
using namespace std;
int open_cnt=0; //��¼open����ÿһ����չ�Ľڵ� 
int open_node_cnt;//open��ڵ���� 
int num=0;//��¼�ƶ����� 

struct Node{
    int a[3][3];
    int x,y;
    int f,g,h;	//����ʽ�����ĺ��� 
    int flag; //��һ���ƶ����� 
    Node *father;
}start,end;


struct Open_Close{
    int f;
    Node *np;
}open[10000],close[10000];


bool isable(){/*�ж��Ƿ��н⣬������֮����ż����ͬ���н�  
				��F(X)��ʾ����Xǰ�����С�����ĸ�����ȫ�����ֵ�F(X)֮��ΪY=��(F(X))�����YΪ�������ԭ���ֵ������������У����YΪż�����ԭ���ֵ�������ż���С�
				*/ 

    int s[9],e[9];
    int tf=0,ef=0;
    int k=0;
    for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
            s[k]=start.a[i][j];	//����ʼ״̬����s[]������ 
            e[k]=end.a[i][j];	//��Ŀ��״̬����e[]������  
            k++;
        }
    }

    for(int i=0;i<9;i++){
        for(int j=0;j<i;j++){
            if(s[i]>s[j]&&s[j]!=0) tf+=1;	//��¼��Ӧλ����ǰ�����С �����ĸ���
            if(e[i]>e[j]&&e[j]!=0) ef+=1;
        }
    }
    
    if((tf%2==1&&ef%2==1)||(tf%2==0&&ef%2==0)) return true;	//��ż��ͬ�����н� 
    else return false;

}


int a_start_h(Node *node){  //�� h���� ����ÿ�����ֵ�ǰ״̬������״̬�������پ��� ֮����Ϊ���� 
    int old_x,old_y,end_x,end_y;
    int h=0;
    for(int k=1;k<9;k++){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(node->a[i][j]==k){	//�ҵ�ÿ�����ڶ������еĶ�Ӧλ�� 
                    old_x=i;
                    old_y=j;
                }
                if(end.a[i][j]==k){	//Ŀ��״̬�����������еĶ�Ӧλ�� 
                    end_x=i;
                    end_y=j;
                }
            }
        }

        h+=abs(old_x-end_x)+abs(old_y-end_y);	//�����Ŀ��״̬ƫ����֮�� 
    }   
    return h;
}


void input(){ 
	printf("�������ʼ״̬:\n");              //���� 
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cin>>start.a[i][j];
            if(start.a[i][j]==0){	//��¼0λ�ã�����λ�� 
                start.x=i;
                start.y=j;
            }
        }
    }
	printf("������Ŀ��״̬:\n"); 
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            cin>>end.a[i][j];
            if(end.a[i][j]==0){	//��¼0λ�ã�����λ�� 
                end.x=i;
                end.y=j;
            }
        }
    }
    start.g=0;	//����g 
    start.h=a_start_h(&start);	//����h 
    start.f=start.g+start.h;	//����f
}


int show(Node *node){    //��ʾ 
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


bool isend(Node *node){         //�ж��Ƿ�ΪĿ��ڵ� 
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(node->a[i][j]!=end.a[i][j])	//��Ӧλ��Ԫ�ز�����Ŀ��״̬����������false 
                return false;
        }
    }
    return true;
} 


void sort(Open_Close *open){      //open������ 
    int min=99999,min_flag=0; 
    Open_Close temp;
    for(int i=0;i<=open_cnt;i++){
        if(min>open[i].f&&open[i].f>0){	//�ҳ���С�ڵ� 
            min=open[i].f;
            min_flag=i;	
        }
    }
    
    temp=open[min_flag];	//�������ӹ�ϵ 
    open[min_flag]=open[0];
    open[0]=temp;   
}

void move(int flag,Node *node){   //���ĸ�������չ 
    int temp;
    if(flag==1&&node->x>0){	//���ұߣ������ƶ� 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];	//n����node���� 
            } 
        }
        
        n->a[node->x][node->y]=node->a[node->x-1][node->y];	//���� 
        n->a[node->x-1][node->y]=0; //��Ӧλ����0�����ÿ� 
        n->x=node->x-1;	//�ı�x��ֵ 
        n->y=node->y;	//�ı�y��ֵ 
        n->flag=3; 	//���
        n->father=node;
        n->g=node->g+1;             //  �� g���� 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;	//��չ�Ľڵ� 
        open_node_cnt++;
        open[open_cnt].np=n;     //��ӵ�open��
        open[open_cnt].f=n->f;  //  �� f���� 
        
    }else if(flag==2&&node->y<2){	//���£����� 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];
            }
        }
        
        n->a[node->x][node->y]=node->a[node->x][node->y+1];	//����
        n->a[node->x][node->y+1]=0;
        n->x=node->x;
        n->y=node->y+1;
        n->flag=4; 
        n->father=node;
        n->g=node->g+1;             //  �� g���� 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;
        open_node_cnt++;
        open[open_cnt].np=n;        //��ӵ�open��
        open[open_cnt].f=n->f;  //  �� f����
        
    }else if(flag==3&&node->x<2){	//����ߣ������� 
        Node *n = new Node();
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                n->a[i][j]=node->a[i][j];
            }
        }
        
        n->a[node->x][node->y]=node->a[node->x+1][node->y];	//����
        n->a[node->x+1][node->y]=0; 
        n->x=node->x+1;
        n->y=node->y;   
        n->flag=1; 
        n->father=node;
        n->g=node->g+1;             //  �� g���� 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;
        open_node_cnt++;
        open[open_cnt].np=n;        //��ӵ�open��
        open[open_cnt].f=n->f;  //  �� f���� 
        
    }else if(flag==4&&node->y>0){	//���ϣ����� 
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
        n->g=node->g+1;             //  �� g���� 
        n->h=a_start_h(n);
        n->f=n->g+n->h;
        open_cnt++;
        open_node_cnt++;
        open[open_cnt].np=n;        //��ӵ�open��
        open[open_cnt].f=n->f;  //  �� f���� 
    }    
} 

void expand(Node *node){    //�ڵ���չ    
    for(int i=1;i<5;i++){	//��4��������չ 
        if(i!=node->flag) move(i,node);
    }
}

int main(){

    input();
    open[0].np = &start;	//start����open�� 
    open_node_cnt=1;	//�ӿ�ʼ״̬ 

    if(isable()){
        while(true){	//open��Ϊ�� 
            if(isend(open[0].np)){
            	printf("\n·����\n");
                show(open[0].np);
                printf("Step=%d",num-1);
                break;
            }

            expand(open[0].np);//��չ���Žڵ���ӽڵ� 
            open[0].np=NULL;
            open[0].f=-1;
            open_node_cnt--; //open������-1 
            sort(open);   //open������
        }
    }else cout<<"Ŀ��״̬���ɴ�";
    system("pause");
}

