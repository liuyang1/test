# How to write Makefile with rule and dependency detect

- use `wildcard` and `subst` or `find` to generate src list and obj list
- use rule to generate build rule
- use `-MM` to generate dependency
    must first generate obj file, then generate dependency file. only next time build, we could reuse that dependency result.

## ref
- http://scottmcpeak.com/autodepend/autodepend.html

## Advanced

- http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
