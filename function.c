#include"quest_head.h"

void clear_input()
{
	if ( feof(stdin) || ferror(stdin) )
	{ 
		/* 如果用户输入文件结束标志（或文件已被读完）， */
		/* 或者发生读写错误，则退出循环               */
		//return;
	}
	/* 没有发生错误，清空输入流。                 */
	/* 通过 while 循环把输入流中的余留数据“吃”掉 */
	int c;
	while ( (c = getchar()) != '\n' && c != EOF ) ;
}

//接收m与n之间的整数
int choice(int m,int n)
{
	int x;
	scanf("%d",&x);
	clear_input();
	while(x<m||x>n){
		printf("输入错误！请重新输入：\n");
		scanf("%d",&x);
		clear_input();
	}
	return x;
}

//获取当前时间，mday为本月第几天，wday为本周第几天，yday为本年第几天
void gettime(int *year,int *month,int *mday,int *wday,int *yday)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	*year=timeinfo->tm_year+1900;
	*month=timeinfo->tm_mon+1;
	*mday=timeinfo->tm_mday;
	*wday=timeinfo->tm_wday;
	if(*wday==0)
		*wday=7;
	*yday=timeinfo->tm_yday+1;
}

//获取指定日期的wday与yday
void get_wyday(int year,int month,int mday,int *wday,int *yday)
{
	int now_year,now_month,now_mday,now_wday,now_yday,i,temp=0;
	//get yday	
	for(i=1;i<month;i++)
		temp+=judge_day_amount(year,i,1);
	temp+=mday;
	*yday=temp;
	//get wday	
	gettime(&now_year,&now_month,&now_mday,&now_wday,&now_yday);
	if(year>now_year||*yday>now_yday){
		if(year>now_year)
			temp=judge_day_amount(now_year,0,2)-now_yday+*yday;
		else
			temp=*yday-now_yday;
		for(i=now_year+1;i<year;i++)
			temp+=judge_day_amount(i,0,2);
		*wday=(temp%7+now_wday)%7;
	}
	else{
		if(now_year>year)
			temp=judge_day_amount(year,0,2)-*yday+now_yday;
		else
			temp=now_yday-*yday;
		for(i=year+1;i<now_year;i++)
			temp+=judge_day_amount(i,0,2);
		*wday=(now_wday-temp%7)%7;
	}	
	if(*wday==0)
		*wday=7;
}

//判断指定月或指定年的天数
int judge_day_amount(int year,int month,int type)//type=1，返回当月有几天；type=2，返回当年有几天
{
	if(type==1){
		if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
			return 31;
		else if(month==4||month==6||month==9||month==11)
			return 30;
		else{
			if(year%4==0&&year%100!=0)
				return 29;
			else
				return 28;
		}
	}
	else if(type==2){
		if(year%4==0&&year%100!=0)
			return 366;
		else
			return 365;
	}
	else
		return 0;
}

//链表数据按num升序插入
QUEST *insert(QUEST *head,QUEST *p)
{
	QUEST *p1=NULL,*p2=NULL;
	if(head==NULL){
		head=p;
		p->next=NULL;
	}
	else{
		p1=head;
		while(p1->next!=NULL){
			if(p->num<p1->num)
				break;
			p2=p1;
			p1=p1->next;
		}
		if(p1==head&&p1->num>p->num){
			p->next=head;
			head=p;
		}
		else if(p1->num>p->num){
			p->next=p1;	
			p2->next=p;
		}
		else{
			p1->next=p;
			p->next=NULL;
		}
	}
	return head;
}

//删除链表节点
QUEST *del(QUEST *head,int num)
{
	QUEST *p1=NULL,*p2=NULL,*p3=NULL;
	if(head==NULL)
		printf("无数据\n");
	else{
		p1=head;
		while(p1->next!=NULL){
			if(p1->num==num)
				break;
			p2=p1;
			p1=p1->next;
		}
		if(p1->num==num){
			if(p1==head){
				head=head->next;
				p3=head;
			}
			else if(p1!=NULL){
				p2->next=p1->next;
				p3=p1->next;
			}
			while(p3!=NULL){
				p3->num--;
				p3=p3->next;
			}
			free(p1);
			printf("任务删除成功\n");
		}
		else
			printf("无数据\n");
	}
	return head;
}

//撤销链表
QUEST *del_all(QUEST *head)
{
	QUEST *p=NULL;
	p=head;
	while(p!=NULL){
		head=head->next;
		free(p);
		p=head;
	}
	return head;
}

