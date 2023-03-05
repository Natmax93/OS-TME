# TME 5 - Gestion de Processus

## 1 - Arborescence de Processus

Programme *arbre.c*:

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

Affichage console :

    $ ./arbre 3 &
    $ pstree -a <pid>

    arbre 3
    ├─arbre 2
    │   ├─arbre 1
    │   │   ├─arbre 0
    │   │   └─arbre 0
    │   └─arbre 1
    │       ├─arbre 0
    │       └─arbre 0
    └─arbre 2
        ├─arbre 1
        │   ├─arbre 0
        │   └─arbre 0
        └─arbre 1
            ├─arbre 0
            └─arbre 0

## 2 - Mini-Shell

### Question 2.1

Programme *shell_v1.c*:

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

### Question 2.2

Programme *shell_v2.c*:

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <assert.h>

    #define TAILLE_PRGM 255

    // Programme qui attend la saisie d’une commande simple (sans
    // argument) au clavier et lance un processus fils exécutant la
    // commande et attend la fin du processus. Si le dernier caractère
    // de la ligne saisie est ‘& ‘ il ne faut pas attendre la fin du
    // fils.

    int main(int argc, char **argv) {

        char cmde[TAILLE_PRGM];
        char cmde_cpt[TAILLE_PRGM];
        //char tmp[TAILLE_PRGM+2];
        int bloquant;
        char **arguments;
        char *ss_chaine;
        int i, cpt;

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

            if (fork() == 0) {
                execv(arguments[0], arguments);
                fprintf(stderr, "Erreur lors du execl");
            }

            for (i=0; i<cpt; i++) {
                free(arguments[i]);
            }

            free(arguments);

            if (bloquant) {
                wait(NULL);
            }
        }

        return EXIT_SUCCESS;
    }

### Question 2.3