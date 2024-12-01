gcc -O3 -c demo.c -o demo.o
ar rcs libdemo.a demo.o
rm -f demo.o
ranlib libdemo.a

go run klotski.go
rm -f libdemo.a
