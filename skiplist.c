#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"skiplist.h"

#define LEVEL_MAX 32

//掷硬币算法
int coinRand()
{
	int i;
	for(i=0;i<LEVEL_MAX;i++)
	{
		if(rand()<RAND_MAX/2)
			break;
	}
	return i+1;
}

//创建一个新的跳跃表节点
zskiplistNode* zslNodeCreate(int levelval)
{
	zskiplistNode *node = (zskiplistNode*)malloc(sizeof(zskiplistNode));
	node->level = (zskiplistLevel*)malloc(sizeof(zskiplistLevel)*levelval);
	return node;
}

//创建一个新的跳跃表
zskiplist* zslCreate()
{
	zskiplist *zsl = (zskiplist*)malloc(sizeof(zskiplist));
	zskiplistNode *node = zslNodeCreate(32);
	zsl->header = node;
	//zsl->tail = node;
	zsl->level = 0;
	zsl->length = 0;
	return zsl;
}

//释放给定的跳跃表，以及表中包含的所有节点
void zslFree(zskiplist **zsl)
{
	zskiplistNode *p, *q;
	p = (*zsl)->header;
	while(NULL != p)
	{
		q = p->level->forward;
		free(p);
		p = q;
	}
	free(*zsl);
	*zsl = NULL;
}

//将包含给定成员的值的新节点添加到跳跃表中
int zslInsert(zskiplist* zsl, int val)
{
	int i;
	int levelval;//节点包含的层数
	int rank = 0;//节点插入后在skiplist中的排位
	int rank_ =0;
	int temp;
	zskiplistNode *p;
	levelval = coinRand();
	zskiplistNode *node = zslNodeCreate(levelval);
	node->val = val;
	//设置zskiplistlevel的forward属性
	p = zsl->header;
	for(i=zsl->level;i>0;i--)
	{
		while(NULL!=(p->level+i-1)->forward && (p->level+i-1)->forward->val<val)
		{
			rank += (p->level+i-1)->span;
			p = (p->level+i-1)->forward;
		}
		if(NULL!=(p->level+i-1)->forward && (p->level+i-1)->forward->val == val)
		{
			printf("插入失败，跳跃表中已包含值为 %d 的元素。\n",val);
			return 0;
		}
		if(i > levelval)
		{
			continue;
		}
		else
		{
			(node->level+i-1)->forward = (p->level+i-1)->forward;
			(p->level+i-1)->forward = node;
		}
	}
	rank++;
	if(zsl->level < levelval)
	{
		for(i=levelval;i>(zsl->level);i--)
		{
			(zsl->header->level+i-1)->forward = node;
		}
		zsl->level = levelval;
	}
	//设置zskiplistlevel的span属性
	p = zsl->header;
	for(i=zsl->level;i>0;i--)
	{
		while(NULL != (p->level+i-1)->forward && (p->level+i-1)->forward->val < val)
		{
			rank_ += (p->level+i-1)->span;
			p = (p->level+i-1)->forward;
		}
		if(NULL == (p->level+i-1)->forward)
		{
			continue;
		}
		else if((p->level+i-1)->forward->val == val)
		{
			temp = rank-rank_;
			if(NULL != (node->level+i-1)->forward)
			{
				(node->level+i-1)->span = (p->level+i-1)->span+1-temp;
			}
			(p->level+i-1)->span = temp;
		}
		else
		{
			(p->level+i-1)->span ++;
		}
	}
	//
	zsl->length++;
	//printf("%d %d\n",val,rank);
	return rank;
}

//查看跳跃表中所有元素
void zslGetAll(zskiplist* zsl)
{
	zskiplistNode *p;
	if(zsl->length == 0)
	{
		printf("查找失败，给定跳跃表中无元素\n");
	}
	else
	{
		p = zsl->header->level->forward;
		while(NULL!=p)
		{
			printf("%d -> ",p->val);
			p = p->level->forward;
		}
		printf("\n");
	}
	return;
}

