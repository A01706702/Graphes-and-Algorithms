#include <stdlib.h>
#include <stdio.h>
 #include <string.h>

#include "graphes.h"

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ALLOCATION / LIBERATION POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn graphe * InitGraphe(int nsom, int nmaxarc)
    \param   nsom (entrée) : nombre de sommets.
    \param nmaxarc (entrée) : nombre maximum d'arcs.
    \return un graphe.
    \brief alloue la memoire nécessaire pour représenter un graphe a 'nsom' sommets,
              possédant au maximum 'nmaxarc' arcs. 
              Retourne un pointeur sur la structure allouée. 
*/
graphe * initGraphe(int nsom, int nmaxarc)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "InitGraphe"
{
  graphe * g;
  int i;
  
  g = (graphe *)malloc(sizeof(graphe));
  if (g == NULL)
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->reserve = (cell *)malloc(nmaxarc * sizeof(cell));
  if (g->reserve == NULL)
  {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
      exit(0);
  }

  g->gamma = (pcell *)calloc(nsom, sizeof(pcell)); /* calloc met a 0 la memoire allouee */
  if (g->gamma == NULL)
  {   fprintf(stderr, "%s : calloc failed\n", F_NAME);
      exit(0);
  }
  g->info = (sInfo*) calloc(nsom, sizeof(sInfo));

  if (g->info == NULL)
  {
    fprintf(stderr, "%s : calloc failed\n", F_NAME);
    exit(0);
  }

  
  g->nsom = nsom;
  g->nmaxarc = nmaxarc;
  g->narc = 0;

  /* construit la liste initiale de cellules libres */
  for (i = 0; i < nmaxarc - 1; i++)
    (g->reserve+i)->suivant = g->reserve+i+1;
  (g->reserve+i)->suivant = NULL;
  g->libre = g->reserve;  

  
  return g;
} /* InitGraphe() */

/* ====================================================================== */
/*! \fn void TermineGraphe(graphe * g)
    \param g (entrée) : un graphe.
    \brief libère la memoire occupée par le graphe g. 
*/
void termineGraphe(graphe * g)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "TermineGraphe"
{
  int i, n = g->nsom;
  free(g->reserve);
  if (g->gamma) free(g->gamma);
  if (g->info)
  {
    for (i = 0; i < n; i++) if(g->info[i].nom) free(g->info[i].nom);
    free(g->info);
  }
  free(g);
} /* TermineGraphe() */


/* ====================================================================== */
/*! \fn graphe * ReadGraphe(char * filename)
    \param   filename (entrée) : nom du fichier graphe.
    \return un graphe.
    \brief Lit les données d'un graphe dans le fichier filename, retourne un pointeur sur la structure graphe construite. 
*/
graphe * litGraphe(char * filename)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ReadGraphe"
{
#define TAILLEBUF 4096
  graphe * g;
  int i, n, m, t, q;
  char buf[TAILLEBUF];
  char *ret;

  FILE *fd = NULL;

  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", F_NAME, filename);
    return NULL;
  }

  fscanf(fd, "%d %d\n", &n, &m);
  g = initGraphe(n, m);
  do
  {
    ret = fgets(buf, TAILLEBUF, fd);
    if ((ret != NULL) && (strncmp(buf, "noms sommets", 12) == 0))
    {
      for (i = 0; i < n; i++)
      {
        fscanf(fd, "%d\n", &t);
        fgets(buf, TAILLEBUF, fd);
	buf[strlen(buf)-1] = '\0';
        g->info[t].nom = (char *)malloc((strlen(buf)+1) * sizeof(char));
        if (g->info[t].nom == NULL)
        {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
            exit(0);
        }
        strcpy(g->info[t].nom, buf);
      } /* for (i = 0; i < n; i++) */
    } /* if ((ret != NULL) && (strcmp(buf, "noms sommets") == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "val sommets", 11) == 0))
    {
      double v;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf\n", &t, &v);
        g->info[t].poids = (double)v; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "val sommets", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0))
    {
      double x, y;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf %lf\n", &t, &x, &y);
        g->info[t].x = x; 
        g->info[t].y = y; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0))
    {
      fprintf(stderr,"LitGraphe : arcs values indisponibles dans cette version\n");
      exit(0);
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0))
    {
      for (i = 0; i < m; i++)  
      {
        fscanf(fd, "%d %d\n", &t, &q);
        ajouteSuccesseur(g, t, q);
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0)) */
  } while (ret != NULL);

  return g;
} /* ReadGraphe() */



