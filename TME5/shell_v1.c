#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TAILLE_PRGM 255

// Programme qui attend la saisie d’une commande simple (sans
// argument) au clavier et lance un processus fils exécutant la
// commande et attend la fin du processus. Si le dernier caractère
// de la ligne saisie est ‘& ‘ il ne faut pas attendre la fin du
// fils.

int main(int argc, char **argv) {

    char cmde[TAILLE_PRGM];
    char tmp[TAILLE_PRGM+2];
    int i;

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

        for (i=0; i<strlen(cmde); i++) {
            if (cmde[i] == ' ' || i >= strlen(cmde)) {
                break;
            }
            tmp[i] = cmde[i];
        }
        
        tmp[i] = 0;

        if (fork() == 0) {
            execl(tmp, tmp, NULL);
            fprintf(stderr, "Erreur lors du execl");
        }

        if (strchr(cmde, '&') == NULL) {
            wait(NULL);
        }
    }

    return EXIT_SUCCESS;
}