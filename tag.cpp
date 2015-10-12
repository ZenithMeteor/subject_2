#include<stdio.h>
#include<stdlib.h>
#include<time.h>
typedef struct database
{
	short int h;
	short int Knew;
	short int Kold;
	short int EPC;
	short int SQN;
};
short int CRC(short int *p, int len)
{
  int crc=0,i=0;
  while(len)
  {
    crc+=*(p+i);
    crc%=137;
    crc<<=16;
    len--;
    i++;
  }
  crc>>=8;
  return (short int)crc;
}
void Hash()
{
	return;
}
short int Inc(short int input)
{
	++input;
	return input;
}
short int Dec(short int input)
{
	--input;
	return input;
}
int main()
{
	short int EPC_tag,SQN_tag;//tag資料
	int h_tag,k_tag;//tag資料
	FILE *rdatabase=fopen("database.txt","r");
	FILE *connect=fopen("connect.txt","r");
    short int r1,r2;//亂數
	char mode;
	database data[100];
	for(int i=0;i<100;i++)
	{
		fscanf(rdatabase,"%hd%hd%hd%hd%hd",&data[i].h,&data[i].Knew,&data[i].Kold,&data[i].EPC,&data[i].SQN);
	}
	if(connect!=NULL)
        fscanf(connect,"%c",&mode);

    if(mode=='s')
    {

        fscanf(connect," %c",&mode);
        if(mode=='0')
        {
            fscanf(connect,"%d",&r1);

            scanf("%d",&mode);
            r2=rand();
            int index=rand()%100;
            EPC_tag=data[index].EPC;
            SQN_tag=data[index].SQN;
            k_tag=data[index].Knew;
            h_tag=data[index].h;
            if(mode==0)
            {
                k_tag=rand()%100;
            //printf("%hd %hd %hd %hd",EPC_tag,SQN_tag,k_tag,h_tag);
            }
            short int M1[3],M2[4];
            M1[0]=EPC_tag;
            M1[1]=SQN_tag;
            M1[2]=CRC(M1,2);
            M2[0]=EPC_tag;
            M2[1]=r1;
            M2[2]=r2;
            M2[3]=CRC(M2,3);
            M1[0]=M1[0]^r2;
            M2[0]=M2[0]^k_tag;
            FILE *tagmemory=fopen("tagmemory.txt","w");
            fclose(connect);
            connect=fopen("connect.txt","w");
            fprintf(tagmemory,"%hd %hd %hd %hd %hd",h_tag,k_tag,EPC_tag,SQN_tag,r2);
            fclose(tagmemory);
            fprintf(connect,"t %hd\n",h_tag);
            fprintf(connect,"%d ",3);
            for(int i=0;i<3;i++)
                fprintf(connect,"%hd ",M1[i]);
            fprintf(connect,"\n%d ",4);
            for(int i=0;i<4;i++)
                fprintf(connect,"%hd ",M2[i]);
            fclose(connect);
        }
        if(mode=='0')
        {
            FILE *tagmemory=fopen("tagmemory.txt","r");
            fscanf(tagmemory,"%hd %hd %hd %hd %hd",&h_tag,&k_tag,&EPC_tag,&SQN_tag,&r2);
            int M3len;
            fscanf(connect,"%d",&M3len);
            short int M3[M3len];
            short int M3_tag[M3len];
            for(int i=0;i<M3len;i++)
                fscanf(connect,"%hd",M3[i]);
            SQN_tag=Inc(SQN_tag);
            M3_tag[0]=EPC_tag;
            M3_tag[1]=r2;
            M3_tag[2]=SQN_tag;
            M3_tag[3]=CRC(M3_tag,3);
            M3_tag[0]=M3_tag[0]^k_tag;
            mode=1;
            for(int i=0;i<M3len;i++)
            {
                if(M3[i]!=M3_tag[i])
                {
                    mode=0;
                    break;
                }
            }
            if(mode)
            {
                printf("tag認證成功\\n");
                system("pause");
				//6.3
				Hash();
				Hash();
			}
			else
			{
				printf("tag認證失敗\n");
				SQN_tag=Dec(SQN_tag);
			}


        }

    }


	//
	/*
	int n;

	printf("資料數量：") ;
	scanf("%d",&n);
	database data[n];
	printf("輸入資料(h Knew Kold EPC SQN)\n");
	for(int i=0;i<n;i++)
	{
		scanf("%d%d%d%hd%hd",&data[i].h,&data[i].Knew,&data[i].Kold,&data[i].EPC,&data[i].SQN);

	}
	printf("輸入tag的資料(h k EPC SQN)");
	scanf("%d%d%hd%hd",&h_tag,&k_tag,&EPC_tag,&SQN_tag);


	srand(time(NULL));
	// reader
	{
		r1=rand();
		printf("r1:%d\n",r1);
	}
	//tag    收到 r1
	{
		long long int temp,p1=EPC_tag,p2=r1;
		r2=rand();

		p1=p1<<16;
		temp=p1+SQN_tag;
		temp=Hash(temp);
		M1=((!temp) | r2) & (temp | (!r2));

		p1=p1<<16;
		p2=p2<<16;
		temp=p1+p2+r2;
		temp=Hash(temp);
		M2=((!temp) | k_tag) & (temp | (!k_tag));
		printf("M1:%d M2:%d r2:%d\n",M1,M2,r2);
	}

	//server   收到  h_tag M1 M2
	{
		//4.1
		int i;
		for(i=0;data[i].h!=h_tag;i++);
		if(i==n)
		{
			printf("找不到tag\n");
		}
		else
		{
			// 4.2
			long long int temp,p1=data[i].EPC;
			int temp2;
			temp=p1<<16+data[i].SQN;
			temp2=Hash(temp);
			int r2_server=((!M1) | temp2) & (M1 | (!temp2));

			//4.3
			temp=(p1<<32)+(r1<<16)+r2_server;
			temp2=Hash(temp);
			temp2=((!temp2) | data[i].Knew ) & ( temp2 | (!data[i].Knew));

			if( M2== (( (!temp2) | (int)data[i].Knew ) & ( temp2 | (!(int)data[i].Knew) ) )  )
			{
				printf("server認證成功\ \n");
				//4.4
				data[i].SQN=Inc(data[i].SQN);
				//4.5
				temp=p1<<32+r2_server<<16+data[i].SQN;
				temp2=Hash(temp);
				M3=( (!temp2)| data[i].Knew) & (temp2 | (!data[i].Knew) );
				printf("M3:%d\n",M3);
				//4.6
				data[i].Kold=data[i].Knew;
				temp=data[i].Knew<<16+r2_server;
				data[i].Knew=Hash(temp);
				//4.7
				temp=p1<<16+data[i].SQN;
				data[i].h=Hash(temp);

			}
			else
			{
				printf("server認證失敗\n");
			}

		}
		//tag  收到 M3
		{
			//6.1
			long long int temp,p1=EPC_tag;
			int temp2;
			SQN_tag=Inc(SQN_tag);
			//6.2
			temp=p1<<32+r2<<16+SQN_tag;
			temp2=Hash(temp);
			if(M3 ==(  ( (!temp2) | k_tag) & ( temp2 | (!k_tag ) ) )  )
			{
				printf("tag認證成功\\n");
				//6.3
				temp=k_tag<<16+r2;
				k_tag=Hash(temp);
				//6.4
				temp=p1<<16+SQN_tag;
				h_tag=Hash(temp);
			}
			else
			{
				printf("tag認證失敗\n");
				SQN_tag=Dec(SQN_tag);
			}




		}

	}



	*/

	return 0;
}
/*
3
1 13 13 10 56
5 26 26 48 93
3 56 56 13 65
5 26 48 93
*/
