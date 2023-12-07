package main

import (
    "fmt"
    "os"
    "time"
)

func main() {
    fmt.Println(os.Getpid())
    time.Sleep(1000 * time.Second)
}
