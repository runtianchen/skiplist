#include<stdio.h>
#include<stdlib.h>
#include"skiplist.h"

int main()
{
	zskiplist *zsl;
	zsl = zslCreate();
	//func(zsl,10);
	//func(zsl,20);
	//func(zsl,15);
	zslInsert(zsl,10);
	zslInsert(zsl,20);
	zslInsert(zsl,30);
	zslInsert(zsl,5);
	zslInsert(zsl,15);
	zslInsert(zsl,25);
	zslInsert(zsl,35);
	zslInsert(zsl,12);
	zslInsert(zsl,9);
	zslInsert(zsl,19);
	zslInsert(zsl,22);
	zslInsert(zsl,29);
	zslInsert(zsl,20);
	zslInsert(zsl,31);
	zslInsert(zsl,33);
	zslInsert(zsl,14);
	zslInsert(zsl,4);
	zslInsert(zsl,21);
	zslInsert(zsl,28);
	int k = zslGetRank(zsl,35);
	printf("%d\n",k);
	printf("%d\n",zslGetByRank(zsl,18));
	printf("%ld\n",zsl->length);
	zslGetAll(zsl);
	zslDel(zsl,15);
	zslGetAll(zsl);
	zslDel(zsl,12);
	zslGetAll(zsl);
	zslDel(zsl,23);
	zslGetAll(zsl);
	printf("%ld\n",zsl->length);
	zslInsert(zsl,50);
	zslGetAll(zsl);
	printf("%d\n",zslGetByRank(zsl,15));
	zslFree(&zsl);
	return 0;
}
