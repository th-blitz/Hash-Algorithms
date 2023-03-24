
files := md5.run sha1.run

make: $(files)

sha1.run: sha1.o
	cc sha1.o -o sha1.run
sha1.o: sha1.c 
	cc -c sha1.c -o sha1.o
sha1.c:
	echo "- file sha1.c does not exist."

md5.run: md5.o
	cc md5.o -o md5.run
md5.o: md5.c
	cc -c md5.c -o md5.o
md5.c:
	echo "- file md5.c does not exist."



clean:
	rm -f $(files) 