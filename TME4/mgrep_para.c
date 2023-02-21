#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXFILS 2

/* Idem à mgrep, sauf qu'on souhaite désormais ne créer simultanément
   qu'un nombre maximum MAXFILS de processus fils*/

int main(int argc, char **argv) {

    int p, fils_en_exec = 0;
    char *chaine;

    if (argc > 2) {

        chaine = argv[1];

        // Parcours des arguments
        for (int i = 2; i<argc; i++) {

            if (fils_en_exec == MAXFILS) {
                printf("On attend un fils...\n");
                wait(NULL);
                fils_en_exec--;
                printf("Un fils a fini !\n");
            }

            if ((p = fork()) == 0) {
                // C'est le fils
                if (execl("/usr/bin/grep", "grep", chaine, argv[i]) == -1) {
                    fprintf(stderr, "Erreur lors de l'exec du fils\n");
                    return EXIT_FAILURE;
                }
                fprintf(stderr, "Problème avec le exec\n");
            }
            fils_en_exec++;
        }
    }
    else {
        printf("Utilisation : %s <chaine> <liste-fichiers>\n", argv[0]);
    }

    // On attend tous les fils
    while (wait(NULL) != -1);
}