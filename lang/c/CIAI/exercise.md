# ch8. Table

## 8.1 Table_put always added a new binding to the table even if the key was already present.

modification style

Pros:
- keep key-value assocation struct, it's clean and easy to understand.
- remove context, reduce complexity.

Cons:
- could modify on value, so need lock when need update value.


accumulation style

Pros:
- It's easy when describle depth first environment

Cons:
- hard to maintain and understand


## 8.2 Table with red-black tree
