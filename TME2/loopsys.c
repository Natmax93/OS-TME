#include <sys/types.h>
#include <unistd.h>

/* Programme qui exécute une boucle vide effectuant 5*10^9 itérations */

int main(int argc, char **argv){
    for (int i=1; i<10000000; i++){
        getpid();
    }
}