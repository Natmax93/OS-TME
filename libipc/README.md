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

L'utilisation du tableau de sémaphores permet d'empêcher l'attente de la lecture du sémaphore pour chaque processus: si chaque émetteur à son propre sémaphore à lire, il n'y a pas de file d'attente.

C'est le *NR* ème récepteur qui va prévenir de la disponibilité de la case : une fois qu'il a lu sa case, il va augmenter la variable partagée *nb_recepteurs* et comme celle-ci sera égale à NR, il va libérer le sémaphore pour les émetteurs et réinitialisé *nb_recepteurs* à 0.

### Question 1.3

Voir fichier *Tab1case.c*

## Mise en oeuvre avec un tampon à NMAX cases

### Question 2.1

Modification apportée à la structure du tampon partagée (on a maintenat *NMAX* cases):

    typedef struct {
        int tab[NMAX];
    } tampon;

Il faut un sémaphore sur le nombre de récepteur qui ont lu le message pour chaque message :

    int nb_recepteurs[NMAX];