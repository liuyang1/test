package main

import "fmt"

func natruals() func() int {
    i := 0
    return func() int {
        i++
        return i // like yield
    }
}

func main() {
    ns := natruals()
    p := 0.
    for i := 0; i != 10; i++ {
        v := ns()
        v1 := 1. / float64(v)
        p += v1
        fmt.Printf("%2v %10.6v %10.6v\n", v, v1, p)
    }
}
