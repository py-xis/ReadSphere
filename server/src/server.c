// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include<strings.h>
// #include<unistd.h>
// #include<fcntl.h>
// #include<sys/types.h>
// #include<sys/socket.h>
// #include<netinet/in.h>
// #include "../include/server.h"
// #include "../include/tokenizeString.h"
// #include "../include/pds.h"
// #include "../include/components.h"
// #include "../include/completeRequest.h"

// void addRootUser() {
//     struct User rootUser;
//     strcpy(rootUser.firstName, "root");
//     strcpy(rootUser.middleName, "root");
//     strcpy(rootUser.lastName, "root");
//     strcpy(rootUser.username, "root");
//     strcpy(rootUser.password, "root");
//     rootUser.isAdmin = 1;

//     if (put_rec_by_key(-1, &rootUser, USER) != PDS_SUCCESS) {
//         perror("Could not add root user");
//         return;
//     }
// }

// char *serverFunc(int incomingSock) {
//     int userId;
//     int isAdmin;
//     char buffer[100];
//     while (1) {
//         int connStatus = recv(incomingSock, buffer, 100, 0);

//         if (connStatus == 0) {
//             printf("Client disconnected\n");
//             close(incomingSock);
//             exit(EXIT_SUCCESS);
//         }

//         if (connStatus < 0) {
//             perror("recv");
//             close(incomingSock);
//             exit(EXIT_FAILURE);
//         }

//         printf("Server with %d received %s\n", getpid(), buffer);
//         int tokenCount;
//         char **tokens = splitString(buffer, &tokenCount);

//         int status = 0;
//         status = completeRequest(tokens, tokenCount, &userId, &isAdmin);
//         char retStatus[5];
//         snprintf(retStatus, sizeof(retStatus), "%d", status);
        
//         printf("Server %d got status as %d\n", status);
        
//         if (send(incomingSock, retStatus, strlen(retStatus), 0) < 0) {
//             perror("send:serverFunc");
//         }

//         // Free tokens
//         for (int i = 0; i < tokenCount; i++) {
//             free(tokens[i]);
//         }
//         free(tokens);
//     }
//     exit(EXIT_FAILURE);
// }

// int main() {
//     printf("Creating the repositories...\n");
//     // Create and initialise the repo
//     if (pds_create("user") != PDS_SUCCESS) {
//         perror("pds_create(user)");
//         exit(EXIT_FAILURE);
//     }

//     if (pds_create("book") != PDS_SUCCESS) {
//         perror("pds_create(book)");
//         exit(EXIT_FAILURE);
//     }

//     printf("Initialising the repositories...\n");
//     if (pds_open("user", sizeof(struct User), USER) != PDS_SUCCESS) {
//         perror("pds_open(user)");
//         exit(EXIT_FAILURE);
//     }

//     if (pds_open("book", sizeof(struct Book), BOOK) != PDS_SUCCESS) {
//         perror("pds_open(book)");
//         exit(EXIT_FAILURE);
//     }
//     addRootUser();
//     printf("Setting up sockets...\n");

//     // Creating sockets
//     int servSock;  // Socket of the server
//     servSock = socket(AF_INET, SOCK_STREAM, 0);

//     if (servSock == -1) {
//         perror("socket");
//         exit(EXIT_FAILURE);
//     }

//     // int opt = 1;
//     // if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
//     //     perror("setsockopt");
//     //     close(servSock);
//     //     exit(EXIT_FAILURE);
//     // }

//     // Binding the socket
//     struct sockaddr_in serverBind, newConn;
//     serverBind.sin_family = AF_INET;
//     serverBind.sin_addr.s_addr = INADDR_ANY;
//     serverBind.sin_port = htons(PORT);

//     if (bind(servSock, (struct sockaddr *)&serverBind, sizeof(serverBind)) == -1) {
//         perror("bind");
//         close(servSock);
//         exit(EXIT_FAILURE);
//     }

//     // Listening on the socket
//     if (listen(servSock, 5) == -1) {
//         perror("listen");
//         close(servSock);
//         exit(EXIT_FAILURE);
//     }

//     pid_t cpid;  // child pid() of the incoming connection.

//     while (1) {
//         // Accept a new connection
//         int newConnSize = sizeof(newConn);
//         int incomingSock = accept(servSock, (struct sockaddr *)&serverBind, (socklen_t *)&newConnSize);

//         if (incomingSock == -1) {
//             perror("accept");
//             continue;
//         }

//         cpid = fork();

