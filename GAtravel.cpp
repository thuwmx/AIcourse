#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h> 
#define MAX 50
#define pop 30
struct element{     //������������ݽṹ   
    int data;  // ����   
    int index;  // ���   
}; 
int cmp(const void *a,const void *b){   // ��������  
    return((struct element*)a)->data - ((struct element*)b)->data;  
} 
void rand_of_n(int a[],int n){//���ڲ���1-n֮��������
    int i;  
    struct element ele[pop*MAX];  
    //srand((int)time(0));  // ��ʼ�����������   
    for(i=0;i<n;i++){  
        ele[i].data=rand();  // �������һ����   
        ele[i].index=i;  
    }  
    qsort(ele,n,sizeof(ele[0]),cmp);  //����   
    for(i=0;i<n;i++){  
        a[i]=ele[i].index;  
    }
}
double calf(int state[],double dis[][MAX],int n){
	double sum=0;
	for(int i=0;i<n-1;i++){
		sum+=dis[state[i]][state[i+1]];
	}
	sum+=dis[state[n-1]][state[0]];
	return sum;
}
int *cross(int father[],int mother[],int n);//������̣����������Ӵ��ϲ�������
int *mutation1(int father[],int n);//���ڴ���ı������
int *mutation2(int father[],int n);//���ҵı������
double *linearspeedup(double f[],int N);//���Լ�����Ӧ�����任
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
	char path[MAX];
	double x[MAX],y[MAX];
	for(int i=0;i<n;i++){
		fscanf(infp,"%c",&path[i]);
		fscanf(infp,"%lf",&x[i]);
		fscanf(infp,"%lf",&y[i]);
		fgetc(infp);
	}//����
	fclose(infp);
	double d[MAX][MAX]={0};
	for(int i=0;i<n;i++)
		for(int j=i+1;j<n;j++){
			d[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
			d[j][i]=d[i][j];
		}//�������
    int N=pop*n;//��Ⱥ����
	double Pc=0.95,Pm=0.03;//������ʺͱ������
	srand((int)time(0));

	int a[pop*MAX]={0};
	int p[pop*MAX][MAX]={0};
	int p0[pop*MAX][MAX]={0};

	for(int i=0;i<n;i++) a[i]=i; 
	for(int i=0;i<N;i++) {
		rand_of_n(a,n);
		for(int j=0;j<n;j++)
			p0[i][j]=a[j];
	}//������ɳ�ʼ��Ⱥp0
	double f[pop*MAX],f0[pop*MAX];
	double minf=1.0e308;
	int minpath[MAX]={0};
	for(int i=0;i<1000*n;i++){//����
		for(int j=0;j<N;j++){//ÿ��Ⱦɫ��
			f0[j]=calf(p0[j],d,n);//��·����ʵֵ����Ӧ����

			if(f0[j]<minf){
				minf=f0[j];
				for(int k=0;k<n;k++)
					minpath[k]=p0[j][k];
			}
		}
		double sumf=0;
		double *ff=linearspeedup(f0,N);//���Լ�����Ӧ����
		for(int j=0;j<N;j++)
			sumf+=ff[j];
		for(int j=0;j<N;j++){//���̶ķ���ѡ���µ���Ⱥ
			double ran=(double)rand()/RAND_MAX;
			double sump=0;
			for(int k=0;k<N;k++){
				if((ran>=sump)&&(ran<=sump+ff[k]/sumf)){//ѡ�е�k��Ⱦɫ��
					for(int m=0;m<n;m++)
					    p[j][m]=p0[k][m];
					f[j]=f0[k];
					break;
				}
				sump+=ff[k]/sumf;
			}
		}		
		for(int j=0;j<N;j++) a[j]=j; 
		rand_of_n(a,N);
		int p2[pop*MAX][MAX]={0};
		for(int j=0;j<N;j++){//���桢�����������Ⱥ
		    if(j%2==0){
				double pcross=(double)rand()/RAND_MAX;
				int *son=(int *)malloc(2*n*sizeof(int));
		        if(pcross<Pc){//���ݽ������pc����Ⱥ��ѡ��Ⱦɫ����н���
					son=cross(p[a[j]],p[a[j+1]],n);
					for(int k=0;k<n;k++){
						p2[j][k]=son[k];
						p2[j+1][k]=son[k+n];
					}
				}
				else
					for(int k=0;k<n;k++){
						p2[j][k]=p[a[j]][k];
						p2[j+1][k]=p[a[j+1]][k];
					}
			}
		    double pmutation=(double)rand()/RAND_MAX;
		    if(pmutation<Pm){//���ݱ������pm����Ⱥ��ѡ��Ⱦɫ����б���
				int *son=(int *)malloc(n*sizeof(int));
				son=mutation2(p2[j],n);
				for(int k=0;k<n;k++)
					p0[j][k]=son[k];
			}
			else{
				for(int k=0;k<n;k++)
					p0[j][k]=p2[j][k];
			}
		}

	}
    outfp = fopen (output,"w");
	for(int i=0;i<n;i++)
		fprintf(outfp,"%c ",path[minpath[i]]);
	fprintf(outfp,"%f\n",minf);
}
int *cross(int father[],int mother[],int n){//���ó��潻�淨
	static int son[2*MAX];
	int u=rand()%n;
	int sign1[MAX]={0},sign2[MAX]={0};
	for(int i=0;i<n;i++){
		if(i<u){
			son[i]=father[i];
			sign1[son[i]]=1;
			son[i+n]=mother[i];
			sign2[son[i+n]]=1;
		}
		else{
			for(int j=0;j<n;j++)
				if(!sign1[mother[j]]){
					son[i]=mother[j];
					sign1[mother[j]]=1;
					break;
				}
			for(int j=0;j<n;j++)
				if(!sign2[father[j]]){
					son[i+n]=father[j];
					sign2[father[j]]=1;
					break;
				}
		}
	}
	return son;
}
//int *cross(int father[],int mother[],int n){//���û��ڲ���ӳ��Ľ��淨
//	static int son[2*MAX];
//	int sign[MAX];
//	int u=rand()%n;
//	int v=rand()%n;
//	if(u>v){
//		int s=v;
//		v=u;
//		u=s;
//	}
//	for(int i=0;i<n;i++)
//		sign[i]=i;
//	for(int i=u;i<=v;i++){
//		sign[father[i]]=mother[i];
//		sign[mother[i]]=father[i];
//	}
//	for(int i=0;i<n;i++){
//		if((i>=u)&&(i<=v)){
//			son[i]=mother[i];
//			son[i+n]=father[i];
//		}
//		else{
//			son[i]=sign[father[i]];
//			son[i+n]=sign[mother[i]];
//		}
//	}
//	return son;
//}
double *linearspeedup(double f[],int N){
	double sum=0,max=0;
	for(int i=0;i<N;i++){
		sum+=1/f[i];
		if(1/f[i]>max) max=1/f[i];
	}
	double M=5.0;
	double b=sum/N;
	double c=max;
	double alpha=(1-M)*b/(b-c);
	double belta=b-(1-M)*b*b/(b-c);
	static double ff[pop*MAX];
	for(int i=0;i<N;i++)
		ff[i]=alpha/f[i]+belta;
	return ff;
}
int *mutation1(static int father[],int n){///���ڴ���ı��취
	int u=rand()%n;
	int v=rand()%n;
	int sw=father[u];
	father[u]=father[v];
	father[v]=sw;
	return father;
}
int *mutation2(static int father[],int n){///���ұ��취
	int u=rand()%n;
	int v=rand()%n;
	if(u>v){
		int s=v;
		v=u;
		u=s;
	}
	int temp[MAX]={0};
	for(int i=u;i<=v;i++)
	    temp[i-u]=father[i];
	int lengthte=v-u+1;
	for(int i=u;i<=v;i++){
		int c=rand()%lengthte;
		father[i]=temp[c];
		for(int j=c;j<lengthte-1;j++)
			temp[j]=temp[j+1];
		lengthte--;
	}
	return father;
}
