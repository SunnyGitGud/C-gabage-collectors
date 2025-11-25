# Garbage Collector Implementations in C

This repository contains implementations of two common garbage collection algorithms: Reference Counting and Mark & Sweep.

## Comparison

### Reference Counting
*   Tracks active references per object
*   Increments count on pointer assignment
*   Decrements count on scope exit
*   Reclaims memory immediately at zero
*   Deterministic object destruction timing
*   Fails to collect cyclic references
*   High overhead on pointer mutation
*   Requires atomic ops for thread safety
*   Poor cache locality due to updates
*   No global "stop-the-world" pauses

### Mark & Sweep
*   Allocates freely until heap triggers
*   Traverses object graph from roots
*   Mark phase identifies live objects
*   Sweep phase reclaims dead memory
*   Naturally handles cyclic structures
*   Zero overhead during pointer writes
*   Non-deterministic destruction timing
*   Incurs latency during collection
*   Susceptible to heap fragmentation
*   Complex to implement correctly
