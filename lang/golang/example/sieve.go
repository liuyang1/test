package main

import "fmt"

func main() {
    const n = 100;
    var a[n] int;
    for i := 0; i != n; i++ {
        a[i] = 1;
    }
    a[0] = 0;
    a[1] = 0;


    for j := 2; j != n; j++ {
        if (a[j] == 0) {
            continue
        }
        for i := 2 * j; i < n; i += j {
            a[i] = 0;
        }
    }
    for j := 2; j != n; j++ {
        if (a[j] != 0) {
            fmt.Println(j);
        }
    }
}
