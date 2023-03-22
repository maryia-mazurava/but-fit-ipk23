#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_INPUT_SIZE 1024
#define MAX_OUTPUT_SIZE 1024

int sockfd;
//Interrupt signal handler function.
void sigint_handler(int sig) {

    printf("\nReceived terminating signal, exiting...\n");
    // Closing the connection.
    if (close(sockfd) < 0)
    {
        printf("Error while closing the connection.\n");
        exit(1);

    } else {
        printf("Connection closed.\n");
    }    
    exit(0);

}

// Function for connecting to the server using TCP mode.
void tcpMode(char *host_tcp, int port_tcp) {

    struct sockaddr_in serv_addr;
    struct hostent *server;
    char message_client[MAX_INPUT_SIZE], message_server[MAX_OUTPUT_SIZE];
    ssize_t n;

    // Opening a socket.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error while opening a socket.\n");
        exit(1);
    }

    server = gethostbyname(host_tcp);
    if (server == NULL) {
        printf("Error: no such host.\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port_tcp);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error while connecting.\n");
        exit(1);
    }

    // Communication with the server.
    while (fgets(message_client, MAX_INPUT_SIZE, stdin) != NULL) {

        n = write(sockfd, message_client, strlen(message_client));
        if (n < 0) {
            printf("Error while writing to socket.\n");
            break;
        }

        n = read(sockfd, message_server, MAX_OUTPUT_SIZE - 1);
        if (n < 0) {
            printf("Error while reading from socket.\n");
            break;
        }

        message_server[n] = '\0';
        printf("%s\n", message_server);

        // If "BYE" from server is received.
        if (strcmp(message_server, "BYE\n") == 0) {
            printf("Closing the connection...\n");
            if (close(sockfd) < 0)
            {
                printf("Error while closing the connection.\n");
                exit(1);

            } else {
                printf("Connection closed.\n");
            }    
            break;
        }
    }
    
}

// Function for connecting to the server using UDP mode.
void udpMode(char *host_udp, int port_udp) {

    int n;
    struct sockaddr_in serv_addr;
    char message_client[1000], message_server[1000];
    
    // Opening a socket.
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        printf("Error while opening a socket.\n");
        exit(1);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    serv_addr.sin_addr.s_addr = inet_addr(host_udp);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_udp);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error while connecting.\n");
        exit(1);
    }

    // Communication with the server.
    while (1) {
        n = 0;
        bzero(message_client, sizeof(message_client));
        bzero(message_server, sizeof(message_server));
        
        while ((message_client[n++] = getchar()) != '\n');
        message_server[0] = '\0';
        message_server[1] = (char)(n - 1);
        strncpy(&(message_server[2]), message_client, n - 1);
         
        if (write(sockfd, message_server, n + 1) < 0) {
            printf("Error while writing to socket.\n");
            break;
        }
        
        bzero(message_server, sizeof(message_server));
        if (read(sockfd, message_server, sizeof(message_server)) < 0) {
            printf("Error while reading from socket.\n");
            break;
        }
        
        printf("%s:", message_server[1] == 1 ? "ERR" : "OK"); 
    
        for(int j = 0; j < (int)(message_server[2]); j++) {
            printf("%c", message_server[3 + j]); 
        }
        printf("\n\n");
    }

}


int main(int argc, char *argv[]) {

    char *host, *mode;
    int port;
    int argument;

    signal(SIGINT, sigint_handler);


    //Start of parsing arguments
    if (argc == 7)
    {
        while ((argument = getopt(argc, argv, "p:h:m:")) != -1)
        {
            switch (argument)
            {
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                host = optarg;
                break;
            case 'm':
                mode = optarg;
                break;        
            default:
                printf("Error: wrong arguments.\n");
                exit(1);
            }
        }
    }else if (argc == 2){
        if ((strcmp(argv[1], "--help")) == 0){
            printf("Usage of the client: ./ipkcpc -h <host> -p <port> -m <mode>\n");
            printf("host  - the IPv4 address of the server.\nport  - the server port.\nmode  - tcp/udp.\n");
            return(0);
        }else{
            printf("Error: wrong arguments.\n");
            exit(1);
        }
    }else{
        printf("Error: wrong arguments.\n");
        exit(1);
    }

    // Choosing a mode.
    if (strcmp(mode, "tcp") == 0) {
        tcpMode(host, port);
    } else if (strcmp(mode, "udp") == 0) {
        udpMode(host, port);
    } else {
        printf("Error: wrong mode, use udp or tcp only.\n");
    }

    return 0;
}