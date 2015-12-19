#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "input_output.hpp"

bool getLine(FILE* file, char * line){
	char temp;
	int i=0;
	while((temp=getc(file))=='\n');
	ungetc(temp,file);
	while(1 == fread(&temp,sizeof(char),1,file)){
		/*
			if(i>=lineSize){
					lineSize+=line_Initial_SIZE;
    				line = (char *) realloc(line,lineSize*sizeof(char));
			}
		*/
			if((temp!='\n')&& (temp!=EOF) && i<=1000){
				line[i]=temp;
				i++;
			}
			else{
				line[i]=0;
				return true;
			}
	}
	return false;
}