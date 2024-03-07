#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "scenarioLib.h"

//Print function
void printLineNum(int lineNum){
	char revPosInfo[100];
    int idx=0;

    if(lineNum==0){
        revPosInfo[idx++]='0';
    }else{
        while(lineNum>0){
            revPosInfo[idx++]=(char)(lineNum%10)+'0';
            lineNum/=10;
        }    
    }

	char posInfo[100];
    for(int i=0;i<idx;i++){
        posInfo[i]=revPosInfo[idx-i-1];
    }

	posInfo[idx]=' ';

    write(STDOUT_FILENO, posInfo, idx+1);
}

void printLineNumAndIndex(int lineNum, int wordIdx){
	char revPosInfo[100];
    int idx=0;

    if(wordIdx==0){
        revPosInfo[idx++]='0';
    }else{
        while(wordIdx>0){
            revPosInfo[idx++]=(char)(wordIdx%10)+'0';
            wordIdx/=10;
        }
    }
    revPosInfo[idx++]=':';
    if(lineNum==0){
        revPosInfo[idx++]='0';
    }else{
        while(lineNum>0){
            revPosInfo[idx++]=(char)(lineNum%10)+'0';
            lineNum/=10;
        }    
    }

    char posInfo[100];
    for(int i=0;i<idx;i++){
        posInfo[i]=revPosInfo[idx-i-1];
    }
	posInfo[idx]=' ';

    write(STDOUT_FILENO, posInfo, idx+1);
}

//Find answer
bool isSameChar(char a, char b){
	if(a>='a'&&a<='z'){
        if(b==a||b==a-('a'-'A')){
            return TRUE;
        }
    }else if(a>='A'&&a<='Z'){
        if(b==a||b==a+('a'-'A')){
            return TRUE;
        }
    }else{
        if(b==a){
            return TRUE;
        }
    }

    return FALSE;
}

bool isWord(char* a, int len){
	if(a[len]==' '||a[len]=='\0'||a[len]=='\n'){
		return TRUE;
	}
	return FALSE;
}

bool isSameWord(char* a, char* b, int len){
	if(!isWord(a, len)){
		return FALSE;
	}

	for(int i=0;i<len;i++){
        if(!isSameChar(a[i], b[i])){
            return FALSE;
        }
    }

	return TRUE;
}

int findWordInLine(char *line, int lineLen, char* word, int wordLen){
	int idx=0;
	if(isSameWord(line, word, wordLen)==TRUE){
		return idx;
	}

	for(idx=1;idx<lineLen-wordLen;idx++){
		if(line[idx]==' '){
			if(isSameWord(line+idx+1, word, wordLen)==TRUE){
				return idx+1;
			}
		}
	}

	return NON_EXIST;
}

int findWordInLineRev(char *line, int lineLen, char* word, int wordLen){
	int idx=0;

	for(idx=lineLen-wordLen;idx>0;idx--){
		if(line[idx]==' '){
			if(isSameWord(line+idx+1, word, wordLen)==TRUE){
				return idx+1;
			}
		}
	}

	if(isSameWord(line, word, wordLen)==TRUE){
		return idx;
	}

	return NON_EXIST;
}


int findWordsInLine(char *line, int lineLen, char* word, int wordLen, int* wordIdx){
	int idx=0;
	int wordNum=0;

	if(isSameWord(line, word, wordLen)==TRUE){
		wordIdx[wordNum++]=idx;
	}

	for(idx=1;idx<lineLen-wordLen;idx++){
		if(line[idx]==' '){
			if(isSameWord(line+idx+1, word, wordLen)==TRUE){
				wordIdx[wordNum++]=idx+1;
			}
		}
	}
	
	if(wordNum==0){
		return 0;
	}else{
		return wordNum;
	}
}

//Type
void singleWord(char* filename, char* cmd, int cmdLen){
	int fd=open(filename, O_RDONLY, 0777);

	char line[100];
	int lineIdx=0;
	int lineNum=0;
	char c;
	int* wordIdx=(int*)malloc(sizeof(int)*100);

	while(read(fd, &c, 1)!=0){
		line[lineIdx++]=c;
		if(c=='\n'){
			lineNum++;
			line[lineIdx]='\0';
			
			int wordNum=findWordsInLine(line, lineIdx, cmd, cmdLen, wordIdx);
			for(int i=0;i<wordNum;i++){
				printLineNumAndIndex(lineNum, wordIdx[i]);
			}
			lineIdx=0;
		}
	}

	lineNum++;
	line[lineIdx]='\0';
	int wordNum=findWordsInLine(line, lineIdx, cmd, cmdLen, wordIdx);
	for(int i=0;i<wordNum;i++){
		printLineNumAndIndex(lineNum, wordIdx[i]);
	}

	write(STDOUT_FILENO, "\n", 1);
	free(wordIdx);
	close(fd);
}

