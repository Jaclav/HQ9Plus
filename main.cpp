#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <map>
using namespace std;

enum Options{compile=0,assembler=1};
typedef void (*Command)(std::fstream&);

fstream file;
fstream assembled;
string line;
Options options=compile;
map<char,Command>commands;
char tmpDirPath[]="/tmp/hq9+XXXXXX";
stringstream ss;

void cmdH(std::fstream&);
void cmdQ(std::fstream&);
void cmd9(std::fstream&);
void cmdP(std::fstream&);

int main(int argc, char** argv){
	//czy są spełnione zależności
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
	else if(strcmp(argv[1],"-S")==0){//show asm
		options=assembler;
	}

	//otwieranie plików
	mkdtemp(tmpDirPath);
	if(argc==2){
		file.open(argv[1],ios::in);
	}
	else{
		file.open(argv[2],ios::in);
	}
	ss<<tmpDirPath<<"main.asm";
	assembled.open(ss.str().c_str(),ios::out);
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
	assembled<<"\t;exit(0)\n";
	assembled<<"\tmov rax, 60\n";
	assembled<<"\tmov rdi, 0\n";
	assembled<<"\tsyscall\n";

	assembled<<"print:\n";
	assembled<<"\tmov rdx, rsi\n";
	assembled<<"\tmov rsi, rdi\n";
	assembled<<"\tmov rax, 1\n";
	assembled<<"\tmov rdi, 1\n";
	assembled<<"\tsyscall\n";
	assembled<<"\tret\n\n";

	//dane
	assembled<<"segment .data\n";

	//h
	assembled<<"h_str db \"Hello World!\",0x0a,0x00\n";
	assembled<<"h_str_len equ $-h_str\n\n";

	//q
	file.close();
	if(argc==2){
		file.open(argv[1],ios::in);
	}
	else{
		file.open(argv[2],ios::in);
	}

	assembled<<"q_str:\n";
	while(getline(file,line)){
		assembled<<"db \""<<line<<"\","<<0x0a<<"\n";
	}
	file.close();
	assembled<<"\n";
	assembled<<"q_str_len equ $-q_str\n\n";

	//9
	assembled<<"nnbob:\n";
	for(int i=99;i>1;i--){
			assembled<<"\tdb \""<<i<<"bottles of beer on the wall, "<<i<<" bottles of beer. Take one down and pass it around - "<<i-1<<"bottles of beer on the wall.\",0x0a\n";
    }
    assembled<<"\tdb \"1 bottle of beer on the wall, 1 bottle of beer. Take it down and pass it around - no more bottles of beer on the wall.\",0x0a,0x00\n";
	assembled<<"nnbob_len: equ $ - nnbob\n\n";

	//+
	assembled<<"accumulator dq 0\n";
	assembled.close();

	//kompilacja i linkowanie
	if(options==compile){
		ss.str("");
		ss<<"nasm -f elf64 "<<tmpDirPath<<"main.asm";
		if(system(ss.str().c_str())){//compilation error
			exit(1);
		}

		ss.str("");
		ss<<"ld -o a.out "<<tmpDirPath<<"main.o";
		system(ss.str().c_str());
		std::cout<<"Compiled succesfull.\n";
	}
	else if(options==assembler){
		ss.str("");
		ss<<"cat "<<tmpDirPath<<"main.asm";
		system(ss.str().c_str());
	}

	ss.str("");
	ss<<"rm -rd "<<tmpDirPath;
	system(ss.str().c_str());
	return 0;
}

void cmdH(std::fstream &assembled){
	assembled<<"\t;H\n";
	assembled<<"\tmov rdi, h_str\n";
	assembled<<"\tmov rsi, h_str_len\n";
	assembled<<"\tcall print\n\n";
}
void cmdQ(std::fstream &assembled){
	assembled<<"\t;Q\n";
	assembled<<"\tmov rdi, q_str\n";
	assembled<<"\tmov rsi, q_str_len\n";
	assembled<<"\tcall print\n\n";
}
void cmd9(std::fstream &assembled){
	assembled<<"\t;9\n";
	assembled<<"\tmov rdi, nnbob\n";
	assembled<<"\tmov rsi, nnbob_len\n";
	assembled<<"\tcall print\n\n";
}
void cmdP(std::fstream &assembled){
	assembled<<"\t;+\n";
	assembled<<"\tmov rax, [accumulator]\n";
	assembled<<"\tinc rax\n";
	assembled<<"\tmov [accumulator], rax\n\n";
}
