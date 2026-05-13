/* run.config
   OPT: -wp-model Typed+Ref -wp-no-print -wp-rte
*/
/* run.config_qualif
   OPT: -wp-model Typed+Ref -then -wp-rte -wp
*/

/* ************/
/* Solution d'une proposition TP donné par Julien à L'IFIPS */
/* ************/

#define BOUND 100

/* La fonction [equal_elements(a, v1, v2)] prends en entrée un tableau [a] de
   longueur [BOUND+2] avec [BOUND >= 2] ne contenant que des valeurs entre [0]
   et [BOUND-1] et contenant aussi au moins deux valeurs différentes
   apparaissent deux fois (il contient donc au moins deux couples de valeurs
   égales). La fonction cherche ces deux valeurs et les stocke dans [v1] et
   [v2]. */

/*
  Questions:

  1. Remplacer le corps du prédicat [appear_twice(int *a, integer len, integer
  v)] qui prend un tableau [a] de longueur [len] et une valeur [v] de façon à ce
  qu'il soit valide si et seulement si [v] apparaît au moins deux fois dans [a].

  Afin de faciliter la tâche ultérieure d'Alt-Ergo (question 3), on veillera à
  faire en sorte de limiter au maximum la portée des quantificateurs. Ainsi, on
  préférera par exemple écrire
     
    \forall integer i; i == 0 ==> \exists integer j; i == j;

  plutôt que
     
    \forall integer i; \exists integer j; i == 0 ==> i == j;


  2. Donner une spécification en ACSL de la fonction [equal_elements].

  3. Prouver cette fonction en utilisant le WP avec le modèle Typed+ref et
  Alt-Ergo. La preuve doit inclure la terminaison et l'absence d'erreur à
  l'exécution.

  Pour activer le modèle Typed+ref directement à partir de la ligne de commandes
  (plutôt que dans la GUI), vous pouvez fournir l'option -wp-model Typed+ref à
  Frama-C.

  Les deux assertions dans le code doivent également être prouvées. Elles
  facilitent la tâche d'Alt-Ergo (et peuvent vous permettre de mieux comprendre
  l'algorithme).

*/




void equal_elements(int *a, int *v1, int *v2)
{
  char seen[BOUND];

  *v1 = -1;
  *v2 = -1;

  
  for(int i = 0; i < BOUND; i++)
    seen[i] = 0;

  
  for(int i = 0; i < BOUND+2; i++) {
    int v = a[i];
    if (seen[v]) {
      if (*v1 == -1) *v1 = v;
      else if (*v2 == -1 && v != *v1) *v2 = v;
      
    } else {
      seen[v] = 1;
      
    }
  }
}
