run: main.cpp main.hq
	g++ main.cpp -o HQ9+
	./HQ9+ main.hq
clean:
	rm /tmp/main.asm
	rm /tmp/main.o
	rm /tmp/a.out
