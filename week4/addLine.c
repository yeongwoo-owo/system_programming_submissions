#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv){
	char c;
	char buf[1000];
	printf("%s", argv[1]);
	int inputFile=open(argv[1], O_RDONLY);
	int outputFile=open("Aladdin_num.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
	
	int lineNum=0;
	int lineLength=addLineNum(++lineNum, buf);

	while(read(inputFile, &c, 1)!=0){
		if(c=='\n'){
			buf[lineLength++]=c;
			write(outputFile, buf, lineLength);
			lineLength=addLineNum(++lineNum, buf);
		}
		else{
			buf[lineLength++]=c;
		}
	}
	
	close(inputFile);
	close(outputFile);
	return 0;
}
