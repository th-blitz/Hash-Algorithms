
files := md5.o sha1.o sha224.o sha256.o sha384.o sha512-224.o sha512-256.o sha512.o endianness_test.o

main: $(files) main.out 
	./main.out

test: $(files) test.out
	./test.out


test.out: $(files) test.o
	gcc -o test.out $(files) test.o

main.out: $(files) main.o
	gcc -o main.out $(files) main.o

test.o: test.c
	gcc -c test.c

main.o: main.c
	gcc -c main.c

$(files): %.o: %.c

clean:
	rm -f *.o main.out test.out

