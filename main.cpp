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

	//asm header
	tmp<<"section .text\n";
	tmp<<"global _start\n";
	tmp<<"_start:\n";

	while(getline(file,line)){
		for(int x=0;x<line.size();x++){
			if(line[x]=='h'){
				tmp<<"call H\n";
			}
			else if(line[x]=='9'){
				tmp<<"call nine\n";
			}
			else{
				tmp<<"inc rax\n";
			}
		}
	}
	//wyjście z programu
	tmp<<"mov rax, 60\n";
	tmp<<"mov rdi, 0\n";
	tmp<<"syscall\n";

	tmp<<"H:\n";
	tmp<<"mov rax, 1\n";
	tmp<<"mov rdi, 1\n";
	tmp<<"mov rsi, hello\n";
	tmp<<"mov rdx, helloL\n";
	tmp<<"syscall\n";
	tmp<<"ret\n";

	tmp<<"nine:\n";
	tmp<<"mov edx, str_len\n";
    tmp<<"mov ecx, str\n";
    tmp<<"mov ebx, 1\n";
    tmp<<"mov eax, 4\n";
    tmp<<"int 0x80\n";
	tmp<<"ret\n";

	//dane
	tmp<<"segment .data\n";
	tmp<<"hello db \"Hello World!\",0x0a,0x00\n";
	tmp<<"helloL equ $-hello\n";
	tmp<<"str:\n";
	tmp<<"%assign bottles 99\n";
	tmp<<"%rep 99\n";
	tmp<<"%defstr bottles_str bottles\n";
	tmp<<"%if bottles == 1\n";
	tmp<<"%define bottle_plur \" bottle\"\n";
	tmp<<"%else\n";
	tmp<<"%define bottle_plur \" bottles\"\n";
	tmp<<"%endif\n";
	tmp<<"db bottles_str, bottle_plur, \" of beer on the wall\", 10\n";
	tmp<<"db bottles_str, bottle_plur, \" of beer\", 10\n";
	tmp<<"db \"Take one down, pass it around\", 10, 10\n";
	tmp<<"%assign bottles bottles-1\n";
	tmp<<"%endrep\n";
	tmp<<"db \"0 bottles of beer on the wall\", 10\n";
	tmp<<"str_len: equ $ - str\n";

	file.close();
	tmp.close();

	return 0;
}
