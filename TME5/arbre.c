#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// Programme qui crée un arbre binaire complet de niveau L. L est
// un argument du programme. Chaque processus s'endort 30 secondes

int main(int argc, char **argv) {

    // Vérification du nombre des arguments
    if (argc < 2) {
        fprintf(stderr, "Utilisation : %s <niveau_arbre>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int niveau = atoi(argv[1]);
    fprintf(stderr, "niveau = %d\n", niveau);
    char niv[255];
    sprintf(niv, "%d", niveau - 1);
        
    if (niveau > 0) {
        for (int i=0; i<2; i++) {
            if (fork() == 0) {
                execl("./arbre", "./arbre", niv, NULL);
                fprintf(stderr, "execl s'est mal déroulé\n");
            }
        }
    }

    sleep(30);

    return EXIT_SUCCESS;
}