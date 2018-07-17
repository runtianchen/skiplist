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

zskiplist* zslCreate();
void zslFree(zskiplist**);
int zslInsert(zskiplist*, int);
void zslGetAll(zskiplist*);
int zslGetRank(zskiplist*, int);
int zslGetByRank(zskiplist*, int);
zskiplistNode* zslDel(zskiplist*, int);
zskiplistNode* zslDelByRank(zskiplist* ,int);
#endif
