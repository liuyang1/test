package main

import "fmt"
import "time"

func f(mode string) {
    for i := 0; i < 3; i++ {
        fmt.Println(mode, ":", i)
        time.Sleep(1 * time.Second)
    }
}

func main() {
    f("direct")

    go f("goroutine")
    go func(msg string) {
        fmt.Println(msg)
    }("one more go routine")

    var input string
    fmt.Println("wait input to exit")
    fmt.Scanln(&input)
    fmt.Println("exit")
}
