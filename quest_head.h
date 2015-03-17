#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define DATA "user.dat"

typedef struct quest
{
	char content[100];
	int finish,
		exp,
		type,//type=1，每天；type=2，每周；type=3，每月；type=4，每年；type=5，自定义
		num,//编号
		amount,
		amount_max,
		ddl_year,
		ddl_month,
		ddl_mday,
		ddl_wday,
		ddl_yday;
	struct quest *next;
}QUEST;

typedef struct user
{
	int exp,
		exp_max,
		level;
}USER;

//SL.c
int save(char *name,USER user,QUEST *head);

QUEST *load(char *name,USER *user);

//function.c
int choice(int m,int n);

void gettime(int *year,int *month,int *mday,int *wday,int *yday);

void get_wyday(int year,int month,int mday,int *wday,int *yday);

int judge_day_amount(int year,int month,int type);//type=1，返回当月有几天；type=2，返回当年有几天

QUEST *insert(QUEST *head,QUEST *p);

QUEST *del(QUEST *head,int num);

QUEST *del_all(QUEST *head);

QUEST *search(QUEST *head,int num);

int check_num(QUEST *head);

QUEST *check_quest(USER *user,QUEST *head);

QUEST *finish_quest(USER *user,QUEST *head);

void level_up(USER *user);

QUEST *new_quest(QUEST *head,QUEST *p);

QUEST *change_quest(USER user,QUEST *head);

//menu.c
void print_userinfo(USER user);

void print_quest(QUEST *head,int type);//type=1，未完成任务；type=2，已完成任务；type=3，所有任务

int menu_main(void);
