# TME 3 - Ordonnancement de tâches

Le fichier contenant le code source (*scen.c*) se trouve dans le dossier *libsched/demo*.

## 1 - Tester la bibliothèque

### Question 1.2

Affichage:

    RANDOM Election !
    Proc. Court1 - 0
    Proc. Court1 - 10000000
    Proc. Court1 - 20000000
    Proc. Court1 - 30000000
    Proc. Court1 - 40000000
    Proc. Court1 - 50000000
    Proc. Court1 - 60000000
    Proc. Court1 - 70000000
    Proc. Court1 - 80000000
    Proc. Court1 - 90000000
    ############ FIN COURT 1

    RANDOM Election !
    Proc. Court0 - 0
    Proc. Court0 - 10000000
    Proc. Court0 - 20000000
    Proc. Court0 - 30000000
    Proc. Court0 - 40000000
    Proc. Court0 - 50000000
    Proc. Court0 - 60000000
    Proc. Court0 - 70000000
    Proc. Court0 - 80000000
    Proc. Court0 - 90000000
    ############ FIN COURT 0

    RANDOM Election !
    Proc. Long0 - 0
    Proc. Long0 - 40000000
    Proc. Long0 - 80000000
    Proc. Long0 - 120000000
    Proc. Long0 - 160000000
    RANDOM Election !
    Proc. Long1 - 0
    Proc. Long1 - 40000000
    Proc. Long1 - 80000000
    Proc. Long1 - 120000000
    Proc. Long1 - 160000000
    RANDOM Election !
    Proc. Long1 - 200000000
    Proc. Long1 - 240000000
    Proc. Long1 - 280000000
    Proc. Long1 - 320000000
    Proc. Long1 - 360000000
    RANDOM Election !
    ############ FIN LONG 1

    RANDOM Election !
    Proc. Long0 - 200000000
    Proc. Long0 - 240000000
    Proc. Long0 - 280000000
    Proc. Long0 - 320000000
    Proc. Long0 - 360000000
    RANDOM Election !
    ############ FIN LONG 0

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	1.782s		0.655s		1.127s
    1	Function1	1.427s		0.655s		0.773s
    2	Function2	0.473s		0.201s		0.271s
    3	Function3	0.271s		0.271s		0.000s
    --------------------------------------------------------------------
    Average:		0.988s		0.445s		0.543s
    --------------------------------------------------------------------

### Question 1.3

On diminuant le quantum à *0.03* soit des commmutations 10 fois plus souvent que *0.3*, on obtient l'affichage suivant:

    RANDOM Election !
    Proc. Court1 - 0
    RANDOM Election !
    Proc. Court0 - 0
    RANDOM Election !
    Proc. Long0 - 0
    RANDOM Election !
    Proc. Court0 - 10000000
    RANDOM Election !
    Proc. Court1 - 10000000
    RANDOM Election !
    Proc. Court0 - 20000000
    RANDOM Election !
    Proc. Court0 - 30000000
    RANDOM Election !
    Proc. Long1 - 0
    RANDOM Election !
    RANDOM Election !
    Proc. Court1 - 20000000
    RANDOM Election !
    Proc. Long1 - 40000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Court1 - 30000000
    Proc. Court1 - 40000000
    RANDOM Election !
    Proc. Court0 - 40000000
    Proc. Court0 - 50000000
    RANDOM Election !
    Proc. Court1 - 50000000
    Proc. Court1 - 60000000
    RANDOM Election !
    Proc. Court1 - 70000000
    Proc. Court1 - 80000000
    RANDOM Election !
    Proc. Court0 - 60000000
    Proc. Court0 - 70000000
    RANDOM Election !
    Proc. Court0 - 80000000
    RANDOM Election !
    Proc. Court1 - 90000000
    RANDOM Election !
    Proc. Court0 - 90000000
    ############ FIN COURT 0

    RANDOM Election !
    ############ FIN COURT 1

    RANDOM Election !
    Proc. Long1 - 80000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 120000000
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 160000000
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 200000000
    RANDOM Election !
    Proc. Long0 - 40000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 240000000
    RANDOM Election !
    Proc. Long0 - 80000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 280000000
    RANDOM Election !
    Proc. Long0 - 120000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 320000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long1 - 360000000
    RANDOM Election !
    RANDOM Election !
    ############ FIN LONG 1

    RANDOM Election !
    Proc. Long0 - 160000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long0 - 200000000
    RANDOM Election !
    RANDOM Election !
    Proc. Long0 - 240000000
    RANDOM Election !
    RANDOM Election !
    Proc. Long0 - 280000000
    RANDOM Election !
    RANDOM Election !
    RANDOM Election !
    Proc. Long0 - 320000000
    RANDOM Election !
    RANDOM Election !
    Proc. Long0 - 360000000
    RANDOM Election !
    RANDOM Election !
    ############ FIN LONG 0

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	1.922s		0.743s		1.179s
    1	Function1	1.479s		0.735s		0.744s
    2	Function2	0.649s		0.228s		0.420s
    3	Function3	0.654s		0.216s		0.439s
    --------------------------------------------------------------------
    Average:		1.176s		0.480s		0.696s
    --------------------------------------------------------------------

