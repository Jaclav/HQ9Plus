#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <map>
using namespace std;

enum Options{compile=0,assembler=1,object=2};
typedef void (*Command)(std::fstream&);

fstream file;
fstream assembled;
string line;
char* temp;
Options options=compile;
map<char,Command>commands;

void cmdH(std::fstream&);
void cmdQ(std::fstream&);
void cmd9(std::fstream&);
void cmdP(std::fstream&);

int main(int argc, char** argv){
	if(system("which nasm > /dev/null")){
		std::cerr<<"hq9+: nasm compiler not found!\n";
		exit(1);
	}
	if(system("which ld > /dev/null")){
		std::cerr<<"hq9+: ld linker not found!\n";
		exit(1);
	}

	if(argc==1){//is any parameter
		std::cerr<<"hq9+: no input file specifed\n";
		cout<<"type 'hq9+ -h' for help\n";
		return 2;
	}
	if(strcmp(argv[1],"-h")==0||strcmp(argv[1],"-help")==0||strcmp(argv[1],"--help")==0){//help
		cout<<"usage: hq9+ [-h] filename\n";
		return 0;
	}
	else if(strcmp(argv[1],"-S")==0){
		options=assembler;
	}

	file.open(argv[1],ios::in);
	assembled.open("/tmp/main.asm",ios::out);
	if(!file.good()){
		return 3;
	}
	commands['h']=cmdH;
	commands['H']=cmdH;

	commands['q']=cmdQ;
	commands['Q']=cmdQ;

	commands['9']=cmd9;

	commands['+']=cmdP;

	//asm header
	assembled<<"section .text\n";
	assembled<<"global _start\n";
	assembled<<"_start:\n";

	while(getline(file,line)){
		for(int x=0;x<line.size();x++){
			if(commands.contains(line[x])){
				commands[line[x]](assembled);
			}
			else{
				cerr<<"'"<<line[x]<<"' is unknown command!";
				return 0;
			}
		}
	}
	//wyjście z programu
	assembled<<"mov rax, 60\n";
	assembled<<"mov rdi, 0\n";
	assembled<<"syscall\n";

	assembled<<"print:\n";
	assembled<<"mov rdx, rsi\n";
	assembled<<"mov rsi, rdi\n";
	assembled<<"mov rax, 1\n";
	assembled<<"mov rdi, 1\n";
	assembled<<"syscall\n";
	assembled<<"ret\n";

	//dane
	assembled<<"segment .data\n";

	//h
	assembled<<"h_str db \"Hello World!\",0x0a,0x00\n";
	assembled<<"h_str_len equ $-h_str\n";

	//q
	file.close();
	file.open(argv[1],ios::in);

	assembled<<"q_str db \"";
	while(getline(file,line)){
		assembled<<line<<"\","<<0x0a;
	}
	file.close();
	assembled<<"\n";
	assembled<<"q_str_len equ $-q_str\n";

	//9
	assembled<<"nnbob:";
	for(int i=99;i>1;i--){
			assembled<<"db \""<<i<<"bottles of beer on the wall, "<<i<<" bottles of beer. Take one down and pass it around - "<<i-1<<"bottles of beer on the wall.\",0x0a\n";
    }
    assembled<<"db \"1 bottle of beer on the wall, 1 bottle of beer. Take it down and pass it around - no more bottles of beer on the wall.\",0x0a,0x00\n";
	assembled<<"nnbob_len: equ $ - nnbob\n";

	//+
	assembled<<"accumulator dq 0\n";
	assembled.close();

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

void cmdH(std::fstream &assembled){
	assembled<<"mov rdi, h_str\n";
	assembled<<"mov rsi, h_str_len\n";
	assembled<<"call print\n";
}
void cmdQ(std::fstream &assembled){
	assembled<<"mov rdi, q_str\n";
	assembled<<"mov rsi, q_str_len\n";
	assembled<<"call print\n";
}
void cmd9(std::fstream &assembled){
	assembled<<"mov rdi, nnbob\n";
	assembled<<"mov rsi, nnbob_len\n";
	assembled<<"call print\n";
}
void cmdP(std::fstream &assembled){
	assembled<<"mov rax, [accumulator]\n";
	assembled<<"inc rax\n";
	assembled<<"mov [accumulator], rax\n";
}
