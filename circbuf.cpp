#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "circbuf.h"

struct circbuf_infos
{
	char *mem;	// memory used to store values 
	int size;	// size in bytes
	short first;	// first value index (first appended)
	short last;	// last value index (last appended)
};

#pragma pack(push, 1)
struct circbuf_item	
{
	short prev, next;	// double linked queue (indices instead of pointers)
	char text[1];
};
#pragma pop

static struct circbuf_item* circbuf_getitem(circbuf buf, short index)
{
	return (struct circbuf_item*)(buf->mem+index);
}

void circbuf_format(void *mem, int size)
{
	struct circbuf_item *item=(struct circbuf_item *)mem;
	
	assert(size>sizeof(struct circbuf_item));
	item->prev=
	item->next=-1;
	strcpy(item->text, "");
}

circbuf circbuf_init(void *mem, int size)
{
	circbuf buf=(circbuf)malloc(sizeof(circbuf_infos));
	buf->mem =(char*)mem;
	buf->size=size;
	
	struct circbuf_item *item=circbuf_getitem(buf,0);

	if(strcmp(item->text,"")!=0)
	{	
		short cur;

		for(buf->first=0; (cur=circbuf_getitem(buf, buf->first)->prev)!=-1; buf->first=cur)
			;
		for(buf->last =0; (cur=circbuf_getitem(buf, buf->last )->next)!=-1; buf->last =cur)
			;
	}
	else
		buf->first=-1;
	return buf;
}

void circbuf_shut(circbuf buf)
{
	free(buf);
}

int circbuf_count(circbuf buf)
{
	int count=0;
	
	for(short cur=buf->first; cur!=-1; cur=circbuf_getitem(buf, cur)->next)
		count++;
	return count;
}

int circbuf_append(circbuf buf, const char *s)
{
	short len=(short)strlen(s);
	int nlost=0;
	
	if(len==0)
		return -1;
	else if(buf->first==-1) 
		buf->first=buf->last=0;
	else
	{
		short new_size	=sizeof(struct circbuf_item)+strlen(s);
		short new_start	=buf->last + sizeof(struct circbuf_item) + strlen(circbuf_getitem(buf, buf->last)->text);

		if(new_start+new_size > buf->size)
		{
			for(; buf->first>buf->last; buf->first=circbuf_getitem(buf, buf->first)->next)
				nlost++;
			new_start=0;
		}
		for(; new_start<=buf->first && buf->first<new_start+new_size; buf->first=circbuf_getitem(buf, buf->first)->next)
			nlost++;

		struct circbuf_item *item=circbuf_getitem(buf, new_start);

		item->prev=buf->last;
		item->next=-1;
		if(buf->first==-1)
			buf->first=new_start;
		else
			circbuf_getitem(buf, buf->last)->next=new_start;
		buf->last=new_start;
		circbuf_getitem(buf, buf->first)->prev=-1;	// if buf->first changed, reset its prev
	}
	strcpy(circbuf_getitem(buf, buf->last)->text,s);	
	return nlost;
}

const char *circbuf_get(circbuf buf, int indice)
{
	short cur;
	
	for(cur=buf->last; indice>0; cur=circbuf_getitem(buf, cur)->prev, indice--)
		assert(cur!=-1);
	return circbuf_getitem(buf, cur)->text;
}
