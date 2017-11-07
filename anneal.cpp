#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h>  

double calt0(){
	return 280.0;
}
double calf(int state[],double dis[][100],int n){
	double sum=0;
	for(int i=0;i<n-1;i++){
		sum+=dis[state[i]][state[i+1]];
	}
	sum+=dis[state[n-1]][state[0]];
	return sum;
}
void main(){
	char input[100],output[100];
	printf("�밴��ʾ���������ļ���������ļ���������·������չ�����粻����·����ΪĬ��·������ÿ���ļ����벻Ҫ����100���ַ�\n");
	printf("�����ļ���:\n");
	gets(input);
	printf("����ļ���:\n");
	gets(output);
	int n;
	FILE *infp,*outfp;
	if((infp=fopen(input,"rt"))==NULL){
        printf("\nCannot open file strike any key exit!");
        getchar();
        return;
    }
	fscanf(infp,"%d",&n);
	fgetc(infp);
	char path[100];
	double x[100],y[100];
	for(int i=0;i<n;i++){
		fscanf(infp,"%c",&path[i]);
		fscanf(infp,"%lf",&x[i]);
		fscanf(infp,"%lf",&y[i]);
		fgetc(infp);
	}//����
	fclose(infp);
	outfp = fopen (output,"w");
	double d[100][100]={0};
	for(int i=0;i<n;i++)
		for(int j=i+1;j<n;j++){
			d[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
			d[j][i]=d[i][j];
		}//�������
	double t=calt0();//��ʼ�¶��趨���������ù̶�ֵ280
	int s[100]={0};
	for(int i=0;i<n;i++)
		s[i]=i;//s�����ʾ��ǰ״̬
	double f0=calf(s,d,n);
	double f=0;
    double P=0,ran=0;
	srand((int)time(0));
	while(abs(f-f0)>1e-6){//�㷨����������Ϊ�����¶��������������
		f0=f;
		for(int i=0;i<100*n;i++){//ͬһ�¶��½���100n�ε�����������̶�������������
			int u=1+rand()%(n-1);
			int v=1+rand()%(n-1);//�̶�A��ֻ���n-1��
			int chan[100]={0},s2[100];
			//for(int j=u+1;j<v;j++)
			//	chan[j-u-1]=s[j];
			int j=u;
			int Nch=0;
			for(int j=0;j<n;j++)
				s2[j]=s[j];
			while(1){
				j++;
				if(j>n-1) j=1;
				if(j==v) break;
				Nch++;
				chan[Nch]=s[j];
			}//�������������������ɷ�����ͷβ������������A��
			j=u;
		    int h=0;
			while(1){
				j++;
				if(j>n-1) j=1;
				if(j==v) break;
				s2[j]=chan[Nch-h];
				h++;
			 }
			//int aa=v-1,bb=u+1;
			//if(aa<0) aa=n-1;
			//if(bb>n-1) bb=0;
			//double delf=d[s[u]][s[aa]]+d[s[bb]][s[v]]-d[s[u]][s[bb]]-d[s[aa]][s[v]];
			double delf=calf(s2,d,n)-calf(s,d,n);
			if(delf<0){//��ʱһ������
				j=u;
				while(1){
				  j++;
				  if(j>n-1) j=1;
				  if(j==v) break;
				  s[j]=s2[j];
			    }
			}
			else{//��ʱ����һ�����ܸ���
				P=exp(-delf/t);
				ran=(double)rand()/RAND_MAX;
				if (ran<P){
					j=u;
				    while(1){
				      j++;
				      if(j>n-1) j=1;
				      if(j==v) break;
				      s[j]=s2[j];
			        }
				}
			}
		}
		f=calf(s,d,n);
		for(int i=0;i<n;i++)
			fprintf(outfp,"%c ",path[s[i]]);
		fprintf(outfp,"%f\n",f0);
		t=t*0.92;//�¶��Թ̶������½�
	}


}