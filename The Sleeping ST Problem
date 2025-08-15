
*** Problem Statement: ***
A Student Tutor Consultation System is being simulated using POSIX threads (pthread), semaphores, and mutexes. In this system:
One ST is available to provide consultation.


A total of 10 students arrive at random intervals seeking consultation.


Only 3 waiting chairs are available in the lobby for students waiting for consultation.


If a student arrives and all chairs are occupied, they leave without receiving consultation.


The ST goes to sleep when no students are waiting.


If a student arrives while the ST is asleep, they wake the ST up and wait for consultation.


Your task is to implement this synchronization system ensuring that:
The ST serves one student at a time.


No race conditions occur when students enter the waiting area.


Proper synchronization is maintained using semaphores and mutex locks.



###   Main code:   ### 


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_STUDENTS 10
#define MAX_WAITING_CHAIRS 3

int served_students = 0;
int waiting_students = 0;
sem_t available_chairs;
pthread_mutex_t mutex;

void *student(void *arg) {
    int student_id = *(int *)arg;
    printf("Student %d started waiting for consultation\n", student_id);
    pthread_mutex_lock(&mutex);
    if (waiting_students < MAX_WAITING_CHAIRS) {
        waiting_students++;
        printf("A waiting student started getting consultation\n");
        printf("Number of students now waiting: %d\n", waiting_students);
        sem_post(&available_chairs);
    } else {
        printf("No chairs remaining in lobby. Student %d Leaving.....\n", student_id);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *student_tutor(void *arg) {
    while (served_students < MAX_STUDENTS) {
        sem_wait(&available_chairs);
        pthread_mutex_lock(&mutex);
        if (waiting_students > 0) {
            waiting_students--;
            served_students++;
            printf("ST giving consultation\n");
            printf("Student %d is getting consultation\n", served_students);
            printf("Student %d finished getting consultation and left\n", served_students);
            printf("Number of served students: %d\n", served_students);
            usleep(1000000);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t tutor_thread;
    pthread_t student_threads[MAX_STUDENTS];
    int student_ids[MAX_STUDENTS];

    sem_init(&available_chairs, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tutor_thread, NULL, student_tutor, NULL);

    for (int i = 0; i < MAX_STUDENTS; i++) {
        student_ids[i] = i;
        pthread_create(&student_threads[i], NULL, student, (void *)&student_ids[i]);
        usleep(rand() % 500000);
    }

    for (int i = 0; i < MAX_STUDENTS; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_join(tutor_thread, NULL);

    sem_destroy(&available_chairs);
    pthread_mutex_destroy(&mutex);

    return 0;
}