On observe qu'avec ce quantum, certains processus n'ont pas le temps d'aller au bout des intervalles dans la boucle.

## 2 - Ecriture d'un nouvel algorithme d'ordonnancement SJF

### Question 2.1

    // Election de SJF "Shortest Job Fisrt"
    int SJFElect(void) {
    int p = -1;
    double duration_min;

    printf("SJF Election !\n");

    // On choisit le processus p dont la durée est la plus courte
    // cad celui dont la date de fin est la plus petite
    p = -1;
    duration_min = LONGTIME; // initialisation à la durée la plus longue
    for (int i = 0; i<MAXPROC; i++) {
        if (Tproc[i].flag == RUN && Tproc[i].duration < duration_min) {
        p = i;
        duration_min = Tproc[i].duration;
        }
    }

    return p;
    }

Affichage de programme pour l'algorithme __SJF__:

    SJF Election !
    Proc. Long 0 - 0
    Proc. Long 0 - 8000000
    Proc. Long 0 - 16000000
    Proc. Long 0 - 24000000
    Proc. Long 0 - 32000000
    Proc. Long 0 - 40000000
    Proc. Long 0 - 48000000
    Proc. Long 0 - 56000000
    Proc. Long 0 - 64000000
    Proc. Long 0 - 72000000
    Proc. Long 0 - 80000000
    Proc. Long 0 - 88000000
    Proc. Long 0 - 96000000
    Proc. Long 0 - 104000000
    Proc. Long 0 - 112000000
    Proc. Long 0 - 120000000
    Proc. Long 0 - 128000000
    Proc. Long 0 - 136000000
    Proc. Long 0 - 144000000
    Proc. Long 0 - 152000000
    SJF Election !
    Proc. Court 0 - 0
    Proc. Court 0 - 8000000
    Proc. Court 0 - 16000000
    Proc. Court 0 - 24000000
    Proc. Court 0 - 32000000
    Proc. Court 0 - 40000000
    Proc. Court 0 - 48000000
    Proc. Court 0 - 56000000
    Proc. Court 0 - 64000000
    Proc. Court 0 - 72000000
    ############ FIN COURT 0

    SJF Election !
    Proc. Long 0 - 160000000
    Proc. Long 0 - 168000000
    Proc. Long 0 - 176000000
    Proc. Long 0 - 184000000
    Proc. Long 0 - 192000000
    Proc. Long 0 - 200000000
    Proc. Long 0 - 208000000
    Proc. Long 0 - 216000000
    Proc. Long 0 - 224000000
    Proc. Long 0 - 232000000
    Proc. Long 0 - 240000000
    Proc. Long 0 - 248000000
    Proc. Long 0 - 256000000
    Proc. Long 0 - 264000000
    Proc. Long 0 - 272000000
    Proc. Long 0 - 280000000
    Proc. Long 0 - 288000000
    Proc. Long 0 - 296000000
    Proc. Long 0 - 304000000
    Proc. Long 0 - 312000000
    Proc. Long 0 - 320000000
    Proc. Long 0 - 328000000
    Proc. Long 0 - 336000000
    Proc. Long 0 - 344000000
    Proc. Long 0 - 352000000
    SJF Election !
    Proc. Court 1 - 0
    Proc. Court 1 - 8000000
    Proc. Court 1 - 16000000
    Proc. Court 1 - 24000000
    Proc. Court 1 - 32000000
    Proc. Court 1 - 40000000
    Proc. Court 1 - 48000000
    Proc. Court 1 - 56000000
    Proc. Court 1 - 64000000
    Proc. Court 1 - 72000000
    ############ FIN COURT 1

    SJF Election !
    Proc. Long 0 - 360000000
    Proc. Long 0 - 368000000
    Proc. Long 0 - 376000000
    Proc. Long 0 - 384000000
    Proc. Long 0 - 392000000
    Proc. Long 0 - 400000000
    Proc. Long 0 - 408000000
    Proc. Long 0 - 416000000
    Proc. Long 0 - 424000000
    Proc. Long 0 - 432000000
    Proc. Long 0 - 440000000
    Proc. Long 0 - 448000000
    Proc. Long 0 - 456000000
    Proc. Long 0 - 464000000
    Proc. Long 0 - 472000000
    Proc. Long 0 - 480000000
    Proc. Long 0 - 488000000
    Proc. Long 0 - 496000000
    Proc. Long 0 - 504000000
    Proc. Long 0 - 512000000
    Proc. Long 0 - 520000000
    Proc. Long 0 - 528000000
    Proc. Long 0 - 536000000
    Proc. Long 0 - 544000000
    Proc. Long 0 - 552000000
    SJF Election !
    Proc. Court 2 - 0
    Proc. Court 2 - 8000000
    Proc. Court 2 - 16000000
    Proc. Court 2 - 24000000
    Proc. Court 2 - 32000000
    Proc. Court 2 - 40000000
    Proc. Court 2 - 48000000
    Proc. Court 2 - 56000000
    Proc. Court 2 - 64000000
    Proc. Court 2 - 72000000
    ############ FIN COURT 2

    SJF Election !
    Proc. Long 0 - 560000000
    Proc. Long 0 - 568000000
    Proc. Long 0 - 576000000
    Proc. Long 0 - 584000000
    Proc. Long 0 - 592000000
    Proc. Long 0 - 600000000
    Proc. Long 0 - 608000000
    Proc. Long 0 - 616000000
    Proc. Long 0 - 624000000
    Proc. Long 0 - 632000000
    Proc. Long 0 - 640000000
    Proc. Long 0 - 648000000
    Proc. Long 0 - 656000000
    Proc. Long 0 - 664000000
    Proc. Long 0 - 672000000
    Proc. Long 0 - 680000000
    Proc. Long 0 - 688000000
    Proc. Long 0 - 696000000
    Proc. Long 0 - 704000000
    Proc. Long 0 - 712000000
    Proc. Long 0 - 720000000
    Proc. Long 0 - 728000000
    Proc. Long 0 - 736000000
    Proc. Long 0 - 744000000
    Proc. Long 0 - 752000000
    SJF Election !
    Proc. Court 3 - 0
    Proc. Court 3 - 8000000
    Proc. Court 3 - 16000000
    Proc. Court 3 - 24000000
    Proc. Court 3 - 32000000
    Proc. Court 3 - 40000000
    Proc. Court 3 - 48000000
    Proc. Court 3 - 56000000
    Proc. Court 3 - 64000000
    Proc. Court 3 - 72000000
    ############ FIN COURT 3

    SJF Election !
    Proc. Long 0 - 760000000
    Proc. Long 0 - 768000000
    Proc. Long 0 - 776000000
    Proc. Long 0 - 784000000
    Proc. Long 0 - 792000000
    ############ FIN LONG 0

    SJF Election !
    Proc. Long 1 - 0
    Proc. Long 1 - 8000000
    Proc. Long 1 - 16000000
    Proc. Long 1 - 24000000
    Proc. Long 1 - 32000000
    Proc. Long 1 - 40000000
    Proc. Long 1 - 48000000
    Proc. Long 1 - 56000000
    Proc. Long 1 - 64000000
    Proc. Long 1 - 72000000
    Proc. Long 1 - 80000000
    Proc. Long 1 - 88000000
    Proc. Long 1 - 96000000
    Proc. Long 1 - 104000000
    Proc. Long 1 - 112000000
    Proc. Long 1 - 120000000
    Proc. Long 1 - 128000000
    Proc. Long 1 - 136000000
    Proc. Long 1 - 144000000
    Proc. Long 1 - 152000000
    Proc. Long 1 - 160000000
    Proc. Long 1 - 168000000
    Proc. Long 1 - 176000000
    SJF Election !
    Proc. Court 4 - 0
    Proc. Court 4 - 8000000
    Proc. Court 4 - 16000000
    Proc. Court 4 - 24000000
    Proc. Court 4 - 32000000
    Proc. Court 4 - 40000000
    Proc. Court 4 - 48000000
    Proc. Court 4 - 56000000
    Proc. Court 4 - 64000000
    Proc. Court 4 - 72000000
    ############ FIN COURT 4

    SJF Election !
    Proc. Long 1 - 184000000
    Proc. Long 1 - 192000000
    Proc. Long 1 - 200000000
    Proc. Long 1 - 208000000
    Proc. Long 1 - 216000000
    Proc. Long 1 - 224000000
    Proc. Long 1 - 232000000
    Proc. Long 1 - 240000000
    Proc. Long 1 - 248000000
    Proc. Long 1 - 256000000
    Proc. Long 1 - 264000000
    Proc. Long 1 - 272000000
    Proc. Long 1 - 280000000
    Proc. Long 1 - 288000000
    Proc. Long 1 - 296000000
    Proc. Long 1 - 304000000
    Proc. Long 1 - 312000000
    Proc. Long 1 - 320000000
    Proc. Long 1 - 328000000
    Proc. Long 1 - 336000000
    Proc. Long 1 - 344000000
    Proc. Long 1 - 352000000
    Proc. Long 1 - 360000000
    Proc. Long 1 - 368000000
    Proc. Long 1 - 376000000
    SJF Election !
    Proc. Court 5 - 0
    Proc. Court 5 - 8000000
    Proc. Court 5 - 16000000
    Proc. Court 5 - 24000000
    Proc. Court 5 - 32000000
    Proc. Court 5 - 40000000
    Proc. Court 5 - 48000000
    Proc. Court 5 - 56000000
    Proc. Court 5 - 64000000
    Proc. Court 5 - 72000000
    ############ FIN COURT 5

    SJF Election !
    Proc. Long 1 - 384000000
    Proc. Long 1 - 392000000
    Proc. Long 1 - 400000000
    Proc. Long 1 - 408000000
    Proc. Long 1 - 416000000
    Proc. Long 1 - 424000000
    Proc. Long 1 - 432000000
    Proc. Long 1 - 440000000
    Proc. Long 1 - 448000000
    Proc. Long 1 - 456000000
    Proc. Long 1 - 464000000
    Proc. Long 1 - 472000000
    Proc. Long 1 - 480000000
    Proc. Long 1 - 488000000
    Proc. Long 1 - 496000000
    Proc. Long 1 - 504000000
    Proc. Long 1 - 512000000
    Proc. Long 1 - 520000000
    Proc. Long 1 - 528000000
    Proc. Long 1 - 536000000
    Proc. Long 1 - 544000000
    Proc. Long 1 - 552000000
    Proc. Long 1 - 560000000
    Proc. Long 1 - 568000000
    Proc. Long 1 - 576000000
    SJF Election !
    Proc. Court 6 - 0
    Proc. Court 6 - 8000000
    Proc. Court 6 - 16000000
    Proc. Court 6 - 24000000
    Proc. Court 6 - 32000000
    Proc. Court 6 - 40000000
    Proc. Court 6 - 48000000
    Proc. Court 6 - 56000000
    Proc. Court 6 - 64000000
    Proc. Court 6 - 72000000
    ############ FIN COURT 6

    SJF Election !
    Proc. Long 1 - 584000000
    Proc. Long 1 - 592000000
    Proc. Long 1 - 600000000
    Proc. Long 1 - 608000000
    Proc. Long 1 - 616000000
    Proc. Long 1 - 624000000
    Proc. Long 1 - 632000000
    Proc. Long 1 - 640000000
    Proc. Long 1 - 648000000
    Proc. Long 1 - 656000000
    Proc. Long 1 - 664000000
    Proc. Long 1 - 672000000
    Proc. Long 1 - 680000000
    Proc. Long 1 - 688000000
    Proc. Long 1 - 696000000
    Proc. Long 1 - 704000000
    Proc. Long 1 - 712000000
    Proc. Long 1 - 720000000
    Proc. Long 1 - 728000000
    Proc. Long 1 - 736000000
    Proc. Long 1 - 744000000
    Proc. Long 1 - 752000000
    Proc. Long 1 - 760000000
    Proc. Long 1 - 768000000
    Proc. Long 1 - 776000000
    SJF Election !
    Proc. Court 7 - 0
    Proc. Court 7 - 8000000
    Proc. Court 7 - 16000000
    Proc. Court 7 - 24000000
    Proc. Court 7 - 32000000
    Proc. Court 7 - 40000000
    Proc. Court 7 - 48000000
    Proc. Court 7 - 56000000
    Proc. Court 7 - 64000000
    Proc. Court 7 - 72000000
    ############ FIN COURT 7

    SJF Election !
    Proc. Long 1 - 784000000
    Proc. Long 1 - 792000000
    ############ FIN LONG 1

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	2.633s		2.109s		0.523s
    1	Function1	5.198s		2.042s		3.156s
    2	Function2	0.631s		0.131s		0.500s
    3	Function3	0.513s		0.131s		0.382s
    4	Function4	0.515s		0.131s		0.384s
    5	Function5	0.519s		0.131s		0.389s
    6	Function6	0.631s		0.131s		0.500s
    7	Function7	0.574s		0.130s		0.444s
    8	Function8	0.579s		0.129s		0.449s
    9	Function9	0.585s		0.133s		0.452s
    --------------------------------------------------------------------
    Average:		1.238s		0.520s		0.718s
    --------------------------------------------------------------------

