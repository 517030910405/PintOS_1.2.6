//show==0 show
//show==1 ****
//show==2 nothing
#ifndef MYIO
#define MYIO
int deal_with_input(char * str, int *pos, char c,int show,int len){
	if (false){
	
	} else{
		if (show==0) printf("%c",c); else
		if (show==1) printf("*");
		str[*pos] = c;
		++(*pos);
	}
	if (*pos>=len) {
		printf("Protection Fault\n");
		return 1;
	}
	return 0;
}
int get_string(char * str,char stopsign,int show,int len){
	char c;
	c = input_getc();
	int pos = 0;
	while (c!=13){
		//printf("\n%d\n",(int)c);
		if (deal_with_input(str,&pos,c,show,len)==1) return 1;
		c = input_getc();
	}
	str[pos]=0;
	printf("\n");
	//printf("aaa");
	return 0;
}
#endif
