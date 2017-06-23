package main

import "fmt"

func sum(nums ...int) {
    fmt.Println(nums)
    s := 0
    for _, i := range nums {
        s += i
    }
    fmt.Println(s)
}

func main() {
    sum(1, 2, 3, 4)
    nums := []int {1, 2, 3, 4, 5}
    sum(nums...)
}