On observe bien que dès qu'une tâche courte est créée, elle réquisitionne le CPU qui était possédé par une tâche longue.

## 3 - Approximation de SJF en temps partagé

### Question 3.1

    // Approximation SJF privilégiant les tâches courtes
    int ApproxSJF(void) {

    int p;
    double temps_cons_min;

    printf("ApproxSJF Election !\n");

    // On va chercher le processus qui a consommé le moins de temps
    // CPU jusqu'à maintenant

    p = -1;
    temps_cons_min = LONGTIME; // initialisation à la durée la plus longue
    for (int i = 0; i<MAXPROC; i++) {
        if (Tproc[i].flag == RUN && Tproc[i].ncpu < temps_cons_min) {
        p = i;
        temps_cons_min = Tproc[i].ncpu;
        }
    }
    return p;
    }

### Question 3.2

Résultat avec un quantum d'une seule seconde:

    ApproxSJF Election !
    Proc. Long 0 - 0
    Proc. Long 0 - 8000000
    Proc. Long 0 - 16000000
    Proc. Long 0 - 24000000
    Proc. Long 0 - 32000000
    Proc. Long 0 - 40000000
    Proc. Long 0 - 48000000
    Proc. Long 0 - 56000000
    Proc. Long 0 - 64000000
    Proc. Long 0 - 72000000
    Proc. Long 0 - 80000000
    Proc. Long 0 - 88000000
    Proc. Long 0 - 96000000
    Proc. Long 0 - 104000000
    Proc. Long 0 - 112000000
    Proc. Long 0 - 120000000
    Proc. Long 0 - 128000000
    Proc. Long 0 - 136000000
    Proc. Long 0 - 144000000
    Proc. Long 0 - 152000000
    Proc. Long 0 - 160000000
    Proc. Long 0 - 168000000
    Proc. Long 0 - 176000000
    Proc. Long 0 - 184000000
    Proc. Long 0 - 192000000
    Proc. Long 0 - 200000000
    Proc. Long 0 - 208000000
    Proc. Long 0 - 216000000
    Proc. Long 0 - 224000000
    Proc. Long 0 - 232000000
    Proc. Long 0 - 240000000
    Proc. Long 0 - 248000000
    Proc. Long 0 - 256000000
    Proc. Long 0 - 264000000
    Proc. Long 0 - 272000000
    Proc. Long 0 - 280000000
    Proc. Long 0 - 288000000
    Proc. Long 0 - 296000000
    Proc. Long 0 - 304000000
    Proc. Long 0 - 312000000
    Proc. Long 0 - 320000000
    Proc. Long 0 - 328000000
    Proc. Long 0 - 336000000
    Proc. Long 0 - 344000000
    Proc. Long 0 - 352000000
    Proc. Long 0 - 360000000
    Proc. Long 0 - 368000000
    Proc. Long 0 - 376000000
    Proc. Long 0 - 384000000
    Proc. Long 0 - 392000000
    Proc. Long 0 - 400000000
    Proc. Long 0 - 408000000
    Proc. Long 0 - 416000000
    Proc. Long 0 - 424000000
    Proc. Long 0 - 432000000
    Proc. Long 0 - 440000000
    Proc. Long 0 - 448000000
    Proc. Long 0 - 456000000
    ApproxSJF Election !
    Proc. Long 1 - 0
    Proc. Long 1 - 8000000
    Proc. Long 1 - 16000000
    Proc. Long 1 - 24000000
    Proc. Long 1 - 32000000
    Proc. Long 1 - 40000000
    Proc. Long 1 - 48000000
    Proc. Long 1 - 56000000
    Proc. Long 1 - 64000000
    Proc. Long 1 - 72000000
    Proc. Long 1 - 80000000
    Proc. Long 1 - 88000000
    Proc. Long 1 - 96000000
    Proc. Long 1 - 104000000
    Proc. Long 1 - 112000000
    Proc. Long 1 - 120000000
    Proc. Long 1 - 128000000
    Proc. Long 1 - 136000000
    Proc. Long 1 - 144000000
    Proc. Long 1 - 152000000
    Proc. Long 1 - 160000000
    Proc. Long 1 - 168000000
    Proc. Long 1 - 176000000
    Proc. Long 1 - 184000000
    Proc. Long 1 - 192000000
    Proc. Long 1 - 200000000
    Proc. Long 1 - 208000000
    Proc. Long 1 - 216000000
    Proc. Long 1 - 224000000
    Proc. Long 1 - 232000000
    Proc. Long 1 - 240000000
    Proc. Long 1 - 248000000
    Proc. Long 1 - 256000000
    Proc. Long 1 - 264000000
    Proc. Long 1 - 272000000
    Proc. Long 1 - 280000000
    Proc. Long 1 - 288000000
    Proc. Long 1 - 296000000
    Proc. Long 1 - 304000000
    Proc. Long 1 - 312000000
    Proc. Long 1 - 320000000
    Proc. Long 1 - 328000000
    Proc. Long 1 - 336000000
    Proc. Long 1 - 344000000
    Proc. Long 1 - 352000000
    Proc. Long 1 - 360000000
    Proc. Long 1 - 368000000
    Proc. Long 1 - 376000000
    Proc. Long 1 - 384000000
    Proc. Long 1 - 392000000
    Proc. Long 1 - 400000000
    Proc. Long 1 - 408000000
    Proc. Long 1 - 416000000
    Proc. Long 1 - 424000000
    Proc. Long 1 - 432000000
    Proc. Long 1 - 440000000
    Proc. Long 1 - 448000000
    Proc. Long 1 - 456000000
    Proc. Long 1 - 464000000
    Proc. Long 1 - 472000000
    Proc. Long 1 - 480000000
    Proc. Long 1 - 488000000
    Proc. Long 1 - 496000000
    Proc. Long 1 - 504000000
    Proc. Long 1 - 512000000
    ApproxSJF Election !
    Proc. Court 0 - 0
    Proc. Court 0 - 8000000
    Proc. Court 0 - 16000000
    Proc. Court 0 - 24000000
    Proc. Court 0 - 32000000
    Proc. Court 0 - 40000000
    Proc. Court 0 - 48000000
    Proc. Court 0 - 56000000
    Proc. Court 0 - 64000000
    Proc. Court 0 - 72000000
    ############ FIN COURT 0

    ApproxSJF Election !
    Proc. Court 1 - 0
    Proc. Court 1 - 8000000
    Proc. Court 1 - 16000000
    Proc. Court 1 - 24000000
    Proc. Court 1 - 32000000
    Proc. Court 1 - 40000000
    Proc. Court 1 - 48000000
    Proc. Court 1 - 56000000
    Proc. Court 1 - 64000000
    Proc. Court 1 - 72000000
    ############ FIN COURT 1

    ApproxSJF Election !
    Proc. Court 2 - 0
    Proc. Court 2 - 8000000
    Proc. Court 2 - 16000000
    Proc. Court 2 - 24000000
    Proc. Court 2 - 32000000
    Proc. Court 2 - 40000000
    Proc. Court 2 - 48000000
    Proc. Court 2 - 56000000
    Proc. Court 2 - 64000000
    Proc. Court 2 - 72000000
    ############ FIN COURT 2

    ApproxSJF Election !
    Proc. Court 3 - 0
    Proc. Court 3 - 8000000
    Proc. Court 3 - 16000000
    Proc. Court 3 - 24000000
    Proc. Court 3 - 32000000
    Proc. Court 3 - 40000000
    Proc. Court 3 - 48000000
    Proc. Court 3 - 56000000
    Proc. Court 3 - 64000000
    Proc. Court 3 - 72000000
    ############ FIN COURT 3

    ApproxSJF Election !
    Proc. Court 4 - 0
    Proc. Court 4 - 8000000
    Proc. Court 4 - 16000000
    Proc. Court 4 - 24000000
    Proc. Court 4 - 32000000
    Proc. Court 4 - 40000000
    Proc. Court 4 - 48000000
    Proc. Court 4 - 56000000
    Proc. Court 4 - 64000000
    Proc. Court 4 - 72000000
    ############ FIN COURT 4

    ApproxSJF Election !
    Proc. Court 5 - 0
    Proc. Court 5 - 8000000
    Proc. Court 5 - 16000000
    Proc. Court 5 - 24000000
    Proc. Court 5 - 32000000
    Proc. Court 5 - 40000000
    Proc. Court 5 - 48000000
    Proc. Court 5 - 56000000
    Proc. Court 5 - 64000000
    Proc. Court 5 - 72000000
    ############ FIN COURT 5

    ApproxSJF Election !
    Proc. Long 0 - 464000000
    Proc. Long 0 - 472000000
    Proc. Long 0 - 480000000
    Proc. Long 0 - 488000000
    Proc. Long 0 - 496000000
    Proc. Long 0 - 504000000
    Proc. Long 0 - 512000000
    Proc. Long 0 - 520000000
    Proc. Long 0 - 528000000
    Proc. Long 0 - 536000000
    Proc. Long 0 - 544000000
    Proc. Long 0 - 552000000
    Proc. Long 0 - 560000000
    Proc. Long 0 - 568000000
    Proc. Long 0 - 576000000
    Proc. Long 0 - 584000000
    Proc. Long 0 - 592000000
    Proc. Long 0 - 600000000
    Proc. Long 0 - 608000000
    Proc. Long 0 - 616000000
    Proc. Long 0 - 624000000
    Proc. Long 0 - 632000000
    Proc. Long 0 - 640000000
    Proc. Long 0 - 648000000
    Proc. Long 0 - 656000000
    Proc. Long 0 - 664000000
    Proc. Long 0 - 672000000
    Proc. Long 0 - 680000000
    Proc. Long 0 - 688000000
    Proc. Long 0 - 696000000
    Proc. Long 0 - 704000000
    Proc. Long 0 - 712000000
    Proc. Long 0 - 720000000
    Proc. Long 0 - 728000000
    Proc. Long 0 - 736000000
    Proc. Long 0 - 744000000
    Proc. Long 0 - 752000000
    Proc. Long 0 - 760000000
    Proc. Long 0 - 768000000
    Proc. Long 0 - 776000000
    Proc. Long 0 - 784000000
    Proc. Long 0 - 792000000
    ############ FIN LONG 0

    ApproxSJF Election !
    Proc. Court 6 - 0
    Proc. Court 6 - 8000000
    Proc. Court 6 - 16000000
    Proc. Court 6 - 24000000
    Proc. Court 6 - 32000000
    Proc. Court 6 - 40000000
    Proc. Court 6 - 48000000
    Proc. Court 6 - 56000000
    Proc. Court 6 - 64000000
    Proc. Court 6 - 72000000
    ############ FIN COURT 6

    ApproxSJF Election !
    Proc. Long 1 - 520000000
    Proc. Long 1 - 528000000
    Proc. Long 1 - 536000000
    Proc. Long 1 - 544000000
    Proc. Long 1 - 552000000
    Proc. Long 1 - 560000000
    Proc. Long 1 - 568000000
    Proc. Long 1 - 576000000
    Proc. Long 1 - 584000000
    Proc. Long 1 - 592000000
    Proc. Long 1 - 600000000
    Proc. Long 1 - 608000000
    Proc. Long 1 - 616000000
    Proc. Long 1 - 624000000
    Proc. Long 1 - 632000000
    Proc. Long 1 - 640000000
    Proc. Long 1 - 648000000
    Proc. Long 1 - 656000000
    Proc. Long 1 - 664000000
    Proc. Long 1 - 672000000
    Proc. Long 1 - 680000000
    Proc. Long 1 - 688000000
    Proc. Long 1 - 696000000
    Proc. Long 1 - 704000000
    Proc. Long 1 - 712000000
    Proc. Long 1 - 720000000
    Proc. Long 1 - 728000000
    Proc. Long 1 - 736000000
    Proc. Long 1 - 744000000
    Proc. Long 1 - 752000000
    Proc. Long 1 - 760000000
    Proc. Long 1 - 768000000
    Proc. Long 1 - 776000000
    Proc. Long 1 - 784000000
    Proc. Long 1 - 792000000
    ############ FIN LONG 1

    ApproxSJF Election !
    Proc. Court 7 - 0
    Proc. Court 7 - 8000000
    Proc. Court 7 - 16000000
    Proc. Court 7 - 24000000
    Proc. Court 7 - 32000000
    Proc. Court 7 - 40000000
    Proc. Court 7 - 48000000
    Proc. Court 7 - 56000000
    Proc. Court 7 - 64000000
    Proc. Court 7 - 72000000
    ############ FIN COURT 7

    PID	FUNCTION	REAL-TIME	CPU-TIME	WAITING-TIME
    --------------------------------------------------------------------
    0	Function0	3.295s		1.658s		1.637s
    1	Function1	3.941s		1.540s		2.401s
    2	Function2	2.106s		0.106s		2.000s
    3	Function3	1.709s		0.106s		1.603s
    4	Function4	1.430s		0.106s		1.324s
    5	Function5	1.425s		0.106s		1.318s
    6	Function6	1.146s		0.106s		1.040s
    7	Function7	0.867s		0.106s		0.761s
    8	Function8	0.491s		0.106s		0.385s
    9	Function9	0.491s		0.106s		0.385s
    --------------------------------------------------------------------
    Average:		1.690s		0.405s		1.285s
    --------------------------------------------------------------------

On 