#include"quest_head.h"

void print_userinfo(USER user)
{
	int i,year,month,mday,wday,yday;
	double temp;
	gettime(&year,&month,&mday,&wday,&yday);
	temp=(double)user.exp/(double)user.exp_max;
	temp*=100;
	printf("                         -----------------------------\n");
	printf("                               等级：%d\n",user.level);
	printf("                               经验：%d/%d\n",user.exp,user.exp_max);
	printf("                               [");
	for(i=10;i<=temp&&i<=100;i+=10)
		printf("=");
	for(;i<=100;i+=10)
		printf(" ");
	printf("]%.2f\n\n",temp);
	printf("                               时间：%4d-%02d-%02d\n",year,month,mday);
	printf("                         -----------------------------\n\n");
}

void print_quest(QUEST *head,int type)//type=1，未完成任务；type=2，已完成任务；type=3，所有任务
{
	int i,t=0;
	double temp;
	QUEST *p=NULL;
	p=head;
	while(p!=NULL){
		if((type==1&&p->finish==1)||(type==2&&p->finish!=1)){
			p=p->next;
			continue;
		}
		temp=(double)p->amount/(double)p->amount_max*100;
		printf	("--------------------------------------------------------------------------------\n");
		printf("内容：%s\n",p->content);
		printf("编号：%d     经验：%d     ",p->num,p->exp);
		if(p->finish==1)
			printf("[已完成]     ");
		else if(p->finish==0)
			printf("[未完成]     ");
		else
			printf("[已超期]     ");
		printf("完成进度：%d/%d     [",p->amount,p->amount_max);
		for(i=10;i<=temp&&i<=100;i+=10)
			printf("=");
		for(;i<=100;i+=10)
			printf(" ");
		printf("]%.2f\n",temp);
		if(p->finish==1)
			printf("完成时间：");
		else
			printf("截止时间：");
		printf("%d-%02d-%02d\n",p->ddl_year,p->ddl_month,p->ddl_mday);
		p=p->next;
		t++;
	}
	if(t==0)
		printf("无任务\n");
	else
		printf("--------------------------------------------------------------------------------\n\n");
}
		
int menu_main(void)
{
	int x;
	printf("--------------------------------------------------------------------------------\n");
	printf("     1.新建任务   2.修改任务   3.完成任务   4.删除任务   5.查看任务   6.退出\n");
	printf("--------------------------------------------------------------------------------\n\n");
	x=choice(1,6);
	return x;
}