//         if (cpid == 0) {
//             printf("Connection Accepted, fulfilling the connection..\n");
//             close(servSock);
//             serverFunc(incomingSock);
//         } else if (cpid == -1) {
//             perror("fork");
//             close(incomingSock);
//         } else {
//             close(incomingSock);
//         }
//     }

//     pds_close("user");
//     pds_close("book");
//     close(servSock);
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "../include/server.h"
#include "../include/tokenizeString.h"
#include "../include/pds.h"
#include "../include/components.h"
#include "../include/completeRequest.h"

#define PORT 8080
pthread_mutex_t lock;

void addRootUser() {
    struct User rootUser;
    strcpy(rootUser.firstName, "root");
    strcpy(rootUser.middleName, "root");
    strcpy(rootUser.lastName, "root");
    strcpy(rootUser.username, "root");
    strcpy(rootUser.password, "root");
    rootUser.isAdmin = 1;

    if (put_rec_by_key(-1, &rootUser, USER) != PDS_SUCCESS) {
        perror("Could not add root user");
        return;
    }
}

void *serverFunc(void *incomingSockPtr) {
    int incomingSock = *(int *)incomingSockPtr;
    free(incomingSockPtr);  // Free the allocated memory for the socket pointer

    int userId;
    int isAdmin;
    char buffer[100];
    while (1) {
        int connStatus = recv(incomingSock, buffer, sizeof(buffer), 0);

        if (connStatus == 0) {
            printf("Client disconnected\n");
            close(incomingSock);
            pthread_exit(NULL);
        }

        if (connStatus < 0) {
            perror("recv");
            close(incomingSock);
            pthread_exit(NULL);
        }

        printf("Server with %d received %s\n", getpid(), buffer);
        int tokenCount;
        char **tokens = splitString(buffer, &tokenCount);

        int status = 0;
        status = completeRequest(tokens, tokenCount, &userId, &isAdmin);
        char retStatus[5];
        snprintf(retStatus, sizeof(retStatus), "%d", status);

        printf("Server %d got status as %d\n", status);

        if (send(incomingSock, retStatus, strlen(retStatus), 0) < 0) {
            perror("send:serverFunc");
        }

        // Free tokens
        for (int i = 0; i < tokenCount; i++) {
            free(tokens[i]);
        }
        free(tokens);
    }
    close(incomingSock);
    pthread_exit(NULL);
}

int main() {
    

    printf("Creating the repositories...\n");
    // Create and initialise the repo
    if (pds_create("user") != PDS_SUCCESS) {
        perror("pds_create(user)");
        exit(EXIT_FAILURE);
    }

    if (pds_create("book") != PDS_SUCCESS) {
        perror("pds_create(book)");
        exit(EXIT_FAILURE);
    }

    printf("Initialising the repositories...\n");
    if (pds_open("user", sizeof(struct User), USER) != PDS_SUCCESS) {
        perror("pds_open(user)");
        exit(EXIT_FAILURE);
    }

    if (pds_open("book", sizeof(struct Book), BOOK) != PDS_SUCCESS) {
        perror("pds_open(book)");
        exit(EXIT_FAILURE);
    }
    addRootUser();
    printf("Setting up sockets...\n");

    // Creating sockets
    int servSock;  // Socket of the server
    servSock = socket(AF_INET, SOCK_STREAM, 0);

    if (servSock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Binding the socket
    struct sockaddr_in serverBind, newConn;
    serverBind.sin_family = AF_INET;
    serverBind.sin_addr.s_addr = INADDR_ANY;
    serverBind.sin_port = htons(PORT);

    if (bind(servSock, (struct sockaddr *)&serverBind, sizeof(serverBind)) == -1) {
        perror("bind");
        close(servSock);
        exit(EXIT_FAILURE);
    }

    // Listening on the socket
    if (listen(servSock, 5) == -1) {
        perror("listen");
        close(servSock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Accept a new connection
        socklen_t newConnSize = sizeof(newConn);
        int *incomingSockPtr = malloc(sizeof(int));  // Allocate memory for the socket
        *incomingSockPtr = accept(servSock, (struct sockaddr *)&serverBind, &newConnSize);

        if (*incomingSockPtr == -1) {
            perror("accept");
            free(incomingSockPtr);  // Free the allocated memory on error
            continue;
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, serverFunc, incomingSockPtr) != 0) {
            perror("pthread_create");
            close(*incomingSockPtr);
            free(incomingSockPtr);
        }

        // Optionally, join the thread if you need to wait for it to complete.
        // pthread_join(thread, NULL);
    }

    pds_close("user");
    pds_close("book");
    close(servSock);
    return 0;
}
