#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "graphes.h"


/* ====================================================================== */
/*! \fn int EstSuccesseur(graphe *g, int x, int y) 
    \param g (entr�e) : un graphe.
    \param i (entr�e) : un sommet de g.
    \param s (entr�e) : un sommet de g.
    \return bool�en.
    \brief retourne 1 si le sommet 'y' est un successeur du sommet 'x', 0 sinon.
*/
int estSuccesseur(graphe *g, int x, int y) 
/* ====================================================================== */
{
  pcell p; /* pointeur de maillon utilise pour parcourir la liste de successeurs */
  int z; /* pour stocker les successeurs de x */
  /* parours de la liste des successeurs de x */
  p = g->gamma[x];
  while(p != NULL){
    z = p->som;
    if (z == y) return 1;
    p = p->suivant;
  }
  /* On a atteint la fin de la liste des successeurs sans trouver y */
  return 0;
} /* EstSuccesseur() */


/* ====================================================================== */
/*! \fn graphe * sym_inefficace(graphe * g)
    \param g (entr�e) : un graphe.
    \return un graphe.
    \brief construit et retourne le graphe g_1 sym�trique du graphe g. 
    \warning  L'algorithme est inefficace (complexit� quadratique).
*/
graphe * symInefficace(graphe * g)
/* ====================================================================== */
{
  graphe *g_1;
  int nsom, narc, k, x, y;
  pcell p;

  nsom = g->nsom;
  narc = g->narc;
  g_1 = initGraphe(nsom, narc); /* Initialise une structure pour un graphe de nsom sommets et narc arcs */
 
  for (y = 0; y < nsom; y++) /* pour tout y sommet de g */
    for (x = 0; x < nsom; x++) /* pour tout x sommet de g */
    if (estSuccesseur(g, x, y)) /* si y est un successeur de x dans g */
        ajouteSuccesseur(g_1, y, x); /*  gamma_1[y] =: gamma_1[y] U {x} */
  return g_1;
} /* Sym() */

/* ====================================================================== */
/*! \fn graphe * sym_inefficace(graphe * g)
    \param g (entr�e) : un graphe.
    \return un graphe.
    \brief construit et retourne le graphe g_1 sym�trique du graphe g. 
    \warning  L'algorithme est inefficace (complexit� quadratique).
*/
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
  graphe *g, *g_1;
  int na, ns;     /* nombre d'arcs, nombre de sommets */
  clock_t debut, fin;
  
  if (argc != 3)
  {
    fprintf(stderr, "usage: %s <donnee.graphe> <resultat.graphe>\n", argv[0]);
    exit(0);
  }

  g = litGraphe(argv[1]);       /* Lit un graphe stock�e dans un fichier */

  fprintf(stderr, "%s lu\n", argv[1]);
  
  debut =  clock();

  g_1 = symEfficace(g);       /* traitement : calcule le symetrique de g */

  fin = clock();

  printf("Symetrique calcule en %lf secondes\n", ((double)fin-debut)/CLOCKS_PER_SEC);
    
  /* observer que l'on n'a pas recopi� les dans le sym�trique les informatioin relative au sommet, on peut le faire avec la fonction cpyInfo(g,g_1) */

  cpyInfo(g,g_1);  
  sauvegardeGraphe(g_1, argv[2], 1); /* Sauvegarde le r�sultat dans un fichier, le troisi�me param�tre sp�cifie si l'on veut sauvegrader les informations additionnelles des sommets. Par exemple, 1 indique de sauvegarder uniquement le nom des sommets, 11 indique de sauver le nom et les coordonn�es des sommets [souhaitable pour le graphes des pistes cyclables], 0 indique de ne sauvegarder aucune information relative aux sommets */
 
  
  termineGraphe(g);
  termineGraphe(g_1);

  return 0;
} /* main() */

