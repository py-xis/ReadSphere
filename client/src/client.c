#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<time.h>
#include "../include/client.h"
#include "../include/authenticationClient.h"
#include "../include/bookManagementClient.h"
#include "../include/tokenizeString.h"

#define PORT 8080
int thisSock;

int destringify(const char *input, char *word1, char *word2, char *word3) {
    // Use sscanf to split the string into three parts
    if (sscanf(input, "%s %s %s", word1, word2, word3) == 3) {
        return STRINGIFY_SUCCESS_INT;
    } else {
        return STRINGIFY_ERROR_INT;
    }
}

int connectToServer(int *thisSock) {
    printf("Connecting to server\n");
    *thisSock = socket(AF_INET, SOCK_STREAM, 0);

    if (*thisSock == -1) {
        perror("socket:Client");
        return NOT_CONNECTED_TO_SERVER;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    if (connect(*thisSock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connectToServer");
        close(*thisSock);
        return NOT_CONNECTED_TO_SERVER;
    }
    printf("Connected to server\n");
    return CONNECTED_TO_SERVER;
}

void show_time() {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    // Get current time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format time as string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Print current time
    printf("Current date and time: %s\n", buffer);
}

void handleServerReturnAdmin(int serverReturn) {
    switch (serverReturn) {
        case BOOK_ADDED:
            printf("Book added successfully...\n");
            break;
        case BOOK_NOT_ADDED:
            printf("Book could not be added...\n");
            break;
        case BOOK_DELETED:
            printf("Book deleted successfully...\n");
            break;
        case BOOK_NOT_DELETED:
            printf("Book could not be deleted...\n");
            break;
        case BOOK_MODIFIED:
            printf("Book modified successfully...\n");
            break;
        case BOOK_NOT_MODIFIED:
            printf("Book could not be modified\n");
            break;
        case ADMIN_GRANTED:
            printf("Admin access granted...\n");
            break;
        case ADMIN_NOT_GRANTED:
            printf("Admin access not granted...\n");
            break;
        default:
            printf("The server returned %d\n", serverReturn);
            break;
    }
}

void handleServerReturnUser(int serverReturn) {
    switch (serverReturn) {
        case BOOK_NOT_EXIST:
            printf("Requested book does not exist...\n");
            break;
        case BOOK_NO_COPIES:
            printf("There are no copies available for borrowing...\n");
            break;
        case USER_NOT_EXIST:
            printf("The specified user does not exist...\n");
            break;
        case BOOK_NOT_BORROWED:
            printf("Book could not be borrowed...\n");
            break;
        case BOOK_BORROWED:
            printf("Book borrowed successfully...\n");
            break;
        case BOOK_NOT_RETURNED:
            printf("Book could not be returned...\n");
            break;
        case BOOK_CANT_RETURN:
            printf("Invalid return operation...\n");
            break;
        case BOOK_RETURNED:
            printf("Book returned successfully...\n");
            break;
        case BOOK_ERROR:
            printf("Requested book was not found...\n");
            break;
        case BOOK_NOT_AVAILABLE:
            printf("There are zero copies available for the given book...\n");
            break;
        case BOOK_AVAILABLE:
            printf("The specified book is available for borrowing...\n");
            break;
        default:
            printf("The server returned %d\n", serverReturn);
            break;
    }
}

void handleServerReturnAuth(int serverReturn){
    switch (serverReturn){
        case USER_NOT_ADDED:
            printf("User could not be added...\n");
            break;
        case USER_ADDED:
            printf("User added, login with your credentials...\n");
            break;
        case AUTHENTICATED_ADMIN:
            printf("Logged in as admin...\n");
            adminOptions();
            break;
        case AUTHENTICATED_USER:
            printf("Logged in as user...\n");
            userOptions();
            break;
        case NOT_AUTHENTICATED:
            printf("Could not be authenticated...\n");
            break;
        case USER_DOES_NOT_EXIST:
            printf("No such record found...\n");
            break;
        case USER_ALREADY_EXISTS:
            printf("You are already signed up, please login with your credentials...\n");
            break;
        default:
            printf("Server Returned %d\n", serverReturn);
            break;
    }
    return ;
}

int completeRequestClient(const char *funcString) {
    printf("funcString sending is %s\n", funcString);
    if (send(thisSock, funcString, strlen(funcString) + 1, 0) < 0) {
        perror("completeRequestClient");
        return COULD_NOT_SEND;
    }
    printf("Request sent to server...\n");
    char buffer[4];
    printf("Waiting for the server to respond...\n");
    while (recv(thisSock, buffer, sizeof(buffer), 0) < 0);
    
    int serverReturn = atoi(buffer);
    return serverReturn;
}

void adminOptions() {

    int choice;
    char *funcString;
    int validChoiceEntered = 0;

    while (1) {
        printf("Administrator Options\n");
        printf("1. Add a book\n");
        printf("2. Modify a book\n");
        printf("3. Give admin access\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                funcString = addBookClient();
                validChoiceEntered = 1;
                break;
            case 2:
                funcString = modifyBookClient();
                validChoiceEntered = 1;
                break;
            case 3:
                funcString = giveAdminAccessClient();
                validChoiceEntered = 1;
                break;
            case 4:
                printf("Logging you out ...\n");
                close(thisSock);
                return;
            default:
                printf("Enter a valid choice\n");
                break;
        }

        if (validChoiceEntered) {
            int serverReturn = completeRequestClient(funcString);
            handleServerReturnAdmin(serverReturn);
        }
    }
}

void userOptions() {
    

    int choice;
    char *funcString;
    int validChoiceEntered = 0;

    while (1) {
        printf("User Options\n");
        printf("1. Borrow a book\n");
        printf("2. Return a book\n");
        printf("3. Query a book\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                funcString = borrowBookClient();
                validChoiceEntered = 1;
                break;
            case 2:
                funcString = returnBookClient();
                validChoiceEntered = 1;
                break;
            case 3:
                funcString = queryBookClient();
                validChoiceEntered = 1;
                break;
            case 4:
                printf("Logging you out ...\n");
                close(thisSock);
                return;
            default:
                printf("Enter a valid choice\n");
                break;
        }

        if (validChoiceEntered) {
            int serverReturn = completeRequestClient(funcString);
            handleServerReturnUser(serverReturn);
        }
    }
}

int main() {
    show_time();
    printf("Welcome to Online Library Management System\n");

    while (1) {
        int entryChoice;
        int connectedToServer = 0;
        char *funcString = NULL;
        int validChoiceEntered = 0;
        int signedUp = 0;

        printf("1. Login\n");
        printf("2. Signup\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &entryChoice);

        switch (entryChoice) {
            case 1:
                funcString = loginClient();
                validChoiceEntered = 1;
                break;
            case 2:
                funcString = signupClient();
                validChoiceEntered = 1;
                signedUp = 1;
                break;
            case 3:
                exit(EXIT_SUCCESS);
                break;
            default:
                printf("Enter a valid choice\n");
                break;
        }

        if (!validChoiceEntered) {
            continue;
        }

        int connStatus = connectToServer(&thisSock);

        if (connStatus == NOT_CONNECTED_TO_SERVER) {
            perror("client:");
            exit(EXIT_FAILURE);
        }
        
        int serverReturn = completeRequestClient(funcString);
        handleServerReturnAuth(serverReturn);

    }
    return 0;
}
