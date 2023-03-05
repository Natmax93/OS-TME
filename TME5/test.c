#include <stdio.h>
#include <string.h>

int main() {
   char str[] = "Hello, World! This is a test";
   const char delim[] = " ,!"; // Délimiteurs: espace, virgule, point d'exclamation
   char *ptr;

   ptr = strtok(str, delim); // Première utilisation de strtok

   while(ptr != NULL) {
      printf("%s\n", ptr);
      ptr = strtok(NULL, delim); // Appel itératif de strtok
   }

   ptr = strtok(str, delim); // Première utilisation de strtok

   while(ptr != NULL) {
      printf("%s\n", ptr);
      ptr = strtok(NULL, delim); // Appel itératif de strtok
   }

   return 0;
}
