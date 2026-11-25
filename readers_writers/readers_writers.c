/*
 * James Sampson
 * CS 5343 - Module 4, Programming Assignment 4 (Program 1)
 *
 * Readers-writers synchronization using POSIX threads and semaphores.
 * Four reader threads may read the shared value at the same time, but the
 * one writer thread must have exclusive access before changing the value.
 *
 * This implements the classic readers-preference solution. It is suitable
 * for the finite assignment demonstration, but it can starve a writer if
 * readers keep arriving without a break.
 */

#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Change these constants to adjust the simulation workload. */
#define NUM_READERS 4
#define READ_ROUNDS 3
#define WRITE_ROUNDS 4
#define INITIAL_VALUE 100
#define WRITE_INCREMENT 10

/* The single shared value that all threads access. */
static int shared_value = INITIAL_VALUE;

/* Number of readers currently inside the reading section. */
static int active_readers = 0;

/*
 * reader_mutex protects active_readers. Only the first reader locks
 * resource_sem, and only the last reader unlocks it.
 * resource_sem provides exclusive writer access to shared_value.
 */
static sem_t reader_mutex;
static sem_t resource_sem;

/* Keep messages from different threads from mixing on one output line. */
static pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Wait on a semaphore, retrying if a signal interrupts the wait. */
static void semaphore_wait(sem_t *sem)
{
    while (sem_wait(sem) == -1) {
        if (errno != EINTR) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
    }
}

/* Pause for a small amount of time so thread overlap is easy to observe. */
static void sleep_ms(long milliseconds)
{
    struct timespec remaining;

    remaining.tv_sec = milliseconds / 1000;
    remaining.tv_nsec = (milliseconds % 1000) * 1000000L;

    while (nanosleep(&remaining, &remaining) == -1) {
        if (errno != EINTR) {
            perror("nanosleep");
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * Every reader follows the same sequence for each read operation:
 * 1. Increase the active reader count while holding reader_mutex.
 * 2. The first reader prevents the writer from entering.
 * 3. Read the value and spend a short time in the critical section.
 * 4. Decrease the count; the final reader allows a writer to enter.
 *
 * reader_mutex stays held while the first reader waits for resource_sem;
 * that prevents later readers from slipping past a waiting first reader.
 */
static void *reader_thread(void *argument)
{
    const int id = *(const int *)argument;

    for (int round = 1; round <= READ_ROUNDS; ++round) {
        /* Stagger starts so the output visibly contains overlapping reads. */
        sleep_ms(10L * id + (round == 1 ? 0 : 35));

        semaphore_wait(&reader_mutex);
        ++active_readers;
        if (active_readers == 1) {
            semaphore_wait(&resource_sem);  /* First reader locks resource. */
        }

        /* No writer can modify shared_value while readers are active. */
        const int observed_value = shared_value;
        pthread_mutex_lock(&output_mutex);
        printf("Reader %d | round %d | ENTER | value=%d | active readers=%d\n",
               id, round, observed_value, active_readers);
        pthread_mutex_unlock(&output_mutex);
        sem_post(&reader_mutex);

        /* Simulate time spent reading while other readers may join. */
        sleep_ms(70);

        semaphore_wait(&reader_mutex);
        --active_readers;
        pthread_mutex_lock(&output_mutex);
        printf("Reader %d | round %d | EXIT  | active readers=%d\n",
               id, round, active_readers);
        pthread_mutex_unlock(&output_mutex);
        if (active_readers == 0) {
            sem_post(&resource_sem);      /* Last reader releases resource. */
        }
        sem_post(&reader_mutex);

        /* Time between consecutive read requests. */
        sleep_ms(25L + id * 10L);
    }

    return NULL;
}

/* Only one writer exists, and it acquires resource_sem for every update. */
static void *writer_thread(void *argument)
{
    (void)argument;  /* The writer has no thread-specific arguments. */

    sleep_ms(45);    /* Give the first group of readers a chance to start. */

    for (int round = 1; round <= WRITE_ROUNDS; ++round) {
        semaphore_wait(&resource_sem);  /* Wait for all readers to leave. */

        pthread_mutex_lock(&output_mutex);
        printf("Writer   | round %d | ENTER | exclusive write begins\n", round);
        pthread_mutex_unlock(&output_mutex);

        sleep_ms(80);                    /* Simulate an expensive update. */
        shared_value += WRITE_INCREMENT;

        pthread_mutex_lock(&output_mutex);
        printf("Writer   | round %d | EXIT  | new value=%d\n",
               round, shared_value);
        pthread_mutex_unlock(&output_mutex);

        sem_post(&resource_sem);         /* Readers can proceed again. */
        sleep_ms(40);                    /* Allow threads to alternate. */
    }

    return NULL;
}

int main(void)
{
    pthread_t readers[NUM_READERS];
    pthread_t writer;
    int reader_ids[NUM_READERS];
    int error;

    /* Initialize both binary semaphores to their unlocked state (value 1). */
    if (sem_init(&reader_mutex, 0, 1) == -1 ||
        sem_init(&resource_sem, 0, 1) == -1) {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    printf("James Sampson - CS 5343 - Programming 4 - Program 1\n");
    printf("Readers-writers demonstration: %d readers, 1 writer\n",
           NUM_READERS);
    printf("Initial value: %d | %d reads/reader | %d writes (+%d each)\n\n",
           INITIAL_VALUE, READ_ROUNDS, WRITE_ROUNDS, WRITE_INCREMENT);
    fflush(stdout);

    /* Give every reader a stable ID by storing IDs in the main stack array. */
    for (int i = 0; i < NUM_READERS; ++i) {
        reader_ids[i] = i + 1;
        error = pthread_create(&readers[i], NULL, reader_thread,
                               &reader_ids[i]);
        if (error != 0) {
            fprintf(stderr, "pthread_create reader: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }

    /* The single writer runs concurrently with the four reader threads. */
    error = pthread_create(&writer, NULL, writer_thread, NULL);
    if (error != 0) {
        fprintf(stderr, "pthread_create writer: %s\n", strerror(error));
        exit(EXIT_FAILURE);
    }

    /* Join threads so main does not print a result before work completes. */
    for (int i = 0; i < NUM_READERS; ++i) {
        error = pthread_join(readers[i], NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join reader: %s\n", strerror(error));
            return EXIT_FAILURE;
        }
    }
    error = pthread_join(writer, NULL);
    if (error != 0) {
        fprintf(stderr, "pthread_join writer: %s\n", strerror(error));
        return EXIT_FAILURE;
    }

    /* Every update adds 10, so four completed writes must produce 140. */
    const int expected = INITIAL_VALUE + WRITE_ROUNDS * WRITE_INCREMENT;
    const int passed = (shared_value == expected && active_readers == 0);
    printf("\nFinal value: %d | Expected value: %d\n", shared_value, expected);
    printf("Verification: %s\n", passed ? "PASS" : "FAIL");

    /* Destroy synchronization objects only after all workers are finished. */
    sem_destroy(&resource_sem);
    sem_destroy(&reader_mutex);
    pthread_mutex_destroy(&output_mutex);

    return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
