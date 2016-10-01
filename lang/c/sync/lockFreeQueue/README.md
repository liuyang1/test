# lock-free queue

Concurrent is complex. Even so basic like a queue, it's hard to implement CORRECT one.

It related a lot of factors:

- compiler optimization
- CPU reorder instructions
- cache or register in multi-core of CPU (flush / invalidate cache, memory barrier)

Lock-based solution, have one issue:

- slow or stoped process will prevent other process from accessing data structure.

We had met this issue, VPP isr calling to SHM module. SHM's global lock may slow down VPP's speed.

## doc

- [Implement Lock-Free Queues](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.53.8674&rep=rep1&type=pdf)
- [Relacy](http://www.1024cores.net/home/relacy-race-detector)

### [An efficient Unbounded Lock-Free Queue for Multi-Core Systems](http://calvados.di.unipi.it/storage/talks/2012_SPSC_Europar.pdf)

| Progress guarantees              | comment                                                         |
|----------------------------------|-----------------------------------------------------------------|
| no guarantee (blocking)          |                                                                 |
| obstruction freedom (梗阻自由？) | A thread, if executed in isolation, make progress               |
| lock freedom                     | with enough time, at least one thread make progress             |
| wait freedom                     | a thread always completes its ops in a bounded number of steps. |

## TODO

- [x] basic function
- [x] test cases
    - [x] basic
    - [x] producer-consumer
    - [x] 2-producers
    - [x] 2-consumers
- [x] multi-threading with lock
- [ ] lock free
- [x] semaphore in dequeue

### Issues

- [x] 2-consumers conflict

Fake issue, as cannot run `q_drain` in two threads same time.
T0 check queue is not empty, but T1 rab that one, so T0 will wait in `q_dequeue` and cannot skip out.

### test result
| methods \ cases | basic | producer-consumer | 2-producers | 2-consumers |
|-----------------|-------|-------------------|-------------|-------------|
| basic           | :o:   | :heavy_exclamation_mark: | :heavy_exclamation_mark: | :heavy_exclamation_mark: |
| lock            | :o:   | :o:               | :o:         | :o:         |
| lock-free       |       |                   |             |             |
