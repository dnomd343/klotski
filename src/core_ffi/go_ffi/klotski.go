package main

/*
#cgo LDFLAGS: -L${SRCDIR}/ -ldemo -lstdc++
#include "demo.h"
*/
import "C"
import "fmt"

func main() {
    fmt.Println("hello from go")
    C.hello()
}
