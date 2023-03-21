#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <assert.h>

#define TAILLE_PRGM 255

// Programme qui attend la saisie d’une commande simple (sans
// argument) au clavier et lance un processus fils exécutant la
// commande et attend la fin du processus. Si le dernier caractère
// de la ligne saisie est ‘& ‘ il ne faut pas attendre la fin du
// fils.

// Cette version propose une implémentation de la commande 'times'

int main(int argc, char **argv) {

    char cmde[TAILLE_PRGM];
    char cmde_cpt[TAILLE_PRGM];
    int bloquant;
    char **arguments;
    char *ss_chaine;
    int i, cpt, first_arg;
    struct rusage r;

    printf("====================\n");
    printf("    Bienvenue !\n");
    printf("====================\n");

    while (1) {

        printf("\n\nTapez une commande : ('quit' pour quitter, taille max autorisée : %d)\n\n", TAILLE_PRGM - 2);
        if (fgets(cmde, TAILLE_PRGM-1, stdin) == NULL) {
            fprintf(stderr, "Erreur lors de la lecture de la commande\n");
            return EXIT_FAILURE;
        }

        // Ajout du caractère nul à la place du retour à la ligne
        cmde[strlen(cmde) - 1] = 0;
        printf("Commande tapée : %s\n\n", cmde);

        if ((strcmp(cmde, "quit") == 0)) {
            break;
        }
            
        // On découpe la chaine pour récupérer la commmande

        // On copie cmde afin de pouvoir utiliser deux fois strtok
        strcpy(cmde_cpt, cmde);
        ss_chaine = strtok(cmde_cpt, " ");
        cpt = 0;

        // On compte le nombre d'argument

        while (ss_chaine != NULL) {

            if (strcmp(ss_chaine, "&") == 0) {
                break;
            }

            ss_chaine = strtok(NULL, " ");
            cpt++;
        }

        arguments = malloc(sizeof(char *) * cpt);
        assert(arguments);

        // On ajoute les arguments aux tableaux d'arguments

        ss_chaine = strtok(cmde, " ");
        i = 0;
        bloquant = 1;

        while (ss_chaine != NULL) {

            if (strcmp(ss_chaine, "&") == 0) {
                bloquant = 0;
                break;
            }

            assert(i < cpt);
            arguments[i] = strdup(ss_chaine);
            assert(arguments[i]);

            ss_chaine = strtok(NULL, " ");
            i++;
        }

        // La première commande est times

        first_arg = 0;
        if (strcmp(arguments[0], "times") == 0) {

            first_arg = 1;
            free(arguments[0]);

            // On décale tous les éléments du tableau
            for (i=0; i<cpt-1; i++) {
                arguments[i] = arguments[i+1];
            }
            arguments[i] = NULL;

            for (i=0; i<cpt-1; i++) {
                printf("arguments[%d] = %s ", i, arguments[i]);
            }
            printf("\n");
        }

        if (fork() == 0) {
            execv(arguments[0], arguments);
            fprintf(stderr, "Erreur lors du execl");
        }

        for (i=0; i<cpt-first_arg; i++) {
                free(arguments[i]);
            }

        free(arguments);

        if (bloquant) {
            wait3(NULL, 0, &r);

            // Cas où l'utilisateur a tapé la commande times
            if (first_arg) {
                printf("Temps utilisateur : %f\n", r.ru_utime.tv_sec + 1E-6*r.ru_utime.tv_usec);
                printf("Temps système : %f\n", r.ru_stime.tv_sec + 1E-6*r.ru_stime.tv_usec);
            }
        }
    }

    return EXIT_SUCCESS;
}