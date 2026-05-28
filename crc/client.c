#include<stdio.h>
#include<string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct packet{
	char data[100];
	char poly[100];
};

void padding(char data[],char poly[]){
	int len=strlen(data);
	int zero=strlen(poly)-1;
	for(int i=0;i<zero;i++){
		data[len+i]='0';
	}
	data[len+zero]='\0';
}

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

void append(char data[],char poly[],char rem[]){    // add the rem to end of data replacing zero
	int dl=strlen(data);
	int pl=strlen(poly);
	int rem_len=pl-1;
    int start_index=dl-rem_len;
    for(int i = 0; i < rem_len; i++){
        data[start_index + i]=rem[i];
    }
}

int main(void){
	//initialise the packet 
	char temp[100];
	char rem[100];
	struct packet p;
	
	//socket setup
	int sd;
    struct sockaddr_in sad;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sd, (struct sockaddr *)&sad, sizeof(sad));
    
    //user input
	printf("Enter Dataword: ");
    scanf("%s", p.data); 
    printf("Enter Generator Polynomial: ");
    scanf("%s", p.poly);
    
    //copy the data to temp
    strcpy(temp,p.data); 
    // add zero at the end   
    padding(temp,p.poly);
    
    //perform division 
    division(temp,p.poly,rem);
    
    // Add the '0's to p.data first
    padding(p.data, p.poly); 
    
    // change the zeros with rem in real data
    append(p.data,p.poly,rem);
    
    //print and send the data
    printf("\nRemainder: %s", rem);
    printf("\nCodeword:  %s\n", p.data);

    send(sd, &p, sizeof(p), 0);

    close(sd);
    return 0;
}
