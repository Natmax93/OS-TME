# TME 6-7 - Synchronisations par sémaphores

## Mise en oeuvre avec un tampon à une seule case

### Question 1.1

Pour assurer la cohérence de *nb_recepteurs* qui est une variable partagée, on doit ajout un sémaphore __MUTEX__ pour définir l'exclusion mutuelle des processus pour l'accès à cette variable.

Le sémaphore émetteur est à 1 au début car il y a une case de libre :

    // Initialisation du sémaphore pour les émetteurs
	init_un_sem(EMET,1);

Le tableau de sémaphores pour les récepteurs est rempli de 0 car il n'y a aucune case à libre au début du programme :

    // Numérotation des sémaphores en partant de 2
    // Puis initialisation des sémaphores pour les récepteurs
    for (int i=0; i<NR; i++) {
        RECEP[i] = i + 2;
        init_un_sem(RECEP[i], 0);
    }

Le sémaphore d'exclusion mutuelle est initialisé à un pour qu'un seul processus puisse le modifier à la fois :

    // Initialisation du sémaphore pour nb_recepteurs
    init_un_sem(MUTEX_NB, 1);

La variable partagée est initialisée à 0, car aucun processus ne lit de message au début du programme :

    nb_recepteurs = 0;

### Question 1.2

L'utilisation du tableau de sémaphores permet d'empêcher l'attente de la lecture du sémaphore pour chaque processus: si chaque émetteur à son propre sémaphore à lire, il n'y a pas de file d'attente.<br><br>
C'est le *NR* ème récepteur qui va prévenir de la disponibilité de la case : une fois qu'il a lu sa case, il va augmenter la variable partagée *nb_recepteurs* et comme celle-ci sera égale à NR, il va libérer le sémaphore pour les émetteurs et réinitialisé *nb_recepteurs* à 0.

### Question 1.3

Voir fichier *Tab1case.c*

## Mise en oeuvre avec un tampon à NMAX cases

### Question 2.1

    /************************************************************/

    /* definition des semaphores */ 

    #define EMET 0                 // Sémaphore pour les émetteurs
    #define MUTEX_NB 1             // Tableau de sémaphores pour le nombre de letures
    #define MUTEX_NCE 2            // Sémaphore d'exclusion pour la prochaine case dans laquelle émettre
    #define MUTEX_NCR 3            // Sémaphore d'exclusion pour la prochaine case dans laquelle lire
    #define ENTRER_SAS 4           // Sémaphore lorsqu'un récepteur a fini sa lecture
    #define ENTRER_CASE 5          // Quand tous les récepteurs ont lu la case, on peut passer à la suivante
    int RECEP[NR];                 // Tableau des sémaphores pour les récepteurs
            
    /************************************************************/

    /* definition de la memoire partagee */ 

        typedef struct {
        int tab[NMAX];
    } tampon;

    tampon *tp;

    int *nb_recepteurs;

    int *next_case_emet;         // La prochaine case à écrire pour les émetteurs
    int *next_case_recep;        // La prochaine case à écrire pour les récepteurs

    /************************************************************/

### Question 2.2

Les émetteurs possèdent un sémaphore EMET initialisé au nombre de cases libres cad *NMAX* au début du programme. La variable partagée *next_case_emet* permet d'indiquer la prochaine case à lire et est protégée par un *MUTEX* afin de garantir la cohérence. Une fois qu'un émetteur a écrit dans une case, il indique que la case est accessible en lecture à tous les récepteurs de la même manière que le programme précédent en libérant le tableau de sémaphores *RECEP* (voir exercice 1).<br><br>
On peut utiliser l'algorithme *ENTRER_SAS* et *ENTRER_BANQUE* vus en cours pour les récepteurs. Quand un récepteur a fini de lire, il regarde s'il y a une nouvelle case à lire, dans tel cas il "entre dans le sas" cad il attend que tous les autres récepteurs aient fini leur lecture. Lorsque le dernier récepteur a fini sa lecture du message, il indique aux émetteurs qu'une nouvelle case est libre en libérant le sémaphore *EMET* puis il "entre dans le sas" comme il est le dernier, il ouvre l'accès au message suivant à tous les récepteurs.

### Question 2.3

Voir fichier *TabNcase.c*.