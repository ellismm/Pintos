#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *threadPrint(void *);

int STARTNUM = 1;
int ENDNUM = 10;

int main() {
      int tnum = 1, tnum1 = 2, tnum2 = 3;
          pthread_t tid;
              pthread_create(&tid, NULL, threadPrint, &tnum);
                  pthread_t tid1;
                      pthread_create(&tid1, NULL, threadPrint, &tnum1);
                          pthread_t tid2;
                              pthread_create(&tid2, NULL, threadPrint, &tnum2);
                                  
                                      pthread_join(tid, NULL);
                                          printf("tid (%d) is finished..\n", tnum);
                                              pthread_join(tid1, NULL);
                                                  printf("tid (%d) is finished..\n", tnum1);
                                                      pthread_join(tid2, NULL);
                                                          printf("tid (%d) is finished..\n", tnum2);

                                                              return 0;
}

void *threadPrint(void * tnum) {
      int i;
          for (i = STARTNUM; i <= ENDNUM; i ++) {
                    printf("%d:%2d\n", * (int*) tnum, i);
                        }
                            return NULL;
}
