/* Diffusion tampon 1 case */

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

/************************************************************/

/* definition des semaphores */ 

	#define EMET        0
  #define MUTEX_NB    1
  int RECEP[NR];      // tableau des sémaphores
        
/************************************************************/

/* definition de la memoire partagee */ 

	typedef struct {
    int a;
  } tampon;

  int nb_recepteurs;

  tampon *tp;

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

    while (1) {

      // Il y a une case dans laquelle il peut écrire
      P(EMET);                   
      tp->a = a;
      printf("Je suis l'émetteur numéro %d et j'écris dans la variable a\n", a);

      // On prévient les récepteurs qu'il y a une case libre
      for (int i=0; i<NR; i++) {
        V(RECEP[i]); 
      }
    }

  }

/************************************************************/

/* fonction RECEPTEUR */ 

	// A completer - contient les instructions executees
        // par un recepteur

  void RECEPTEUR(int id_recep) {

    while (1) {

      // Le récepteur peut lire la ressource
      P(RECEP[id_recep]);

      P(MUTEX_NB);
      nb_recepteurs++;

      // C'est le premire récepteur on bloque les écrivains
      if (nb_recepteurs == 1) {
        P(EMET);
      }

      // On prévient les autres lecteurs qu'ils peuvent lire
      V(MUTEX_NB);


      printf("Je suis le récepteur numéro %d et je lis dans la variable a = %d\n", id_recep, tp->a);
      
      // Fin de la lecture de la ressource
      P(MUTEX_NB);
      nb_recepteurs--;

      // C'est le dernier récepteur à lire la ressource
      if (nb_recepteurs == 0) {
        
        // On prévient les écrivains qu'ils peuvent écrire
        V(EMET);
      }
      V(MUTEX_NB);
    }
  }

/************************************************************/

int main() { 
    struct sigaction action;
    /* autres variables (a completer) */

    int semid, pid;
    nb_recepteurs = 0;

    setbuf(stdout, NULL);

/* Creation du segment de memoire partagee */

	tp = malloc(sizeof(tampon));
  assert(tp);

/* creation des semaphores */ 

  // On crée un sémaphore par récepteur, un pour les émetteurs et
  // un pour la variable partagée des émetteurs
  if ((semid = creer_sem(NR + 2)) == -1) {
    perror("creer_sem");
    exit(1);
  }

/* initialisation des semaphores */ 

	// Initialisation du sémaphore pour les émetteurs
	init_un_sem(EMET,1);

  // Numérotation des sémaphores en partant de 2
  // Puis initialisation des sémaphores pour les récepteurs
  for (int i=0; i<NR; i++) {
    RECEP[i] = i + 2;
    init_un_sem(RECEP[i], 0);
  }

  // Initialisation du sémaphore pour nb_recepteurs
  init_un_sem(MUTEX_NB, 1);

/* creation des processus emetteurs */ 

	// A completer - les pid des processus crees doivent
        // etre stockes dans le tableau emet_pid

  for (int i=0; i<NE; i++) {
    if ((pid = fork() == 0)) {
      emet_pid[i] = pid;
      EMETTEUR(i, i);
    }
  }

/* creation des processus recepteurs */ 

	// A completer - les pid des processus crees doivent
        // etre stockes dans le tableau recep_pid

  for (int i=0; i<NR; i++) {
    if ((pid = fork() == 0)) {
      recep_pid[i] = pid;
      RECEPTEUR(i);
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
