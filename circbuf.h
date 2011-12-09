#ifndef _CIRCBUF_H
#define _CIRCBUF_H

/*
    Ce module permet d'utiliser un espace mémoire donné comme buffer circulaire de chaînes de caractères.
    
    Lors de sa première utilisation, cet espace doit être formatté : circbuf_format
    
    Ensuite lorsqu'un programme veut utiliser le buffer, il doit au préalable récupérer un circbuf via circbuf_init en précisant l'espace mémoire.
    
    Utiliser un buffer circulaire signifie :
    - Consulter son contenu : circbuf_count ou circbuf_get
    - Ajouter du contenu : circbuf_append
    
    Enfin, lorsque le programme ne souhaite plus utiliser ce buffer, il appelle circbuf_shut.
    L'espace mémoire reste néanmoins rempli et susceptible d'être utilisé comme buffer circulaire.
*/

typedef struct circbuf_infos *circbuf;

void            circbuf_format  (void *mem, int size);
circbuf		circbuf_init	(void *mem, int size);
void		circbuf_shut	(circbuf buf);

int		circbuf_count	(circbuf buf);
int		circbuf_append	(circbuf buf, const char *s);
const char *    circbuf_get	(circbuf buf, int indice);

#endif
