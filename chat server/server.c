#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sd, cd;
    struct sockaddr_in sad;
    char buffer[100];
    int n;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    sad.sin_family = AF_INET;
    sad.sin_port = htons(9995);
    sad.sin_addr.s_addr = INADDR_ANY;

    bind(sd, (struct sockaddr *)&sad, sizeof(sad));
    listen(sd, 1);

    printf("Server waiting...\n");
    cd = accept(sd, NULL, NULL);

    while (1) {
        // Receive from client
        n = recv(cd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) break;

        buffer[n] = '\0';
        printf("Client: %s", buffer);

        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Client ended chat.\n");
            break;
        }

        // Send reply
        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin);

        send(cd, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Server ended chat.\n");
            break;
        }
    }

    close(cd);
    close(sd);
    return 0;
}

