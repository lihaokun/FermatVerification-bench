/* run.config
   OPT: -wp-no-print -wp-rte
*/

/* run.config_qualif
   OPT: -then -wp-rte -wp
*/

/* ************/
/* Solution de TP donné par Julien à L'IFIPS */
/* ************/

#define BOUND 100

/* La fonction [pair(a, len)] prend en entrée un tableau [a] de longueur
   [len] ne contenant que des valeurs entre 0 et [BOUND-1] (inclus) et retourne
   [-1] si [a] ne contient pas deux éléments de même valeur et retourne un des
   indices correspondant à une valeur en double sinon. */

/*
  Questions:

  1. Définir un prédicat [has_pair(a, len)] qui est valide si et seulement s'il
  existe (au moins) deux éléments du tableau [a] de longueur [len] qui sont
  égaux.

  2. En utilisant des comportements (behaviors), donner une spécification en
  ACSL de la fonction [pair].

  3. Prouver cette fonction en utilisant le WP et Alt-Ergo. La preuve doit
  inclure la terminaison et l'absence d'erreur à l'exécution.
*/




int pair(int *a, int len)
{
  char seen[BOUND];

  
  for(int i = 0; i < BOUND; i++)
    seen[i] = 0;

  
  for(int i = 0; i < len; i++) {
    int v = a[i];
    if (seen[v]) return i;
    else seen[v] = 1;
  }

  return -1;
}