void severalWords(char* filename, char* cmd, int cmdLen){
	int fd=open(filename, O_RDONLY, 0777);

	char words[100][100];
	int wordsLen[100];
	int wordsNum=0;
	int idx=0;

	for(int i=0;i<=cmdLen;i++){
		if(cmd[i]==' '){
			words[wordsNum][idx]='\0';
			wordsLen[wordsNum]=idx;
			idx=0;
			wordsNum++;
		}else if(cmd[i]=='\0'){
			words[wordsNum][idx]='\0';
			wordsLen[wordsNum]=idx;
			wordsNum++;
		}else{
			words[wordsNum][idx++]=cmd[i];
		}
	}
	
	char line[100];
	int lineIdx=0;
	int lineNum=0;
	char c;

	while(read(fd, &c, 1)!=0){
		line[lineIdx++]=c;
		if(c=='\n'){
			lineNum++;
			line[lineIdx]='\0';
			bool flag=TRUE;
			for(int i=0;i<wordsNum;i++){
				if(findWordInLine(line, lineIdx, words[i], wordsLen[i])==NON_EXIST){
					flag=FALSE;
					break;
				}
			}

			if(flag==TRUE){
				printLineNum(lineNum);
			}
			lineIdx=0;
		}
	}

	lineNum++;
	line[lineIdx]='\0';
	bool flag=TRUE;
	for(int i=0;i<wordsNum;i++){
		if(findWordInLine(line, lineIdx, words[i], wordsLen[i])==NON_EXIST){
			flag=FALSE;
			break;
		}
	}

	if(flag==TRUE){
		printLineNum(lineNum);
	}
	
	write(STDOUT_FILENO, "\n", 1);
	close(fd);
}

void consecutiveWords(char* filename, char* cmd, int cmdLen){
	int fd=open(filename, O_RDONLY, 0777);

	char word[100];
	int wordLen=cmdLen-2;
	for(int i=0;i<wordLen;i++){
		word[i]=cmd[i+1];
	}
	word[wordLen]='\0';
	
	char line[100];
	int lineIdx=0;
	int lineNum=0;
	char c;
	int* wordIdx=(int*)malloc(sizeof(int)*100);

	while(read(fd, &c, 1)!=0){
		line[lineIdx++]=c;
		if(c=='\n'){
			lineNum++;
			line[lineIdx]='\0';
			int wordNum=findWordsInLine(line, lineIdx, word, wordLen, wordIdx);
			for(int i=0;i<wordNum;i++){
				printLineNumAndIndex(lineNum, wordIdx[i]);
			}
			lineIdx=0;
		}
	}

	lineNum++;
	line[lineIdx]='\0';
	int wordNum=findWordsInLine(line, lineIdx, word, wordLen, wordIdx);
	for(int i=0;i<wordNum;i++){
		printLineNumAndIndex(lineNum, wordIdx[i]);
	}
	
	write(STDOUT_FILENO, "\n", 1);
	free(wordIdx);
	close(fd);
}

void regexWords(char* filename, char* cmd, int cmdLen){
	int fd=open(filename, O_RDONLY, 0777);

	char word1[100];
	char word2[100];

	int word1Len;
	int word2Len;
	for(word1Len=0;word1Len<cmdLen;word1Len++){
		if(cmd[word1Len]=='*'){
			word1[word1Len]='\0';
			break;
		}
		word1[word1Len]=cmd[word1Len];
	}

	for(word2Len=0;word2Len<cmdLen-word1Len-1;word2Len++){
		word2[word2Len]=cmd[word1Len+word2Len+1];
	}
	word2[word2Len]='\0';
	
	char line[100];
	int lineIdx=0;
	int lineNum=0;
	char c;

	while(read(fd, &c, 1)!=0){
		line[lineIdx++]=c;
		if(c=='\n'){
			lineNum++;
			line[lineIdx]='\0';
			int word1Idx=findWordInLine(line, lineIdx, word1, word1Len);
			int word2Idx=findWordInLineRev(line, lineIdx, word2, word2Len);

			if((word1Idx!=NON_EXIST)&&(word2Idx!=NON_EXIST)&&((word1Idx+word1Len+1)!=word2Idx)&&(word1Idx<word2Idx)){
				printLineNum(lineNum);
			}
			lineIdx=0;
		}
	}

	lineNum++;
	line[lineIdx]='\0';
	int word1Idx=findWordInLine(line, lineIdx, word1, word1Len);
	int word2Idx=findWordInLine(line, lineIdx, word2, word2Len);

	if((word1Idx!=NON_EXIST)&&(word2Idx!=NON_EXIST)&&((word1Idx+word1Len+1)!=word2Idx)&&(word1Idx<word2Idx)){
		printLineNum(lineNum);
	}

	write(STDOUT_FILENO, "\n", 1);
	close(fd);
}

//Execute
void execute(char* filename, char* cmd, int cmdLen){
	if(cmd[0]=='"'){
		consecutiveWords(filename, cmd, cmdLen);
		return;
	}

	for(int i=0;i<cmdLen;i++){
		if(cmd[i]=='*'){
			regexWords(filename, cmd, cmdLen);
			return;
		}else if(cmd[i]==' '){
			severalWords(filename, cmd, cmdLen);
			return;
		}
	}

	singleWord(filename, cmd, cmdLen);
	return;
}
