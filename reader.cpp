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
int Hash(char arr[])
{
	int sum=0,i=0;
	while(arr[i]!=-1)
		sum=(sum<<1)+arr[i++];
	return sum;

}
void prepare(int *index,char *arr,int number)
{
	while(number)
	{
		arr[(*index)++]=number%10;
		number/=10;
	}		
}
int Hash_generate(int p1,int p2,int p3,int p4)
{
	char string[41];
	int i=0;
	prepare(&i,string,p1);				
	prepare(&i,string,p2);								
	prepare(&i,string,p3);								
	prepare(&i,string,p4);								
	string[i]=-1;
	return Hash(string);
}
int verify(int EPC,int SQN,int K,int M1,int M2,int r1)
{
	int r2=M1^(Hash_generate(EPC,SQN,0,0));
	printf("!! %d %d\n",M2,Hash_generate(EPC,r1,r2,K));
	printf("r2=%d",r2);
	if(M2 == Hash_generate(EPC,r1,r2,K)  )
		return 1;
	else 
		return 0;
}
void Restart()
{
	printf("要重新開始嗎1/0 ");
	int mode;
	FILE *message;
	scanf("%d",&mode);
	if(mode)
	{
		message=fopen("message.txt","w");
		fprintf(message,"1");
		fclose(message);
	}
}
void Write_to_database(node *root)
{
	FILE *file=fopen("database.txt","w");
	int i=0,max=1;
	node *queue[1000];
	node *now;
	queue[0]=root;
				
	while(i!=max)
	{
		now=queue[i];
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
int main()
{
	srand(time(NULL));
	int r1,r2;
	int M1,M2,M3;	
	int step;
	FILE *message;
	
	
	//讀取資料庫 
	node *root=(node*)malloc(sizeof(node));	
	node *now;
	int empty=1;
	root->parent=NULL;
	root->left=NULL;
	root->right=NULL;		
	FILE *file=fopen("database.txt","r");
	int Si,EPCi,SQNi,Koldi,Knewi;
	while(fscanf(file," %d%d%d%d%d",&Si,&EPCi,&SQNi,&Knewi,&Koldi)==5)
	{		
		now=root;
		if(empty==0)	
			Newnode(Si,&now);
		now->S=Si;
		now->EPC=EPCi;
		now->SQN=SQNi;
		now->Knew=Knewi;
		now->Kold=Koldi;
		now->left=NULL;
		now->right=NULL;
		empty=0;
	}
	fclose(file);
	
	
	while(1)
	{
		system("pause");
		message=fopen("message.txt","r");
		if(message)
		{
			fscanf(message,"%d",&step);
			if(step==1)
			{
				r1=rand();
				fclose(message);
				message=fopen("message.txt","w");
				fprintf(message,"2\n%d",r1);
				fclose(message);
				printf("step 1:送出r1給tag   r1=%d\n",r1);
				//step 1
			}
			else if(step==3)
			{
				int S,EPC,SQN,Kold,Knew;

				fscanf(message,"%d%d%d",&S,&M1,&M2);
				fclose(message);
				printf("step 3:收到 S M1 M2  S=%d M1=%d M2=%d\n",S,M1,M2);
				now=root;
				if(Findnode(S,&now)==0)
				{
					printf("在資料庫中找不到該tag\n");
					Restart();
				}
				else
				{
					printf("D:%d %d %d %d %d\n",now->S,now->EPC,now->SQN,now->Knew,now->Kold);
					if(verify(now->EPC,now->SQN,now->Knew,M1,M2,r1)==0) 
					{
						if(verify(now->EPC,(now->SQN-1),now->Kold,M1,M2,r1)==0)
						{
							printf("tag認證失敗\n");
							Restart();
						}
						else
						{
							r2=M1^(Hash_generate(now->EPC,(now->SQN-1),0,0));
							M3=Hash_generate(now->EPC,r2,(now->SQN-1),now->Kold);
							now->Knew=Hash_generate(now->Kold,r2,0,0);
							
							message=fopen("message.txt","w");
							fprintf(message,"4\n%d",M3);
							fclose(message);
							printf("tag認證成功\(Kold) 送出M3  M3=%d\n",M3);
							
							Write_to_database(root);
						}
					}
					else
					{
						r2=M1^(Hash_generate(now->EPC,now->SQN,0,0));
						now->SQN++;
						M3=Hash_generate(now->EPC,r2,now->SQN,now->Knew);						
						now->Kold=now->Knew;
						now->Knew=Hash_generate(now->Knew,r2,0,0);
						now->S=Hash_generate(now->EPC,now->SQN,0,0);
						
						message=fopen("message.txt","w");
						fprintf(message,"4\n%d",M3);
						fclose(message);
						printf("tag認證成功\(Knew) 送出M3  M3=%d\n",M3);
						
						Write_to_database(root);
						
					}
					
				}
				
				
			}
			
		}
		else
		{
			r1=rand();
			fclose(message);
			message=fopen("message.txt","w");
			fprintf(message,"2\n%d",r1);
			printf("step 1:送出r1給tag   r1=%d\n",r1);
			fclose(message);
			
			//step 1
		}
		
	}
	return 0;
}
