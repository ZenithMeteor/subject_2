#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct node
{
	int S;
	int EPC;
	int SQN;
	int Kold;
	int Knew;
	node *parent;
	node *left;
	node *right;
};
void prepare(int *index,char *arr,int number)
{
	while(number)
	{
		arr[(*index)++]=number%10;
		number/=10;
	}		
}
int Findnode(int S,node **now)
{
	while((*now)->S!=S)	
	{
		if((*now)->left!=NULL&&S<(*now)->S)
			(*now)=(*now)->left;
		else if((*now)->right!=NULL&&S>(*now)->S)
			(*now)=(*now)->right;
		else		
			return 0;		
	}
	return 1;
	
}
void Newnode(int S,node **now)
{
	int existence;
	existence=Findnode(S,now);	
	if(existence==0)
	{
		if(S<(*now)->S)
		{
			(*now)->left=(node*)malloc(sizeof(node));
			(*now)->left->parent=(*now);
			(*now)=(*now)->left;
		}
		else
		{
			(*now)->right=(node*)malloc(sizeof(node));
			(*now)->right->parent=(*now);
			(*now)=(*now)->right;
		}
			
			
	}
}
void Balance_tree(node *root)
{
	return;
}
int Hash(char arr[])
{
	int sum=0,i=0;
	while(arr[i]!=-1)
		sum=(sum<<1)+arr[i++];
	return sum;

}
int Calculate_S(int EPC,int SQN)
{
	char string[40];
	int i=0;
	prepare(&i,string,EPC);				
	prepare(&i,string,SQN);								
	string[i]=-1;
	return Hash(string);
}
void Node_value_assign(node *now,int EPC,int SQN,int Knew,int Kold)
{
	int S=Calculate_S(EPC,SQN);			
	now->SQN=SQN;
	now->EPC=EPC;
	now->Kold=Kold;
	now->Knew=Knew;
	now->S=S;
	now->left=NULL;
	now->right=NULL;
}
int main()
{
	int operation;
	char string[40];
	FILE *file;
	node *root=(node*)malloc(sizeof(node));	
	node *now;
	int empty=1;
	root->parent=NULL;
	root->left=NULL;
	root->right=NULL;
	
	srand(time(NULL));
	do
	{		
		printf("��ܫ��O\n");
		printf("1.���U1��tag\n");
		printf("2.�H������100��tag\n");
		printf("3.�j�Mtag\n");
		printf("4.�s��\n");
		printf("5.Ū��\n");
		printf("6.�p��S\n");
		scanf("%d",&operation);
		if(operation==1)
		{			
			int S,EPC,SQN,Kold,Knew;
			now=root;	
			
			printf("��JEPC SQN Knew Klod\n");
			scanf("%d%d%d%d",&EPC,&SQN,&Knew,&Kold);
			S=Calculate_S(EPC,SQN);
			
						
			//system("pause");
			if(empty==0)	
				Newnode(S,&now);
			
			Node_value_assign(now, EPC, SQN, Knew, Kold);
			empty=0;
						
		}
		else if(operation==2)
		{
			int S,EPC,SQN,Kold,Knew;
			for(int i=0;i<100;i++)
			{
				now=root;
				EPC=rand();
				SQN=rand();
				Kold=rand();
				Knew=rand();
				if(empty==0)				
					Newnode(S,&now);												
			
				Node_value_assign(now, EPC, SQN, Knew, Kold);
				empty=0;
			}
		}
		else if(operation==3)
		{
			printf("��Jtag��S\n");
			
			int S;
			scanf("%d",&S);
			now=root;
			if(Findnode(S,&now)==0)
			{
				printf("�䤣��tag\n");
			}
			else
			{
				printf("S=%d EPC=%d SQN=%d Knew=%d Kold=%d\n",S,now->EPC,now->SQN,now->Knew,now->Kold);
			}
		}
		else if(operation==4)
		{
			if(empty==0)
			{
				file=fopen("database.txt","w");
				int i=0,max=1;
				node *queue[1000];
				queue[0]=root;
				
				while(i!=max)
				{
					now=queue[i];
					//system("pause");
					//printf("%d %d %d %d %d\n",now->S,now->EPC,now->SQN,now->Knew,now->Kold);
					fprintf(file,"%d %d %d %d %d\n",now->S,now->EPC,now->SQN,now->Knew,now->Kold);
					if(now->left!=NULL)
					{
						queue[max]=now->left;
						max=(max+1)%1000;
					}
					if(now->right!=NULL)
					{
						queue[max]=now->right;
						max=(max+1)%1000;
					}
					i=(i+1)%1000;
				}
				fclose(file);
			}
			else
			{
				printf("�S��tag\n");
			}
		}
		else if(operation==5)
		{
			file=fopen("database.txt","r");
			if(file)
			{
				int S,EPC,SQN,Kold,Knew;
				while(fscanf(file,"%d%d%d%d%d",&S,&EPC,&SQN,&Knew,&Kold)==5)
				{
					now=root;
					if(empty==0)	
						Newnode(S,&now);
					Node_value_assign(now, EPC, SQN, Knew, Kold);
					now->S=S;
					empty=0;
				}
			}
			else
			{
				printf("�ɮפ��s�b\n");
			}
			
			
			fclose(file);
		}
		else if(operation==6)
		{
			printf("��JEPC SQN\n");
			int S=5,EPC,SQN;
			scanf("%d%d",&EPC,&SQN);
			S=Calculate_S(EPC,SQN);
			printf("S=%d\n",S);
		}
		
		
	}
	while(operation>=1&&operation<=6);
	return 0;
}
