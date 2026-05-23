// The role of client is to check the code received
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
int decode(char data[],int parity){
	if(parity==1 && count_one(data)%2==0){  // odd parity but even no of 1 
		return 0;
	}else if(parity==0 && count_one(data)%2!=0){ // even parity but odd no of 1
		return 0;
	}
	return 1;
}
void manipulate_data(char data[],int pos){
	if(pos>=strlen(data)) return;
	if(data[pos]=='0') data[pos]='1';
	if(data[pos]=='1') data[pos]='0';
}
int main(){
	int sd,cd,cadl;
	struct sockaddr_in sad,cad;
	struct packet pkt;
	sd=socket(AF_INET,SOCK_STREAM,0);
	sad.sin_family=AF_INET;
	sad.sin_port=htons(9995);
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(sd,(struct sockaddr *)&sad,sizeof(sad));
	listen(sd,1);
	cadl=sizeof(cad);
	cd=accept(sd,(struct sockaddr*)&cad,&cadl);
	recv(cd,&pkt,sizeof(pkt),0);
	/// to manipulate the data to check if code works
	int choice;
	printf("Do you want to manipulate the data? 1 for YES and 0 for NO:  ");
	scanf("%d",&choice);
	if(choice==1){
		int pos;
		printf("Enter position: ");
		scanf("%d",&pos);
		manipulate_data(pkt.data,pos);
	}
	int result=decode(pkt.data,pkt.parity);
	if(result==0){
		printf("Wrong data received\n");
	}else{
		printf("Correct data received\n");
	}
	printf(" Data received : %s",pkt.data);
	close(cd);
	close(sd);
}

