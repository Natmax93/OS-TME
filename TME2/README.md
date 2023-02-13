# TME 2 - Gestion du Temps

## 1 - Statistiques d'exécution d'une commande shell

### Question 1.1

Résultat:

> real 0m5,008s<br>
> user 0m0,003s<br>
> sys  0m0,001s<br>

On constate que la commande possède un temps de réponse de total de 5,008 secondes soit dont 0,003 secondes en mode *user* et 0,001 secondes en mode *système*.


### Question 1.2

Programme:

    for (int i=1; i<1000000000; i++);

Résultat:

> time ./loopcpu
>
> real 0m1,476s<br>
> user 0m1,475s<br>
> sys  0m0,000s<br>

On constate que le temps total d'exécution du programme est identique au temps d'utilisation CPU en mode *user* à un millième de seconde près et qu'aucun temps CPU est consommé en mode *système*.

### Question 1.3

Programme:

    for (int i=1; i<10000000; i++){
        getpid();
    }

Résultat:

> time ./loopsys
>
> real 0m4,894s<br>
> user 0m3,221s<br>
> sys  0m1,673s<br>

On constate cette fois-ci que le temps total d'exécution du programme est de 4,894 secondes qui correspond exactement à la somme du temps CPU consommé en mode *user* et du temps CPU consommé en mode *système*.

## 2 - Lancement d'une commande shell depuis un programme C

### Question 2.1

Programme:

    void lance_commande(char *commande){
        if (system((const char *)commande) == -1){
            fprintf(stderr, "Erreur: la commande '%s' n'a pas pu être exécutée correctement\n", commande);
        }
    }

### Question 2.2

Main:

    int main(int argc, char **argv){
        if (argc < 1){
            fprintf(stderr, "Erreur: au moins un argument\n");
            return EXIT_FAILURE;
        }

        for (int i=1; i<argc; i++){
            lance_commande(argv[i]);
        }

        return 0;
    }

## 3 - Calcul du temps de réponse en utilisant gettimeofday

### Question 3.1

Programme:

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

        tps1 = temps1.tv_sec + temps1.tv_usec * 0.000001;
        tps2 = temps2.tv_sec + temps2.tv_usec * 0.000001;
        printf("Temps mis par l'exécution de la commande : %.2f\n", tps2-tps1);
}

### Question 3.2

Résultat de la commande :

> ./mytimes "sleep 5" "sleep 10"
>
> Temps mis par l'exécution de la commande : 5.00<br>
> Temps mis par l'exécution de la commande : 10.00<br>

## 4 - Calcul des statistiques

### Question 4.1

Programme:

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

Résultat de la commande:

> ./mytimes "ls -l" ./loopcpu ./loopsys<br>
>
> total 76<br>
> -rwxrwxr-x 1 natmax93 natmax93 16464 févr.  4 15:26 loopcpu<br>
> -rw-rw-r-- 1 natmax93 natmax93   148 févr.  4 15:26 loopcpu.c<br>
> -rwxrwxr-x 1 natmax93 natmax93 16704 févr.  4 15:29 loopsys<br>
> -rw-rw-r-- 1 natmax93 natmax93   214 févr.  4 15:26 loopsys.c<br>
> -rwxrwxr-x 1 natmax93 natmax93 17128 févr.  4 15:38 mytimes<br>
> -rw-rw-r-- 1 natmax93 natmax93  1720 févr.  4 15:38 mytimes.c<br>
> -rw-rw-r-- 1 natmax93 natmax93  2699 févr.  4 15:25 README.md<br>
> Statistiques de "ls -l" :<br>
> Temps total : 0.010000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 0.000000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 1.420000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 1.420000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopsys" :<br>
> Temps total : 4.840000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.960000<br>
> Temps système fils : 1.880000<br>

## 5 - Changement de priorité

### Question 5.1

La priorité du processus *ps* est de 80, soit une priorité faible.

### Question 5.2

A l'issue de la commande:

> nice -19 ps -l

La priorité de la commande *ps* est devenue 99.

### Question 5.3

A l'issue de la commande:

> ./mytimes "./loopcpu" & ./mytimes "nice -19 ./loopcpu" & ./mytimes "./loopcpu" & ./mytimes "./loopcpu" & ./mytimes "./loopcpu" & ./mytimes "./loopcpu" & ./mytimes "./loopcpu" & ./mytimes "./loopcpu" & ./mytimes "./loopcpu" &<br>

On obtient le résultat suivant:

> Temps total : 2.060000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.050000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.060000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.050000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.060000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.050000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.080000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.060000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.090000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.060000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.130000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.050000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.170000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.050000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "./loopcpu" :<br>
> Temps total : 2.230000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 2.010000<br>
> Temps système fils : 0.000000<br>
> Statistiques de "nice -19 ./loopcpu" :<br>
> Temps total : 3.250000<br>
> Temps utilisateur : 0.000000<br>
> Temps système : 0.000000<br>
> Temps utilisateur fils : 1.580000<br>
> Temps système fils : 0.000000<br>

On observe donc bien que le processus auquel on a baissé la priorité prend le plus de temps à s'exécuter et se termine en dernier.