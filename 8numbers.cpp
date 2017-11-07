/* credit by WangMingxuan 2016210726
本程序利用A*算法解决八数码问题
采用队列结构，使用了数组而不是链表（因为不熟）
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct node
{ 
	char a[9];
	int g,f;
};//队列中每个元素的数据结构，a即当前状态字符串，g、f即为对应耗散值
int father[400000];//存放父节点
int distance[9][9]={0};
char target[10]="123804765";//目标字符串
void caldis(){//该函数用于计算距离
	for(int i=0;i<9;i++)
		for(int j=i;j<9;j++){
			distance[i][j]=(j/3-i/3)+abs(j%3-i%3);
			distance[j][i]=distance[i][j];
		}
}

int calculateh(char m[]){//该函数用于计算当前状态的评价函数h，采用课件中h2(n) = 将牌“不在位”的距离和
	char c;
	char* ptr1,*ptr2;
	int pos1,pos2,sum=0;
	for(c='1';c<'9';c++){
		char* ptr1 = strchr(target, c);  
        int pos1 = ptr1-target;
		char* ptr2 = strchr(m, c);  
        int pos2 = ptr2-m;
		sum+=distance[pos1][pos2];
	}
	return sum;
}
void insert(struct node *q,int first,int end,char ch[],int chg){//该函数对队列进行插入操作，q为队列，first和end为当前队列首尾指针，ch[]为待插入字符串，chg为ch[]的g耗散值
	int chf=calculateh(ch);
	int insertorno=0;
	for(int i=first+1;i<=end;i++){
		if(chf+chg<q[i].f){
			for(int j=i+1;j<=end+1;j++){
				q[j]=q[j-1];
				father[j]=father[j-1];
			}
			strcpy(q[i].a,ch);
			q[i].f=chf+chg;
			q[i].g=chg;
			father[i]=first;
			insertorno=1;
			break;
		}
	}
	if(!insertorno){
	  strcpy(q[end+1].a,ch);
      q[end+1].f=chf+chg;
      q[end+1].g=chg;
	  father[end+1]=first;
	}
}


const int position[4]={-3,-1,1,3};
void main(){
//	struct node q[5000];
	int stop=0;
	struct node *q=(struct node*)malloc(400000 * sizeof(struct node)); 
//	int *father=(int*)malloc(400000 * sizeof(int)); 
	char cha[10];
  //  char cha[10]="013824765";
	/*读入操作:*/
	char input[100],output[100];
	printf("请按提示键入输入文件名和输出文件名（包含路径及扩展名，如不键入路径则为默认路径），每个文件名请不要超过100个字符\n");
	printf("输入文件名:\n");
	gets(input);
	printf("输出文件名:\n");
	gets(output);
	FILE *infp,*outfp;
	if((infp=fopen(input,"rt"))==NULL){
        printf("\nCannot open file strike any key exit!");
        getchar();
        return;
    }
	int num;
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++){
			fscanf(infp,"%d",&num);
			cha[3*i+j]='0'+num;
		}
    cha[9]='\0';

	/*采用判断奇偶排列的数学方法先排除没有解的情况:*/
	int solution=0;
	for(int i=0;i<9;i++){
		int little=0;
	    if(cha[i]!='0')
		  for(int j=0;j<i;j++)
			if(cha[j]>cha[i])
				little++;

		solution+=little;
	}
	if(solution%2==0){
		outfp = fopen (output,"w");
	    fprintf(outfp,"no solution");
		return;
	}

	caldis();

    int first=0,end=0,chg=0;
	strcpy(q[0].a,cha);
	q[0].g=0;
	q[0].f=calculateh(cha);//初始状态入队列
	while(first<=end){
	  if(strcmp(q[first].a,target)==0){
			stop=first;
			break;
	  }//已到目标
	  char* ptr = strchr(q[first].a, '0');  
      int pos = ptr-q[first].a;
      for(int k=0;k<4;k++)
       if((pos+position[k]<9)&&(pos+position[k]>=0)&&(((pos+position[k])/3==pos/3)||((pos+position[k])%3==pos%3))){//可拓展状态判断
        strcpy(cha,q[first].a);
		char swap=cha[pos];
		cha[pos]=cha[pos+position[k]];
		cha[pos+position[k]]=swap;
		chg=q[first].g+1;//状态拓展完毕
		int abandon=0;
		for(int i=0;i<first;i++)
			if(strcmp(q[i].a,cha)==0){
				abandon=1;
				break;
			}//如果和队首指针以前的部分重叠，直接遗弃
		if(!abandon){
			int equinq=0;
			for(int i=first+1;i<=end;i++)
				if(strcmp(q[i].a,cha)==0){
				   equinq=1;
				   if(chg<q[i].g){
					   q[i].g=chg;
					   father[i]=first;
				   }
				   break;
				}//如果和队中待拓展节点重复，则进行耗散值g和父节点的更新
			if(!equinq){
				insert(q,first,end,cha,chg);
				end++;
			}//不重复则将新状态入队
		}
	   }
	  first++;//队首指针加1
	}

	int ans[500];
	int n=0;
	ans[n]=stop;
	while(stop!=0){
		n++;
		ans[n]=father[stop];
		stop=father[stop];
	}//记录解法路径
	/*输出：*/

	outfp = fopen (output,"w");
	fprintf(outfp,"%d\n\n",n);
	for(int i=n-1;i>=0;i--){
		for(int j=0;j<9;j++)
			if(j%3==2)
			    fprintf(outfp,"%d\n",q[ans[i]].a[j]-'0');
			else
				fprintf(outfp,"%d ",q[ans[i]].a[j]-'0');
		if(i!=0)
			fprintf(outfp,"\n");
	}
}