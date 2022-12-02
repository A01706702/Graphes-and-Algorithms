#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "graphes.h"

/*************************************************
    Fonctions pour la generation de postscript
    Michel Couprie
**************************************************/


/*========================================*/
void PSDraw (FILE *fd, double x, double y)
/*========================================*/
{
  int xx, yy;
  xx = (int)x;
  yy = (int)y;
  fprintf(fd, "%d %d lineto ", xx, yy);
} /* PSDraw() */

/* ========================================== */
void EPSHeader(FILE *fd, double figure_width, double figure_height, double line_width, int font_size )
/* ========================================== */
{
  fprintf(fd, "%%!PS-Adobe-2.0 EPSF-2.0\n");
  fprintf(fd, "%%%%BoundingBox: 0 0 %g %g\n", figure_width, figure_height);
  fprintf(fd, "%g setlinewidth\n", line_width);
  fprintf(fd, "/corps %d def\n", font_size);
  fprintf(fd, "/Fn /Courier findfont corps scalefont def\n");
  fprintf(fd, "/setn {	/CurrentFont Fn def } def\n");
  fprintf(fd, "setn\n");
  fprintf(fd, "/o { CurrentFont setfont show } bind def\n");
} /* EPSHeader() */


/*========================================*/
void PSDrawcircle (FILE *fd, double x, double y, double r)
/*========================================*/
{
  int xx, yy, rr;
  xx = (int)x;
  yy = (int)y;
  rr = (int)r;
  fprintf(fd, "newpath ");
  fprintf(fd, "%d %d %d 0 360 arc ", xx, yy, rr);
  fprintf(fd, "stroke\n");
} /* PSDrawcircle() */


/*========================================*/
void PSString (FILE *fd, double x, double y, char *s)
/*========================================*/
{
  int xx, yy, rr;
  xx = (int)x;
  yy = (int)y;
  fprintf(fd, "newpath ");
  fprintf(fd, "%d %d moveto ", xx, yy);
  fprintf(fd, "(%s)o ", s);
  fprintf(fd, "stroke\n");
} /* PSString() */

/*========================================*/
void PSMove (FILE *fd, double x, double y) 
/*========================================*/
{
  int xx, yy;
  xx = (int)x;
  yy = (int)y;
  fprintf(fd, "%d %d moveto ", xx, yy);
} /* PSMove() */


/*========================================*/
void PSLine (FILE *fd, double xd, double yd, double xf, double yf)
/*========================================*/
{
  fprintf(fd, "newpath ");
  PSMove(fd, xd, yd);
  PSDraw(fd, xf, yf);
  fprintf(fd, "stroke\n");
} /* PSLine() */

/* ========================================== */
void PSFooter(FILE *fd)
/* ========================================== */
{
  fprintf(fd, "showpage\n");
} /* PSFooter() */

/* ====================================================================== */
/* ====================================================================== */
/* GENERATION POSTSCRIPT */
/* ====================================================================== */
/* ====================================================================== */

/* ====================================================================== */
/*! \fn void PSGraphe(graphe * g, char *filename, double r, double t, double marge) 
    \param g (entrée) : un graphe.
    \param filename (entrée) : nom du fichier postscript à générer.
    \param r (entrée) : rayon des cercles qui représentent les sommets (0 pour ne pas les dessiner).
    \param t (entrée) : taille (demi-longueur) des flèches pour les arcs (0 pour ne pas les dessiner).
    \param marge (entrée) : marge en x et en y.
    \brief génère une figure PostScript d'après la représentation "successeurs" du graphe g. 
*/
void PSGraphe(graphe * g, char *filename, double r, double t, double marge) 
/* ====================================================================== */
#undef F_NAME
#define F_NAME "PSGraphe"
{
  int i, j, n = g->nsom;
  double xmin, xmax, ymin, ymax;
  double x1, y1, x2, y2, x3, y3, x, y, a, b, d;
  pcell p;
  FILE * fd = NULL;
  
  if (g->gamma == NULL) 
  {  fprintf(stderr, "%s: representation successeurs absente\n", F_NAME);
     return;
  }
  
  if (g->info == NULL) 
  {  fprintf(stderr, "%s: coordonnees des sommets absentes\n", F_NAME);
     return;
  }

  fd = fopen(filename,"w");
  if (!fd)
  {
    fprintf(stderr, "%s: cannot open file: %s\n", F_NAME, filename);
    return;
  }

  /* determine le rectangle englobant et genere le header */
  xmin = xmax = g->info[0].x;
  ymin = ymax = g->info[0].y;
  for (i = 1; i < n; i++) 
  {
    if (g->info[i].x < xmin) xmin = g->info[i].x; else if (g->info[i].x > xmax) xmax = g->info[i].x;
    if (g->info[i].y < ymin) ymin = g->info[i].y; else if (g->info[i].y > ymax) ymax = g->info[i].y;
  }
  EPSHeader(fd, xmax - xmin + 2.0 * marge, ymax - ymin + 2.0 * marge, 2.0, 6);
  
  /* dessine les sommets */
  for (i = 0; i < n; i++) 
    PSDrawcircle(fd, g->info[i].x-xmin+marge, g->info[i].y-ymin+marge, r);
  if (g->info[0].nom)
    for (i = 0; i < n; i++) 
      PSString(fd, g->info[i].x-xmin+marge+2*r, g->info[i].y-ymin+marge+2*r, g->info[i].nom);

  /* dessine les arcs */
  if (r > 0.0)
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->suivant)
    {
      j = p->som;
      PSLine(fd, g->info[i].x-xmin+marge, g->info[i].y-ymin+marge, g->info[j].x-xmin+marge, g->info[j].y-ymin+marge);
    }

  /* dessine les fleches sur les arcs */
  if (t > 0.0)
  for (i = 0; i < n; i++) 
    for (p = g->gamma[i]; p != NULL; p = p->suivant)
    {
      j = p->som;
      x1 = g->info[i].x-xmin+marge;
      y1 = g->info[i].y-ymin+marge;
      x2 = g->info[j].x-xmin+marge;
      y2 = g->info[j].y-ymin+marge;
      x = (x2 + x1) / 2;
      y = (y2 + y1) / 2;
      a = x2 - x1;
      b = y2 - y1;             /* (a,b) est un vecteur directeur de l'arc */
      d = sqrt(a * a + b * b); /* longueur de l'arc */
      if (d > 1) // sinon on ne dessine rien
      { 
        a /= d; b /= d;          /* norme le vecteur */
        x1 = x + 2 * t * a;
        y1 = y + 2 * t * b;      /* pointe de la fleche */
        x2 = x - 2 * t * a;
        y2 = y - 2 * t * b;      /* base de la fleche */
        x3 = x2 + t * -b;        /* (-b,a) est normal a (a,b) */
        y3 = y2 + t * a;
        x2 = x2 - t * -b;
        y2 = y2 - t * a;
        PSLine(fd, x1, y1, x2, y2);
        PSLine(fd, x2, y2, x3, y3);
        PSLine(fd, x3, y3, x1, y1);
      }
    }
  
  PSFooter(fd);
  fclose(fd);
} /* PSGraphe() */

