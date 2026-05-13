/* run.config
   OPT: -wp-no-print -wp-rte
*/

/* run.config_qualif
   OPT: -then -wp-rte -wp
*/

/* ************/
/* Solution de TP donné par Julien à L'IFIPS */
/* ************/

/*
  Questions:

   1. Informellement, que calcule cette fonction?

   2. Donner une spécification en ACSL de cette fonction.

   3. Prouver cette fonction avec WP et Alt-Ergo. La preuve doit inclure la
  terminaison et l'absence d'erreur à l'exécution.

*/


int exo1(int *a, int len) {
  int i = 0, j = len - 1;
  
  while (i < j)
    if (a[i] <= a[j]) i++;
    else j--;
  return i;
}
