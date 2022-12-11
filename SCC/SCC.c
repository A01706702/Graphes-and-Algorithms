#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "graphes.h"

/* ci-dessous trois macros pour pouvoir utiliser les termes, booleens, VRAI et FAUX dans les programmes */
#define booleen unsigned char
#define VRAI 1
#define FAUX 0


/* ci-dessous, d�finition d'une structure de liste FIFO d'entiers (pour stocker des ensembles de sommets d'un graphe) */

typedef struct {
  int n; /* nombre d'elements */
  int prem; /* indice du premier element */
  int der; /* indice du dernier elemeent */
  int capacite; /* nombre d'elements que la liste peut contenir */
  int * elements; /* taleau contenant les elements de la liste*/
} ListeFIFO;

ListeFIFO* initListeFIFO(int capacite){
  ListeFIFO* l;
  l = (ListeFIFO*)malloc(sizeof(ListeFIFO));
  l->n = 0;
  l->capacite = capacite;
  l->prem = 0;
  l->der = 0;
  l->elements = (int*)calloc(capacite, sizeof(int));
  return l;
}

void termineListeFIFO(ListeFIFO *l){
  free(l->elements);
  free(l);  
}

int selectionSuppressionListeFIFO(ListeFIFO *l){
  int el;
  if (l->n > 0){
    el = l->elements[l->prem];
    l->n --;
    l->prem = (l->prem+1)%l->capacite;
    return el;
  }
  else{
    fprintf(stderr, "Erreur : liste vide\n");
    return -1;
  }      
}

booleen insertionListeFIFO(ListeFIFO *l, int x){
  if (l->n < l->capacite){
    l->elements[l->der] = x;
    l->der = (l->der+1)%l->capacite;
    l->n ++;
    return VRAI; /* pour indiquer que l'insertion ns'est bien deroulee */
  }
  else{
    fprintf(stderr, "Erreur : liste pleine \n");
    return FAUX; /* pour indiquer que l'insertion n'a pas etre effectuee */
  }      
}


booleen estNonVideListeFIFO(ListeFIFO *l){
  if(l->n == 0) return FAUX;
  else return VRAI;
}

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
/* retourne l'exploration du graphe G depuis le sommet x                   */
/* en utilisant l'algorithme exploration largeur (version 1)               */
/***************************************************************************/
booleen * explorationLargeur(graphe* G, int x){
  booleen *Z;       /* tableau booleens pour stocker l'exploration */
  ListeFIFO *E, *D; /* Liste pour les ensembles de sommets */
  ListeFIFO *tmp;   /* variable temporaire pour permettre l'echange des liste E et D */
  pcell p; /* pointeur-maillon pour parcourir des listes de successeurs */
  int y, z;         /* sommets du graphe */
  int k;            /* num�ro d'iteration */

  /* initialisation de l'ensemble E : E := {x}*/
  E = initListeFIFO(G->nsom);
  insertionListeFIFO(E, x);
  /* initialisation de l'ensemble Z : Z := {x}*/
  Z = (booleen*) calloc(G->nsom, sizeof(booleen));
  Z[x] = VRAI;
  /* initialisation de l'ensemble D */
  D = initListeFIFO(G->nsom);

  /* Completer ici avec le code l'algorithme exploration largeur */

  while(estNonVideListeFIFO(E)) {
    y = selectionSuppressionListeFIFO(E);

    booleen *Y = (booleen*) calloc(G->nsom, sizeof(booleen));
    Y[y] = VRAI;

    Y = dilatation(G, Y);

    for(z = 0; z < G->nsom; z++) {
      if(Y[z] && !Z[z]) {
        insertionListeFIFO(E, z);
        Z[z] = VRAI;
      }
    }
  }

  termineListeFIFO(D);
  termineListeFIFO(E);
  
  return Z;
}

graphe * symEfficace(graphe * g)
/* ====================================================================== */
{
  graphe *g_1;
  int nsom, narc, k, x, y;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  g_1 = initGraphe(nsom, narc);

  /* A Completer ci-dessous, possible en 3 lignes, souhaitable en moins de 6 :-) */
    for (x = 0; x < nsom; x++) {
    p = g->gamma[x];          
    while(p != NULL) {
      ajouteSuccesseur(g_1, p->som, x);
      p = p->suivant;
    }
  }


  return g_1;
} /* Sym() */


/* ====================================================================== */
int main(int argc, char **argv)
/* ====================================================================== */
{
  graphe *G, *Gs;
  int x;               /* sommet de depart � explorer */
  int y;               /* indice muet pour un sommet*/
  booleen *X, *Y;          /* tableau pour stocker l'exploration du graphe */
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

  X = explorationLargeur(G,x);
  Gs = symEfficace(G);
  Y = explorationLargeur(Gs, x);

  for(int n = 0; n < G->nsom; n++) X[n] = X[n] && Y[n];

  fin = clock();

  /* On afffiche l'exploration et on compte le nombre de sommets explor�s */
  nZ = 0;
  printf("The SCC to the node %d contain the nodes", x);
  for(y = 0; y < G->nsom; y++)
    if(X[y] == VRAI){
      printf(" %d", y);
      nZ++;
    }
  printf(".\n\n\n%d nodes encontered in the SCC\n", nZ);

//   /* si un nom est diponible pour les sommets, on affiche aussi les noms */
//   if( (G->info != NULL) && G->info[0].nom != NULL){
//     printf("--\nWith the names :\n");
//     for(y = 0; y < G->nsom; y++)
//       if(X[y] == VRAI)
// 	printf("%s\n", G->info[y].nom);
//   }

  //printf("%d sommets poeuvent etre atteints depuis %d et %d sommets ne peuvent pas etre atteints \n", nZ, x, G->nsom - nZ);
  
  free(X);

  printf("SCC calculated in %lf seconds\n", ((double)fin-debut)/CLOCKS_PER_SEC);

  return 0;
} /* main() */

