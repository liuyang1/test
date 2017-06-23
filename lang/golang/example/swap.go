package main

import "fmt"

// for multiple return values
func swap(a, b int) (int, int) {
    return b, a
}

func main() {
    a, b := swap(1, 2)
    fmt.Printf("a=%v b=%v\n", a, b)
}
