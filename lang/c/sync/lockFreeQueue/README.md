# lock-free queue

Concurrent is complex. Even so basic like a queue, it's hard to implement CORRECT one.

It related a lot of factors:

- compiler optimization
- CPU reorder instructions
- cache or register in multi-core of CPU (flush / invalidate cache, memory barrier)

Lock-based solution, have issues:

- dead lock
- live lock
- slow or stopped process will prevent other process from accessing data structure. It's [Priority inversion](https://en.wikipedia.org/wiki/Priority_inversion).
- significantly reduce opportunities for parallelism,

We had met this issue, VPP isr calling to SHM module. SHM's global lock may slow down VPP's speed.

## doc

- [Implement Lock-Free Queues](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.53.8674&rep=rep1&type=pdf)
- [Relacy](http://www.1024cores.net/home/relacy-race-detector)
- [Non-blocking_algorithm](https://en.wikipedia.org/wiki/Non-blocking_algorithm)

Chinese blog

- http://coolshell.cn/articles/8239.html
- http://blog.csdn.net/jadedrip/article/details/5682196

### [An efficient Unbounded Lock-Free Queue for Multi-Core Systems](http://calvados.di.unipi.it/storage/talks/2012_SPSC_Europar.pdf)

| Progress guarantees              | comment                                                         |
|----------------------------------|-----------------------------------------------------------------|
| no guarantee (blocking)          |                                                                 |
| obstruction freedom (梗阻自由？) | A thread, if executed in isolation, make progress               |
| lock freedom                     | with enough time, at least one thread make progress             |
| wait freedom                     | a thread always completes its ops in a bounded number of steps. |

Lock free generally use atomic read-compare-write primitives. such as CAS(compare-and-swap). STM is high level solution to implement non-blocking algo.

## TODO

- [x] basic function
- [x] test cases
    - [x] basic
    - [x] producer-consumer
    - [x] 2-producers
    - [x] 2-consumers
- [x] multi-threading with lock
- [x] lock free
- [x] semaphore in dequeue
- [ ] remove semaphore when lock free

### Issues

- [x] 2-consumers conflict

Fake issue, as cannot run `q_drain` in two threads same time.
T0 check queue is not empty, but T1 rob that one, so T0 will wait in `q_dequeue` and cannot skip out.

- [x] head & tail conflict when lock-free

Use moving dummy node solution.

开始的时候，队列里有一个空节点O（它的内容没有意义），头指针指向O, 执行一次插入操作后，队列中包含 O->A 两个节点，有效内容保存在 A 节点中，读线程的读取流程如下：

1. 获取 Head 指向的节点 O
1. 发现 O 的 Next 指针不为空（如果为空代表队列为空），取 Next 指针指向的 A 节点
1. 取A的值，然后使用 CAS操作修改 **头指针指向 A**
1. 如果 3 成功，那么 **删除 O**，否则从 1 重新开始。

### test result

| methods \ cases | basic | interleave | 2-producers              | 2-consumers              | producer-consumer        |
|-----------------|-------|------------|--------------------------|--------------------------|--------------------------|
| basic           | :o:   | :o:        | :heavy_exclamation_mark: | :heavy_exclamation_mark: | :heavy_exclamation_mark: |
| lock            | :o:   | :o:        | :o:                      | :o:                      | :o:                      |
| lock-free       | :o:   | :o:        | :o:                      | :o:                      | :o:                      |

#### performance test
