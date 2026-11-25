# James Sampson

## Computer Science Project Portfolio

I study computer science at **Southern Methodist University**, with a **cybersecurity specialization**. I expect to graduate with a **B.S. in Computer Science in May 2027**.

My selected individual coursework covers AI search, text classification, and concurrent systems programming. I focus on understandable implementations, measured behavior, and clear explanations of engineering tradeoffs.

## Included source example

**[Readers-writers synchronization in C](readers_writers/README.md)** is a self-contained Linux example with source, a Makefile, design notes, and a captured execution log.

| File | Purpose |
| --- | --- |
| [readers_writers.c](readers_writers/readers_writers.c) | POSIX threads and semaphore implementation |
| [Makefile](readers_writers/Makefile) | Build, run, and clean commands |
| [DESIGN.txt](readers_writers/DESIGN.txt) | Synchronization invariants and tradeoffs |
| [sample_output.txt](readers_writers/sample_output.txt) | Recorded execution ending in a successful final-state check |

## Selected projects

### 1. Adversarial search for Connect 4

**Python, NumPy, pandas, Matplotlib**

Extended a supplied course notebook with bitboard-based game logic, exact and depth-limited negamax alpha-beta search, a transposition table with exact/lower/upper bounds, move ordering, and flat Monte Carlo simulation.

- **Recorded result:** exact minimax completed 100 games against random play on **4 x 4 boards**, with **79 wins, 21 draws, and no losses**.
- **Validation:** an independent grid-based minimax reference checked **144 search/value/move comparisons**.
- **Scope:** larger full-depth searches were capped and marked incomplete. These results do not establish perfect play for every standard 6 x 7 position. The Monte Carlo agent uses flat random playouts, not MCTS/UCT.

Evidence: the executed JamesSampson_Connect4.ipynb coursework notebook, November 24, 2026.

### 2. BBC news text classification

**Python, scikit-learn, NLTK, pandas, Matplotlib**

Built a text-processing pipeline that cleans and stems articles, fits TF-IDF features on training data, compares five classifiers, and inspects confusion matrices and article-level errors.

- **Recorded result:** linear SVM achieved **96.12% accuracy** and **94.71% macro F1** in a separate deduplicated experiment, correctly classifying 347 of 361 test articles.
- **Data quality:** the supplied archive contained 1,927 articles. Removing 124 extra normalized copies produced 1,803 unique articles for that experiment.
- **Scope:** the shared holdout was used to compare models, so it is not an untouched final validation after model selection. Deduplication also changes split membership.

Evidence: James_Sampson_BBC_News_Classification.py and its accompanying report, November 24, 2026.

### 3. Readers-writers synchronization

**C11, Linux, POSIX threads, semaphores, GCC, Make**

Implemented a finite readers-writers demonstration with **four reader threads and one writer**. One semaphore protects the active-reader count; another protects the shared resource. The first reader acquires the resource, the last reader releases it, and the writer obtains exclusive access. A separate mutex keeps log messages intact.

- **Recorded result:** four increments of 10 changed the shared value from 100 to **140**. The [saved log](readers_writers/sample_output.txt) ends with **Verification: PASS**.
- **Engineering detail:** stable thread arguments, thread joins before cleanup, interrupted-wait handling, and a build with warnings treated as errors.
- **Scope:** this is the classic readers-preference algorithm. Continuous reader arrivals can starve a writer; the example does not guarantee fairness.

Individual CS 5343 coursework, November 6, 2026. See the [run instructions](readers_writers/README.md) and [design explanation](readers_writers/DESIGN.txt).

### 4. Maze search and planning

**Python, NumPy, graph search, performance analysis**

Extended an instructor-provided notebook with BFS, DFS, greedy best-first search, A*, and iterative deepening. Experiments measure path cost, node expansions, tree depth, frontier size, and live search nodes. Extensions include weighted A*, multiple goals, and corridor compression with uniform-cost search.

- **Recorded result:** BFS and A* agreed on optimal path costs for **all eight tested mazes**. On the small maze, A* reduced expansions from 92 to 53 while preserving cost 19.
- **Coverage:** **31 of 32** main search runs solved their maze within the allowance. Corridor-compressed search matched BFS costs on 8 of 8 maps, and all 32 weighted-A* trials returned valid routes.
- **Scope:** one DFS run reached the 100,000-expansion cap. Iterative deepening solved 4 of 8 maps within its separate 200,000-expansion allowance. Higher weighted-A* values can sacrifice optimality.

Evidence: the executed JamesSampson_Maze.ipynb coursework notebook, October 6, 2026.

## Additional work samples and attribution

The complete Connect 4 and maze notebooks, and the BBC classification script/report, are available on request.

The Connect 4 and maze projects build on instructor-provided course notebooks. The original notebooks retain attribution for task wording, initial representations or examples, maps, and helper utilities. The implementations and experiments described above are the coursework extensions; this portfolio does not claim the supplied course materials as original work.

The C source, Makefile, design notes, and sample log are preserved from the individual readers-writers submission. The run guide is adapted from its original README.

Metrics on this page are recorded outputs from saved coursework artifacts. No new benchmark run was performed when preparing this portfolio.
