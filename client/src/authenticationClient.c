#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>
#include "../include/authenticationClient.h"
#include "../include/client.h"

// 1 means signup operation to the server
// 2 means login operation to the server
// 9 means giveadmin access to the user

char *
stringifyUser(char *firstName, char *middleName, char *lastName, char *username, char *password){
    int length;
    int len = snprintf(NULL, 0, "1 %s %s %s %s %s",firstName, middleName, lastName, username, password) + 1;
    len += 1;
    char *str = malloc(len * sizeof(char));

    if(str == NULL){
        perror("stringifyUser:malloc");
    }
    length = len;
    int status = snprintf(str, len, "1 %s %s %s %s %s",firstName, middleName, lastName,username, password);
    if(status < 0){
        return STRINGIFY_ERROR;
    }
    str[len - 1] = '\0';
    return str;
}

char*
stringifyUserPass(char *username, char *password){
    int length;
    int len = snprintf(NULL, 0, "2 %d %s %s",username, password) + 1;
    len += 1;
    char *str = malloc(len * sizeof(char));

    if(str == NULL){
        perror("stringifyUser:malloc");
    }
    length = len;
    int status = snprintf(str, len, "2 %s %s",username, password);

    if(status < 0){
        return STRINGIFY_ERROR;
    }
    str[len - 1] = '\0';
    return str;
}


char *
signupClient(){
    
    char firstName[10];
    char middleName[10];
    char lastName[10];
    
    char username[10];
    char password[10];


    // Get user input for firstName
    printf("Enter first name: ");
    scanf("%s", firstName);

    // Get user input for middleName
    printf("Enter middle name: ");
    scanf("%s", middleName);

    // Get user input for lastName
    printf("Enter last name: ");
    scanf("%s", lastName);

    // Get user input for username
    printf("Enter username: ");
    scanf("%s", username);

    // Get user input for password
    printf("Enter password: ");
    scanf("%s", password);

    return stringifyUser(firstName, middleName, lastName, username, password);
}

char *
loginClient(){

    char username[10];
    char password[10];

    // Get user input for username
    printf("Enter username: ");
    scanf("%s", username);

    // Get user input for password
    printf("Enter password: ");
    scanf("%s", password);

    return stringifyUserPass(username, password);
}

char *
giveAdminAccessClient(){
    printf("Enter the userId of the user you want to give admin access\n");
    int userId;
    scanf("%d", &userId);
    int length;
    int len = snprintf(NULL, 0, "9 %d",userId);
    len += 1;
    char *str = malloc(len * sizeof(char));

    if(str == NULL){
        perror("giveAdminAccessClient");
    }
    length = len;
    int status = snprintf(str, len, "9 %d",userId);

    if(status < 0){
        return STRINGIFY_ERROR;
    }
    str[len - 1] = '\0';
    printf("%s\n", str);
    return str;
}