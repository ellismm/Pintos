#include <stdio.h>
#include <stdlib.h>

void greetings(int x) {
 
  printf("Hello, World!\n");
  --x; 
  if(x > 0 ) {
    greetings(x);
  }
}


int main(int argc, char* argv[]) {
  int x;
  x = atoi(argv[1]);

  greetings(x);  

  if(x == x)
    printf("Goodbye\n");
  
  return 0;
}


