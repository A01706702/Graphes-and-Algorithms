/* strcture pour mémoriser les informations additionelles des sommets */

typedef struct {
  //!  nom du sommet
  char *nom; 
  //!  abcisse du sommet 
  double x;        
  //!  ordonnee du sommet
  double y;        
  //!  altitude du sommet
  double alt;        
  //!  tableau de poids associées aux sommets
  double poids;
} sInfo;


/* structure de maillon pour les listes chaînees de successeurs. */
typedef struct cell {
//! index du sommet
  int som;
//! suite de la liste ou pointeur NULL
  struct cell * suivant; 
} cell;

/*! \var pcell
    \brief pointeur sur une cellule
*/
typedef cell * pcell; 


/* structure pour la representation des graphes */
typedef struct graphe {

  /* informations globales */
//!  nombre de sommets 
  int nsom;          
//!  nombre d'arcs
  int narc;
//!  tableau des informations aditionnelles pour les sommets
  sInfo * info;
//!  tableau des listes de successeurs indexé par les sommets 
  pcell * gamma;    

  /* Champs additionnels utilisés pour gérer efficacement et
     dynamiquement la mémoire des graphes. */
  /* Il n'est pas nécessaire de s'y intéresser dans le cadre du cours
     graphes et algorithmes */
  //!  nombre maximum d'arcs
  int nmaxarc;
  //!  tableau des cellules en réserve 
  pcell reserve;    
  //!  liste des cellules libres gérée en pile lifo 
  pcell libre;      
} graphe;

typedef struct graphePondere {

  /* informations globales */
//!  nombre de sommets 
  int nsom;          
//!  nombre d'arcs
  int narc;

  //! tableau de sommets initiaux des aretes
  int* I;
  //! tableau de sommets terminaux des aretes
  int* T;
  //! ponderation des aretes
  double *poids;

  //!  tableau des informations aditionnelles pour les sommets
  sInfo * info;
 
} graphePondere;


/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS D'ALLOCATION / LIBERATION POUR UN GRAPHE */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * initGraphe(int nsom, int nmaxarc);
extern graphePondere * initGraphePondere(int nsom, int nmaxarc);
extern void termineGraphe(graphe * g);
extern void termineGraphePondere(graphePondere *Gp);


/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE lECTURE ET DE SAUVEGARDE EN FICHIER  */
/* ====================================================================== */
/* ====================================================================== */

extern graphe * litGraphe(char * filename);
extern graphePondere *litGraphePondere(char * filename);
extern void sauvegardeGraphe(graphe * g, char *filename, int opt);
extern void cpyInfo(graphe *g1, graphe *g2);
extern void cpyInfo2(graphePondere *g1, graphe *g2);

/* ====================================================================== */
/* ====================================================================== */
/* FONCTIONS DE MANIPULATION DES ARCS */
/* ====================================================================== */
/* ====================================================================== */

extern void ajouteSuccesseur(graphe * g, int i, int s);
