#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h> 
#define MAX 50
#define pop 30
struct element{     //用来排序的数据结构   
    int data;  // 数据   
    int index;  // 序号   
}; 
int cmp(const void *a,const void *b){   // 升序排序  
    return((struct element*)a)->data - ((struct element*)b)->data;  
} 
void rand_of_n(int a[],int n){//用于产生1-n之间的随机数
    int i;  
    struct element ele[pop*MAX];  
    //srand((int)time(0));  // 初始化随机数种子   
    for(i=0;i<n;i++){  
        ele[i].data=rand();  // 随机生成一个数   
        ele[i].index=i;  
    }  
    qsort(ele,n,sizeof(ele[0]),cmp);  //排序   
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
int *cross(int father[],int mother[],int n);//交叉过程，返回两个子代合并的数组
int *mutation1(int father[],int n);//基于次序的变异过程
int *mutation2(int father[],int n);//打乱的变异过程
double *linearspeedup(double f[],int N);//线性加速适应函数变换
void main(){
	char input[100],output[100];
	printf("请按提示键入输入文件名和输出文件名（包含路径及扩展名，如不键入路径则为默认路径），每个文件名请不要超过100个字符\n");
	printf("输入文件名:\n");
	gets(input);
	printf("输出文件名:\n");
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
	}//读入
	fclose(infp);
	double d[MAX][MAX]={0};
	for(int i=0;i<n;i++)
		for(int j=i+1;j<n;j++){
			d[i][j]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
			d[j][i]=d[i][j];
		}//计算距离
    int N=pop*n;//种群数量
	double Pc=0.95,Pm=0.03;//交叉概率和变异概率
	srand((int)time(0));

	int a[pop*MAX]={0};
	int p[pop*MAX][MAX]={0};
	int p0[pop*MAX][MAX]={0};

	for(int i=0;i<n;i++) a[i]=i; 
	for(int i=0;i<N;i++) {
		rand_of_n(a,n);
		for(int j=0;j<n;j++)
			p0[i][j]=a[j];
	}//随机生成初始种群p0
	double f[pop*MAX],f0[pop*MAX];
	double minf=1.0e308;
	int minpath[MAX]={0};
	for(int i=0;i<1000*n;i++){//代数
		for(int j=0;j<N;j++){//每个染色体
			f0[j]=calf(p0[j],d,n);//用路径真实值作适应函数

			if(f0[j]<minf){
				minf=f0[j];
				for(int k=0;k<n;k++)
					minpath[k]=p0[j][k];
			}
		}
		double sumf=0;
		double *ff=linearspeedup(f0,N);//线性加速适应函数
		for(int j=0;j<N;j++)
			sumf+=ff[j];
		for(int j=0;j<N;j++){//轮盘赌方法选择新的种群
			double ran=(double)rand()/RAND_MAX;
			double sump=0;
			for(int k=0;k<N;k++){
				if((ran>=sump)&&(ran<=sump+ff[k]/sumf)){//选中第k个染色体
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
		for(int j=0;j<N;j++){//交叉、变异产生新种群
		    if(j%2==0){
				double pcross=(double)rand()/RAND_MAX;
				int *son=(int *)malloc(2*n*sizeof(int));
		        if(pcross<Pc){//依据交叉概率pc从种群中选择染色体进行交叉
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
		    if(pmutation<Pm){//依据变异概率pm从种群中选择染色体进行变异
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
int *cross(int father[],int mother[],int n){//采用常规交叉法
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
//int *cross(int father[],int mother[],int n){//采用基于部分映射的交叉法
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
int *mutation1(static int father[],int n){///基于次序的变异法
	int u=rand()%n;
	int v=rand()%n;
	int sw=father[u];
	father[u]=father[v];
	father[v]=sw;
	return father;
}
int *mutation2(static int father[],int n){///打乱变异法
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
