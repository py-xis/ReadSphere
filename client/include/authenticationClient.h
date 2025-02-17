#ifndef AUTHENTICATIONCLIENT_H
#define AUTHENTICATIONCLIENT_H

#define USER_NOT_ADDED -140
#define USER_ADDED 141
#define AUTHENTICATED_ADMIN 142
#define AUTHENTICATED_USER 146
#define NOT_AUTHENTICATED -143
#define ADMIN_NOT_GRANTED -144
#define ADMIN_GRANTED 145
#define USER_DOES_NOT_EXIST 147
#define USER_ALREADY_EXISTS 148

char *
stringifyUser(char *firstName, char *middleName, char *lastName, char *username, char *password);

char*
stringifyUserPass(char *username, char *password);

char *
signupClient();

char *
loginClient();

char *
giveAdminAccessClient();



int
logout();

#endif