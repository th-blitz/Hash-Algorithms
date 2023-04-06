
files := main.o md5.o sha1.o sha224.o sha256.o sha384.o sha512.o sha512-224.o sha512-256.o 


all: hash.out

hash.out: $(files)
	gcc -o hash.out $(files)

$(files): %.o: %.c 

clean:
	rm -f *.o hash.out