/* ====================================================================== */
/*! \fn graphe * InitGraphe(int nsom, int nmaxarc)
    \param   nsom (entrée) : nombre de sommets.
    \param nmaxarc (entrée) : nombre maximum d'arcs.
    \return un graphePondere.
    \brief alloue la memoire nécessaire pour représenter un graphe a 'nsom' sommets,
              possédant au maximum 'nmaxarc' arcs. 
              Retourne un pointeur sur la structure allouée. 
*/

graphePondere * initGraphePondere(int nsom, int narete){
  graphePondere *Gp;

  Gp =(graphePondere*)malloc(sizeof(graphePondere));
  Gp->nsom = nsom;
  Gp->narc = narete;
  Gp->I = (int*)calloc(narete, sizeof(int));
  Gp->T = (int*)calloc(narete, sizeof(int));
  Gp->poids= (double*)calloc(narete, sizeof(double));
  Gp->info = (sInfo*) calloc(nsom, sizeof(sInfo));
  if ((Gp->info == NULL) || (Gp->I == NULL) || (Gp->T == NULL) || (Gp->poids == NULL) )
  {
    fprintf(stderr, "%s : calloc failed\n", F_NAME);
    exit(0);
  }
  return Gp;
}

void termineGraphePondere(graphePondere *Gp){
  free(Gp->I);
  free(Gp->T);
  free(Gp->poids);
  free(Gp->info);
  free(Gp);
}


/* ====================================================================== */
/*! \fn graphe * ReadGraphePondere(char * filename)
    \param   filename (entrée) : nom du fichier graphe.
    \return un graphe pondere.
    \brief Lit les données d'un graphe dans le fichier filename, retourne un pointeur sur la structure graphePondere construite. 
*/
graphePondere * litGraphePondere(char * filename)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "ReadGraphe"
{
#define TAILLEBUF 4096
  graphePondere * g;
  int i, n, m, t, q;
  char buf[TAILLEBUF];
  char *ret;

  FILE *fd = NULL;

  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "%s: file not found: %s\n", F_NAME, filename);
    return NULL;
  }

  fscanf(fd, "%d %d\n", &n, &m);
  g = initGraphePondere(n, m);
  do
  {
    ret = fgets(buf, TAILLEBUF, fd);
    if ((ret != NULL) && (strncmp(buf, "noms sommets", 12) == 0))
    {
      for (i = 0; i < n; i++)
      {
        fscanf(fd, "%d\n", &t);
        fgets(buf, TAILLEBUF, fd);
	buf[strlen(buf)-1] = '\0';
        g->info[t].nom = (char *)malloc((strlen(buf)+1) * sizeof(char));
        if (g->info[t].nom == NULL)
        {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
            exit(0);
        }
        strcpy(g->info[t].nom, buf);
      } /* for (i = 0; i < n; i++) */
    } /* if ((ret != NULL) && (strcmp(buf, "noms sommets") == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "val sommets", 11) == 0))
    {
      double v;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf\n", &t, &v);
        g->info[t].poids = (double)v; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "val sommets", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0))
    {
      double x, y;
      for (i = 0; i < n; i++)  
      {
        fscanf(fd, "%d %lf %lf\n", &t, &x, &y);
        g->info[t].x = x; 
        g->info[t].y = y; 
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "coord sommets", 13) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0))
    {
      double v;
      for (i = 0; i < m; i++)  
      {
        fscanf(fd, "%d %d %lf\n", &t, &q, &v);
	g->I[i] = t;
	g->T[i] = q;
	g->poids[i] = v;
      }
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs values", 11) == 0)) */
    else if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0))
    {
      fprintf(stderr,"LitGraphe : arcs non values indisponibles dans cette version\n");
      
    } /*  if ((ret != NULL) && (strncmp(buf, "arcs", 4) == 0)) */
  } while (ret != NULL);

  return g;
} /* ReadGraphe() */


