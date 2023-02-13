#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>

void lance_commande(char *commande){

    struct timeval temps1, temps2;  // Structures pour la fonction gettimeofday()
    int res;
    double tps1, tps2;              

    gettimeofday(&temps1, NULL);
    res = system((const char *)commande);
    gettimeofday(&temps2, NULL);

    if (res == -1){
        fprintf(stderr, "Erreur: la commande '%s' n'a pas pu être exécutée correctement\n", commande);
    }
    else{
        tps1 = temps1.tv_sec + temps1.tv_usec * 0.000001;
        tps2 = temps2.tv_sec + temps2.tv_usec * 0.000001;
        printf("Temps mis par l'exécution de la commande : %.2f\n", tps2-tps1);
    }
}

void lance_commande_v2(char *commande){

    struct tms temps1, temps2;              // Structures pour la fonction times
    int res;
    clock_t t1, t2;                         // Pour le temps total
    double ticks = sysconf(_SC_CLK_TCK);    // Le nombre de ticks horloges par secondes           

    t1 = times(&temps1);
    res = system((const char *)commande);
    t2 = times(&temps2);

    if (res == -1){
        fprintf(stderr, "Erreur: la commande '%s' n'a pas pu être exécutée correctement\n", commande);
    }
    else{
        printf("Statistiques de \"%s\" :\n", commande);
        printf("Temps total : %f\n", (t2 - t1) / ticks);
        printf("Temps utilisateur : %f\n", (temps2.tms_utime - temps1.tms_utime) / ticks);
        printf("Temps système : %f\n", (temps2.tms_stime - temps1.tms_stime) / ticks);
        printf("Temps utilisateur fils : %f\n", (temps2.tms_cutime - temps1.tms_cutime) / ticks);
        printf("Temps système fils : %f\n", (temps2.tms_cstime - temps1.tms_cstime) / ticks);
    }
}


int main(int argc, char **argv){
    if (argc < 1){
        fprintf(stderr, "Erreur: au moins un argument\n");
        return EXIT_FAILURE;
    }

    for (int i=1; i<argc; i++){
        lance_commande_v2(argv[i]);
    }

    return 0;
}