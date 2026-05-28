#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct packet{
	char data[100];
	char poly[100];
};

void division(char data[],char poly[],char rem[]){
	int dl=strlen(data);
	int pl=strlen(poly);
	for(int i=0;i<dl-pl+1;i++){
		if(data[i]=='1'){
			for(int j=0;j<pl;j++){
				if(data[i+j]==poly[j]){
					data[i+j]='0';
				}else{
					data[i+j]='1';
				}
			}
		}
		
	}
	int rem_len=pl-1;
    int start_index=dl-rem_len;
    
    for(int i = 0; i < rem_len; i++){
        rem[i] = data[start_index + i];
    }
    rem[rem_len] = '\0';
}

int checkerror(char rem[]){
	int len=strlen(rem);
	for(int i=0;i<len;i++){
		if(rem[i]!='0') return 0;
	}
	return 1;
}
void manipulate_data(char data[],int pos){
	if(data[pos]=='0') data[pos]='1';
	else if(data[pos]=='1') data[pos]='0';
}


int main(void){
	// Socket initialise
	int sd,cd,cadl;
	struct sockaddr_in sad,cad;
	sd=socket(AF_INET,SOCK_STREAM,0);
	sad.sin_family=AF_INET;
	sad.sin_port=htons(9995);
	sad.sin_addr.s_addr=inet_addr("127.0.0.1");
	bind(sd,(struct sockaddr *)&sad,sizeof(sad));
	listen(sd,1);
	cadl=sizeof(cad);
	cd=accept(sd,(struct sockaddr*)&cad,&cadl);
	
	// packet declarations
	struct packet p;
	char rem[100];
	char temp[100];
	
	
	//rec the data
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
	
	// copy the manipulated data to temp
	strcpy(temp,p.data);
	printf("\nData:  %s\n", p.data);
	
	
	// perform div and get rem
    division(temp,p.poly,rem);
    printf("\nRem:  %s\n", rem);
    
    // check the error
    int result=checkerror(rem);
    
    if(result>0) printf("No error found");
    else printf("Error found");
	close(cd);
	close(sd);

}
