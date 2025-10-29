#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define min(a,b) ((a) < (b) ? (a) : (b))


int *arr;
int array_size;
int num_threads;

void *thread_function(void *arg) {
  long ID = *(long *)arg;
  int start = ID * (double) array_size / num_threads;
  int end = min((ID + 1) * (double) array_size / num_threads, array_size);

  struct timespec st, fi; 
  double elapsed; 
  clock_gettime(CLOCK_MONOTONIC, &st); 

  for(int i = start; i < end; i++) {
    arr[i] += 100;
  }

  clock_gettime(CLOCK_MONOTONIC, &fi);

  elapsed = (fi.tv_sec - st.tv_sec);
  elapsed += (fi.tv_nsec - st.tv_nsec) / 1000000000.0;

  printf("Thread %ld took %.6f seconds\n", ID, elapsed);

  pthread_exit(NULL);

}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Specificati dimensiunea array-ului si numarul de thread-uri\n");
    exit(-1);
  }

  array_size = atoi(argv[1]);
  num_threads = atoi(argv[2]);


  arr = malloc(array_size * sizeof(int));
  for (int i = 0; i < array_size; i++) {
    arr[i] = i;
  }

  for (int i = 0; i < array_size; i++) {
    printf("%d", arr[i]);
    if (i != array_size - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }
  pthread_t threads[num_threads];
  long ids[num_threads];
  for (long i = 0; i < num_threads; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, thread_function, &ids[i]);
  }

  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < array_size; i++) {
    printf("%d", arr[i]);
    if (i != array_size - 1) {
      printf(" ");
    } else {
      printf("\n");
    }
  }

  return 0;
}
