#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int num_terms;
    long long *fibonacci_sequence;
} FibonacciData;

typedef struct {
    long long *fibonacci_sequence;
    int *search_indices;
    int num_searches;
    long long *search_results;
} SearchData;

void *generate_fibonacci(void *arg) {
    FibonacciData *data = (FibonacciData *)arg;
    int n = data->num_terms;
    long long *fib = data->fibonacci_sequence;
    fib[0] = 0;
    if (n >= 1) fib[1] = 1;
    for (int i = 2; i <= n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    return NULL;
}

void *search_fibonacci(void *arg) {
    SearchData *data = (SearchData *)arg;
    long long *fib = data->fibonacci_sequence;
    int *indices = data->search_indices;
    int num_searches = data->num_searches;
    long long *results = data->search_results;
    for (int i = 0; i < num_searches; i++) {
        if (indices[i] >= 0 && indices[i] <= 40) {
            results[i] = fib[indices[i]];
        } else {
            results[i] = -1;
        }
    }
    return NULL;
}

int main() {
    int n, num_searches;
    printf("Enter the number of terms for the Fibonacci sequence:\n");
    scanf("%d", &n);
    if (n < 0 || n > 40) return 1;
    long long *fib_sequence = (long long *)malloc((n + 1) * sizeof(long long));
    if (!fib_sequence) return 1;
    FibonacciData fibonacci_data = {n, fib_sequence};
    pthread_t fibonacci_thread;
    pthread_create(&fibonacci_thread, NULL, generate_fibonacci, &fibonacci_data);
    pthread_join(fibonacci_thread, NULL);
    for (int i = 0; i <= n; i++) {
        printf("a[%d] = %lld\n", i, fib_sequence[i]);
    }
    printf("How many numbers would you like to search for?\n");
    scanf("%d", &num_searches);
    if (num_searches <= 0) {
        free(fib_sequence);
        return 1;
    }
    int *search_indices = (int *)malloc(num_searches * sizeof(int));
    long long *search_results = (long long *)malloc(num_searches * sizeof(long long));
    if (!search_indices || !search_results) {
        free(fib_sequence);
        free(search_indices);
        free(search_results);
        return 1;
    }
    for (int i = 0; i < num_searches; i++) {
        printf("Enter search index %d:\n", i + 1);
        scanf("%d", &search_indices[i]);
    }
    SearchData search_data = {fib_sequence, search_indices, num_searches, search_results};
    pthread_t search_thread;
    pthread_create(&search_thread, NULL, search_fibonacci, &search_data);
    pthread_join(search_thread, NULL);
    for (int i = 0; i < num_searches; i++) {
        printf("Result of search #%d = %lld\n", i + 1, search_results[i]);
    }
    free(fib_sequence);
    free(search_indices);
    free(search_results);
    return 0;
}
