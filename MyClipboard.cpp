#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include "circbuf.h"

/*
myclipboard : Programme qui une fois lancé, créée et initialise la mémoire partagée et la détruit lorsqu'il est terminé par 
Ctrl-C,
*/

void CtrlCException(int sig);

int main ()
{	
	char *p;	
	circbuf a;
	key_t ma_cle = ftok("/home/alex/Documents/TP-IPC/PressePapier.conf", 1);
	int idshm;

	idshm = shmget(ma_cle,1024*8,IPC_CREAT | IPC_EXCL | 0666);
	if(idshm == -1)
	{
		perror("shmget : ");
		return 0;
	}

	p = (char *)shmat(idshm, 0, SHM_RND);
	
	if(p == 0)
	{
		perror("shmat : ");
		return 0;
	}

	circbuf_format(p, 100);
	a = circbuf_init(p, 100);
	
	printf("p : %p",p);
	//printf("p : %s",*(p));
	printf("Mémoire Partagé OK - attente ctrl-c pour fin\n");

	signal(SIGINT,CtrlCException);

	pause();	
	
	shmctl(idshm, IPC_RMID, NULL);

	return 0;
}

void CtrlCException(int sig)
{
	printf("ctrl-c reçu - suppression Mémoire partagé - Fermeture programme\n");
}
