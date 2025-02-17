#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/completeRequest.h"
#include "../include/authentication.h"
#include "../include/bookManagement.h"
#include "../include/tokenizeString.h"


int completeRequest(char **tokens, int numberOfTokens, int *userId, int *isAdmin) {
    if (numberOfTokens < 1) {
        fprintf(stderr, "Error: No tokens provided\n");
        return INSUFFICIENT_TOKENS;
    }

    // Get the operation number
    int op = atoi(tokens[0]);

    // Switch based on the operation number
    switch (op) {
        case 1: // addUserServer
            if (numberOfTokens != 6) {
                fprintf(stderr, "Error: Invalid number of tokens for addUserServer\n");
                return INSUFFICIENT_TOKENS;
            }

            int status = addUserServer(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
            printf("COmpleteRequest %d\n", status);

        case 2: // userAuthenticateServer
            if (numberOfTokens != 3) {
                fprintf(stderr, "Error: Invalid number of tokens for userAuthenticateServer\n");
                return INSUFFICIENT_TOKENS;
            }
            
            return userAuthenticateServer(tokens[1], tokens[2], userId, isAdmin);
            

        case 3: // borrowBookServer
            if (numberOfTokens != 2) {
                fprintf(stderr, "Error: Invalid number of tokens for borrowBookServer\n");
                return INSUFFICIENT_TOKENS;
            }
            return borrowBookServer(*userId, atoi(tokens[1]));

        case 4: // returnBookServer
            if (numberOfTokens != 2) {
                fprintf(stderr, "Error: Invalid number of tokens for returnBookServer\n");
                return INSUFFICIENT_TOKENS;
            }

            return returnBookServer(*userId, atoi(tokens[1]));

        case 5: // queryBookServer
            if (numberOfTokens != 2) {
                fprintf(stderr, "Error: Invalid number of tokens for queryBookServer\n");
                return INSUFFICIENT_TOKENS;
            }
            return queryBookServer(atoi(tokens[1]));

        case 6: // addBookServer
            if (numberOfTokens != 5) {
                fprintf(stderr, "Error: Invalid number of tokens for addBookServer\n");
                return INSUFFICIENT_TOKENS;
            }
            return addBookServer(tokens[1], tokens[2], atoi(tokens[3]), atoi(tokens[4]), atoi(tokens[5]));

        case 7: // deleteBookServer
            if (numberOfTokens != 2) {
                fprintf(stderr, "Error: Invalid number of tokens for deleteBookServer\n");
                return INSUFFICIENT_TOKENS;
            }
            return deleteBookServer(atoi(tokens[1]));

        case 8: // modifyBookServer
            if (numberOfTokens != 6) {
                fprintf(stderr, "Error: Invalid number of tokens for modifyBookServer\n");
                return INSUFFICIENT_TOKENS;
            }
            return modifyBookServer(tokens[1], tokens[2], atoi(tokens[3]), atoi(tokens[4]), atoi(tokens[5]));

        case 9: // giveAdminAccessServer
            if (numberOfTokens != 2) {
                fprintf(stderr, "Error: Invalid number of tokens for giveAdminAccessServer\n");
                return INSUFFICIENT_TOKENS;
            }
            return giveAdminAccessServer(atoi(tokens[1]));

        default:
            fprintf(stderr, "Error: Unknown operation number %d\n", op);
            return INVALID_OPERATION;
    }
}
