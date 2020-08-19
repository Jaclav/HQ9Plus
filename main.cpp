#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

fstream file;
fstream asembled;
string line;
char* temp;

enum Options{compile=0,assembler=1,object=2};
Options options=compile;

int main(int argc, char** argv){
	if(argc==1){//is any parameter
		std:cerr<<"hq9+: no input file specifed\n";
		cout<<"type 'hq9+ -h' for help\n";
		return 1;
	}
	if(strcmp(argv[1],"-h")==0||strcmp(argv[1],"-help")==0||strcmp(argv[1],"--help")==0){//help
		cout<<"usage: hq9+ [-h] filename\n";
		return 0;
	}
	else if(strcmp(argv[1],"-S")==0){
		options=assembler;
	}

	file.open(argv[1],ios::in);
	asembled.open("/tmp/main.asm",ios::out);
	if(!file.good()){
		return 2;
	}

	//asm header
	asembled<<"section .text\n";
	asembled<<"global _start\n";
	asembled<<"_start:\n";

	while(getline(file,line)){
		for(int x=0;x<line.size();x++){
			if(line[x]=='h'||line[x]=='H'){
				asembled<<"mov rdi, h_str\n";
				asembled<<"mov rsi, h_str_len\n";
				asembled<<"call print\n";
			}
			else if(line[x]=='q'||line[x]=='Q'){
				asembled<<"mov rdi, q_str\n";
				asembled<<"mov rsi, q_str_len\n";
				asembled<<"call print\n";
			}
			else if(line[x]=='9'){
				asembled<<"mov rdi, nnbob\n";
				asembled<<"mov rsi, nnbob_len\n";
				asembled<<"call print\n";
			}
			else if(line[x]=='+'){
				asembled<<"inc rax\n";
			}
			else{
				cerr<<"'"<<line[x]<<"' is unknown command!";
				return 0;
			}
		}
	}
	//wyjście z programu
	asembled<<"mov rax, 60\n";
	asembled<<"mov rdi, 0\n";
	asembled<<"syscall\n";

	asembled<<"print:\n";
	asembled<<"mov rdx, rsi\n";
	asembled<<"mov rsi, rdi\n";
	asembled<<"mov rax, 1\n";
	asembled<<"mov rdi, 1\n";
	asembled<<"syscall\n";
	asembled<<"ret\n";

	//dane
	asembled<<"segment .data\n";

	//h
	asembled<<"h_str db \"Hello World!\",0x0a,0x00\n";
	asembled<<"h_str_len equ $-h_str\n";

	//q
	file.close();
	file.open(argv[1],ios::in);

	asembled<<"q_str db \"";
	while(getline(file,line)){
		asembled<<line<<"\","<<0x0a;
	}
	file.close();
	asembled<<"\n";
	asembled<<"q_str_len equ $-q_str\n";

	//9
	asembled<<"nnbob:";
	for(int i=99;i>1;i--){
			asembled<<"db \""<<i<<"bottles of beer on the wall, "<<i<<" bottles of beer. Take one down and pass it around - "<<i-1<<"bottles of beer on the wall.\",0x0a\n";
    }
    asembled<<"db \"1 bottle of beer on the wall, 1 bottle of beer. Take it down and pass it around - no more bottles of beer on the wall.\",0x0a,0x00\n";
	asembled<<"nnbob_len: equ $ - nnbob\n";
	asembled.close();

	if(options==compile){
		system("nasm -f elf64 /tmp/main.asm");
		system("ld -o a.out /tmp/main.o");
		std::cout<<"Compiled succesfull.\n";
	}
	else if(options==assembler){
		system("cat /tmp/main.asm");
	}
	else{
		system("nasm -f elf64 /tmp/main.asm");
		system("cat /tmp/main.o");
	}

	return 0;
}
