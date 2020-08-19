run: main.cpp main.hq
	g++ -std=c++2a main.cpp -o hq9+
	./hq9+ main.hq
clean:
	rm /tmp/main.asm
	rm /tmp/main.o
	rm /tmp/a.out
