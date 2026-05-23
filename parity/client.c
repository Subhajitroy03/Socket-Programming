// The role of client is to send data with parity
#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>

struct packet{
	char data[100];
	int parity;
};

int count_one(char s[]){
	int len=strlen(s);
	int count=0;
	for(int i=0;i<len;i++){
		if(s[i]=='1'){
			count++;
		}
	}
	return count;
}
void encode(char data[],int parity){ // 0-> even and 1-> odd
	int paritybit=0;
	if(parity==1 && count_one(data)%2==0){  // odd parity but even no of 1 
		paritybit=1;
	}else if(parity==0 && count_one(data)%2!=0){ // even parity but odd no of 1
		paritybit=1;
	}
	int len=strlen(data);
	data[len]=paritybit+'0';   // add the parity bit at the end of data and put a \0
	data[len+1]='\0';
}
int main(void){
	int sd;
    struct sockaddr_in sad;
    struct packet pkt;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sd, (struct sockaddr *)&sad, sizeof(sad));
	printf("Enter the data: ");
	scanf("%[^\n]", pkt.data);
	printf("Enter the parity( 0 for EVEN and 1 for ODD): ");
	scanf("%d",&pkt.parity);
	encode(pkt.data,pkt.parity);
	send(sd,&pkt,sizeof(pkt), 0);
   
    close(sd);
    return 0;
	
}
