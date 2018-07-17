#ifndef __SKIPLIST_H__
#define __SKIPLIST_H__

typedef struct zskiplistLevel
{
	struct zskiplistNode *forward;
	unsigned int span;
} zskiplistLevel;

typedef struct zskiplistNode
{
	struct zskiplistNode *backward;
	int val;
	struct zskiplistLevel *level;
} zskiplistNode;

typedef struct zskiplist
{
	struct zskiplistNode *header;//*tail
	unsigned long length;
	int level;
} zskiplist;

//创建一个新的跳跃表
zskiplist* zslCreate();
//释放给定跳跃表，以及表中包含的所有节点
void zslFree(zskiplist**);
//将包含给定值的节点添加到跳跃表中
int zslInsert(zskiplist*, int);
//展示跳跃表中所有元素
void zslGetAll(zskiplist*);
//返回指定值的节点的排位
int zslGetRank(zskiplist*, int);
//通过排位返回指定节点
int zslGetByRank(zskiplist*, int);
//删除指定值的节点
zskiplistNode* zslDel(zskiplist*, int);
//通过排位删除指定节点
zskiplistNode* zslDelByRank(zskiplist* ,int);
#endif
