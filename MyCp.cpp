#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "circbuf.h"

/*
mycp : Programme qui permet de stocker dans le presse papier le(s) nom(s) du/des fichier(s) passé(s) en ligne de 
commande ou de coller (ie. dupliquer le fichier sur disque) le fichier de l'historique dont le numéro est passé (ex pour le 
premier : mycp -1) ou tous (ex : mycp -a)
*/

char* getSharedMemory();
void pastFile(char *dest, const char *shm);

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("manque paramétre\n");
		return -1;
	}

	char *p;

	p = getSharedMemory();
	circbuf a = circbuf_init(p, 120);
	int count = circbuf_count(a);	
	
	if(argv[1][0] == '-')
	{
		printf("coller\n");

		if(argv[1][1] == 'a')
		{
			int i;
			printf("count : %d\n",count);
			for(i = 0; i < count; i++)
			{
				char dest[20];
				sprintf(dest, "./Test%d.txt", i);
				printf("dest : %s\n",dest);
				printf("src : %s\n",circbuf_get(a,i));
				fflush (stdout);
				pastFile(dest,circbuf_get(a,i));
			}
		}
		else
		{
			int nb = (argv[1][1]) - 48;
			char *dest = *(argv + 2);
			if(dest == 0)
			{	dest = (char *)malloc(sizeof(char *));
				strcpy(dest,"./Test.txt");
			}
			pastFile(dest,circbuf_get(a,nb));
		}
	}
	else
	{
		printf("copier\n");
		int i;
		for(i = 1; i < argc; i++)
		{
			count = circbuf_append(a,*(argv + i));
			//copyFile(*(argv + i),*(p+(i-1)));
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
	idshm = shmget(ma_cle, 0, 0);

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

void pastFile(char *dest,const char *shm)
{
	char *commande= (char *)malloc(sizeof(char *));
	strcpy(commande,"cp ");
	strcat(commande,shm);
	strcat(commande," ");
	strcat(commande,dest);
	printf("commande : %s\n",commande);
	system(commande);
}


