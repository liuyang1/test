package main

import "fmt"

func swap(a *int, b *int) {
    t := *a
    *a = *b
    *b = t
}

func main() {
    a, b := 1, 2
    swap(&a, &b)
    fmt.Printf("a=%v b=%v\n", a, b)
}
