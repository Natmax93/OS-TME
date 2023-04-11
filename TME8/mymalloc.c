#include "affiche_tas.h"
#include <stdio.h>

char tas[TAILTAS+1];
int libre;

/* Pour chaque stratégie recherche dans le tas une zone libre de
   taille octets. Cette fonction retourne l'adresse du début de
   la zone, -1 si aucune zone de taille suffisante n’existe.
   *pred est l’adresse dans le tas du début de la zone précédant
   la zone retournée.
*/

// Recherche le premier emplacement libre trouvé
int first_fit(int taille, int *pred) {

  int zone_libre = libre;
  int taille_libre = tas[zone_libre];

  printf("zone_libre = %d\n", zone_libre);
  printf("taille_libre = %d\n\n", taille_libre);

  // On recherche une zone tant que l'on a pas trouvé une zone
  // de taille suffisante
  while (taille_libre < taille) {

    // On regarde la zone libre suivante
    zone_libre = tas[zone_libre+1];
    taille_libre = tas[zone_libre];
    printf("prochaine zone_libre = %d\n", zone_libre);
    printf("prochaine taille_libre = %d\n", taille_libre);
  }

  printf("\nzone_libre trouvée = %d\n", zone_libre);
  printf("taille_libre trouvée = %d\n", taille_libre);

  return 0;
}

// 
int tas_malloc();

int main() {
  tas_init();
  afficher_tas();
}
