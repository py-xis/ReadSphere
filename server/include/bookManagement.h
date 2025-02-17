#ifndef BOOKMANAGEMENT_H
#define BOOKMANAGEMENT_H

#include "components.h"

#define BOOK_ADDED 101
#define BOOK_NOT_ADDED -102
#define BOOK_NOT_DELETED -103
#define BOOK_DELETED 104
#define BOOK_MODIFIED 105
#define BOOK_NOT_MODIFIED -106
#define BOOK_BORROWED 107
#define BOOK_NOT_BORROWED -108
#define BOOK_NOT_EXIST -109
#define BOOK_NO_COPIES -110
#define USER_NOT_EXIST -111
#define BOOK_RETURNED 112
#define BOOK_NOT_RETURNED -113
#define BOOK_CANT_RETURN -114
#define BOOK_AVAILABLE 115
#define BOOK_NOT_AVAILABLE -116
#define BOOK_ERROR -117
#define BOOK_ALREADY_EXISTS 118

// Operation number : 3
int 
borrowBookServer(int userId, int ISBN);

// Operation number : 4
int
returnBookServer(int userId, int ISBN);

// Operation number : 5
int
queryBookServer(int ISBN);

// Operationg number : 6
int
addBookServer(char *bookName, char *author, int ISBN, int totalCopies, int availableCopies);

// Operation number : 7
int
deleteBookServer(int ISBN);

// Operation number : 8

int
modifyBookServer(char *bookName, char *author, int ISBN, int totalCopies, int availableCopies);

void
printBook(const struct Book *book);

#endif