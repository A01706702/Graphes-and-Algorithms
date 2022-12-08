#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "graphes.h"

/* ci-dessous trois macros pour pouvoir utiliser les termes, booleens, VRAI et FAUX dans les programmes */
#define booleen unsigned char
#define VRAI 1
#define FAUX 0

/***************************************************************************/
/* retourne le dilate D de l'ensemble E dans le graphe G                   */
/* les ensembles E et D sont repr�sent�s par des tableaux booleens         */
/* - la m�moire pour E est suppos�e allou�e avec autant de case            */
/*   que de sommets dans G                                                 */
/* - la m�moire pour D est allou�e a l'int�rieur de la fonction            */
/***************************************************************************/
booleen * dilatation(graphe* G, booleen *E){
  booleen *D; /* tableau booleen pour stocker le r�sultat de ma dilatation */
  int x; /* pour parcourir les sommets de G */
  pcell p; /* pointeur-maillon pour parcourir des listes de successeurs */
  
  /* comme D est un sous ensemble des sommets de G on a besoin
     d'autant de cases que de sommets de G */
  D = (booleen *) calloc(G->nsom, sizeof(booleen));

  for (x = 0; x < G->nsom; x++)
    if(E[x] == VRAI){
      for(p = G->gamma[x]; p != NULL; p = p->suivant)
	D[p->som] = VRAI;
    }
  return D;
}

/***************************************************************************/
/* retourne l'exploration du grapge G depuis le sommet x                   */
/* en utilisant l'algorithme naif                                          */
/***************************************************************************/
booleen * explorationNaif(graphe* G, int x){
  booleen *D,*E,*Z; /* tableaux booleens pour stocker les ensembles de sommets */
  int y;            /* indice muet pour un sommet  */
  int k;            /* num�ro d'iteration */

  
  /* initialisation de l'ensemble E : E := {x}*/
  E = (booleen*) calloc(G->nsom, sizeof(booleen)); /* calloc alloue la m�moire et l'initialise � 0 */
  E[x] = VRAI;
  Z = (booleen*) calloc(G->nsom, sizeof(booleen));
  Z[x] = VRAI;
  
  /* Completez ci-dessous avec votre implementation de l'algorithme exploration naif */
  
  for(k = 0; k < G->nsom; k++) {
    D = dilatation(G, E);
    for(int i = 0; i < G->nsom; i++) Z[i] = Z[i] || D[i];
    E = D;
  }

  return Z;
}


/* ====================================================================== */
int main(int argc, char **argv)
/* ====================================================================== */
{
  graphe *G;
  int x;               /* sommet de depart � explorer */
  int y;               /* indice muet pour un sommet*/
  booleen *Z;          /* tableau pour stocker l'exploration du graphe */
  int i;               /* indice muet*/
  int nZ;              /* pour compter le nombre de sommets explor�s */
  clock_t debut, fin;  /* pour mesurer les temps d'execution */
  
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <donnee.graphe> <sommet_initial_a_explorer> \n", argv[0]);
    fprintf(stderr, "            ou %s explore le graphe donnee.graphe deuis sommet_initial_a_explorer\n", argv[0]);
    exit(0);
  }

  /**********************************************************/
  /* Lecture des arguments passes au programmme             */
  /**********************************************************/
  
  G = litGraphe(argv[1]);       /* Lit un graphe stock�e dans un fichier */
  
  x = atoi(argv[2]);

  if(x >= G->nsom){
    fprintf(stderr, "Erreur: pas de sommet d'indice %d dans %s\n", x, argv[1]);
    exit(0);
  }

  fprintf(stderr, "%s lu\n", argv[1]);
  
  debut =  clock();

  Z = explorationNaif(G,x);       /* traitement : calcule le symetrique de g */

  fin = clock();

  /* On afffiche l'exploration et on compte le nombre de sommets explor�s */
  nZ = 0;
  printf("L'exploration depuis le sommet %d contient les sommets", x);
  for(y = 0; y < G->nsom; y++)
    if(Z[y] == VRAI){
      printf(" %d", y);
      nZ++;
    }
  printf(".\n");

  /* si un nom est diponible pour les sommets, on affiche aussi les noms */
  if( (G->info != NULL) && G->info[0].nom != NULL){
    printf("--\nDont les noms sont :\n");
    for(y = 0; y < G->nsom; y++)
      if(Z[y] == VRAI)
	printf("%s\n", G->info[y].nom);
  }

  printf("%d sommets poeuvent etre atteints depuis %d et %d sommets ne peuvent pas etre atteints \n", nZ, x, G->nsom - nZ);
  
  free(Z);

  printf("Exploration calculee en %lf secondes\n", ((double)fin-debut)/CLOCKS_PER_SEC);

  return 0;
} /* main() */