/* ====================================================================== */
/*! \fn void PlongementCirculaire(graphe * g, double r)
    \param g (entrée/sortie) : un graphe.
    \param r (entrée) : le rayon du cercle.
    \brief affecte à chaque sommet des coordonnées (x,y) régulierement réparties sur un cercle. 
*/
void PlongementCirculaire(graphe * g, double r)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "PlongementCirculaire"
{
  int i, n = g->nsom;
  double alpha = 2 * M_PI / n;
  
  if (g->info == NULL) 
  {  fprintf(stderr, "%s: coordonnees des sommets non allouees\n", F_NAME);
     return;
  }

  for (i = 0; i < n; i++) 
  {
    g->info[i].x = cos(alpha * i) * r;
    g->info[i].y = sin(alpha * i) * r;
  }
} /* PlongementCirculaire() */


/* ====================================================================== */
/*! \fn void AutoNomsSommets(graphe * g, int mode)
    \param g (entrée) : un graphe.
    \param mode (entrée) : 1 pour que les noms soient les indices des sommets,
                           2 pour que les noms soient les valeurs associées aux sommets,
                           3 pour des noms composes de l'indice et de la valeur.
    \brief génère automatiquement des noms pour les sommets du graphe g. 
*/
void AutoNomsSommets(graphe * g, int mode)
/* ====================================================================== */
#undef F_NAME
#define F_NAME "AutoNomsSommets"
{
  int len, i, n = g->nsom;
  char buf[80];



  if ((mode < 1) || (mode > 3))
  {   fprintf(stderr, "%s : bad mode : %d\n", F_NAME, mode);
      exit(0);
  }

  for (i = 0; i < n; i++)
  {
    if (mode == 1)
      sprintf(buf, "%d", i);
    else if (mode == 2)
      sprintf(buf, "%g", (double)(g->info[i].poids));
    else
      sprintf(buf, "%d [%g]", i, (double)(g->info[i].poids));
    len = strlen(buf);
    g->info[i].nom = (char *)malloc((len+1) * sizeof(char));
    if (g->info[i].nom == NULL)
    {   fprintf(stderr, "%s : malloc failed\n", F_NAME);
        exit(0);
    }
    strcpy(g->info[i].nom, buf);
  }
} /* AutoNomsSommets() */


int main(int argc, char **argv)
{
  graphe *g;
  int option;
  char buf[256];

  if (argc != 3)
  {
    fprintf(stderr, "usage: %s option \n", argv[0]);
    fprintf(stderr, ", ou option est un entier indiquant\n");
    fprintf(stderr, ", si option=0, de generer automatiquement des positions pour les sommets\n");
    fprintf(stderr, ", si option=1, de generer automatiquement un nom pour les sommets\n");
    fprintf(stderr, ", si option=2, de generer automatiquement un nom et des positions pour les sommets\n");
    fprintf(stderr, ", sinon, d'utiliser les noms et les positions presents dans le fichier d'entre.\n");
    exit(0);
  }

  g = litGraphe(argv[1]);             /* lit le graphe a partir du fichier */

  option = atoi(argv[2]);
  
  if( (option == 0) || (option == 2))
    PlongementCirculaire(g, 150);        /* plonge le graphe dans le plan */
  if ( (option == 0) || (option == 2))
      AutoNomsSommets(g, 1);               /* noms des sommet = indices */
  sprintf(buf, "%s.eps", argv[1]);     /* construit le nom du fichier PostScript */
  PSGraphe(g, buf, 5, 5, 60);         /* genere une figure en PostScript */

  termineGraphe(g);

  return 0;
} /* main() */
