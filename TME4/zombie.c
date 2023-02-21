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