# Readers-Writers Synchronization

**James Sampson | C11 and POSIX threads on Linux**

This program demonstrates the readers-writers synchronization problem. Four readers can access a shared integer concurrently. One writer waits for exclusive access before changing it.

## Build and run

From this directory on Linux:

    make
    ./readers_writers

Or compile directly:

    gcc -std=c11 -Wall -Wextra -Werror -pedantic -O2 -pthread readers_writers.c -o readers_writers
    ./readers_writers

To remove the compiled program:

    make clean

The program starts at 100. Each of four readers performs three reads; the writer performs four updates of +10. After joining the threads, the program checks that the final value is 140 and that no readers remain active.

## Recorded output

The included [sample_output.txt](sample_output.txt) preserves a successful Linux execution from the coursework submission. Its final lines are:

    Final value: 140 | Expected value: 140
    Verification: PASS

Thread-event ordering can vary between executions. This package retains the recorded output; it was not rerun when preparing the portfolio.

## Design

- **reader_mutex** is a binary semaphore protecting the active-reader count.
- **resource_sem** gives the writer exclusive access to the shared value.
- The first active reader acquires the resource; the last active reader releases it.
- A separate pthread mutex prevents interleaved log lines.
- Main joins every worker before checking the final state and destroying synchronization objects.

The [design notes](DESIGN.txt) explain the invariants and choices in more detail.

## Limitation

This is a **readers-preference** solution with a finite demonstration workload. If readers continually arrive, the writer can starve. A service queue or writer-priority policy would be needed for different fairness requirements.

## Attribution

This is James Sampson's individual CS 5343 Module 4 readers-writers coursework. The C source, Makefile, design notes, and recorded sample output are unchanged from the saved submission. This run guide adapts the original README for the standalone example.
