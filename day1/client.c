#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sd;
    struct sockaddr_in sad;
    char buffer[50];

    sd = socket(AF_INET, SOCK_STREAM, 0);

    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sd, (struct sockaddr *)&sad, sizeof(sad));

    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);

    send(sd, buffer, strlen(buffer), 0);

    close(sd);
    return 0;
}

