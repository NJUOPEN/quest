#include"quest_head.h"

int main(void)
{
	int x,y,z,i,j,k,temp;
	QUEST *p=NULL,*head=NULL;
	USER user;
	FILE *fp;
	user.level=0;
	user.exp=0;
	user.exp_max=50;
	head=load(DATA,&user);
loop0:	system("clear");
	print_userinfo(user);
	if(head==NULL){
		printf("未创建任务，新建任务请输入1，退出请输入0：\n");
		x=choice(0,1);
		if(x==1)
			goto loop1;
		if(x==0)
			exit(1);
	}
	head=check_quest(&user,head);
	system("clear");
	print_userinfo(user);
	x=menu_main();
	if(x==1){
loop1:		p=(QUEST *)malloc(sizeof(QUEST));
		printf("请输入任务内容，返回请输入0：\n");
		scanf("%90s",p->content);
//		fflush(stdin);
		setbuf(stdin,NULL);
		if(strcmp(p->content,"0")==0){
			free(p);
			goto loop0;
		}
		printf("请输入任务经验值：\n");
		p->exp=choice(0,1000000000);
		printf("请输入需完成数量：\n");
		p->amount_max=choice(1,1000000000);
		p->amount=0;
		p->num=check_num(head)+1;
		p->finish=0;
		printf("请选择任务类型：\n1.每天  2.每周  3.每月  4.每年  5.仅一次\n");
		y=choice(1,5);
		p->type=y;
		if(y<5){
			gettime(&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
			z=judge_day_amount(p->ddl_year,p->ddl_month,2);
			k=judge_day_amount(p->ddl_year,p->ddl_month,1);
			if(y==2){
				printf("1.每周一  2.每周二  3.每周三  4.每周四  5.每周五  6.每周六  7.每周日\n");
				x=choice(1,7);
				if(x<p->ddl_wday){
					p->ddl_mday+=7-p->ddl_wday+x;
					p->ddl_yday+=7-p->ddl_wday+x;
				}
				else{
					p->ddl_mday+=x-p->ddl_wday;
					p->ddl_yday+=x-p->ddl_wday;
				}
				if(p->ddl_yday>z){
					p->ddl_year++;
					p->ddl_month=1;
					p->ddl_mday=p->ddl_yday-z;
				}
				else if(p->ddl_mday>k){
					p->ddl_month++;
					p->ddl_mday-=k;
				}
			}
			if(y==3)
				p->ddl_mday=k;
			if(y==4){
				p->ddl_month=12;
				p->ddl_mday=31;
			}
		}
		else{
			printf("请输入任务截止年：\n");
			p->ddl_year=choice(0,1000000000);
			printf("请输入任务截止月：\n");
			p->ddl_month=choice(1,12);
			printf("请输入任务截止日：\n");
			p->ddl_mday=choice(1,judge_day_amount(p->ddl_year,p->ddl_month,1));
		}
		get_wyday(p->ddl_year,p->ddl_month,p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
		head=insert(head,p);
		printf("新建任务成功！\n");
		y=save(DATA,user,head);
		if(y==0)
			goto loop0;
		printf("继续创建任务请输入1，返回请输入0\n");
		z=choice(0,1);
		if(z==1)
			goto loop1;
		if(z==0)
			goto loop0;
	}
	else if(x==2){
		head=change_quest(user,head);
		goto loop0;
	}
	else if(x==3){
		head=finish_quest(&user,head);
		goto loop0;
	}
	else if(x==4){
loop4:		system("clear");
		print_quest(head,3);
		printf("请输入要删除任务的编号，返回主菜单请输入0：\n");
		y=choice(0,check_num(head));
		if(y==0)
			goto loop0;
		printf("是否确定删除编号为%d的任务？\n",y);
		printf("确定请输入1，返回请输入0：\n");
		z=choice(0,1);
		if(z==0)
			goto loop4;
		else{
			head=del(head,y);
			y=save(DATA,user,head);
			if(y==0)
				goto loop0;
			printf("删除成功\n继续删除请输入1，返回主菜单请输入0：\n");
			y=choice(0,1);
			if(y==0)
				goto loop0;
			if(y==1)
				goto loop4;
		}
	}
	else if(x==5){
loop5:		printf("1.未完成任务     2.已完成任务     3.全部任务     4.返回\n");
		y=choice(1,4);
		if(y==4)
			goto loop0;
		else{
			system("clear");
			print_quest(head,y);
			printf("返回上一级请输入1，返回主菜单请输入0：\n");
			z=choice(0,1);
			if(z==0)
				goto loop0;
			else
				goto loop5;
		}
	}
	else
		exit(1);	
	return 0;
}
