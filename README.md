*This project has been created as part of the 42 curriculum by thours.*

# Codexion

## Description

Codexion is a C concurrency project in which multiple coders compete for a limited number of shared USB dongles.

Each coder is represented by a POSIX thread and repeatedly goes through the following cycle:

1. Debugging
2. Refactoring
3. Requesting the two adjacent dongles required to compile
4. Compiling
5. Releasing the dongles, which may then enter a cooldown period

A coder must start compiling again before its burnout deadline. If a coder misses this deadline, the simulation stops. The simulation also stops successfully once every coder has completed the required number of compilations.

Two scheduling policies are implemented:

- **FIFO (First In First Out)**: requests are served in their arrival order.
- **EDF (Earliest Deadline First)**: the request with the earliest burnout deadline has priority. Request order is used as a deterministic tie-breaker.

Requests are managed through a custom binary heap priority queue.

## Instructions

### Compilation

```bash
make
```

The executable produced is:

```bash
./codexion
```

Useful Makefile rules:

```bash
make clean
make fclean
make re
```

### Execution

```text
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

The scheduler must be either `fifo` or `edf`.

Example:

```bash
./codexion 5 2000 200 200 200 10 0 fifo
```

## Implementation overview

The simulation contains:

- one thread for each coder;
- one monitor thread responsible for detecting burnout and successful completion;
- shared dongles arranged in a circle;
- a custom binary heap priority queue for scheduling compilation requests.

A request stores the requesting coder, its arrival order, and its burnout deadline. The heap reorganizes requests according to the selected scheduler.

## Blocking cases handled

### Deadlock prevention

Coders do not independently hold one dongle while waiting indefinitely for another. A coder can start compiling only when:

- its request has the highest priority;
- both required dongles are available;
- the simulation has not finished.

This prevents circular waiting and avoids the classical deadlock situation in which each participant holds a resource while waiting for another one.

### Starvation prevention

Requests are explicitly scheduled through the priority queue.

FIFO serves requests according to their arrival order. EDF gives priority to the coder with the closest burnout deadline. Equal deadlines are resolved using request order to keep EDF deterministic.

### Dongle cooldown

After compilation, the two used dongles enter a cooldown state. A dongle cannot be granted again until its configured cooldown has expired.

The monitor checks cooldown deadlines and makes eligible dongles available again.

### Precise burnout detection

A dedicated monitor thread continuously checks whether a coder has exceeded its allowed time without starting another compilation.

When burnout is detected, the simulation is marked as finished and waiting threads are awakened so that the program can terminate cleanly.

### Race condition prevention

Shared data accessed by multiple threads is protected by synchronization. This includes:

- the simulation completion state;
- dongle states and cooldown deadlines;
- the priority queue;
- request ordering;
- compilation counters;
- compilation timestamps.

### Log serialization

Output is protected by a dedicated mutex so that messages from multiple threads cannot interleave on the same line.

## Thread synchronization mechanisms

### `pthread_mutex_t`

The main simulation state is protected by `state_mutex`, which synchronizes access to shared state including the priority queue, dongles, cooldown information, and simulation completion state.

A separate `log_mutex` protects output operations and ensures serialized logs.

### `pthread_cond_t`

A condition variable coordinates waiting coders.

When a coder cannot compile because its request is not first in the priority queue or one of its required dongles is unavailable, it waits on the condition variable.

Threads are notified when a state change may allow another coder to proceed, including:

- insertion of a new request;
- removal of the highest-priority request;
- expiration of a dongle cooldown;
- simulation termination.

After waking up, a coder always checks the required conditions again while holding the appropriate mutex.

### Monitor and worker coordination

The monitor and coder threads communicate through shared state protected by mutexes and through the condition variable.

When the simulation ends because of burnout or successful completion, waiting threads are awakened so that they can terminate cleanly.

## Resources

### Documentation

- POSIX threads manual pages
- POSIX mutex manual pages
- POSIX condition variable manual pages`
- Memory management manual pages
- 42 Norm documentation

### AI usage

AI was used as a learning and development assistant throughout the project.

Its main uses included:

- explaining POSIX threads, mutexes, and condition variables;
- discussing concurrency architecture and synchronization strategies;
- explaining binary heaps and priority queues;
- reviewing implementation ideas and identifying potential race conditions, deadlocks, and synchronization issues;
- helping analyze runtime errors reported by AddressSanitizer;
- assisting with the drafting of this README.
