int addLineNum(int num, char* buf){
	int curPos=0;
	while(num!=0){
		int n=num%10;
		for(int i=curPos;i>0;i--){
			buf[i]=buf[i-1];
		}
		buf[0]=n+'0';
		curPos++;
		num/=10;
	}
	buf[curPos++]=' ';
	buf[curPos++]='|';
	buf[curPos++]=' ';
	return curPos;
}
