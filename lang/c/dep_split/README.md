# dependency split

We have one internal magic struct, we want hide detail of it. But we need to expose its size.
However, we cannot `sizeof` it now, as we don't have its strcut.

How to solve this problem?

hardcode is bad idea.

I find one solution only exposing one magic number at `export.h`, but keep consistency by [static_assert](https://en.wikipedia.org/wiki/Assertion_(software_development)#Static_assertions).
