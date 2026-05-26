#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
struct packet{
	char data[100];
	int sl;
};


void checksum(char data[],int sum[],int sl){
	int dl=strlen(data);
	
	for(int i=dl;i>0;i=i-sl){
		int pos=i-1;
		int c=0;
		for(int j=0;j<sl;j++){
			int temp=sum[j]+(data[pos-j]-'0')+c;
			sum[j]=temp%2;
			c=temp/2;
		}
		if(c==1){
			for(int j=0;j<sl;j++){
				int temp=sum[j]+c;
				sum[j]=temp%2;
				c=temp/2;
				if(c==0) break;
			}
		}
	}
	
}
void onecomplement(int sum[],int sl){
	for(int i=0;i<sl;i++){
		if(sum[i]==1) sum[i]=0;
		else if (sum[i]==0) sum[i]=1;
	}
}

void manipulate_data(char data[],int pos){
	if(data[pos]=='0') data[pos]='1';
	else if(data[pos]=='1') data[pos]='0';
}


int main(void){
	int sd,cd,cadl;
	struct sockaddr_in sad,cad;
	sd=socket(AF_INET,SOCK_STREAM,0);
	sad.sin_family=AF_INET;
	sad.sin_port=htons(9999);
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(sd,(struct sockaddr *)&sad,sizeof(sad));
	listen(sd,1);
	cadl=sizeof(cad);
	cd=accept(sd,(struct sockaddr*)&cad,&cadl);
	struct packet p;
	recv(cd,&p,sizeof(p),0);
	/// to manipulate the data to check if code works
	int choice;
	printf("Do you want to manipulate the data? 1 for YES and 0 for NO:  ");
	scanf("%d",&choice);
	if(choice==1){
		int pos;
		printf("Enter position: ");
		scanf("%d",&pos);
		manipulate_data(p.data,pos);
	}
	printf("Data received: %s \n",p.data);
	int sum[100]={0};
	checksum(p.data,sum,p.sl);
	onecomplement(sum,p.sl);
	
	for(int i=0;i<p.sl;i++){
		if(sum[i]!=0){
			
			printf("Wrong data received");
			return 0;
		}
	}
	
	printf("Correct data received");
	close(cd);
    close(sd);
    return 0;
}
