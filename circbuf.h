#ifndef _CIRCBUF_H
#define _CIRCBUF_H

/*
    Ce module permet d'utiliser un espace m�moire donn� comme buffer circulaire de cha�nes de caract�res.
    
    Lors de sa premi�re utilisation, cet espace doit �tre formatt� : circbuf_format
    
    Ensuite lorsqu'un programme veut utiliser le buffer, il doit au pr�alable r�cup�rer un circbuf via circbuf_init en pr�cisant l'espace m�moire.
    
    Utiliser un buffer circulaire signifie :
    - Consulter son contenu : circbuf_count ou circbuf_get
    - Ajouter du contenu : circbuf_append
    
    Enfin, lorsque le programme ne souhaite plus utiliser ce buffer, il appelle circbuf_shut.
    L'espace m�moire reste n�anmoins rempli et susceptible d'�tre utilis� comme buffer circulaire.
*/

typedef struct circbuf_infos *circbuf;

void            circbuf_format  (void *mem, int size);
circbuf		circbuf_init	(void *mem, int size);
void		circbuf_shut	(circbuf buf);

int		circbuf_count	(circbuf buf);
int		circbuf_append	(circbuf buf, const char *s);
const char *    circbuf_get	(circbuf buf, int indice);

#endif
