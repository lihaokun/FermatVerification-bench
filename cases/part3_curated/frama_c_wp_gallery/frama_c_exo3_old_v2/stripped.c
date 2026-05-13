/* run.config
   OPT: -wp-model Typed+Ref -wp-no-print -wp-rte
*/
/* run.config_qualif
   OPT: -wp-model Typed+Ref -then -wp-rte -wp
*/
/* ************/
/* Autre solution d'une proposition de TP donné par Julien à L'IFIPS */
/* ************/

#define BOUND 100

/* La fonction [equal_elements(a, v1, v2)] prends en entrée un tableau [a] de
   longueur [BOUND+2] avec [BOUND >= 2] ne contenant que des valeurs entre [0]
   et [BOUND-1] et contenant aussi au moins deux valeurs différentes
   apparaissent deux fois (il contient donc au moins deux couples de valeurs
   égales). La fonction cherche ces deux valeurs et les stocke dans [v1] et
   [v2]. */

/*

  Cette version des spécifications utilise la notion du nombre d'occurence

*/






void equal_elements(int *a, int *v1, int *v2)
{
  char seen[BOUND];

  *v1 = -1;
  *v2 = -1;

  
  for(int i = 0; i < BOUND; i++){
  ICI:
    seen[i] = 0;
    
  }

  
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
