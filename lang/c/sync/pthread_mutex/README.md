# Demo to show sync

| case         | comment                            |
|--------------|------------------------------------|
| without lock | wrong result                       |
| mutex        | right, slow, flexible, easy to use |
| atomic       | right, fast                        |

## test case

One global int value, two threading increment times same time.
Finish that, check result.

## analysis

When without lock, there are [ABA problem](https://en.wikipedia.org/wiki/ABA_problem).
