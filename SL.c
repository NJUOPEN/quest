#include"quest_head.h"

int save(char *name,USER user,QUEST *head)
{
	FILE *fp;
	QUEST *p=NULL;
	if((fp=fopen(name,"w+"))==NULL){
		printf("Can't open file\n");
		return 0;
	}
	fprintf(fp,"%d %d %d\n",user.level,user.exp,user.exp_max);
	if(ferror(fp)){
		printf("Find a writing error.\n");
		return 0;
	}
	p=head;
	while(p!=NULL){
		fprintf(fp,"%d %s %d %d %d %d %d %d %d %d %d %d\n",p->num,p->content,p->type,p->exp,p->amount,p->amount_max,p->finish,p->ddl_year,p->ddl_month,p->ddl_mday,p->ddl_wday,p->ddl_yday);
		if(ferror(fp)){
			printf("Find a writing error.\n");
			return 0;
		}
		p=p->next;
	}
	fclose(fp);
	return 1;
}

QUEST *load(char *name,USER *user)
{
	FILE *fp;
	QUEST *head=NULL,*p=NULL;
	if((fp=fopen(name,"r+"))==NULL)
		return NULL;
	fscanf(fp,"%d %d %d\n",&user->level,&user->exp,&user->exp_max);
	if(ferror(fp)){
		printf("Find a reading error.\n");
		exit(1);
	}
	while(!feof(fp)){
		p=(QUEST *)malloc(sizeof(QUEST));
		p->next=NULL;
		fscanf(fp,"%d %s %d %d %d %d %d %d %d %d %d %d\n",&p->num,p->content,&p->type,&p->exp,&p->amount,&p->amount_max,&p->finish,&p->ddl_year,&p->ddl_month,&p->ddl_mday,&p->ddl_wday,&p->ddl_yday);	
		if(ferror(fp)){
			printf("Find a reading error.\n");
			exit(1);
		}
		head=insert(head,p);
		p=p->next;
	}
	fclose(fp);
	return head;
}

