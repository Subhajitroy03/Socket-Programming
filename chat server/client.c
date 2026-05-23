#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sd;
    struct sockaddr_in sad;
    char buffer[100];
    int n;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sd, (struct sockaddr *)&sad, sizeof(sad));

    while (1) {
        // Send message
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Client ended chat.\n");
            break;
        }

        // Receive reply
        n = recv(sd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) break;

        buffer[n] = '\0';
        printf("Server: %s", buffer);

        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Server ended chat.\n");
            break;
        }
    }

    close(sd);
    return 0;
}

