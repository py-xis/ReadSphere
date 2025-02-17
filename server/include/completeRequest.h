#ifndef COMPLETEREQUEST_H
#define COMPLETEREQUEST_H

#define INSUFFICIENT_TOKENS 300
#define INVALID_OPERATION 301

int completeRequest(char **tokens, int numberOfTokens, int *userId, int *isAdmin);

#endif