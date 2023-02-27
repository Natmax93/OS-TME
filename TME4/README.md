# TME 4 - Gestion de Processus

## 1 - Multi-Grep Simple

Programme (*mgrep.c*):

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

## 2 - Multi-Grep à parallélisme contraint

Programme (*mgrep_para.c*):

    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>

    #define MAXFILS 2

    /* Idem à mgrep, sauf qu'on souhaite désormais ne créer simultanément
    qu'un nombre maximum MAXFILS de processus fils */

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

## 3 - Multi-Grep avec statistiques

Programme (*mgrep_stats.c*):

    #include <stdlib.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <sys/resource.h>
    #include <sys/wait.h>

    #define MAXFILS 2

    /* Idem à mgrep_para, sauf qu'on affiche les statistiques
    d'utilisation CPU système et utilisateur de chaque fils.*/

    int main(int argc, char **argv) {

        int p, status, fils_en_exec = 0;
        char *chaine;
        pid_t fils;
        struct rusage rusage;

        if (argc > 2) {

            chaine = argv[1];

            // Parcours des arguments
            for (int i = 2; i<argc; i++) {

                if (fils_en_exec == MAXFILS) {
                    printf("On attend un fils...\n");
                    fils = wait3(&status, 0, &rusage);
                    fils_en_exec--;
                    printf("Un fils a fini !\n");

                    if (WIFEXITED(status)) {
                        printf("(dans le if) Statistiques de '%d':\n", fils);
                        printf("\tTemps utilisateur consommé : %f\n", (rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec * 1E-6));
                        printf("\tTemps système consommé : %f\n", (rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec * 1E-6));
                    }
                    else {
                        fprintf(stderr, "Le fils %d s'est mal terminé\n", fils);
                    }                
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
        while ((fils = wait3(&status, 0, &rusage)) != -1) {
            if (WIFEXITED(status)) {
                printf("Statistiques de '%d':\n", fils);
                printf("\tTemps utilisateur consommé : %f\n", (rusage.ru_utime.tv_sec + rusage.ru_utime.tv_usec * 1E-6));
                printf("\tTemps système consommé : %f\n", (rusage.ru_stime.tv_sec + rusage.ru_stime.tv_usec * 1E-6));
            }
            else {
                fprintf(stderr, "Le fils %d s'est mal terminé\n", fils);
            }
        }
    }

## 4 - Processus "Zombie"

### Question 4.1

Un processus "zombie" est un processus qui n'a plus de père. C'est à dire un processus dont le père s'est terminé avant celui-ci et qui ne l'a pas attendu avec un *wait*

### Question 4.2

Programme (*zombie.c*):

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>

    /* Programme qui crée pendant 10 secondes deux processus zombie */

    int main(int argc, char **argv) {

        printf("Je suis le processus père : %d\n", getpid());
        for (int i = 0; i<2; i++) {
            if (fork() == 0) {
                printf("Nouveau processus %d fils de %d\n", getpid(), getppid());
                sleep(10);
                break;
            }
        }
        sleep(2); // Le temps de l'affichage des fils
        printf("Je suis le processus %d et mon père est %d\n", getpid(), getppid());
    }