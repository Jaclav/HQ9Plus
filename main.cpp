#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

fstream file;
fstream tmp;
string line;

int main(int argc, char** argv){
	if(argc==1){//is any parameter
		std:cout<<"hq9+: no input file specifed\n";
		std::cout<<"type 'hq9+ -h' for help\n";
		return 1;
	}
	if(strcmp(argv[1],"-h")==0||strcmp(argv[1],"-help")==0||strcmp(argv[1],"--help")==0){//help
		std::cout<<"usage: hq9+ [-h] filename\n";
		return 0;
	}

	file.open(argv[1],ios::in);
	tmp.open("/tmp/main.asm",ios::out);
	if(!file.good()){
		return 2;
	}

	//start .asm
	tmp<<"section .text\n";
	tmp<<"global _start\n";
	tmp<<"_start:\n";

	while(getline(file,line)){
		for(int x=0;x<line.size();x++){
			if(line[x]=='h'){
				tmp<<"mov rax, 1\n";
				tmp<<"mov rdi, 1\n";
				tmp<<"mov rsi, hello\n";
				tmp<<"mov rdx, helloL\n";
				tmp<<"syscall\n";
			}
			else(line[x]=='+'){
				tmp<<"inc rax\n";
			}
		}
	}
	//wyjście z programu
	tmp<<"mov rax, 60\n";
	tmp<<"mov rdi, 0\n";
	tmp<<"syscall\n";

	//dane
	tmp<<"segment .data\n";
	tmp<<"hello db \"Hello World!\",0x0a,0x00\n";
	tmp<<"helloL equ $-tekst";

	file.close();
	tmp.close();

	return 0;
}
