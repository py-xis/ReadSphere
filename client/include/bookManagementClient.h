#ifndef BOOKMANAGEMENTCLIENT_H
#define BOOKMANAGEMENTCLIENT_H

#define MAX_ISBN_LENGTH 13
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

char*
stringifyBook(int op, int ISBN);

char*
stringifyBookObj(int op, char *title, char *author, int ISBN, int totalCopies);

char *
borrowBookClient();

char *
returnBookClient();

char *
queryBookClient();

char *
addBookClient();

char *
deleteBookClient();

char *
modifyBookClient();

#endif