//查找编号为num的节点
QUEST *search(QUEST *head,int num)
{
	QUEST *p1=NULL;
	if(head==NULL){
		printf("无数据\n");
		return NULL;
	}
	else{
		p1=head;
		while(p1->next!=NULL){
			if(p1->num==num)
				break;
			p1=p1->next;
		}
		if(p1->num==num)
			return p1;
		else{
			printf("无数据\n");
			return NULL;
		}
	}
}

//检查链表节点数
int check_num(QUEST *head)
{
	QUEST *p=NULL;
	int i=0;
	p=head;
	while(p!=NULL){
		i++;
		p=p->next;
	}
	return i;
}

//检查任务是否超期，是否完成
QUEST *check_quest(USER *user,QUEST *head)
{
	int year,month,mday,wday,yday,x,i=1,exp;
	double temp;	
	gettime(&year,&month,&mday,&wday,&yday);
	QUEST *p=NULL;
	if(head==NULL)
		return NULL;
	while(i<=check_num(head)){
		p=search(head,i);
		if(p->finish==1){
			i++;
			continue;
		}
		if((year>p->ddl_year)||(yday>p->ddl_yday)){
			p->finish=-1;
			printf("任务：%s  编号：%d  [已超期]  请尽快完成！\n",p->content,p->num);
			getchar();			
			if(p->type!=5){
				head=new_quest(head,p);
			}
			if(p->amount_max>1&&p->amount>0){
				temp=(double)p->amount/(double)p->amount_max;
				exp=p->exp*temp;
				printf("任务：%s  编号：%d  完成进度：%d/%d   %.2f\n",p->content,p->num,p->amount,p->amount_max,temp*100);
				if(p->amount>=p->amount_max){
					p->finish=1;
					head=new_quest(head,p);
				}
				printf("获得%d经验值\n",exp);
				getchar();
				user->exp+=exp;
				level_up(user);
				x=save(DATA,*user,head);
				if(x==0)
					return NULL;
			}
		}
		else if(p->finish==-1)
			p->finish=0;
		else if(p->amount>=p->amount_max){
			printf("任务：%s  编号：%d  [已完成]\n",p->content,p->num);
			p->finish=1;
			head=new_quest(head,p);
			gettime(&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
			exp=p->exp*(1+2*(double)(p->amount-p->amount_max)/(double)p->amount_max);
			printf("获得%d经验值\n",exp);
			getchar();
			user->exp+=exp;
			level_up(user);
			x=save(DATA,*user,head);
			if(x==0)
				return NULL;
		}
		x=save(DATA,*user,head);
		if(x==0)
			return NULL;
		i++;
	}
	return head;	
}

//完成任务
QUEST *finish_quest(USER *user,QUEST *head)
{
	int x,y,z,exp,temp,t,flag=0;
	QUEST *p=NULL;
loop0:	system("clear");
	print_userinfo(*user);
	p=head;
	t=0;
	while(p!=NULL){
		if(p->finish!=1){
			t=1;
			break;
		}
		p=p->next;
	}
	if(t==0){
		printf("当前没有未完成的任务\n");
		getchar();
		return head;
	}
	print_quest(head,1);
	printf("请输入任务编号，返回请输入0：\n");
	scanf("%d",&x);	
	//fflush(stdin);
	setbuf(stdin,NULL);	
	if(x==0)
		return head;
	p=search(head,x);
	while(p==NULL||p->finish==1){
		printf("无法找到对应的任务，请重新输入，返回请输入0：\n");
		scanf("%d",&x);
		//fflush(stdin);
		setbuf(stdin,NULL);		
		if(x==0)
			return head;
		p=search(head,x);
	}
	if(p->amount_max>1){
		printf("该任务当前完成进度为：%d/%d\n",p->amount,p->amount_max);
		if(p->finish==-1){
			printf("确认继续请输入1，返回请输入0：\n");
			y=choice(0,1);
			if(y==0)
				return head;
			printf("请输入新增的[按时]完成数量：\n");
			y=choice(0,1000000000);
			printf("请输入新增的[超时]完成数量：\n");
			z=choice(0,1000000000);
			p->amount+=y+z;
		}
		else{		
			printf("请输入新增的完成数量，返回请输入0：\n");
			y=choice(0,1000000000);
			if(y==0)
				return head;
			p->amount+=y;
		}
		printf("该任务当前完成进度为：%d/%d\n",p->amount,p->amount_max);
		if(p->amount>=p->amount_max){
			flag=1;
			//gettime(&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
			if(p->finish==-1){
				temp=p->amount-z;
				if(temp<=p->amount_max)
					exp=p->exp*((double)z/(double)p->amount_max/2+(double)y/(double)p->amount_max);
				else
					exp=p->exp*((double)z/(double)p->amount_max/2+(double)(temp-p->amount_max)/(double)p->amount_max*2+(double)(y+p->amount_max-temp)/(double)p->amount);
			}
			else
				exp=p->exp*(1+2*(double)(p->amount-p->amount_max)/(double)p->amount_max);
			goto loop1;			
		}
		else
			goto loop2;
	}
	else if(p->finish==-1){
		printf("当前时间已超过该任务截止时间\n");
		printf("按时完成请输入1，超时完成请输入2，返回请输入0：\n");
		x=choice(0,2);
		if(x==0)
			return head;
		if(x==1)
			exp=p->exp;
		if(x==2){
			flag=1;
			exp=p->exp/2;
		}
		p->amount=p->amount_max;
	}
	else{
		printf("确认完成该任务请输入1，返回请输入0：\n");
		x=choice(0,1);
		if(x==0)
			return head;
		exp=p->exp;
		flag=1;
		//gettime(&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
		p->amount=p->amount_max;
	}
loop1:	head=new_quest(head,p);
	if(flag==1)
		gettime(&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
	get_wyday(p->ddl_year,p->ddl_month,p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
	p->finish=1;
	printf("任务：%s  编号：%d  [已完成]\n",p->content,p->num);
	printf("获得%d经验值\n",exp);
	getchar();
	user->exp+=exp;
	level_up(user);
	x=save(DATA,*user,head);
	if(x==0)
		return head;
loop2:	printf("继续完成任务请输入1，返回请输入0：\n");
	x=choice(0,1);
	if(x==1)
		goto loop0;
	else
		return head;
}

//判断能否升级
void level_up(USER *user)
{
	while(user->exp>=user->exp_max){
		printf("等级由%d提升至%d！\n",user->level,user->level+1);
		user->level++;
		user->exp-=user->exp_max;
		user->exp_max+=50;
	}
}

//比较两个任务是否相同（除了num）
int judge_quest(QUEST *p1,QUEST *p2)
{
	if(strcmp(p1->content,p2->content)==0&&p1->type==p2->type&&p1->exp==p2->exp&&p1->amount_max==p2->amount_max&&p1->ddl_year==p2->ddl_year&&p1->ddl_month==p2->ddl_month&&p1->ddl_mday==p2->ddl_mday)
		return 1;
	else 
		return 0;
}

//新建任务
QUEST *new_quest(QUEST *head,QUEST *p)
{
	int i,j,k,year,month,mday,wday,yday,temp1,temp2;
	QUEST *p0=NULL,*p1=NULL;
	gettime(&year,&month,&mday,&wday,&yday);
	i=p->ddl_yday;
	j=p->ddl_month;
	k=p->ddl_year;	
	if(p->type==5)
		return head;
	else{
		while(1){
loop4:			p0=(QUEST *)malloc(sizeof(QUEST));
			p0->next=NULL;
			p0->num=check_num(head)+1;
			strcpy(p0->content,p->content);
			p0->type=p->type;
			p0->exp=p->exp;
			p0->amount=0;
			p0->amount_max=p->amount_max;
			p0->finish=0;
			p0->ddl_year=k;
			p0->ddl_month=j;
			if(p->type==3){
				j++;
				p0->ddl_month=j;
				if(j>12){
					k++;
					p0->ddl_year=k;
					j=1;
				}
				p0->ddl_mday=judge_day_amount(p0->ddl_year,p0->ddl_month,1);
				if(p0->ddl_year>year||(p0->ddl_year==year&&p0->ddl_month>month+1)){
					free(p0);
					break;
				}
				goto loop3;
			}
			if(p->type==4){
				k++;
				if(k>year+1){
					free(p0);
					break;
				}
				p0->ddl_year=k;
				p0->ddl_month=12;
				p0->ddl_mday=31;
				goto loop3;
			}
			if(p->type==1)
				i+=1;
			if(p->type==2)
				i+=7;
			p0->ddl_mday=p->ddl_mday+i-p->ddl_yday;
			temp1=judge_day_amount(p0->ddl_year,p0->ddl_month,2);
			temp2=judge_day_amount(p0->ddl_year,p0->ddl_month,1);
			if(i>temp1){
				k++;
				p0->ddl_year=k;
				p0->ddl_month=1;
				p0->ddl_mday=i-temp1;
				i-=temp1;
			}
			else if(p0->ddl_mday>temp2){
				p0->ddl_month+=1;
				p0->ddl_mday-=temp2;
			}
			if(p0->ddl_year==year&&(((p->type==1)&&(i>yday+1))||((p->type==2)&&(i>yday+7)))){
				free(p0);
				break;
			}
loop3:			get_wyday(p0->ddl_year,p0->ddl_month,p0->ddl_mday,&p0->ddl_wday,&p0->ddl_yday);
			p1=head;
			while(p1!=NULL){
				if(p1->finish==1){
					p1=p1->next;
					continue;
				}
				if(judge_quest(p1,p0)==1){
					free(p0);
					goto loop4;
				}
				p1=p1->next;
			}
			head=insert(head,p0);
			p0=p0->next;
		}
		return head;
	}
}

//修改任务			
QUEST *change_quest(USER user,QUEST *head)
{
	QUEST *p=NULL;
	int x,y,z,j,k,t;
loop6:	system("clear");
	print_quest(head,1);
	printf("请输入要修改任务的编号,返回请输入0：\n");
	x=choice(0,check_num(head));
	if(x==0)
		return head;
	p=search(head,x);					
loop5:	printf("请选择要修改的数据：\n");
	printf("0.返回   1.内容   2.经验值   3.需完成数量   4.截止时间\n");
	x=choice(0,4);
	if(x==0)
		return head;
	if(x==1){
		printf("该任务当前内容为：%s\n",p->content);
		printf("请输入修改后的内容：\n");
		scanf("%90s",p->content);
		setbuf(stdin,NULL);
		printf("修改成功！\n该任务当前内容为：%s\n",p->content);
	}
	if(x==2){
		printf("该任务当前经验值为：%d\n",p->exp);
		printf("请输入修改后的经验值：\n");
		p->exp=choice(1,1000000000);
		printf("修改成功！\n该任务当前经验值为：%d\n",p->exp);
	}
	if(x==3){
		printf("该任务当前需完成数量为：%d\n",p->amount_max);
		printf("请输入修改后的需完成数量：\n");
		p->amount_max=choice(1,1000000000);
		printf("修改成功！\n该任务当前需完成数量为：%d\n",p->amount_max);
	}
	if(x==4){
		printf("该任务类型为：");
		switch(p->type){
			case 1: printf("每天\n");break;
			case 2: printf("每周");
				switch(p->ddl_wday){
					case 1: printf("一\n");break;
					case 2: printf("二\n");break;
					case 3: printf("三\n");break;
					case 4: printf("四\n");break;
					case 5: printf("五\n");break;
					case 6: printf("六\n");break;
					case 7: printf("七\n");break;
				}
				break;
			case 3: printf("每月\n");break;
			case 4: printf("每年\n");break;
			case 5: printf("仅一次\n");break;
		}
		printf("该任务当前截止时间为：%d-%02d-%02d\n",p->ddl_year,p->ddl_month,p->ddl_mday);
		printf("请选择修改后的类型：\n");
		printf("1.每天  2.每周  3.每月  4.每年  5.仅一次\n");
		z=choice(1,5);
		p->type=z;
		gettime(&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
		j=judge_day_amount(p->ddl_year,p->ddl_month,2);
		k=judge_day_amount(p->ddl_year,p->ddl_month,1);
		switch(z){
			case 1: break;
			case 2: printf("请选择截止时间：\n");
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
				if(p->ddl_yday>j){
					p->ddl_year++;
					p->ddl_month=1;
					p->ddl_mday=p->ddl_yday-j;
				}
				else if(p->ddl_mday>k){
					p->ddl_month++;
					p->ddl_mday-=k;
				}
				break;
			case 3: p->ddl_mday=k;break;
			case 4: p->ddl_month=12;
				p->ddl_mday=31;
				break;
			case 5: printf("请输入任务截止年：\n");
				p->ddl_year=choice(0,1000000000);
				printf("请输入任务截止月：\n");
				p->ddl_month=choice(1,12);
				printf("请输入任务截止日：\n");
				p->ddl_mday=choice(1,judge_day_amount(p->ddl_year,p->ddl_month,1));
				break;
		}
		get_wyday(p->ddl_year,p->ddl_month,p->ddl_mday,&p->ddl_wday,&p->ddl_yday);
		printf("修改成功！\n");
	}
	t=save(DATA,user,head);
	if(t==0)
		return head;
	printf("返回上一级请输入1，重新选择任务请输入2，返回主菜单请输入0：\n");
	y=choice(0,2);
	if(y==0)
		return head;
	if(y==1)
		goto loop5;
	if(y==2)
		goto loop6;
	else
		return head;
}