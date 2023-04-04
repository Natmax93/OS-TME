/* Diffusion tampon N case */

  #include <stdio.h> 
  #include <unistd.h> 
  #include <stdlib.h> 
  #include <signal.h> 
  #include <libipc.h>

  #include <assert.h>

/************************************************************/

/* definition des parametres */ 

  #define NE          2     /*  Nombre d'emetteurs         */ 
  #define NR          5     /*  Nombre de recepteurs       */
  #define NMAX        3     /*  Taille du tampon           */

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

/* variables globales */ 
  int emet_pid[NE], recep_pid[NR]; 

/************************************************************/

/* traitement de Ctrl-C */ 

  void handle_sigint(int sig) { 
      int i;
      for (i = 0; i < NE; i++) kill(emet_pid[i], SIGKILL); 
      for (i = 0; i < NR; i++) kill(recep_pid[i], SIGKILL); 
      det_sem(); 
      det_shm((char *)tp); 
  } 

/************************************************************/

/* fonction EMETTEUR */ 

	// A completer - contient les instructions executees
        // par un emetteur

  void EMETTEUR(int id_emet, int a) {

    // La case dans laquelle il devra émettre
    int num_case;

    while (1) {

      // Il y a une case dans laquelle il peut écrire
      P(EMET);

      // On détermine cette case
      P(MUTEX_NCE);
      num_case = *next_case_emet % NMAX;
      (*next_case_emet)++;
      V(MUTEX_NCE);

      tp->tab[num_case] = a;
      printf("Je suis l'émetteur numéro %d et j'écris %d dans la variable case %d du tampon\n", a, a, num_case);

      // On prévient les récepteurs qu'il y a une case libre
      for (int i=0; i<NR; i++) {
        V(RECEP[i]);
      }
    }

  }

/************************************************************/

/* fonction RECEPTEUR */ 

  void RECEPTEUR(int id_recep) {

    int num_case;

    while (1) {

      //printf("Je suis le récepteur %d et RECEP[%d] = %d\n", id_recep, id_recep, RECEP[id_recep]);

      // On regarde s'il y a une case à lire
      P(RECEP[id_recep]);

      // On attend que tous les autres récepteurs aient fini de lire
      P(ENTRER_SAS);

      // On détermine la prochaine case à lire
      P(MUTEX_NB);

      num_case = *next_case_recep % NMAX;

      // C'est le premier arrivé à la nouvelle case
      if (*nb_recepteurs == 0) {
        
        // On incrémente le compteur pour la prochaine case
        (*next_case_recep)++;
      }

      (*nb_recepteurs)++;

      // Le sas n'est pas rempli
      if (*nb_recepteurs < NR) {
        V(MUTEX_NB);
        P(ENTRER_CASE);
      }
      else{
        // Le sas est rempli tous les récepteurs ont lut la case précédente
        // On libère donc cette case pour les émetteurs si ce n'est pas la
        // première lecture
        if (num_case != -1) {
          V(EMET);
        }
        V(MUTEX_NB);
      }

      // On entre dans la case à lire
      P(MUTEX_NB);
      (*nb_recepteurs)--;
      
      // Il y a encore des récepteurs dans le sas
      if (*nb_recepteurs > 0) {
        V(MUTEX_NB);
        V(ENTRER_CASE);
      }
      else {
        // C'est le dernier récepteur dans le sas
        V(MUTEX_NB);
        
        // On libère l'entrée du sas
        for (int i=0; i<NR; i++) {
          V(ENTRER_SAS);
        }
      }

      //printf("Lecture ressource pour %d\n", id_recep);

      printf("Je suis le récepteur numéro %d (RECEP[%d] = %d) et je lis dans la case %d : %d\n", id_recep, id_recep, RECEP[id_recep], num_case, tp->tab[num_case]);
    }
  }

/************************************************************/

int main() { 
  struct sigaction action;
  /* autres variables (a completer) */

  int semid, pid;

  setbuf(stdout, NULL);

/* Creation du segment de memoire partagee */

  /* Creer le segment de memoire partager */
	if ((tp = (tampon *)init_shm(sizeof(tampon))) == NULL) {
		perror("init_shm (tampon)");
		exit(1);
	}

  if ((nb_recepteurs = (int *)init_shm(sizeof(int))) == NULL) {
		perror("init_shm (nb_recepteurs)");
		exit(1);
	}

  *nb_recepteurs = 0;

  if ((next_case_emet = (int *)init_shm(sizeof(int))) == NULL) {
		perror("init_shm (next_case_emet)");
		exit(1);
	}

  *next_case_emet = 0;

  if ((next_case_recep = (int *)init_shm(sizeof(int))) == NULL) {
		perror("init_shm (next_case_recep)");
		exit(1);
	}

  *next_case_recep = 0;

/* creation des semaphores */ 

  // 6 sémaphores plus le tableau de NR sémaphores
  if ((semid = creer_sem(NR + 6)) == -1) {
    perror("creer_sem");
    exit(1);
  }

/* initialisation des semaphores */ 

	// Initialisation du sémaphore pour les émetteurs
  // Au début il y a NMAX cases libres
	init_un_sem(EMET, NMAX);

  // Numérotation des sémaphores en partant de 6
  // Puis initialisation des sémaphores pour les récepteurs
  for (int i=0; i<NR; i++) {
    RECEP[i] = i + 6;
    init_un_sem(RECEP[i], 0);
  }

  // Initialisation du sémaphore pour nb_recepteurs
  init_un_sem(MUTEX_NB, 1);

  // Initialisation du sémaphore pour la prochaine case des émetteurs
  init_un_sem(MUTEX_NCE, 1);

  // Initialisation du sémaphore pour la prochaine case des récepteurs
  init_un_sem(MUTEX_NCR, 1);

  // Initialisation du sémaphore pour la entrer dans le sas
  // Il y a NR récepteurs qui peuvent y entrer
  init_un_sem(ENTRER_SAS, NR);

  // Initialisation du sémaphore pour la entrer dans la case suivante
  // Au début, personne ne peut y entrer
  init_un_sem(ENTRER_CASE, 0);

/* creation des processus emetteurs */ 

	// A completer - les pid des processus crees doivent
        // etre stockes dans le tableau emet_pid

  for (int i=0; i<NE; i++) {
    if ((pid = fork() == 0)) {
      EMETTEUR(i, i);
      break;
    }
    else {
      emet_pid[i] = pid;
    }
  }

/* creation des processus recepteurs */ 

	// A completer - les pid des processus crees doivent
        // etre stockes dans le tableau recep_pid

  for (int i=0; i<NR; i++) {
    if ((pid = fork() == 0)) {
      RECEPTEUR(i);
      break;
    }
    else {
      recep_pid[i] = pid;
    }
  }

/* redefinition du traitement de Ctrl-C pour arreter le programme */ 

    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    action.sa_handler = handle_sigint;
    sigaction(SIGINT, &action, 0); 
    
    pause();                    /* attente du Ctrl-C  */
    return EXIT_SUCCESS;
} 
