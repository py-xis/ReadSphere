#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "components.h"

#define USER_NOT_ADDED -140
#define USER_ADDED 141
#define AUTHENTICATED_ADMIN 142
#define AUTHENTICATED_USER 146
#define NOT_AUTHENTICATED -143
#define ADMIN_NOT_GRANTED -144
#define ADMIN_GRANTED 145
#define USER_DOES_NOT_EXIST 147
#define USER_ALREADY_EXISTS 148


 // Operation number : 1
int 
addUserServer(char *firstName, char *middleName, char *lastName, char *username, char *password);

// Operation number : 2
int
userAuthenticateServer(char *username, char *password, int *userId, int *isAdmin);

// Operation number : 9
int 
giveAdminAccessServer(int userId);

// matcher Function
int 
match_username(void *rec, void *non_ndx_key);

// For debugging
void
printUser(const struct User *user);



#endif