/* ====================================================================== */
/*! \fn void SaveGraphe(graphe * g, char *filename, int opt) 
    \param g (entrée) : un graphe.
    \param filename (entrée) : nom du fichier à générer.
    \brief sauve le graphe g dans le fichier filename, les informations aditionnelles du graphe sont sauvegardées en fonction de la valeur du parametre opt. Si 
    - le chiffre des unités d'opt vaut 1, le nom des sommets est sauvegardé 
    - le chiffre des dizaines d'opt vaut 1, l'abscice et l'ordonnée des sommet sont sauvegardés
    - le chiffre des dizaines d'opt vaut 2, l'abscice, l'ordonnée et l'altitude des sommet sont sauvegardés
    - le chiffre des centaines d'opt vaut 1, le poids des sommets est sauvegarde.
*/
void sauvegardeGraphe(graphe * g, char *filename, int opt) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "SaveGraphe"
{
  int i, j, n = g->nsom, m = g->narc;
  pcell p;
  FILE * fd = NULL;
  double v;

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return;
  }
  
  fprintf(fd, "%d %d\n", n, m);

  if ( (opt/100)%10 == 1)
  {
    fprintf(fd, "val sommets\n");
    for (i = 0; i < n; i++) 
      fprintf(fd, "%d %lf\n", i, g->info[i].poids);
  }

  if ( (opt/10)%10 > 0 )
  {
    fprintf(fd, "coord sommets\n");
    for (i = 0; i < n; i++) 
      fprintf(fd, "%d %g %g\n", i, g->info[i].x, g->info[i].y);
  }

  if ( (opt/10)%10 == 2 )
  {
    fprintf(fd, "alt sommets\n");
    for (i = 0; i < n; i++) 
      fprintf(fd, "%d %g\n", i, g->info[i].alt);
  }

  if ( opt%10 ==  1)
  {
    fprintf(fd, "noms sommets\n");
    for (i = 0; i < n; i++) 
      fprintf(fd, "%d %s\n", i, g->info[i].nom);
  }
    
  fprintf(fd, "arcs\n");
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->suivant)
      fprintf(fd, "%d %d\n", i, p->som);
  
  fclose(fd);
} /* sauvegardeGraphe() */


pcell cellLibre(graphe *g)
{
  pcell p;
  
  /* prendre et retirer un maillon de la liste des maillons libres */
  if (g->libre == NULL) 
  {
    fprintf(stderr, "%s : plus de cellules libres\n", F_NAME);
    exit(0);
  }
  p = g->libre;
  g->libre = p->suivant;
  return p;
  
}

/* ====================================================================== */
/*! \fn void ajouteSuccesseur(graphe * g, int i, int s)
    \param g (entrée/sortie) : un graphe.
    \param i (entrée) : extrémité initiale de l'arc.
    \param s (entrée) : extrémité final de l'arc.
    \brief ajoute l'arc (i,f) au graphe g (application gamma seulement). 
*/
void ajouteSuccesseur(graphe * g, int i, int f)
/* ====================================================================== */
{ 
  pcell p = cellLibre(g);

  /* Utilise ce maillon pour représenter le sommet f */
  p->som = f;

  /* Insere ce maillon en tete de la liste des successeurs de i */
  p->suivant = g->gamma[i];
  g->gamma[i] = p;
  
  g->narc++;
} /* AjouteArc() */


void cpyInfo(graphe *g1, graphe *g2){
  int i;
  if(g1->nsom != g2->nsom){
    printf("Ne peut pas copier les informatioins des sommets des graphes\n");
    return;
  }
  
  for(i = 0; i < g1->nsom; i++){
    if(g1->info[i].nom != NULL){
      g2->info[i].nom  = malloc(sizeof(char)*strlen(g1->info[i].nom) +1);
      if(g2->info[i].nom == NULL){
	fprintf(stderr, "cpyInfo : ne peut allouer la mémoire pour stocker le nom du sommet\n");
	exit(0);
      }
      strcpy(g2->info[i].nom, g1->info[i].nom);
    }
    g2->info[i].x =  g1->info[i].x;
    g2->info[i].y =  g1->info[i].y;
    g2->info[i].alt =  g1->info[i].alt;
    g2->info[i].poids =  g1->info[i].poids;
  }
    
}

void cpyInfo2(graphePondere *g1, graphe *g2){
  int i;
  if(g1->nsom != g2->nsom){
    printf("Ne peut pas copier les informatioins des sommets des graphes\n");
    return;
  }
  
  for(i = 0; i < g1->nsom; i++){
    if(g1->info[i].nom != NULL){
      g2->info[i].nom  = malloc(sizeof(char)*strlen(g1->info[i].nom) +1);
      if(g2->info[i].nom == NULL){
	fprintf(stderr, "cpyInfo : ne peut allouer la mémoire pour stocker le nom du sommet\n");
	exit(0);
      }
      strcpy(g2->info[i].nom, g1->info[i].nom);
    }
    g2->info[i].x =  g1->info[i].x;
    g2->info[i].y =  g1->info[i].y;
    g2->info[i].alt =  g1->info[i].alt;
    g2->info[i].poids =  g1->info[i].poids;
  }
    
}
