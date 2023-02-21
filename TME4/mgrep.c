#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* On souhaite implanter en C un "multi-grep" qui exécute la commande
   standard Unix grep en parallèle */

int main(int argc, char **argv) {

    int p;
    char *chaine;

    if (argc > 2) {
        chaine = argv[1];
        // Parcours des arguments
        for (int i = 2; i<argc; i++) {
            if ((p = fork()) == 0) {
                // C'est le fils
                if (execl("/usr/bin/grep", "grep", chaine, argv[i]) == -1) {
                    fprintf(stderr, "Erreur lors de l'exec du fils\n");
                    return EXIT_FAILURE;
                }
                fprintf(stderr, "Problème avec le exec\n");
            }
        }
    }
    else {
        printf("Utilisation : %s <chaine> <liste-fichiers>\n", argv[0]);
    }

    // On attend tous les fils
    while (wait(NULL) != -1);
}