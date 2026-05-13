/* run.config
   OPT: -wp-no-print -wp-rte -no-warn-signed-overflow
*/

/* run.config_qualif
   OPT: -then -wp-rte -no-warn-signed-overflow -wp
*/

// note: exo hors vérification de l'absence de débordements

/* ************/
/* Solution de TP donné par Julien à L'IFIPS */
/* ************/

/* Un sous-tableau [b] d'un tableau [a] est un sous-ensemble d'éléments
   contigüs de [a]. Par exemple, si a = { 0, 3, -1, 4 }, des sous-tableaux
   possibles sont {}, { 0 }, { 3, -1 }, { 0, 3, -1, 4 }.

   Un sous-tableau de [a] est dit maximal si la somme de ses éléments est au
   moins aussi grande que celle de n'importe quel autre sous-tableau de
   [a]. L'unique sous-tableau maximal de l'exemple précédent est { 3, -1, 4 }.

   Les sous-tableaux de taille 0 étant autorisés, un tableau avec uniquement des
   valeurs négatives a une somme maximale de 0.

   La fonction [max_subarray(a, len)] retourne la somme d'un sous-tableau
   maximal de [a], [len] étant la longueur de [a]. */

/*
  Questions:

   1. Donner une spécification en ACSL de cette fonction. Vous pouvez vous aider
   de la fonction logique [sum(int *a, integer low, integer high, integer len)]
   calculant la somme des éléments entre les indices [low] et [high] (inclus)
   d'un tableau [a] de longueur [len] et dont une version axiomatisée est
   fournie.

   2. Modifier le programme de manière à ajouter 3 variables ghosts [cur_low],
   [low] et [high] calculant respectivement:
   - l'indice minimum du sous-tableau duquel on est en train de calculer la 
   somme courante [cur];
   - l'indice minimum du sous-tableau maximal courant;
   - l'indice maximum du sous-tableau maximal courant;

   3. Prouver cette fonction avec WP et Alt-Ergo. Prouver la terminaison et
   l'absence d'accès mémoire indéfini, mais pas l'absence de débordements
   arithmétiques. Les options à fournir à Frama-C pour cela sont, dans cet
   ordre: -rte -no-warn-signed-overflow -then -wp
*/




int max_subarray(int *a, int len) {
  int max = 0;
  int cur = 0;
  

  
  for(int i = 0; i < len; i++) {
    cur = a[i] + cur;
    if (cur < 0) {
      cur = 0;
      
    }
    if (cur > max) {
      max = cur;
      
      
    }
  }
  return max;
}
