package main

import "fmt"

func main() {
    msg := make(chan string)
    go func() {msg <- "ping"}()

    fmt.Println(<- msg)
}
