#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

int main()
{	
	srand(time(NULL));
	int r1,r2;
	int step;
	int M1,M2,M3;
	int S,EPC,SQN,K;
	FILE *message,*tagdata;
	
	//tag的資料要先寫在 tagdata.txt
	//資料要包含 EPC SQN K 
	while(!(tagdata=fopen("tagdata.txt","r")))
	{
		printf("沒有tag的資料\n");
		system("pause");
	}
	fscanf(tagdata,"%d%d%d",&EPC,&SQN,&K);
	S=Hash_generate(EPC,SQN,0,0);
	fclose(tagdata);
	
	
	while(1)
	{
		system("pause");
		printf("%d\n",r2);
		message=fopen("message.txt","r");
		if(message)
		{
			fscanf(message,"%d",&step);
			if(step==2)
			{				
				fscanf(message,"%d",&r1);
				printf("step 2: tag收到 r1=%d\n",r1);
				r2=rand();
				M1=Hash_generate(EPC,SQN,0,0)^r2;
				M2=Hash_generate(EPC,r1,r2,K);
				fclose(message);
				message=fopen("message.txt","w");
				fprintf(message,"3\n%d %d %d",S,M1,M2);	
				fclose(message);
				
				printf("送出S M1 M2給reader S=%d M1=%d M2=%d\n",S,M1,M2);
			}
			else if(step==4)
			{
				fscanf(message,"%d",&M3);
				printf("step 4: tag收到 M3=%d\n",M3);
				if(M3==Hash_generate(EPC,r2,SQN+1,K))
				{
					printf("tag認證成功\\n");
					SQN++;
					K=Hash_generate(K,r2,0,0);
					S=Hash_generate(EPC,SQN,0,0);
					
					tagdata=fopen("tagdata.txt","w");
					fprintf(tagdata,"%d %d %d",EPC,SQN,K);
					
					message=fopen("message.txt","w");
					fprintf(message,"1");	
					fclose(message);
				}
				else
				{
					printf("tag認證失敗\n");
				}
			}
		}
		else
		{
			printf("等待reader發送請求");
		}
	}
	return 0;
}
