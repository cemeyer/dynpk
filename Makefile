
audit.so: audit.o
	gcc -shared -Wl,-soname,audit -o libaudit.so audit.o

audit.o: audit.c
	gcc -fPIC -g -c -Wall audit.c -o audit.o

.PHONY: clean

clean:
	-rm -f *.o *.so


