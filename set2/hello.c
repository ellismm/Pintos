#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void forkThem(int n) {
  if(n>0) {
    fork();
    forkThem(n-1);
  }
}

void main(int argc, char** argv) {
  forkThem(5);
}