//查找指定元素的排位
int zslGetRank(zskiplist* zsl, int val)
{
	int i;
	int rank = 0;
	zskiplistNode *p;
	p = zsl->header;
	for(i=zsl->level;i>0;i--)
	{
		while(NULL != (p->level+i-1)->forward && (p->level+i-1)->forward->val < val)
		{
			rank += (p->level+i-1)->span;
			p = (p->level+i-1)->forward;
		}
		if(NULL != (p->level+i-1)->forward && val == (p->level+i-1)->forward->val)
		{
			return rank + (p->level+i-1)->span;
		}
	}
	printf("查找失败，跳跃表中不包含值为 %d 的元素\n",val);
	return 0;
}

//查找指定排位元素的值
int zslGetByRank(zskiplist* zsl, int rank)
{
	if(rank < 0 || rank > zsl->length)
	{
		printf("查找失败，排位 %d 超出跳跃表已有元素范围\n",rank);
		return 0;
	}
	int i;
	zskiplistNode* p;
	p = zsl->header;
	for(i=zsl->level;i>0;i--)
	{
		while(NULL != (p->level+i-1)->forward && rank > 0)
		{
			rank -= (p->level+i-1)->span;
			if(rank > 0)
			{
				p = (p->level+i-1)->forward;
			}
			else if(rank < 0)
			{
				rank += (p->level+i-1)->span;
				break;
			}
			else
			{
				return (p->level+i-1)->forward->val;
			}
		}
	}
	return 0;
}

//删除元素
zskiplistNode* zslDel(zskiplist* zsl, int val)
{
	if(zsl->length <= 0)
	{
		printf("删除失败，跳跃表中无元素\n");
		return NULL;
	}
	int i;
	int rank = 0;
	zskiplistNode *p, *node;
	zskiplistNode *nodeList[zsl->level];
	p = zsl->header;
	for(i=zsl->level;i>0;i--)
	{
		while(NULL != (p->level+i-1)->forward && (p->level+i-1)->forward->val < val)
		{
			p = (p->level+i-1)->forward;
		}
		nodeList[i-1] = p;
	}
	if(NULL == p->level->forward || p->level->forward->val > val)
	{
		printf("删除失败，跳跃表中不包含值为 %d 的元素\n",val);
		return NULL;
	}
	else
	{
		for(i=zsl->level;i>0;i--)
		{
			if(NULL == (nodeList[i-1]->level+i-1)->forward)
			{
				continue;
			}
			else if((nodeList[i-1]->level+i-1)->forward->val > val)
			{
				(nodeList[i-1]->level+i-1)->span --;
			}
			else
			{
				node = (nodeList[i-1]->level+i-1)->forward;
				rank = (nodeList[i-1]->level+i-1)->span + (node->level+i-1)->span;
				(nodeList[i-1]->level+i-1)->span = (node->level+i-1)->span == 0 ? 0 : rank-1;
				(nodeList[i-1]->level+i-1)->forward = (node->level+i-1)->forward;
				(node->level+i-1)->span = 0;
				(node->level+i-1)->forward = NULL;
			}
		}
		zsl->length --;
		return nodeList[0];
	}
}


//通过排位删除元素
zskiplistNode* zslDelByRank(zskiplist* zsl, int rank)
{
	if(rank < 0 || rank > zsl->length)
	{
		printf("删除失败，排位 %d 超出跳跃表已有元素范围\n",rank);
		return NULL;
	}
	int i, rank_, temp;
	zskiplistNode *p, *node;
	p = zsl->header;
	for(i=zsl->level;i>0;i--)
	{
		rank_ = rank;
		while(NULL != (p->level+i-1)->forward)
		{
			rank_ -= (p->level+i-1)->span;
			if(rank_ > 0)
			{
				p = (p->level+i-1)->forward;
				rank = rank_;
			}
			else if(rank_ < 0)
			{
				(p->level+i-1)->span --;
				break;
			}
			else
			{
				node = (p->level+i-1)->forward;
				temp = (p->level+i-1)->span + (node->level+i-1)->span;
				(p->level+i-1)->span = (node->level+i-1)->span == 0 ? 0 : temp-1;
				(p->level+i-1)->forward = (node->level+i-1)->forward;
				(node->level+i-1)->span = 0;
				(node->level+i-1)->forward = NULL;
				break;
			}
		}
	}
	zsl->length --;
	return node;
}
