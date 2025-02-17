#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>
#include "../include/bookManagementClient.h"
#include "../include/client.h"

char*
stringifyBook(int op,int ISBN){
    int length;
    int len = snprintf(NULL, 0, "%d %d",op, ISBN) + 1;
    len += 1;
    char *str = malloc(len * sizeof(char));

    if(str == NULL){
        perror("stringifyUser:malloc");
    }

    length = len;
    int status = snprintf(str, len, "%d %d",op,ISBN);

    if(status < 0){
        return STRINGIFY_ERROR;
    }
    str[len - 1] = '\0';
    return str;
}

char*
stringifyBookObj(int op, char *title, char *author, int ISBN, int totalCopies){

     if(strlen(title) > 100 || strlen(author) > 100 ){
        return STRINGIFY_ERROR;
    }
    int length;
    int len = snprintf(NULL, 0, "%d %s %s %d %d",op,title, author,ISBN, totalCopies) + 1;
    len += 1;
    char *str = malloc(len * sizeof(char));

    if(str == NULL){
        perror("stringifyUser:malloc");
    }
    length = len;
    int status = snprintf(str, len, "%d %s %s %d %d",op,title, author,ISBN, totalCopies);

    if(status < 0){
        return STRINGIFY_ERROR;
    }
    str[len - 1] = '\0';
    return str;
}

//op number : 3
char *borrowBookClient() {
    printf("Borrowing a book\n");

    int ISBN;

    // Get user input for book title
    printf("Enter the ISBN of the book you want to borrow: ");
    scanf("%d", &ISBN);

    // Construct and return a string representing the user and book information
    return stringifyBook(3,ISBN);
}

//op number : 4
char *returnBookClient() {
    printf("Returning a book\n");

    int ISBN;

    // Get user input for book title
    printf("Enter the ISBN of the book you want to return: ");
    scanf("%d", &ISBN);

    // Construct and return a string representing the user and book information
    return stringifyBook(4,ISBN);
}

//op number : 5
char *queryBookClient() {
    printf("Query if a book is available\n");

    int ISBN;

    // Get user input for book title
    printf("Enter the ISBN of the book you want to query: ");
    scanf("%d", &ISBN);

    // Construct and return a string representing the user and book information
    return stringifyBook(5,ISBN);
}

//op number 6
char *
addBookClient() {
    printf("Add a book\n");

    char title[100];
    char author[100];
    char description[100];
    int ISBN;
    int totalCopies;

    // Get user input for book details
    printf("Enter title of the book: \n");
    scanf("%s", title);
    printf("Enter author of the book: \n");
    scanf("%s", author);
    printf("Enter the ISBN of the book: \n");
    scanf("%d", &ISBN);
    printf("Enter the number of totalCopies: ");
    scanf("%d", &totalCopies);

    // Construct and return a string representing the book information
    return stringifyBookObj(6, title, author,ISBN, totalCopies);
    //                      0   1      2        3            4     5
}

//op number : 7
char *deleteBookClient() {
    printf("Delete a book\n");

    int ISBN;

    // Get user input for book title
    printf("Enter the ISBN of the book you want to delete: ");
    scanf("%d", &ISBN);

    // Construct and return a string representing the user and book information
    return stringifyBook(7,ISBN);
}

//op number : 8
char *modifyBookClient() {
    printf("Modify a book\n");

    char title[100];
    char author[100];
    int ISBN;
    int totalCopies;

    // Get user input for book details
    printf("Enter the modified title of the book: \n");
    scanf("%s", title);
    printf("Enter the modified author of the book: \n");
    scanf("%s", author);
    printf("Enter the modified ISBN of the book: \n");
    scanf("%d", &ISBN);
    printf("Enter the modified number of copies the book: \n");
    scanf("%d", &totalCopies);

    // Construct and return a string representing the book information
    return stringifyBookObj(8, title, author,ISBN, totalCopies);
}