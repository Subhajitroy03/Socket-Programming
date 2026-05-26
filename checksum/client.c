#include<stdio.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
struct packet{
	char data[100];
	int sl;
};

int check_segmentlength(int n){
	if(n==1) return 1;
	if(n==0) return 0;
	if(n%2!=0) return 0;
	return check_segmentlength(n/2);
}

void padding(char data[],int sl){
	int dl=strlen(data);
	if (dl%sl==0) return;
	int extra=sl-(dl%sl);
	char zero[100];
	for(int i=0;i<extra;i++){
		zero[i]='0';
	}
	zero[extra]='\0';
	strcat(zero,data);
	strcpy(data,zero);
}

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

void append(char data[],int sum[],int sl){
	int dl=strlen(data);
	int k=0;

	for(int i=sl-1;i>=0;i--){
		data[dl+k]=sum[i]+'0';
		k++;
	}

	data[dl+sl]='\0';
}

int main(void){
	int sd;
	struct sockaddr_in sad;
	sd=socket(AF_INET,SOCK_STREAM,0);
	sad.sin_family=AF_INET;
	sad.sin_port=htons(9999);
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	connect(sd, (struct sockaddr *)&sad, sizeof(sad));
	struct packet p;
	printf("Enter your data: ");
	scanf("%s",p.data);
	printf("Enter your segment length: ");
	scanf("%d",&p.sl);
	if(check_segmentlength(p.sl)==0){
		printf("Segment length must be in power of 2");
		close(sd);
		return -1;
	}
	
	padding(p.data,p.sl);
	int sum[100]={0};
	checksum(p.data,sum,p.sl);
	onecomplement(sum,p.sl);
	append(p.data,sum,p.sl);
	printf("Code Word: %s",p.data);
	
	send(sd,&p,sizeof(p),0);

    close(sd);
    return 0;
}
