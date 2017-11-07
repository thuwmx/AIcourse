/* credit by WangMingxuan 2016210726
����������A*�㷨�������������
���ö��нṹ��ʹ�������������������Ϊ���죩
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
struct node
{ 
	char a[9];
	int g,f;
};//������ÿ��Ԫ�ص����ݽṹ��a����ǰ״̬�ַ�����g��f��Ϊ��Ӧ��ɢֵ
int father[400000];//��Ÿ��ڵ�
int distance[9][9]={0};
char target[10]="123804765";//Ŀ���ַ���
void caldis(){//�ú������ڼ������
	for(int i=0;i<9;i++)
		for(int j=i;j<9;j++){
			distance[i][j]=(j/3-i/3)+abs(j%3-i%3);
			distance[j][i]=distance[i][j];
		}
}

int calculateh(char m[]){//�ú������ڼ��㵱ǰ״̬�����ۺ���h�����ÿμ���h2(n) = ���ơ�����λ���ľ����
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
void insert(struct node *q,int first,int end,char ch[],int chg){//�ú����Զ��н��в��������qΪ���У�first��endΪ��ǰ������βָ�룬ch[]Ϊ�������ַ�����chgΪch[]��g��ɢֵ
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
	/*�������:*/
	char input[100],output[100];
	printf("�밴��ʾ���������ļ���������ļ���������·������չ�����粻����·����ΪĬ��·������ÿ���ļ����벻Ҫ����100���ַ�\n");
	printf("�����ļ���:\n");
	gets(input);
	printf("����ļ���:\n");
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

	/*�����ж���ż���е���ѧ�������ų�û�н�����:*/
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
	q[0].f=calculateh(cha);//��ʼ״̬�����
	while(first<=end){
	  if(strcmp(q[first].a,target)==0){
			stop=first;
			break;
	  }//�ѵ�Ŀ��
	  char* ptr = strchr(q[first].a, '0');  
      int pos = ptr-q[first].a;
      for(int k=0;k<4;k++)
       if((pos+position[k]<9)&&(pos+position[k]>=0)&&(((pos+position[k])/3==pos/3)||((pos+position[k])%3==pos%3))){//����չ״̬�ж�
        strcpy(cha,q[first].a);
		char swap=cha[pos];
		cha[pos]=cha[pos+position[k]];
		cha[pos+position[k]]=swap;
		chg=q[first].g+1;//״̬��չ���
		int abandon=0;
		for(int i=0;i<first;i++)
			if(strcmp(q[i].a,cha)==0){
				abandon=1;
				break;
			}//����Ͷ���ָ����ǰ�Ĳ����ص���ֱ������
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
				}//����Ͷ��д���չ�ڵ��ظ�������к�ɢֵg�͸��ڵ�ĸ���
			if(!equinq){
				insert(q,first,end,cha,chg);
				end++;
			}//���ظ�����״̬���
		}
	   }
	  first++;//����ָ���1
	}

	int ans[500];
	int n=0;
	ans[n]=stop;
	while(stop!=0){
		n++;
		ans[n]=father[stop];
		stop=father[stop];
	}//��¼�ⷨ·��
	/*�����*/

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