#include "affiche_tas.h"
#include <stdio.h>
#include <string.h>

char tas[TAILTAS+1];
int libre;

/* Pour chaque stratégie recherche dans le tas une zone libre de
   taille octets. Cette fonction retourne l'adresse du début de
   la zone, -1 si aucune zone de taille suffisante n’existe.
   *pred est l’adresse dans le tas du début de la zone précédant
   la zone retournée.
*/

// Recherche l'indice du premier emplacement libre trouvé
int first_fit(int taille, int *pred) {

  if (libre >= TAILTAS+1) {
    printf("Plus de place dans le tas\n");
    return -1;
  }

  int zone_libre = libre;
  int taille_libre = tas[zone_libre];

  printf("zone_libre = %d\n", zone_libre);
  printf("taille_libre = %d\n", taille_libre);

  // On recherche une zone tant que l'on a pas trouvé une zone
  // de taille suffisante ou que l'on a parcouru toutes les zones
  // libres
  while (taille_libre < taille && zone_libre != -1) {

    // On sauvegarde la zone libre précédente
    *pred = zone_libre;

    // On regarde la zone libre suivante
    zone_libre = tas[zone_libre+1];
    taille_libre = tas[zone_libre];
    printf("zone_libre précédente = %d\n", *pred);
    printf("prochaine zone_libre = %d\n", zone_libre);
    printf("prochaine taille_libre = %d\n", taille_libre);
  }

  // Aucune zone libre de taille suffisante trouvée
  if (zone_libre == -1 && taille_libre < taille) {
    printf("Aucune zone libre de taille suffisante disponible\n");
    return -1;
  }

  printf("zone_libre trouvée = %d\n", zone_libre);
  printf("taille_libre trouvée = %d\n", taille_libre);

  return zone_libre;
}

// Réserve dans le tas une zone de taille octets. Cette fonction
// retourne l'adresse du début de la zone allouée. En cas d'erreur
// (si l'allocation est impossible), la fonction retourne NULL.
char *tas_malloc(unsigned int taille) {

  int current_ind, pred_ind;

  // On recherche l'indice de la prochaine zone libre
  current_ind = first_fit(taille, &pred_ind);
  printf("\nIndice trouvé : current_ind = %d\n", current_ind);

  // Aucune zone libre n'a été trouvée
  if (current_ind == -1) {
    return NULL;
  }

  // Cas où la taille de la zone à allouer est inférieure à celle
  // de la zone libre trouvée
  if (taille < tas[current_ind]) {

    printf("Taille strictement inférieure à la taille de la zone libre\n");

    // Si la zone libre retournée correspond à la première zone
    // libre du tas (dont l'indice correspond à la variable libre),
    // on modifie la variable libre
    if (current_ind == libre) {
      libre += taille + 1;
      printf("C'est la première zone libre, nouvelle zone libre = %d\n", libre);
    }
    // Sinon on modifie la zone libre précédente pour qu'elle
    // indique l'indice de la nouvelle prochaine zone libre
    else {
      tas[pred_ind+1] = current_ind; 
    }

    // On calcule la taille de la zone libre restante
    tas[libre] = tas[current_ind] - taille - 1;

    // On calcule la prochaine zone libre
    tas[libre+1] = tas[current_ind+1];
  }

  // Cas où la taille de la zone à allouer est exactement égale à
  // celle de la zone libre trouvée
  else {

    printf("Taille exactement égale à la taille de la zone libre\n");
    
    // Si la zone libre retournée correspond à la première zone
    // libre du tas (dont l'indice correspond à la variable libre),
    // on modifie la variable libre pour qu'elle corresponde à la
    // zone libre suivante
    if (current_ind == libre) {
      libre = tas[current_ind+1];
      printf("C'est la première zone libre, nouvelle zone libre = %d\n", libre);
    }
    // Sinon on modifie la zone libre précédente pour qu'elle
    // indique l'indice de la nouvelle prochaine zone libre
    else {
      tas[pred_ind+1] = tas[current_ind+1]; 
    }
  }

  // On indique la taille de la zone nouvellement allouée
  tas[current_ind] = taille;

  return &(tas[current_ind+1]);
}

int main() {
  tas_init();
  afficher_tas();
  char *p1, *p2, *p3;
  p1 = (char *) tas_malloc(10);
  p2 = (char *) tas_malloc(9);
  p3 = (char *) tas_malloc(5);
  strcpy(p1, "tp 1");
  strcpy(p2, "tp 2");
  strcpy(p3, "tp 3");
  afficher_tas();
}
