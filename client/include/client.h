#ifndef CLIENT_H
#define CLIENT_H

#define STRINGIFY_ERROR NULL
#define CONNECTED_TO_SERVER 40
#define NOT_CONNECTED_TO_SERVER 41
#define STRINGIFY_SUCCESS_INT 42
#define STRINGIFY_ERROR_INT 43
#define INSUFFICIENT_TOKENS 300
#define INVALID_OPERATION 301
#define COULD_NOT_SEND 302
#define REPO_ERROR 119

int 
connectToServer(int *thisSock);

void
show_time();

void
adminOptions();

void
userOptions();

// TODO
void
handleServerReturnAdmin(int serverReturn);

void
handleServerReturnUser(int serverReturn);

void 
handleServerReturnAuth(int serverReturn);


#endif