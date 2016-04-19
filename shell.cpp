#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

using namespace std;

#define MAX 50

int check_command(char cmnd[]);
void make_new_dir(char pathname[]);
void change_dir(char pathname[]);
void remove_dir(char pathname[]);
void display_dir();
void separate(char *statment, char **words);
void execute(char **words);

char txt[MAX];

int main(){
	string expression;
	char cmd1[MAX];
	char *words[MAX];
	char cmd[MAX];
	int len,pos,i;
	cout << "\n\nPress q to quit:\n\n";
	while(1){
		cout << "shubham~/singh$ ";
		getline(cin,expression);
		if(expression.size()==1 && expression[0]=='q'){
			cout << "\n";
			break;
		}
		strcpy(cmd1,expression.c_str());
		separate(cmd1,words);

		len = expression.size();
		pos = expression.find(' ');
		if(pos>=0){
			strcpy(cmd,expression.substr(0,pos).c_str());

			strcpy(cmd,expression.substr(0,pos).c_str());
			strcpy(txt,expression.substr(pos+1,len).c_str());
		}else{
			strcpy(cmd,expression.substr(0,len).c_str());
			strcpy(txt,"");
		}
		if(check_command(cmd)){
			cout << ":check:";
		}else{
			execute(words);
		}
	}

}

int check_command(char cmnd[]){
	if(strcmp(cmnd,"mkdir")==0){
		make_new_dir(txt);
	}else if(strcmp(cmnd,"rmdir")==0){
		remove_dir(txt);
	}else if(strcmp(cmnd,"cd")==0){
		change_dir(txt);
	}else if(strcmp(cmnd,"pwd")==0){
		display_dir();
	}else{
		return 0;
		//cout << cmnd << ": command not found\n";
	}
	return 1;
}

void make_new_dir(char pathname[]){
	//cout << "path:" << pathname << ":";
	if(strlen(pathname)==0){
		cout << "mkdir: missing operand\n";
		return;
	}
	int num;
	num = mkdir(pathname,0777);
	if(num==-1){
		cout << "mkdir: cannot create directory '"<< pathname << "': File exists\n";
	}
	return;
}

void change_dir(char pathname[]){
	if(strlen(pathname)==0){
		cout << "chdir: missing operand\n";
		return;
	}
	int num;
	//cout << "&" << pathname << "&";
	num = chdir(pathname);
	if(num==-1){
		cout << "chdir: cannot change directory '"<< pathname << "': No such file or directory\n";
	}
	return;
}

void remove_dir(char pathname[]){
	if(strlen(pathname)==0){
		cout << "rmdir: missing operand\n";
		return;
	}
	int num;
	num = rmdir(pathname);
	if(num==-1){
		cout << "rmdir: failed to remove '"<< pathname << "': No such file or directory\n";
	}
	return;
}

void display_dir(){
	char path[20];
	getcwd(path,20);
	cout << path << "\n";
	if(strcmp(path,"")==0){
		cout << "pwd: failed to display content of current directory\n";
	}
	return;
}



void separate(char *statment, char **words){
//	cout << statment;
	//cout << "hello";
	while(*statment != '\0'){
		while(*statment == ' ' || *statment=='\n')
			*statment++ = '\0';
		*words++ = statment;
		while(*statment != ' ' && *statment != '\0' && *statment != '\n')
			statment++;
	}
	*words = '\0';
	return;
}

void execute(char **words){
	pid_t pid;
	int status;
	if((pid=fork())<0){
		cout << "fork failed.\n";
	}else if(pid==0){
		if(execvp(*words,words)<0){
			cout << "execvp failed\n";
			exit(1);
		}
		cout << "\n";
	}else{
		while(wait(&status) != pid);
	}
	return;
}