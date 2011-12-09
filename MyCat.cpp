#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "circbuf.h"

/*
Programme qui permet la saisie de texte simple et son copie/collage vers/depuis le presse papier, la commande 
paste collant le dernier texte copié, paste1, l'avant dernier texte copié, etc...
*/

char* getSharedMemory();

int main()
{
	char* str;
	char* tmp;
	char *p;
	circbuf a;	

	str = (char *)malloc(sizeof(char *));
	tmp = (char *)malloc(sizeof(char *));


	p = getSharedMemory();
	a=circbuf_init(p, 120);
	int count = circbuf_count(a);	

	while (1)
	{
		printf ("A vous de saisir : ");
		scanf ("%s",str);
		
		if(strcmp("copier", str) == 0)
		{
			printf (" > copie \n");
			count = circbuf_append(a,tmp);
							
		}
		else if (strcmp("coller", strndup(str,6)) == 0)
		{	
			int j = count - 1;
			if(strlen(str) > 6)
			{
				char *val = strndup(str+6,1);
				j = atoi(val) - 1;
			}
			strcpy(tmp,circbuf_get(a,j));
			printf (" > %s \n",tmp);	
		}
		else if (strcmp("quitter", str) == 0)
		{
			break;
		}
		else
		{
			strcpy(tmp,str);
			printf("vous avez saisie : %s \n", tmp);
		}
		
	}

	return 0;	
}

char* getSharedMemory()
{
	int idshm;
	key_t ma_cle;
	char *p;

	ma_cle = ftok("/home/alex/Documents/TP-IPC/PressePapier.conf", 1); 
	idshm = shmget(ma_cle, 8192, 0);

	if (idshm > -1)
	{
		p = (char *)shmat(idshm, 0, SHM_RND);	
		if(p == 0)
		{
			perror("shmat : ");
		}
	}
	else
	{
		perror("shmget : ");
	}

	return p;
}


