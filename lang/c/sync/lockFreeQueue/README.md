# lock-free queue

Concurrent is complex. Even so basic like a queue, it's hard to implement CORRECT one.

## doc
[Implement Lock-Free Queues](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.53.8674&rep=rep1&type=pdf)

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
| basic           | :o:   |                   |             |             |
| lock            | :o:   | :o:               | :o:         | :o:         |
| lock-free       |       |                   |             |             |
