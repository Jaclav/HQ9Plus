run: main.cpp
	g++ main.cpp -o HQ9+
	./HQ9+ main.hq
	@echo "++++++++++++++"
	@cat /tmp/main.asm
	@echo "\n++++++++++++++"
	nasm -f elf64 /tmp/main.asm
	ld -o /tmp/a.out /tmp/main.o
	/tmp/a.out
clean:
	rm /tmp/main.asm
	rm /tmp/main.o
	rm /tmp/a.out
