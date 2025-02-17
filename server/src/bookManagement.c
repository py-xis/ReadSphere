#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include "../include/bookManagement.h"
#include "../include/pds.h"
#include "../include/components.h"

int bookId = 0;

void printBook(const struct Book *book) {
    printf("Book Name: %s\n", book->bookName);
    printf("Author: %s\n", book->author);
    printf("ISBN: %d\n", book->ISBN);
    printf("Total Copies: %d\n", book->totalCopies);
    printf("Available Copies: %d\n", book->availableCopies);

    // Print users who have borrowed the book
    printf("Borrowed By:\n");
    struct UserNode *current = book->head;
    while (current != NULL) {
        printf("  User ID: %d, Returned: %s\n", current->userId, current->returned ? "Yes" : "No");
        current = current->next;
    }
}


int
addBookServer(char *bookName, char *author, int ISBN, int totalCopies, int availableCopies){
    struct Book newBook;

    strcpy(newBook.bookName, bookName);
    strcpy(newBook.author, author);
    newBook.ISBN = ISBN;
    newBook.totalCopies = totalCopies;
    newBook.availableCopies = availableCopies;
    newBook.head = NULL;

    int putStatus = put_rec_by_key(ISBN, &newBook, BOOK);

    if(putStatus == PDS_DUPLICATE_KEY_ADD){
        return BOOK_ALREADY_EXISTS;
    }else if(putStatus == PDS_ADD_FAILED){
        return BOOK_NOT_ADDED;
    }else if(putStatus == PDS_REPO_NOT_OPEN){
        return REPO_ERROR;
    }

    return BOOK_ADDED;
} 

int
deleteBookServer(int ISBN){

    int putStatus = delete_rec_by_ndx_key(ISBN, BOOK);

    if(putStatus == PDS_REPO_NOT_OPEN){
        return REPO_ERROR;
    }else if(putStatus == PDS_REC_NOT_FOUND){
        return BOOK_NOT_EXIST;
    }else if(putStatus == PDS_DELETE_FAILED){
        return BOOK_NOT_DELETED;
    }

    return BOOK_DELETED;
}

int modifyBookServer(char *bookName, char *author, int ISBN, int totalCopies, int availableCopies){
    
    struct Book oldBook;

    struct Book newBook;

    strcpy(newBook.bookName, bookName);
    strcpy(newBook.author, author);
    newBook.ISBN = ISBN;
    newBook.totalCopies = totalCopies;
    newBook.availableCopies = availableCopies;
    newBook.head = oldBook.head;

    int status = modify_by_ndx_key(ISBN, &newBook, BOOK);

    if(status == PDS_REPO_NOT_OPEN){
        return REPO_ERROR;
    }else if(status == PDS_MODIFY_FAILED){
        return BOOK_NOT_MODIFIED;
    }

    return BOOK_MODIFIED;
}

int 
borrowBookServer(int userId, int ISBN){

    struct Book thisBook;
    if(get_rec_by_ndx_key(ISBN, &thisBook, BOOK) != PDS_SUCCESS){
        return BOOK_NOT_EXIST;
    }

    if(thisBook.availableCopies == 0){
        return BOOK_NO_COPIES;
    }

    thisBook.availableCopies -= 1;
    struct UserNode *newNode = malloc(sizeof(struct UserNode));

    newNode -> userId = userId;
    newNode -> returned = 0;
    newNode -> next = NULL;

    if(thisBook.head == NULL){
        thisBook.head = newNode;
    }

    newNode -> next = thisBook.head;
    thisBook.head = newNode;


    struct User thisUser;
    if(get_rec_by_ndx_key(userId, &thisUser, USER) != PDS_SUCCESS){
        return USER_NOT_EXIST;
    }

    struct ISBNNode *newNodee = malloc(sizeof(struct ISBNNode));
    newNodee -> isbn = ISBN;
    newNodee -> returned = 0;
    newNodee -> next = NULL;
    
    if(thisUser.head == NULL){
        thisUser.head = newNodee;
    }

    newNodee -> next = thisUser.head;
    thisUser.head = newNodee;

    if((modify_by_ndx_key(userId, &thisBook, BOOK) != PDS_MODIFIED) || (modify_by_ndx_key(ISBN, &thisUser, USER) != PDS_MODIFIED)){
        return BOOK_NOT_BORROWED;
    }

    return BOOK_BORROWED;
}

int
returnBookServer(int userId, int ISBN){

    struct User thisUser;
    struct Book thisBook;

    if(get_rec_by_ndx_key(userId, &thisUser, USER) != PDS_SUCCESS){
        return BOOK_NOT_RETURNED;
    }

    if(get_rec_by_ndx_key(ISBN, &thisBook, BOOK) != PDS_SUCCESS){
        return BOOK_NOT_RETURNED;
    }

    if(thisBook.availableCopies == thisBook.totalCopies){
        return BOOK_CANT_RETURN;
    }

    struct UserNode *ptr = thisBook.head;
    while(ptr != NULL){
        if(ptr -> userId == userId){
            ptr -> returned = 1;
            thisBook.availableCopies += 1;
            break;
        }
        ptr = ptr -> next;
    }

    if(ptr == NULL){
        return BOOK_CANT_RETURN;
    }

    struct ISBNNode *ptr2 = thisUser.head;
    while(ptr2 != NULL){
        if(ptr2 -> isbn == ISBN){
            ptr2 -> returned = 1;
            break;
        }
        ptr = ptr -> next;
    }

    if(ptr == NULL){
        return BOOK_CANT_RETURN;
    }

    if(modify_by_ndx_key(userId, &thisUser, USER) != PDS_SUCCESS){
        return BOOK_NOT_RETURNED;
    }

    if(modify_by_ndx_key(ISBN, &thisBook, BOOK) != PDS_SUCCESS){
        return BOOK_NOT_RETURNED;
    }

    return BOOK_RETURNED;
}

int
queryBookServer(int ISBN){

    struct Book thisBook;
    if(get_rec_by_ndx_key(ISBN, &thisBook, BOOK) != PDS_SUCCESS){
        return BOOK_ERROR;
    }

    if(thisBook.availableCopies == 0){
        return BOOK_NOT_AVAILABLE;
    }

    return BOOK_AVAILABLE;
}